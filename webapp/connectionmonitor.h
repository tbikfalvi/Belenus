//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include "database.h"

#include <QTimer>
#include <QUrl>

class WebCommunication;

// A helyi adatbázist és a távoli HTTPS-szervert rendszeresen, üzleti művelet nélkül ellenőrzi.
class ConnectionMonitor : public QObject {
    Q_OBJECT

public:
    explicit ConnectionMonitor(DatabaseManager &database, WebCommunication &web);
    void start();
    void stop();
    void setManualBusy(bool busy);

signals:
    void databaseChecked(DatabaseManager::Status status, const UiMessage &details);
    void serverChecked(bool ok);
    void serverUnavailable(const UiMessage &details);
    void probeBusyChanged(bool busy);

private:
    void check();
    DatabaseManager::Status checkDatabase(UiMessage &details);

    DatabaseManager &database;
    WebCommunication &web;
    QTimer timer;
    QUrl lastServer;
    bool manualBusy = false;
    bool probeBusy = false;
};
