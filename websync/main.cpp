#include <QApplication>
#include <QtGui>
#include <QTranslator>

#include "dlgmain.h"

QTranslator     *poTransApp;
QTranslator     *poTransQT;
QApplication    *apMainApp;

#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"

cQTLogger            g_obLogger;
FileWriter           g_obLogFileWriter("websync_%1.log");

cQTMySQLConnection  *g_poDB;

using namespace std;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(websync);

    apMainApp = new QApplication(argc, argv);

    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);

    g_obLogger(cSeverity::INFO) << "Belenus WebSync started." << EOM;

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );
    QString     qsLang = obPref.value( "Lang", "en" ).toString();

    poTransApp = new QTranslator();
    poTransQT = new QTranslator();

    poTransApp->load( QString("%1\\websync_%2.qm").arg( QDir::currentPath() ).arg(qsLang) );
    poTransQT->load( QString("%1\\qt_%2.qm").arg( QDir::currentPath() ).arg(qsLang) );

    apMainApp->installTranslator( poTransApp );
    apMainApp->installTranslator( poTransQT );

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical( 0, QObject::tr("websync"),
                               QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    dlgMain obDlgMain;

    if( argc > 1 )
    {
        for( int i=1; i<argc; i++ )
        {
            QStringList qslParam = QString( argv[i] ).split(':');

//            if( qslParam.at(0).compare( "-t" ) == 0 && atoi( qslParam.at(1).toStdString().c_str() ) > 0 )
//            {
//                obDlgMain.setTimerLength( atoi( qslParam.at(1).toStdString().c_str() ) );
//            }
        }
    }

    return apMainApp->exec();
}
