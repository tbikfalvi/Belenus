//====================================================================================
//
// Belenus Install alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.3.2.0"

//====================================================================================

#include <QApplication>
#include <QTranslator>
#include <QDir>

//====================================================================================

#include "mainwindow.h"

//====================================================================================

QTranslator     *poTransStartup;
QTranslator     *poTransQT;
QApplication    *apMainApp;
QString         g_qsCurrentPath;

//====================================================================================
int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    g_qsCurrentPath = QDir::currentPath();

    poTransStartup = new QTranslator();
    poTransQT = new QTranslator();

    poTransStartup->load( QString("%1\\lang\\startup_en.qm").arg( g_qsCurrentPath ) );
    poTransQT->load( QString("%1\\lang\\qt_en.qm").arg( g_qsCurrentPath ) );

    apMainApp->installTranslator( poTransStartup );
    apMainApp->installTranslator( poTransQT );

    MainWindow w;
    w.show();

    return apMainApp->exec();
}
