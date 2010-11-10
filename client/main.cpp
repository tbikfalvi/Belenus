//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : main.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas, Kovacs Gabor
//
//====================================================================================
// Alkalmazas fo allomanya.
//====================================================================================

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
#include "../framework/logger/FileWriter.h"
#include "../framework/qtmysqlconnection.h"
#include "db/dbuser.h"
#include "db/dbpatient.h"
#include "db/dbmirror.h"
#include "preferences.h"
#include "communication.h"
#include "communication_demo.h"
#include "bs_connection.h"
#include "cassa.h"
#include "licenceManager.h"
#include "general.h"
#ifdef __WIN32__
    #include "communication_serial.h"
#endif
#include "wndmain.h"

//====================================================================================

cQTLogger                g_obLogger;
DatabaseWriter           g_obLogDBWriter;
GUIWriter                g_obLogGUIWriter;
ConsoleWriter            g_obLogConsoleWriter;
FileWriter               g_obLogFileWriter("client_%1_%2.log");
cQTMySQLConnection      *g_poDB;
cDBUser                  g_obUser;
cPreferences            *g_poPrefs;
CS_Communication        *g_poHardware;
BelenusServerConnection *g_poServer;
cDBPatient               g_obPatient;
unsigned int             g_uiPatientAttendanceId;
cCassa                   g_obCassa;
LicenceManager           g_obLicenceManager;
cDBMirror                g_obDBMirror;
cGeneral                 g_obGen;

//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication     apMainApp( argc, argv );

    g_obLogger.attachWriter("gui", &g_obLogGUIWriter);
    g_obLogger.attachWriter("db", &g_obLogDBWriter);
    g_obLogger.attachWriter("console", &g_obLogConsoleWriter);
    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("console", cSeverity::DEBUG);
    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);
    g_poDB     = new cQTMySQLConnection;

    g_poPrefs  = new cPreferences( QString::fromAscii( "./belenus.ini" ) );
    g_poPrefs->setVersion( "1.0.2" );
    g_poPrefs->setLangFilePrefix( "belenus_" );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

    if( g_poPrefs->getLang() != "uk" )
    {
        QTranslator *poTrans = new QTranslator();
        QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
        poTrans->load( qsTransFile );
        apMainApp.installTranslator( poTrans );
    }

    QPixmap          obPixmap("resources/splash.png");
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

        qsSpalsh += QObject::tr(" SUCCEEDED.\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " started." << EOM;

        qsSpalsh += QObject::tr("Connecting to Belenus server ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poServer = new BelenusServerConnection();
        g_poServer->moveToThread(g_poServer);
        g_poServer->start();

        g_obLicenceManager.initialize();

        if( g_obLicenceManager.getType() != LicenceManager::DEMO )
        {  // start server connection only if licence key is provided. Do not try to validate demo licence

            g_obLicenceManager.validateLicence();     // begins connection

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
        }

        qsSpalsh += "  ";
        if ( g_poServer->isConnected() )
        {
            qsSpalsh += QObject::tr("SUCCEEDED");
        }
        else
        {
            qsSpalsh += QObject::tr("FAILED");
        }
        qsSpalsh += "\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("License is ... ");
        if ( g_obLicenceManager.isDemo() )
            qsSpalsh += QObject::tr("DEMO");
        else
            qsSpalsh += QObject::tr("OK");

        if ( g_obLicenceManager.getType()==LicenceManager::VALID_SERVER_ERROR ||
             g_obLicenceManager.getType()==LicenceManager::VALID_CODE_2_ERROR )
        {
            qsSpalsh += QObject::tr(" (needs server validation in %1 days)").arg(g_obLicenceManager.getDaysRemaining());
        } else
        if ( g_obLicenceManager.getType()==LicenceManager::VALID_EXPIRED ||
             g_obLicenceManager.getType()==LicenceManager::VALID_CODE_2_EXPIRED )
        {
            qsSpalsh += QObject::tr(" (licence validation limit expired)");
        } else if ( g_obLicenceManager.getType()==LicenceManager::NOT_VALID ) {
            qsSpalsh += QObject::tr(" (licence not accepted by server)");
        }
        qsSpalsh += "\n";
        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("Initialize database synchronization ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_obDBMirror.initialize(); // enough to call once at the begining
        if( g_obDBMirror.start() )
        {
            qsSpalsh += QObject::tr("SUCCEEDED\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            if( g_obDBMirror.checkIsSynchronizationNeeded() )
            {
                qsSpalsh += QObject::tr("Local database has to synchronized with server.\n");
            }
            else
            {
                qsSpalsh += QObject::tr("Local database synchronized with server.\n");
            }
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            /*qsSpalsh += "-----------------------------------------------------\n";
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            qsSpalsh += QObject::tr("Checking studio independent data on server ...\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
            if( g_obDBMirror.checkIsGlobalDataDownloadInProgress() )
            {
                qsSpalsh += QObject::tr("There are new studio independent data on server.\n");
            }
            else
            {
                qsSpalsh += QObject::tr("Studio independent data match with server.\n");
            }
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));*/
        }
        else
        {
            qsSpalsh += QObject::tr("FAILED\n");
        }

        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

#ifdef __WIN32__

        qsSpalsh += QObject::tr("Checking hardware connection ...");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poHardware = new CS_Communication_Serial();
        g_poHardware->init( g_poPrefs->getCommunicationPort() );
        if( !g_poHardware->isHardwareConnected() || g_obLicenceManager.isDemo() )
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
        Sleep(5000);
#else
        sleep( 5 );
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
