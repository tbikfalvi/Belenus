
#include <QApplication>
#include <QProcessEnvironment>
#include <QtGui>
#include <QTranslator>

#include "dlgmain.h"

QTranslator     *poTransApp;
QTranslator     *poTransQT;
QApplication    *apMainApp;

#define APPLICATION_VERSION_NUMBER  "2.0.0.0"
#define DATABASE_VERSION_NUMBER     "2.0.0.0"

#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"
#include "../language/language.h"
#include "http.h"

#include <windows.h>

cQTLogger                g_obLogger;
FileWriter               g_obLogFileWriter("websync_%1.log");
cBlnsHttp               *g_poBlnsHttp;
cQTMySQLConnection      *g_poDB;
cLanguage                g_obLanguage;

using namespace std;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(websync);

    apMainApp = new QApplication(argc, argv);

    QProcessEnvironment qpeInfo = QProcessEnvironment::systemEnvironment();

    QString qsBelenusTarget = QString( qpeInfo.value( "BelenusTarget", "C:/KiwiSun/Belenus" ) );
    QDir::setCurrent( qsBelenusTarget );

    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);

    g_obLogger(cSeverity::INFO) << "Belenus WebSync started." << EOM;
    g_obLogger(cSeverity::DEBUG) << "Current directory is: "
                                 << QDir::currentPath()
                                 << EOM;

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    g_obLanguage.getLanguages();
    g_obLanguage.init( apMainApp, "websync", "_" );

    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)obPref.value( "LogLevel", cSeverity::DEBUG ).toUInt() );

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical( 0, QObject::tr("websync"),
                               QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    QFile   fileCheck( "websync.chk" );

    fileCheck.open( QIODevice::WriteOnly );
    fileCheck.write( "CURRENTLY NOT RUNNING" );
    fileCheck.close();

    Sleep(3000);

    if( fileCheck.size() > 0 )
    {
        g_obLogger(cSeverity::DEBUG) << "Previous instance not running. Starting application main processes." << EOM;
    }
    else
    {
        g_obLogger(cSeverity::DEBUG) << "Previous instance is running. Closing this instance." << EOM;
        return 0;
    }

    dlgMain obDlgMain( 0, QString( APPLICATION_VERSION_NUMBER ), QString( DATABASE_VERSION_NUMBER ) );

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
