//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "licencemanager.h"

#include "filemanager.h"
#include "logger.h"

#include <QSqlError>
#include <QUrlQuery>
#include <QVariant>

LicenceManager::LicenceManager(DatabaseManager &database, WebCommunication &web, FileManager &files, Logger &logger)
    : database(database), web(web), files(files), logger(logger)
{
}

// A modul minden művelet előtt ellenőrzi a kapcsolatot, és jelzi az eredményt a felületnek.
UiMessage LicenceManager::openDatabase()
{
    const auto status = database.open();
    UiMessage error;
    if (status == DatabaseManager::Status::DriverUnavailable)
        error = database.driverMessage();
    else if (status == DatabaseManager::Status::OpenFailed)
        error = database.connectionError();
    emit databaseStatusChanged(status, error);
    return error;
}

UiMessage LicenceManager::readLiveLicence(Licence &licence)
{
    // A legnagyobb azonosítójú éles licencet olvassa. A Cod a kliens-, az Act a szerverkód.
    auto query = database.executeSelect(QStringLiteral(
        "SELECT serial, Cod, Act, Type, lastValidated FROM licences WHERE licenceId > 1 ORDER BY licenceId DESC LIMIT 1"));
    const auto queryFailure = [this, &query] {
        const auto error = query.lastError();
        logger.write("ERROR", QStringLiteral("Licence record read failed%1").arg(DatabaseManager::errorCodeSuffix(error)));
        return UiMessage([error] {
            return QObject::tr("Database error while reading the licence key%1.")
                .arg(DatabaseManager::errorCodeSuffix(error));
        });
    };
    if (!query.isActive())
        return queryFailure();
    if (!query.next()) {
        if (query.lastError().isValid())
            return queryFailure();
        logger.write("WARN", QStringLiteral("No live licence record found"));
        return UiMessage([] { return QObject::tr("No live licence record was found (licenceId > 1)."); });
    }

    licence = {query.value(0).toString(), query.value(1).toString(),
               query.value(2).toString(), query.value(3).toString(), query.value(4).toString()};
    logger.write(licence.serial.trimmed().isEmpty() ? "WARN" : "INFO",
                   licence.serial.trimmed().isEmpty() ? QStringLiteral("Licence record read; serial is empty")
                                                      : QStringLiteral("Licence record read"));
    return {};
}

UiMessage LicenceManager::readLicenceServer(QUrl &server)
{
    // A meglévő WebSync-beállítás adja az API alapcímét.
    auto query = database.executeSelect(
        QStringLiteral("SELECT value FROM settings WHERE identifier = 'SERVER_Address'"));
    if (!query.isActive() || (!query.next() && query.lastError().isValid()))
        return UiMessage([error = query.lastError()] {
            return QObject::tr("Database error while reading the server address%1.")
                .arg(DatabaseManager::errorCodeSuffix(error));
        });

    if (query.isValid())
        server = QUrl(query.value(0).toString().trimmed(), QUrl::StrictMode);
    if (server.scheme() == QStringLiteral("http"))
        server.setScheme(QStringLiteral("https"));
    if (!server.isValid() || server.scheme() != QStringLiteral("https") || server.host().isEmpty() ||
        !server.userInfo().isEmpty() || server.hasQuery() || server.hasFragment())
        return UiMessage([] { return QObject::tr("Missing or invalid HTTPS server address (SERVER_Address)."); });
    return {};
}

// A lekérdezések itt már lezárultak, mielőtt a hívó bontja az adatbázis-kapcsolatot.
UiMessage LicenceManager::loadLocalLicence(Licence &licence)
{
    const auto error = readLiveLicence(licence);
    if (error.isEmpty())
        emit localLicenceRead(licence);
    return error;
}

UiMessage LicenceManager::loadServer(QUrl &server)
{
    const auto error = readLicenceServer(server);
    emit serverAddressChanged(error.isEmpty() ? server.toDisplayString() : QString());
    return error;
}

