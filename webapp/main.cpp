//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.4.0.0"
#define DATABASE_VERSION_NUMBER     "2.4.0.0"

#include "database.h"
#include "filemanager.h"
#include "licencemanager.h"
#include "logger.h"
#include "mainwindow.h"
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

    DatabaseManager database(logger);
    if (app.arguments().contains(QStringLiteral("--check-qmysql"))) {
        QTextStream(stdout) << database.driverMessage() << Qt::endl;
        return database.driverAvailable() ? 0 : 1;
    }

    FileManager files;
    WebCommunication web(logger);
    LicenceManager licences(database, web, files, logger);
    MainWindow window(licences, logger);
    window.show();
    return app.exec();
}
