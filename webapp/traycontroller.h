//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QCoreApplication>
#include <QMenu>
#include <QSystemTrayIcon>

#include <optional>

class ConnectionMonitor;
class MainWindow;
class QAction;

// A WebSync felhőikonját és a két kapcsolat jelzőjét jeleníti meg az értesítési területen.
class TrayController {
    Q_DECLARE_TR_FUNCTIONS(TrayController)

public:
    TrayController(MainWindow &window, ConnectionMonitor &monitor);
    bool available() const;
    void stop();

private:
    void updatePresentation();
    static QString statusText(std::optional<bool> status);
    static QIcon statusIcon(std::optional<bool> database, std::optional<bool> server);

    QMenu menu;
    QSystemTrayIcon icon;
    QAction *openAction;
    QAction *databaseAction;
    QAction *serverAction;
    QAction *exitAction;
    std::optional<bool> databaseStatus;
    std::optional<bool> serverStatus;
    bool trayAvailable;
};
