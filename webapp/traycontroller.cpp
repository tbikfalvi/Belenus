//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "traycontroller.h"

#include "connectionmonitor.h"
#include "mainwindow.h"

#include <QAction>
#include <QIcon>
#include <QPainter>
#include <QPixmap>

TrayController::TrayController(MainWindow &window, ConnectionMonitor &monitor)
    : openAction(menu.addAction(QString())),
      databaseAction(menu.addAction(QString())),
      serverAction(menu.addAction(QString())),
      exitAction(nullptr),
      trayAvailable(QSystemTrayIcon::isSystemTrayAvailable())
{
    menu.insertSeparator(databaseAction);
    menu.addSeparator();
    exitAction = menu.addAction(QIcon(QStringLiteral(":/icons/exit.png")), QString());
    databaseAction->setEnabled(false);
    serverAction->setEnabled(false);
    icon.setContextMenu(&menu);

    QObject::connect(openAction, &QAction::triggered, &window, &MainWindow::bringToFront);
    QObject::connect(exitAction, &QAction::triggered, &window, &MainWindow::quitApplication);
    QObject::connect(&icon, &QSystemTrayIcon::activated, &window, [&window](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick)
            window.bringToFront();
    });
    QObject::connect(&window, &MainWindow::languageChanged, &icon, [this] { updatePresentation(); });
    QObject::connect(&monitor, &ConnectionMonitor::databaseChecked, &icon, [this](DatabaseManager::Status status) {
        databaseStatus = status == DatabaseManager::Status::Connected;
        updatePresentation();
    });
    QObject::connect(&monitor, &ConnectionMonitor::serverChecked, &icon, [this](bool ok) {
        serverStatus = ok;
        updatePresentation();
    });
    QObject::connect(&monitor, &ConnectionMonitor::serverUnavailable, &icon, [this] {
        serverStatus.reset();
        updatePresentation();
    });
    updatePresentation();
    if (trayAvailable)
        icon.show();
}

bool TrayController::available() const
{
    return trayAvailable;
}

void TrayController::stop()
{
    icon.hide();
    icon.setContextMenu(nullptr);
}

QString TrayController::statusText(std::optional<bool> status)
{
    if (!status)
        return tr("Not checked");
    return *status ? tr("Connected") : tr("Connection failed");
}

// A felhő összesített színe mellett balra az adatbázis, jobbra a szerver állapota látható.
QIcon TrayController::statusIcon(std::optional<bool> database, std::optional<bool> server)
{
    const auto cloud = database == false || server == false ? QStringLiteral("websync_red")
                     : database == true && server == true ? QStringLiteral("websync")
                                                          : QStringLiteral("websync_yellow");
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(QRect(0, 0, 32, 32), QPixmap(QStringLiteral(":/icons/%1.png").arg(cloud)));
    const auto badge = [&](int x, std::optional<bool> status) {
        const auto color = !status ? QStringLiteral("yellow") : *status ? QStringLiteral("green") : QStringLiteral("red");
        painter.drawPixmap(QRect(x, 19, 13, 13), QPixmap(QStringLiteral(":/icons/status_%1.png").arg(color)));
    };
    badge(1, database);
    badge(18, server);
    painter.end();
    return QIcon(pixmap);
}

void TrayController::updatePresentation()
{
    const auto databaseText = tr("Database: %1").arg(statusText(databaseStatus));
    const auto serverText = tr("HTTPS server: %1").arg(statusText(serverStatus));
    openAction->setText(tr("Open Belenus WebApp"));
    exitAction->setText(tr("Exit"));
    databaseAction->setText(databaseText);
    serverAction->setText(serverText);
    icon.setIcon(statusIcon(databaseStatus, serverStatus));
    icon.setToolTip(QStringLiteral("Belenus WebApp\n%1\n%2").arg(databaseText, serverText));
}
