
#include "../framework/qtlogger.h"
#include "BelenusUpdater.h"
#include "mainwindow.h"

QTranslator     *poTransTool;
QTranslator     *poTransQT;
QApplication    *apMainApp;
QString          g_qsCurrentPath;
cQTLogger        g_obLogger;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    g_qsCurrentPath = QDir::currentPath();
    g_qsCurrentPath.replace( '/', '\\' );

    poTransTool = new QTranslator();
    poTransQT = new QTranslator();

    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsLang = obPrefFile.value( "Lang", "hu" ).toString();

    poTransTool->load( QString("%1\\lang\\BelenusUpdater_%2.qm").arg(g_qsCurrentPath).arg(qsLang) );
    poTransQT->load( QString("%1\\lang\\qt_%2.qm").arg(g_qsCurrentPath).arg(qsLang) );

    apMainApp->installTranslator( poTransTool );
    apMainApp->installTranslator( poTransQT );

    apMainApp->setWindowIcon( QIcon(":/BelenusUpdater.ico") );

    MainWindow w;
    w.show();

    return apMainApp->exec();
}
