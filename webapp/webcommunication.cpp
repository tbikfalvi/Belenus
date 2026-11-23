//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "webcommunication.h"

#include "logger.h"

#include <QNetworkRequest>
#include <QObject>
#include <QRegularExpression>
#include <QSslError>
#include <QStringList>
#include <QVariant>

WebCommunication::WebCommunication(Logger &logger) : logger(logger)
{
}

// Kilépéskor megszakítja a függő kéréseket, az üzleti visszahívások futtatása nélkül.
void WebCommunication::shutdown()
{
    stopping = true;
    const auto replies = network.findChildren<QNetworkReply *>();
    for (auto *reply : replies) {
        if (reply->isRunning())
            reply->abort();
    }
}

QString WebCommunication::Response::statusText() const
{
    return status ? QString::number(*status) : QStringLiteral("unavailable");
}

bool WebCommunication::Response::tlsFailed() const
{
    return networkError == QNetworkReply::SslHandshakeFailedError || !certificateErrors.isEmpty();
}

// A konfigurált alapcím útvonalához hozzáfűzi a WebSync API végpontját.
QUrl WebCommunication::endpoint(QUrl server, const QString &file)
{
    auto path = server.path();
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    server.setPath(path + QStringLiteral("kiwi_ticket/") + file);
    return server;
}

// Aszinkron GET. A válasz törzsét egyszer olvassa ki, és a technikai státusszal adja tovább.
void WebCommunication::get(const QUrl &url, QObject *context, std::function<void(const Response &)> finished)
{
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::SameOriginRedirectPolicy);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setTransferTimeout(12000);

    auto *reply = network.get(request);
    // A tanúsítványhibákat rögzíti a válaszhoz, a Qt ellenőrzését nem bírálja felül.
    QObject::connect(reply, &QNetworkReply::sslErrors, reply, [reply](const QList<QSslError> &errors) {
        QStringList messages;
        for (const auto &error : errors)
            messages.append(error.errorString());
        reply->setProperty("certificateErrors", messages.join(QStringLiteral("; ")));
    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
    QObject::connect(reply, &QNetworkReply::finished, context, [this, reply, finished] {
        if (stopping)
            return;
        const auto status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        finished(Response{
            status.isValid() ? std::optional<int>(status.toInt()) : std::nullopt,
            reply->error(), reply->errorString(), reply->property("certificateErrors").toString(),
            reply->rawHeader("Content-Type"), reply->readAll()
        });
    });
}

// A server alapcímről friss tokent kér. A naplóba csak a technikai eredmény kerül.
void WebCommunication::requestToken(const QUrl &server, QObject *context,
                                    std::function<void(const TokenResult &)> finished)
{
    get(endpoint(server, QStringLiteral("token.php")), context, [this, finished](const Response &response) {
        const auto token = parseToken(response);
        logger.write(token.ok ? "INFO" : "ERROR",
                     QStringLiteral("Token request %1; HTTP=%2; networkError=%3")
                         .arg(token.ok ? QStringLiteral("succeeded") : QStringLiteral("failed"))
                         .arg(response.statusText())
                         .arg(static_cast<int>(response.networkError)));
        finished(token);
    });
}

// A WebSync protokollban a token az első sor első 16 karaktere.
WebCommunication::TokenResult WebCommunication::parseToken(const Response &response)
{
    if (!response.certificateErrors.isEmpty())
        return {false, {}, UiMessage([details = response.certificateErrors] {
            return QObject::tr("TLS certificate verification failed: %1").arg(details);
        })};

    const int status = response.status.value_or(0);
    if (status != 0 && status != 200)
        return {false, {}, UiMessage([status] { return QObject::tr("Unexpected HTTP status: %1.").arg(status); })};
    if (response.networkError != QNetworkReply::NoError)
        return {false, {}, UiMessage([details = response.errorMessage] {
            return QObject::tr("HTTPS request failed: %1").arg(details);
        })};

    const auto lineEnd = response.body.indexOf('\n');
    const auto token = response.body.left(lineEnd < 0 ? 16 : qMin<qsizetype>(16, lineEnd + 1));
    static const QRegularExpression format(QStringLiteral("^[A-Za-z0-9_-]{16}$"));
    if (status != 200 || !format.match(QString::fromLatin1(token)).hasMatch())
        return {false, {}, UiMessage([] {
            return QObject::tr("The server response does not contain a recognizable token.");
        })};
    return {true, QString::fromLatin1(token), {}};
}