// Induláskor és kézi frissítéskor csak a helyi licencadatokat olvassa.
void LicenceManager::refreshLocalLicence()
{
    Licence licence;
    auto error = openDatabase();
    if (error.isEmpty())
        error = loadLocalLicence(licence);
    database.close();
    if (!error.isEmpty())
        emit localLicenceReadFailed(error);
}

// A Státusz fül külön szerverpróbája a konfigurált címről próbál tokent kérni.
void LicenceManager::checkServer()
{
    QUrl server;
    auto error = openDatabase();
    if (error.isEmpty())
        error = loadServer(server);
    database.close();
    if (!error.isEmpty()) {
        emit serverCheckNotStarted(error);
        return;
    }

    emit networkBusyChanged(true);
    web.requestToken(server, this, [this](const WebCommunication::TokenResult &result) {
        emit serverTokenResult(result.ok);
        emit networkBusyChanged(false);
    });
}

// 1. A licencet és a szervercímet egy helyi kapcsolaton olvassa, majd bezárja a kapcsolatot.
void LicenceManager::checkOnline()
{
    Licence licence;
    QUrl server;
    auto error = openDatabase();
    if (error.isEmpty()) {
        error = loadLocalLicence(licence);
        if (error.isEmpty())
            error = loadServer(server);
        else
            emit localLicenceReadFailed(error);
    }
    database.close();
    if (!error.isEmpty()) {
        emit onlineFinished(error);
        return;
    }

    emit networkBusyChanged(true);
    emit onlineStarted();
    requestLicenceToken(server, licence);
}

// 2. Friss tokent kér. Tokenhiba esetén nem küld licenckérést.
void LicenceManager::requestLicenceToken(const QUrl &server, const Licence &licence)
{
    web.requestToken(server, this, [this, server, licence](const WebCommunication::TokenResult &token) {
        emit serverTokenResult(token.ok);
        if (!token.ok) {
            logMissingLicenceResponse();
            finishOnline(UiMessage([error = token.error] { return tr("Token request: %1").arg(error.text()); }));
            return;
        }
        sendLicenceCheck(server, licence, token.value);
    });
}

// 3. A licenc és a token alapján összeállítja a WebSync LICENCE_CHECK kérését.
// Az üres kódokat üres paraméterként továbbítja.
void LicenceManager::sendLicenceCheck(const QUrl &server, const Licence &licence, const QString &token)
{
    auto url = WebCommunication::endpoint(server, QStringLiteral("licence.php"));
    QUrlQuery query;
    const auto add = [&query](const QString &key, const QString &value) {
        query.addQueryItem(key, value.isEmpty() ? QStringLiteral("")
                                                : QString::fromLatin1(QUrl::toPercentEncoding(value)));
    };
    add(QStringLiteral("token"), token);
    add(QStringLiteral("Cmd"), QStringLiteral("LICENCE_CHECK"));
    add(QStringLiteral("StudioId"), licence.serial);
    add(QStringLiteral("ClientCode"), licence.clientCode);
    add(QStringLiteral("ServerCode"), licence.serverCode);
    url.setQuery(query);
    web.get(url, this, [this](const WebCommunication::Response &response) {
        finishOnline(processResponse(response));
    });
}

void LicenceManager::logMissingLicenceResponse()
{
    logger.write("WARN", QStringLiteral("No LICENCE_CHECK response received; any existing %1 is from a previous attempt")
                             .arg(files.applicationFilePath(QStringLiteral("licence-response.txt"))));
}

void LicenceManager::saveLicenceResponse(const QByteArray &body)
{
    const auto path = files.applicationFilePath(QStringLiteral("licence-response.txt"));
    QString error;
    if (!files.save(path, body, error)) {
        logger.write("ERROR", QStringLiteral("Could not save LICENCE_CHECK response to %1; responseBytes=%2; error=%3")
                                  .arg(path).arg(body.size()).arg(error));
        return;
    }
    logger.write("INFO", QStringLiteral("Saved LICENCE_CHECK response to %1; responseBytes=%2")
                             .arg(path).arg(body.size()));
}

