//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.4.0.0"
#define DATABASE_VERSION_NUMBER     "2.4.0.0"

//====================================================================================


#include <QApplication>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QRegularExpression>
#include <QSaveFile>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSslError>
#include <QTextStream>
#include <QTimer>
#include <QUrlQuery>

#include "ui_mainwindow.h"

namespace {

class EventLog {
public:
    bool open()
    {
        file.setFileName(QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("webapp.log")));
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            return true;
        error = file.errorString();
        return false;
    }

    void write(const char *level, const QString &event)
    {
        if (!file.isOpen())
            return;
        const auto line = QStringLiteral("%1 [%2] %3\n")
                              .arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs),
                                   QString::fromLatin1(level), event);
        const auto data = line.toUtf8();
        if (file.write(data) != data.size() || !file.flush()) {
            error = file.errorString();
            file.close();
        }
    }

    QString error;

private:
    QFile file;
};

EventLog eventLog;

struct ProbeResult {
    bool ok;
    QString message;
};

struct Licence {
    QString serial;
    QString clientCode;
    QString serverCode;
    QString status;
    QString checkedAt;
};

class TokenProbe : public QNetworkAccessManager {
public:
    using QNetworkAccessManager::QNetworkAccessManager;

    template<typename Callback>
    void requestToken(const QUrl &url, Callback callback)
    {
        auto *reply = request(url);
        connect(reply, &QNetworkReply::finished, reply, [reply, callback] {
            const auto result = readResult(*reply);
            eventLog.write(result.ok ? "INFO" : "ERROR",
                           QStringLiteral("Token request %1; HTTP=%2; networkError=%3")
                               .arg(result.ok ? QStringLiteral("succeeded") : QStringLiteral("failed"))
                               .arg(httpStatus(*reply))
                               .arg(static_cast<int>(reply->error())));
            reply->deleteLater();
            callback(result);
        });
    }

    template<typename ConnectionCallback, typename Callback>
    void checkLicence(const QUrl &server, const Licence &licence, ConnectionCallback connection, Callback callback)
    {
        requestToken(endpoint(server, QStringLiteral("token.php")),
                     [this, server, licence, connection, callback](const ProbeResult &token) {
            connection(token.ok);
            if (!token.ok) {
                logMissingLicenceResponse();
                callback(ProbeResult{false, QObject::tr("Tokenkérés: %1").arg(token.message)});
                return;
            }
            QUrl url = endpoint(server, QStringLiteral("licence.php"));
            QUrlQuery query;
            const auto add = [&query](const QString &key, const QString &value) {
                query.addQueryItem(key, value.isEmpty() ? QStringLiteral("")
                                                        : QString::fromLatin1(QUrl::toPercentEncoding(value)));
            };
            add(QStringLiteral("token"), token.message);
            add(QStringLiteral("Cmd"), QStringLiteral("LICENCE_CHECK"));
            add(QStringLiteral("StudioId"), licence.serial);
            add(QStringLiteral("ClientCode"), licence.clientCode);
            add(QStringLiteral("ServerCode"), licence.serverCode);
            url.setQuery(query);
            auto *reply = request(url);
            connect(reply, &QNetworkReply::finished, reply, [reply, callback] {
                const auto result = licenceResult(*reply);
                reply->deleteLater();
                callback(result);
            });
        });
    }

