//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "connectionmonitor.h"

#include "applicationsettings.h"
#include "webcommunication.h"

#include <QVariant>

namespace {
constexpr int checkIntervalMs = 10000;
}

ConnectionMonitor::ConnectionMonitor(DatabaseManager &database, WebCommunication &web)
    : database(database), web(web)
{
    timer.setInterval(checkIntervalMs);
    connect(&timer, &QTimer::timeout, this, &ConnectionMonitor::check);
}

void ConnectionMonitor::start()
{
    timer.start();
    QTimer::singleShot(0, this, &ConnectionMonitor::check);
}

void ConnectionMonitor::stop()
{
    timer.stop();
}

void ConnectionMonitor::setManualBusy(bool busy)
{
    manualBusy = busy;
}

// A nyitott kapcsolat mellé tényleges SQL-választ is megkövetel.
DatabaseManager::Status ConnectionMonitor::checkDatabase(UiMessage &details)
{
    const auto status = database.open();
    if (status == DatabaseManager::Status::OpenFailed) {
        details = database.connectionError();
        return status;
    }
    if (status != DatabaseManager::Status::Connected)
        return status;

    auto query = database.executeSelect(QStringLiteral("SELECT 1"));
    if (query.isActive() && query.next() && query.value(0).toInt() == 1)
        return DatabaseManager::Status::Connected;
    details = UiMessage([] { return tr("The database connection check failed."); });
    return DatabaseManager::Status::OpenFailed;
}

// A MySQL-próba minden ciklusban lefut. A tokenkérés megvárja az előző HTTPS-művelet végét.
void ConnectionMonitor::check()
{
    if (!timer.isActive())
        return;

    UiMessage databaseDetails;
    const auto status = checkDatabase(databaseDetails);
    UiMessage serverDetails;
    if (status == DatabaseManager::Status::Connected) {
        QUrl address;
        serverDetails = ApplicationSettings::readServerAddress(database, address);
        lastServer = serverDetails.isEmpty() ? address : QUrl();
    }
    database.close();
    emit databaseChecked(status, databaseDetails);
    if (manualBusy || probeBusy)
        return;

    if (lastServer.isEmpty()) {
        // Ha már van ismert cím, adatbázishiba mellett is folytatható a valódi HTTPS-próba.
        emit serverUnavailable(serverDetails);
        return;
    }
    probeBusy = true;
    emit probeBusyChanged(true);
    web.requestToken(lastServer, this, [this](const WebCommunication::TokenResult &result) {
        probeBusy = false;
        if (!timer.isActive())
            return;
        emit serverChecked(result.ok);
        emit probeBusyChanged(false);
    });
}
