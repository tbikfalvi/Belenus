#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"

#define APPLICATION_VERSION_NUMBER  "2.2.2.0"

QTranslator         *poTransTool;
QTranslator         *poTransQT;
QApplication        *apMainApp;
cQTLogger            g_obLogger;
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

    MainWindow *wndMain = new MainWindow( 0, APPLICATION_VERSION_NUMBER );
    wndMain->show();

    return apMainApp->exec();
}


