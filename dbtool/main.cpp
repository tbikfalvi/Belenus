//====================================================================================
//
// Belenus Database Manager alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.3.0.0"

//====================================================================================

#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

//====================================================================================

#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"

//====================================================================================

QTranslator         *poTransTool;
QTranslator         *poTransQT;
QApplication        *apMainApp;
cQTLogger            g_obLogger;
FileWriter           g_obLogFileWriter("dbtool_%1.log");
QString              g_qsCurrentPath;
cQTMySQLConnection  *g_poDB;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    g_qsCurrentPath = QDir::currentPath();

    poTransTool = new QTranslator();
    poTransQT = new QTranslator();

    poTransTool->load( QString("%1\\lang\\dbtool_hu.qm").arg(QDir::currentPath()) );
    poTransQT->load( QString("%1\\lang\\qt_hu.qm").arg(QDir::currentPath()) );

    apMainApp->installTranslator( poTransTool );
    apMainApp->installTranslator( poTransQT );

    apMainApp->setWindowIcon( QIcon(":/icons/DBTool.ico") );

    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("file", cSeverity::INFO);

    g_obLogger(cSeverity::INFO) << "Belenus Database Manager started." << EOM;

    MainWindow *wndMain = new MainWindow( 0, APPLICATION_VERSION_NUMBER );
    wndMain->show();

    return apMainApp->exec();
}


