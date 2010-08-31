#include <QApplication>
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QTranslator>
#include <QSplashScreen>

#include "../framework/qtlogger.h"
#include "../framework/logger/GUIWriter.h"
#include "../framework/logger/DatabaseWriter.h"
#include "../framework/logger/ConsoleWriter.h"
#include "../framework/qtmysqlconnection.h"
#include "db/dbuser.h"
#include "preferences.h"
#include "communication.h"
#include "communication_demo.h"
#include "bs_connection.h"
#include "db/dbpatient.h"
#include "cassa.h"
#ifdef __WIN32__
    #include "communication_serial.h"
#endif
#include "wndmain.h"

cQTLogger                g_obLogger;
DatabaseWriter           g_obLogDBWriter;
GUIWriter                g_obLogGUIWriter;
ConsoleWriter            g_obLogConsoleWriter;
cQTMySQLConnection      *g_poDB;
cDBUser                  g_obUser;
cPreferences            *g_poPrefs;
CS_Communication        *g_poHardware;
BelenusServerConnection *g_poServer;
cDBPatient               g_obPatient;
unsigned int             g_uiPatientAttendanceId;
cCassa                   g_obCassa;

int main( int argc, char *argv[] )
{
    QApplication     apMainApp( argc, argv );

    g_obLogger.attachWriter("gui", &g_obLogGUIWriter);
    g_obLogger.attachWriter("db", &g_obLogDBWriter);
    g_obLogger.attachWriter("console", &g_obLogConsoleWriter);
    g_obLogger.setMinimumSeverity("console", cSeverity::DEBUG);
    g_poDB     = new cQTMySQLConnection;

    g_poPrefs  = new cPreferences( QString::fromAscii( "./belenus.ini" ) );
    g_poPrefs->setVersion( "1.0.0" );
    g_poPrefs->setLangFilePrefix( "belenus_" );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

    if( g_poPrefs->getLang() != "uk" )
    {
        QTranslator *poTrans = new QTranslator();
        QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
        poTrans->load( qsTransFile );
        apMainApp.installTranslator( poTrans );
    }

    QPixmap          obPixmap("resources/splash.jpg");
    QSplashScreen    obSplash( obPixmap );
    QString          qsSpalsh;

    QFont   obFont = obSplash.font();

    obFont.setBold(true);
    obSplash.setFont(obFont);
    obSplash.show();
    apMainApp.processEvents();

    int r = 1;
    try
    {
        qsSpalsh += QObject::tr("Connecting to database ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poDB->open();
        g_obLogDBWriter.setDBConnection(g_poDB);
        g_poPrefs->loadDBSettings();

        qsSpalsh += QObject::tr(" CONNECTED.\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " started." << EOM;

        qsSpalsh += QObject::tr("Connecting to Belenus server ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poServer = new BelenusServerConnection();
        g_poServer->moveToThread(g_poServer);
        g_poServer->start();
        g_poServer->setLoginKeys(g_poPrefs->getClientSerial(), "yipiee-code2");
        g_poServer->connectTo( QHostAddress(g_poPrefs->getServerAddress()), g_poPrefs->getServerPort().toInt() );

        int nCount = 0;
        while( g_poServer->getStatus()==BelenusServerConnection::NOT_CONNECTED || g_poServer->getStatus()==BelenusServerConnection::CONNECTING )
        {
            QString qsTemp;
            qsTemp.fill('.', nCount%5+1);
            obSplash.showMessage(qsSpalsh+qsTemp, Qt::AlignLeft, QColor(59,44, 75));
            if( ++nCount > 20 ) // timeout handling: 20*500 = 10 sec
                break;
            QMutex dummy;
            dummy.lock();
            QWaitCondition waitCondition;
            waitCondition.wait(&dummy, 500);
        }

        if ( g_poServer->getStatus()==BelenusServerConnection::AUTHENTICATED ) {
            qsSpalsh += QObject::tr("Connected. License ok.") + "\n";
        } else if ( g_poServer->getStatus()==BelenusServerConnection::CONNECTING ) {
            qsSpalsh += QObject::tr("Still connecting. Unable to check license.") + "\n";
        } else if ( g_poServer->getStatus()==BelenusServerConnection::CONNECTION_FAILED ) {
            QString s;
            switch (g_poServer->getLastResult()) {
                case Result::INVALID_VERSION: s = QObject::tr("Server has different version. Unable to connect."); break;
                case Result::INVALID_LICENSE_KEY: s = QObject::tr("License key authentication failed"); break;
                case Result::INVALID_SECOND_ID: s = QObject::tr("Server authentication failed. Call service."); break;
                default: s = QObject::tr("Unable to connect to server"); break;
            }
            qsSpalsh += s + "\n";
        } else { /* NOT_CONNECTED state should not happen as a connectTo() automatically transfers to CONNECTING state */
            qsSpalsh += QObject::tr("Unknown connection error") + "\n";
            g_obLogger(cSeverity::INFO) << "[main::splash] unknown status when connecting to server" << g_poServer->getStatus() << EOM;
        }
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));



#ifdef __WIN32__

        qsSpalsh += QObject::tr("Checking hardware connection ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poHardware = new CS_Communication_Serial();
        g_poHardware->init( g_poPrefs->getCommunicationPort() );
        if( !g_poHardware->isHardwareConnected() || !g_poServer->isLicenseValid() )
        {
            qsSpalsh += QObject::tr("FAILED\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
            qsSpalsh += QObject::tr("Starting application in DEMO mode.\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            delete g_poHardware;
            g_poHardware = new CS_Communication_Demo();
        }
        else
        {
            qsSpalsh += QObject::tr("CONNECTED\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            qsSpalsh += QObject::tr("Initializing hardware device ... ");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
            g_poHardware->setApplicationModuleCount( g_poPrefs->getPanelCount() );

            qsSpalsh += QObject::tr("FINISHED\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            qsSpalsh += QObject::tr("Caption stored in hardware: %1\n").arg( QString::fromStdString(g_poHardware->getCustomCaption()) );
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            qsSpalsh += QObject::tr("Number of hardware panels: %1\n").arg( g_poHardware->getHardwareModuleCount() );
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            qsSpalsh += QObject::tr("Checking hardware panels:\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            for( int i=0; i<g_poHardware->getPanelCount(); i++ )
            {
                qsSpalsh += QObject::tr("     Checking hardware panel -%1- ").arg(i+1);
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                if( g_poHardware->checkHardwarePanel( i ) )
                {
                    qsSpalsh += QObject::tr(" SUCCEEDED\n");
                    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
                }
                else
                {
                    qsSpalsh += QObject::tr(" FAILED\n");
                    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
                }
            }
        }
#else

        qsSpalsh += QObject::tr("Starting application in DEMO mode.\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poHardware = new CS_Communication_Demo();

#endif

        cWndMain  obMainWindow;

        obMainWindow.move( g_poPrefs->getMainWindowLeft(), g_poPrefs->getMainWindowTop() );
        obMainWindow.resize( g_poPrefs->getMainWindowWidth(), g_poPrefs->getMainWindowHeight() );

#ifdef __WIN32__
        Sleep(3000);
#else
        sleep( 3 );
#endif

        obSplash.finish( &obMainWindow );

        obMainWindow.show();

        if( obMainWindow.showLogIn() )
        {
            obMainWindow.initPanels();
            obMainWindow.startMainTimer();
            r = apMainApp.exec();
        }
        else
        {
            obMainWindow.close();
            r = 0;
        }

        g_poPrefs->setMainWindowSizePos( obMainWindow.x(),
                                         obMainWindow.y(),
                                         obMainWindow.width(),
                                         obMainWindow.height(),
                                         true );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

//    g_poServer->quit();

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " ended." << EOM;

    delete g_poPrefs;
    delete g_poDB;

    return r;
}
