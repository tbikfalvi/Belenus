//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.4.0.0"
#define DATABASE_VERSION_NUMBER     "2.4.0.0"

#include "applicationsettings.h"
#include "connectionmonitor.h"
#include "database.h"
#include "filemanager.h"
#include "languagemanager.h"
#include "licencemanager.h"
#include "logger.h"
#include "mainwindow.h"
#include "traycontroller.h"
#include "webcommunication.h"

#include <QApplication>
#include <QTextStream>

// A központi komponenseket a főablak előtt hozza létre, így azok tovább élnek az ablaknál.
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion(QStringLiteral(APPLICATION_VERSION_NUMBER));

    Logger logger;
    logger.open();
    logger.write("INFO", QStringLiteral("WebApp started"));

    LanguageManager language(logger);
    language.install();

    DatabaseManager database(logger);
    if (app.arguments().contains(QStringLiteral("--check-qmysql"))) {
        QTextStream(stdout) << database.driverMessage().text() << Qt::endl;
        return database.driverAvailable() ? 0 : 1;
    }

    FileManager files;
    WebCommunication web(logger);
    LicenceManager licences(database, web, files, logger);
    ApplicationSettings settings(database, logger);
    const bool showAtStartup = settings.loadShowWindow();
    ConnectionMonitor monitor(database, web);
    MainWindow window(licences, logger, language, settings, monitor, showAtStartup);
    TrayController tray(window, monitor);
    window.setTrayAvailable(tray.available());
    QApplication::setQuitOnLastWindowClosed(false);

    QObject::connect(&licences, &LicenceManager::networkBusyChanged, &monitor, &ConnectionMonitor::setManualBusy);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [&] {
        monitor.stop();
        web.shutdown();
        tray.stop();
        database.close();
    });
    if (showAtStartup || !tray.available())
        window.show();
    monitor.start();
    return app.exec();
}