    template<typename Callback>
    void checkServer(const QUrl &server, Callback callback)
    {
        requestToken(endpoint(server, QStringLiteral("token.php")),
                     [callback](const ProbeResult &result) { callback(result.ok); });
    }

private:
    static QString licenceResponsePath()
    {
        return QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("licence-response.txt"));
    }

    static void logMissingLicenceResponse()
    {
        eventLog.write("WARN", QStringLiteral("No LICENCE_CHECK response received; any existing %1 is from a previous attempt")
                                   .arg(licenceResponsePath()));
    }

    static void saveLicenceResponse(const QByteArray &body)
    {
        const auto path = licenceResponsePath();
        QSaveFile file(path);
        if (!file.open(QIODevice::WriteOnly) || file.write(body) != body.size() || !file.commit()) {
            eventLog.write("ERROR", QStringLiteral("Could not save LICENCE_CHECK response to %1; responseBytes=%2; error=%3")
                                        .arg(path).arg(body.size()).arg(file.errorString()));
            return;
        }
        eventLog.write("INFO", QStringLiteral("Saved LICENCE_CHECK response to %1; responseBytes=%2")
                                   .arg(path).arg(body.size()));
    }

    static QString httpStatus(const QNetworkReply &reply)
    {
        const auto status = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute);
        return status.isValid() ? status.toString() : QStringLiteral("unavailable");
    }

    static QUrl endpoint(QUrl server, const QString &file)
    {
        auto path = server.path();
        if (!path.endsWith(QLatin1Char('/')))
            path += QLatin1Char('/');
        server.setPath(path + QStringLiteral("kiwi_ticket/") + file);
        return server;
    }

    QNetworkReply *request(const QUrl &url)
    {
        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                             QNetworkRequest::SameOriginRedirectPolicy);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        request.setTransferTimeout(12000);

        auto *reply = get(request);
        connect(reply, &QNetworkReply::sslErrors, reply, [reply](const QList<QSslError> &errors) {
            QStringList messages;
            for (const auto &error : errors)
                messages.append(error.errorString());
            reply->setProperty("certificateErrors", messages.join(QStringLiteral("; ")));
        });
        return reply;
    }

    static ProbeResult licenceResult(QNetworkReply &reply)
    {
        const auto body = reply.readAll();
        if (reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).isValid() || !body.isEmpty())
            saveLicenceResponse(body);
        else
            logMissingLicenceResponse();

        const auto contentType = reply.rawHeader("Content-Type");
        // Match known media types without copying untrusted header values or parameters to the log.
        const auto mediaType = contentType.split(';').first().trimmed().toLower();
        const auto safeType = mediaType == "text/html" || mediaType == "text/plain" ||
                              mediaType == "application/json" || mediaType == "application/octet-stream" ||
                              mediaType == "application/xhtml+xml"
            ? QString::fromLatin1(mediaType)
            : (contentType.isEmpty() ? QStringLiteral("unavailable") : QStringLiteral("other"));
        const auto logResult = [&](const char *level, const char *code, const QString &details = {}) {
            eventLog.write(level, QStringLiteral("LICENCE_CHECK /kiwi_ticket/licence.php; HTTP=%1; "
                                                 "Content-Type=%2; responseBytes=%3; result=%4%5")
                                      .arg(httpStatus(reply), safeType)
                                      .arg(body.size())
                                      .arg(QString::fromLatin1(code), details));
        };
        const auto describeBody = [&] {
            const auto prefix = body.left(256).trimmed().toLower();
            if (body.trimmed().isEmpty())
                return QStringLiteral("; response=empty");
            if (mediaType == "text/html" || mediaType == "application/xhtml+xml" ||
                prefix.startsWith("<!doctype html") || prefix.startsWith("<html"))
                return QStringLiteral("; response=HTML");
            if (mediaType.startsWith("text/") ||
                (!prefix.contains('\0') && !QString::fromUtf8(prefix).contains(QChar::ReplacementCharacter)))
                return QStringLiteral("; response=text");
            return QStringLiteral("; response=other");
        };
        const int status = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (reply.error() == QNetworkReply::SslHandshakeFailedError ||
            !reply.property("certificateErrors").toString().isEmpty()) {
            logResult("ERROR", "TLS_ERROR");
            return {false, QObject::tr("Hálózati hiba: a TLS-kapcsolat ellenőrzése sikertelen.")};
        }
        if (status != 0 && status != 200) {
            logResult("WARN", "UNEXPECTED_HTTP_STATUS", describeBody());
            return {false, QObject::tr("Váratlan HTTP-válasz (%1).").arg(status)};
        }
        if (reply.error() != QNetworkReply::NoError) {
            logResult("ERROR", "NETWORK_ERROR", QStringLiteral("; networkError=%1").arg(static_cast<int>(reply.error())));
            return {false, QObject::tr("Hálózati hiba a licencellenőrzéskor (Qt hibakód: %1).")
                               .arg(static_cast<int>(reply.error()))};
        }

        const auto lineEnd = body.indexOf('\n');
        const auto line = body.first(lineEnd < 0 ? body.size() : lineEnd).trimmed();
        if (line.contains("LICENCE_OK")) {
            logResult("INFO", "LICENCE_OK");
            return {true, QObject::tr("LICENCE_OK – A licenc érvényes.")};
        }

        if (line.contains("SERVER_CODE_INVALID")) {
            logResult("WARN", line.contains("LICENCE_SERVER_CODE_INVALID")
                                  ? "LICENCE_SERVER_CODE_INVALID" : "SERVER_CODE_INVALID");
            return {false, QObject::tr("A helyi licenchez tartozó szerveroldali ellenőrzőkód érvénytelen (SERVER_CODE_INVALID).")};
        }

        constexpr const char *errors[] = {
            "LICENCE_INVALID", "LICENCE_ALREADY_REGISTERED", "LICENCE_INACTIVE",
            "LICENCE_CLIENT_CODE_INVALID", "Missing token"
        };
        for (const auto *error : errors) {
            if (line.contains(error)) {
                logResult("WARN", error);
                return {false, QObject::tr("Szerveroldali licenchiba: %1.").arg(QString::fromLatin1(error))};
            }
        }
        logResult("WARN", "UNEXPECTED_RESPONSE", describeBody());
        return {false, QObject::tr("Váratlan válasz a licencszervertől.")};
    }

    static ProbeResult readResult(QNetworkReply &reply)
    {
        const auto certificateErrors = reply.property("certificateErrors").toString();
        if (!certificateErrors.isEmpty())
            return {false, QObject::tr("TLS certificate verification failed: %1").arg(certificateErrors)};

        const int status = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status != 0 && status != 200)
            return {false, QObject::tr("Unexpected HTTP status: %1.").arg(status)};
        if (reply.error() != QNetworkReply::NoError)
            return {false, QObject::tr("HTTPS request failed: %1").arg(reply.errorString())};

        // The legacy server returns the token in the first 16 characters of the first line.
        const auto token = reply.readLine().left(16);
        static const QRegularExpression format(QStringLiteral("^[A-Za-z0-9_-]{16}$"));
        if (status != 200 || !format.match(QString::fromLatin1(token)).hasMatch())
            return {false, QObject::tr("The server response does not contain a recognizable token.")};
        return {true, QString::fromLatin1(token)};
    }
};

