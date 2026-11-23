//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include "database.h"
#include "webcommunication.h"

#include <QObject>

class FileManager;
class Logger;

struct Licence {
    QString serial;
    QString clientCode;
    QString serverCode;
    QString status;
    QString checkedAt;
};

// A helyi licencolvasást és a WebSync szerinti online ellenőrzést vezérli.
// Az SQL-t a DatabaseManager, a HTTPS-kéréseket a WebCommunication hajtja végre.
class LicenceManager : public QObject {
    Q_OBJECT

public:
    LicenceManager(DatabaseManager &database, WebCommunication &web, FileManager &files, Logger &logger);

public slots:
    void refreshLocalLicence();
    void checkServer();
    void checkOnline();

signals:
    void databaseStatusChanged(DatabaseManager::Status status, const UiMessage &details);
    void localLicenceRead(const Licence &licence);
    void localLicenceReadFailed(const UiMessage &message);
    void serverAddressChanged(const QString &address);
    void serverCheckNotStarted(const UiMessage &message);
    void serverTokenResult(bool ok);
    void networkBusyChanged(bool busy);
    void onlineStarted();
    void onlineFinished(const UiMessage &message);

private:
    UiMessage openDatabase();
    UiMessage readLiveLicence(Licence &licence);
    UiMessage readLicenceServer(QUrl &server);
    UiMessage loadLocalLicence(Licence &licence);
    UiMessage loadServer(QUrl &server);

    void requestLicenceToken(const QUrl &server, const Licence &licence);
    void sendLicenceCheck(const QUrl &server, const Licence &licence, const QString &token);
    UiMessage processResponse(const WebCommunication::Response &response);
    void finishOnline(const UiMessage &message);
    void logMissingLicenceResponse();
    void saveLicenceResponse(const QByteArray &body);

    DatabaseManager &database;
    WebCommunication &web;
    FileManager &files;
    Logger &logger;
};