// 4. Elmenti a kapott törzset, majd megkülönbözteti a technikai hibát az üzleti választól.
// A szerver visszajelzése megjelenik a felületen, a helyi adatbázisba nem ír.
UiMessage LicenceManager::processResponse(const WebCommunication::Response &response)
{
    const auto &body = response.body;
    if (response.status.has_value() || !body.isEmpty())
        saveLicenceResponse(body);
    else
        logMissingLicenceResponse();

    const auto contentType = response.contentType;
    // Csak ismert médiatípusokat naplózunk, nyers fejlécet vagy törzset nem.
    const auto mediaType = contentType.split(';').first().trimmed().toLower();
    const auto safeType = mediaType == "text/html" || mediaType == "text/plain" ||
                          mediaType == "application/json" || mediaType == "application/octet-stream" ||
                          mediaType == "application/xhtml+xml"
        ? QString::fromLatin1(mediaType)
        : (contentType.isEmpty() ? QStringLiteral("unavailable") : QStringLiteral("other"));
    const auto logResult = [&](const char *level, const char *code, const QString &details = {}) {
        logger.write(level, QStringLiteral("LICENCE_CHECK /kiwi_ticket/licence.php; HTTP=%1; "
                                             "Content-Type=%2; responseBytes=%3; result=%4%5")
                                  .arg(response.statusText(), safeType)
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
    const int status = response.status.value_or(0);
    if (response.tlsFailed()) {
        logResult("ERROR", "TLS_ERROR");
        return UiMessage([] { return QObject::tr("Network error: TLS verification failed."); });
    }
    if (status != 0 && status != 200) {
        logResult("WARN", "UNEXPECTED_HTTP_STATUS", describeBody());
        return UiMessage([status] { return QObject::tr("Unexpected HTTP response (%1).").arg(status); });
    }
    if (response.networkError != QNetworkReply::NoError) {
        logResult("ERROR", "NETWORK_ERROR", QStringLiteral("; networkError=%1").arg(static_cast<int>(response.networkError)));
        return UiMessage([code = static_cast<int>(response.networkError)] {
            return QObject::tr("Network error during licence check (Qt error code: %1).").arg(code);
        });
    }

    const auto lineEnd = body.indexOf('\n');
    const auto line = body.first(lineEnd < 0 ? body.size() : lineEnd).trimmed();
    if (line.contains("LICENCE_OK")) {
        logResult("INFO", "LICENCE_OK");
        return UiMessage([] { return QObject::tr("LICENCE_OK – The licence is valid."); });
    }

    if (line.contains("SERVER_CODE_INVALID")) {
        logResult("WARN", line.contains("LICENCE_SERVER_CODE_INVALID")
                              ? "LICENCE_SERVER_CODE_INVALID" : "SERVER_CODE_INVALID");
        return UiMessage([] {
            return QObject::tr("The server verification code for the local licence is invalid (SERVER_CODE_INVALID).");
        });
    }

    constexpr const char *errors[] = {
        "LICENCE_INVALID", "LICENCE_ALREADY_REGISTERED", "LICENCE_INACTIVE",
        "LICENCE_CLIENT_CODE_INVALID", "Missing token"
    };
    for (const auto *error : errors) {
        if (line.contains(error)) {
            logResult("WARN", error);
            return UiMessage([error] { return QObject::tr("Licence server error: %1.").arg(QString::fromLatin1(error)); });
        }
    }
    logResult("WARN", "UNEXPECTED_RESPONSE", describeBody());
    return UiMessage([] { return QObject::tr("Unexpected response from the licence server."); });
}

// 5. Jelzi az eredményt, majd újra engedélyezi a hálózati műveleteket.
void LicenceManager::finishOnline(const UiMessage &message)
{
    emit onlineFinished(message);
    emit networkBusyChanged(false);
}