constexpr auto connectionName = "webapp-mysql-probe";

ProbeResult checkDriver(const QSqlDatabase &db)
{
    if (db.isValid())
        return {true, QObject::tr("QMYSQL loaded.")};

    const auto drivers = QSqlDatabase::drivers().join(QStringLiteral(", "));
    return {false, QObject::tr("QMYSQL could not be loaded (%1). Available drivers: %2.")
                       .arg(db.lastError().text(), drivers)};
}

QString sqlError(const QSqlError &error)
{
    // Use numeric driver codes only. Connection errors can contain account information.
    const auto code = error.nativeErrorCode();
    static const QRegularExpression numeric(QStringLiteral("^[0-9]+$"));
    return numeric.match(code).hasMatch()
        ? QObject::tr(" (MySQL error %1)").arg(code)
        : QString();
}

QString connectionErrorText(QString text, const QString &password)
{
    static const QRegularExpression connectionDetails(
        QStringLiteral(R"([A-Za-z][A-Za-z0-9+.-]*://\S+|\b(?:dsn|driver|server|host|database|uid|user(?:name)?|pwd|password)\s*=.*)"),
        QRegularExpression::CaseInsensitiveOption);
    text.replace(connectionDetails, QStringLiteral("[redacted]"));
    if (!password.isEmpty())
        text.replace(password, QStringLiteral("[redacted]"));
    return text;
}

ProbeResult connectionFailure(const QSqlDatabase &db)
{
    const QSqlError error = db.lastError();
    const auto password = db.password();
    return {false, QObject::tr("QMYSQL loaded, but the connection failed.\n"
                              "nativeErrorCode(): %1\n"
                              "driverText(): %2\n"
                              "databaseText(): %3")
                       .arg(connectionErrorText(error.nativeErrorCode(), password),
                            connectionErrorText(error.driverText(), password),
                            connectionErrorText(error.databaseText(), password))};
}

