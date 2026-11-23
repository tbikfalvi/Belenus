//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <functional>
#include <optional>

class Logger;

// A HTTPS és a közös tokenprotokoll technikai rétege. Az üzleti válaszokat a hívó értelmezi.
class WebCommunication {
public:
    struct Response {
        std::optional<int> status;
        QNetworkReply::NetworkError networkError;
        QString errorMessage;
        QString certificateErrors;
        QByteArray contentType;
        QByteArray body;

        QString statusText() const;
        bool tlsFailed() const;
    };

    struct TokenResult {
        bool ok;
        QString value;
        QString error;
    };

    explicit WebCommunication(Logger &logger);

    // Az eredménykezelő csak a context élettartama alatt hívható.
    void get(const QUrl &url, QObject *context, std::function<void(const Response &)> finished);
    void requestToken(const QUrl &server, QObject *context, std::function<void(const TokenResult &)> finished);
    static QUrl endpoint(QUrl server, const QString &file);

private:
    static TokenResult parseToken(const Response &response);

    Logger &logger;
    QNetworkAccessManager network;
};