ProbeResult readLiveLicence(QSqlDatabase &db, Licence &licence)
{
    QSqlQuery query(db);
    const auto queryFailure = [&query] {
        eventLog.write("ERROR", QStringLiteral("Licence record read failed%1").arg(sqlError(query.lastError())));
        return ProbeResult{false, QObject::tr("Adatbázishiba a licenckulcs lekérdezésekor%1.")
                                      .arg(sqlError(query.lastError()))};
    };
    if (!query.exec(QStringLiteral(
            "SELECT serial, Cod, Act, Type, lastValidated FROM licences WHERE licenceId > 1 ORDER BY licenceId DESC LIMIT 1")))
        return queryFailure();
    if (!query.next()) {
        if (query.lastError().isValid())
            return queryFailure();
        eventLog.write("WARN", QStringLiteral("No live licence record found"));
        return {false, QObject::tr("Nincs éles licencrekord (licenceId > 1).")};
    }

    licence = {query.value(0).toString(), query.value(1).toString(),
               query.value(2).toString(), query.value(3).toString(), query.value(4).toString()};
    eventLog.write(licence.serial.trimmed().isEmpty() ? "WARN" : "INFO",
                   licence.serial.trimmed().isEmpty() ? QStringLiteral("Licence record read; serial is empty")
                                                      : QStringLiteral("Licence record read"));
    return {true, {}};
}

ProbeResult readLicenceServer(QSqlDatabase &db, QUrl &server)
{
    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("SELECT value FROM settings WHERE identifier = 'SERVER_Address'")) ||
        (!query.next() && query.lastError().isValid()))
        return {false, QObject::tr("Adatbázishiba a szervercím lekérdezésekor%1.").arg(sqlError(query.lastError()))};

    if (query.isValid())
        server = QUrl(query.value(0).toString().trimmed(), QUrl::StrictMode);
    if (server.scheme() == QStringLiteral("http"))
        server.setScheme(QStringLiteral("https"));
    if (!server.isValid() || server.scheme() != QStringLiteral("https") || server.host().isEmpty() ||
        !server.userInfo().isEmpty() || server.hasQuery() || server.hasFragment())
        return {false, QObject::tr("Hiányzó vagy érvénytelen HTTPS-szervercím (SERVER_Address).")};
    return {true, {}};
}

class Window : public QDialog {
public:
    Window()
    {
        ui.setupUi(this);
        setWindowTitle(QStringLiteral("Belenus WebApp - v" APPLICATION_VERSION_NUMBER));
        ui.logStatus->setVisible(!eventLog.error.isEmpty());
        if (!eventLog.error.isEmpty())
            ui.logStatus->setText(tr("A webapp.log nem megnyitható vagy nem írható: %1").arg(eventLog.error));

        connect(ui.exitButton, &QPushButton::clicked, this, &QDialog::close);
        connect(ui.databaseRefresh, &QPushButton::clicked, this, &Window::refreshLocalLicence);
        connect(ui.serverRefresh, &QPushButton::clicked, this, &Window::checkServer);
        connect(ui.licenceRefresh, &QPushButton::clicked, this, &Window::refreshLocalLicence);
        connect(ui.onlineButton, &QPushButton::clicked, this, &Window::checkOnline);
        QTimer::singleShot(0, this, &Window::refreshLocalLicence);
    }

private:
    static void showConnection(QLabel *light, QLabel *status, const char *icon, const QString &message)
    {
        light->setPixmap(QPixmap(QStringLiteral(":/icons/status_%1.png").arg(QString::fromLatin1(icon))));
        status->setText(message);
        status->setToolTip({});
    }

    void showServerResult(bool ok)
    {
        showConnection(ui.serverLight, ui.serverStatus, ok ? "green" : "red",
                       ok ? tr("HTTPS-kapcsolat létrejött.") : tr("A tokenkérés sikertelen."));
    }

    void setNetworkBusy(bool busy)
    {
        ui.serverRefresh->setEnabled(!busy);
        ui.onlineButton->setEnabled(!busy);
        if (busy)
            showConnection(ui.serverLight, ui.serverStatus, "yellow", tr("Kapcsolódás…"));
    }

    void checkServer()
    {
        QUrl server;
        const auto result = withDatabase([&](QSqlDatabase &db) { return loadServer(db, server); });
        if (!result.ok) {
            showConnection(ui.serverLight, ui.serverStatus, "yellow", tr("A szerver ellenőrzése nem indult el."));
            ui.serverStatus->setToolTip(result.message);
            return;
        }
        setNetworkBusy(true);
        tokenProbe.checkServer(server, [this](bool ok) {
            showServerResult(ok);
            setNetworkBusy(false);
        });
    }

    void checkOnline()
    {
        resetLocalLicence();
        ui.onlineStatus->clear();
        Licence licence;
        QUrl server;
        const auto result = withDatabase([&](QSqlDatabase &db) {
            const auto local = loadLocalLicence(db, licence);
            return local.ok ? loadServer(db, server) : local;
        });
        if (!result.ok) {
            ui.onlineStatus->setText(result.message);
            return;
        }

        setNetworkBusy(true);
        ui.onlineStatus->setText(tr("Online licencellenőrzés folyamatban…"));
        tokenProbe.checkLicence(server, licence, [this](bool ok) { showServerResult(ok); },
                                [this](const ProbeResult &response) {
            ui.onlineStatus->setText(response.message);
            setNetworkBusy(false);
        });
    }

    template<typename Action>
    ProbeResult withDatabase(Action action)
    {
        ProbeResult result;
        {
            auto db = QSqlDatabase::addDatabase(QStringLiteral("QMYSQL"), connectionName);
            result = openDatabase(db);
            if (result.ok)
                result = action(db);
        }
        QSqlDatabase::removeDatabase(connectionName);
        return result;
    }

    ProbeResult openDatabase(QSqlDatabase &db)
    {
        const auto driver = checkDriver(db);
        if (!driver.ok) {
            eventLog.write("ERROR", QStringLiteral("Database connection unavailable; QMYSQL did not load"));
            showConnection(ui.databaseLight, ui.databaseStatus, "red", tr("A QMYSQL meghajtó nem tölthető be."));
            return driver;
        }
        const auto localAccount = QStringLiteral("belenus");
        db.setHostName(QStringLiteral("127.0.0.1"));
        db.setPort(3306);
        db.setDatabaseName(localAccount);
        db.setUserName(localAccount);
        db.setPassword(localAccount);
        db.setConnectOptions(QStringLiteral("MYSQL_OPT_CONNECT_TIMEOUT=5;MYSQL_OPT_READ_TIMEOUT=5;"
                                            "MYSQL_OPT_SSL_VERIFY_SERVER_CERT=0"));
        if (!db.open()) {
            eventLog.write("ERROR", QStringLiteral("Database connection failed%1").arg(sqlError(db.lastError())));
            const auto error = connectionFailure(db);
            showConnection(ui.databaseLight, ui.databaseStatus, "red", tr("A kapcsolat sikertelen."));
            ui.databaseStatus->setToolTip(error.message);
            return error;
        }
        eventLog.write("INFO", QStringLiteral("Database connection opened"));
        showConnection(ui.databaseLight, ui.databaseStatus, "green", tr("A kapcsolat sikeres."));
        return {true, {}};
    }

    void resetLocalLicence()
    {
        ui.localSerial->clear();
        ui.localDate->clear();
        ui.localServerCode->clear();
        ui.localClientCode->clear();
        ui.licenceStatus->setText(tr("Még nincs beolvasva."));
    }

    void refreshLocalLicence()
    {
        Licence licence;
        resetLocalLicence();
        const auto result = withDatabase([&](QSqlDatabase &db) { return loadLocalLicence(db, licence); });
        if (!result.ok)
            ui.licenceStatus->setText(result.message);
    }

    ProbeResult loadLocalLicence(QSqlDatabase &db, Licence &licence)
    {
        const auto result = readLiveLicence(db, licence);
        if (!result.ok) {
            ui.licenceStatus->setText(result.message);
            return result;
        }
        ui.localSerial->setText(licence.serial);
        ui.localDate->setText(licence.checkedAt);
        ui.localServerCode->setText(licence.serverCode);
        ui.localClientCode->setText(licence.clientCode);
        ui.licenceStatus->setText(licence.serial.trimmed().isEmpty() ? tr("A helyi licenckulcs üres.")
                                  : licence.status.isEmpty() ? tr("Ismeretlen") : licence.status);
        return result;
    }

    ProbeResult loadServer(QSqlDatabase &db, QUrl &server)
    {
        const auto result = readLicenceServer(db, server);
        ui.serverAddress->setText(result.ok ? server.toDisplayString() : QString());
        return result;
    }

    Ui::WebAppWindow ui;
    TokenProbe tokenProbe;
};
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    eventLog.open();
    eventLog.write("INFO", QStringLiteral("WebApp started"));
    const auto args = app.arguments();
    if (args.contains(QStringLiteral("--check-qmysql"))) {
        ProbeResult result;
        {
            const auto db = QSqlDatabase::addDatabase(QStringLiteral("QMYSQL"), connectionName);
            result = checkDriver(db);
        }
        QSqlDatabase::removeDatabase(connectionName);
        QTextStream(stdout) << result.message << Qt::endl;
        return result.ok ? 0 : 1;
    }

    Window window;
    window.show();
    return app.exec();
}
