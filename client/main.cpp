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

#define APPLICATION_VERSION_NUMBER  "1.4.5"

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
#include "db/dbguest.h"
#include "preferences.h"
#include "communication.h"
#include "communication_demo.h"
#include "bs_connection.h"
#include "cassa.h"
#include "licenceManager.h"
#include "general.h"
#include "gibbig.h"
#ifdef __WIN32__
    #include "communication_serial.h"
#endif
#include "wndmain.h"

//====================================================================================

QApplication            *apMainApp;
cQTLogger                g_obLogger;
DatabaseWriter           g_obLogDBWriter;
GUIWriter                g_obLogGUIWriter;
ConsoleWriter            g_obLogConsoleWriter;
//FileWriter               g_obLogFileWriter("client_%1_%2.log");
FileWriter               g_obLogFileWriter("belenus_%1.log");
cQTMySQLConnection      *g_poDB;
cDBUser                  g_obUser;
cPreferences            *g_poPrefs;
CS_Communication        *g_poHardware;
BelenusServerConnection *g_poServer;
cCassa                   g_obCassa;
cGeneral                 g_obGen;
cDBGuest                 g_obGuest;
cLicenceManager          g_obLicenceManager;
cGibbig                 *g_poGibbig;

// 'TO BE SOLVED' felirat, ahol még valamit meg kell oldani
// g_obLogger(cSeverity::DEBUG) << QString("") << EOM;
// cTracer obTrace( "" );

//====================================================================================
int main( int argc, char *argv[] )
{
    apMainApp = new QApplication(argc, argv);

    g_obGen.setApplication( apMainApp );

    apMainApp->setWindowIcon( QIcon(":/icons/Belenus.ico") );

    g_obLogger.attachWriter("gui", &g_obLogGUIWriter);
    g_obLogger.attachWriter("db", &g_obLogDBWriter);
//    g_obLogger.attachWriter("console", &g_obLogConsoleWriter);
    g_obLogger.attachWriter("file", &g_obLogFileWriter);

    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);

    g_poDB     = new cQTMySQLConnection;

    g_poPrefs  = new cPreferences( QString::fromAscii( "./belenus.ini" ) );
    g_poPrefs->setVersion( APPLICATION_VERSION_NUMBER );
    g_poPrefs->setLangFilePrefix( "belenus_" );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

    g_obGen.setApplicationLanguage( g_poPrefs->getLang() );

    QPixmap          obPixmap("resources/splash.png");
    QSplashScreen    obSplash( obPixmap );
    QString          qsSpalsh;

    QFont   obFont = obSplash.font();

    obFont.setBold(true);
    obSplash.setFont(obFont);
    obSplash.show();
    apMainApp->processEvents();

    QString qsSystemID = QObject::tr( "SystemID: " );

    if( g_poPrefs->isComponentSensoliteInstalled() ) qsSystemID.append( "S" );
    if( g_poPrefs->isComponentKiwiSunInstalled() )   qsSystemID.append( "K" );
    if( g_poPrefs->isComponentDatabaseInstalled() )  qsSystemID.append( "D" );
    if( g_poPrefs->isComponentHardwareInstalled() )  qsSystemID.append( "H" );
    if( g_poPrefs->isComponentInternetInstalled() )  qsSystemID.append( "I" );
    if( g_poPrefs->isComponentClientInstalled() )    qsSystemID.append( "C" );
    if( g_poPrefs->isComponentViewerInstalled() )    qsSystemID.append( "V" );
    qsSystemID.append( "\n" );

    qsSpalsh += qsSystemID;
    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

    int r = 1;
    try
    {
        g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " started." << EOM;

        qsSpalsh += QObject::tr("Connecting to database ...");
        g_obLogger(cSeverity::INFO) << "Connecting to database ..." << EOM;
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poDB->open();
        g_obLogDBWriter.setDBConnection(g_poDB);
        g_poPrefs->loadDBSettings();

        g_obLogger(cSeverity::INFO) << "SUCCEEDED" << EOM;
        qsSpalsh += QObject::tr(" SUCCEEDED.\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += "\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("Another instance of application ... ");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        QFile   fileCheck( "belenus.chk" );

        fileCheck.open( QIODevice::WriteOnly );
        fileCheck.write( "CURRENTLY NOT RUNNING" );
        fileCheck.close();
#ifdef __WIN32__
            Sleep(3000);
#else
            sleep( 3 );
#endif
        if( fileCheck.size() > 0 )
        {
            qsSpalsh += QObject::tr(" NOT RUNNING.\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
        }
        else
        {
            qsSpalsh += QObject::tr(" ALREADY RUNNING.\n");
            qsSpalsh += QObject::tr("\n\nPlease use the currently running application.\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
#ifdef __WIN32__
            Sleep(5000);
#else
            sleep( 3 );
#endif
            return 0;
        }

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences" ) );

        qsSpalsh += "\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("License is ... ");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        int         nId = 0;
        QString     qsSerial = QObject::tr("NO_SERIAL_DETECTED");

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences" ) );
        if( poQuery->last() )
        {
            nId = poQuery->value( 0 ).toInt();
            qsSerial = poQuery->value( 1 ).toString();
        }
        if( poQuery ) delete poQuery;

        g_obLogger(cSeverity::INFO) << "Licence is: " << qsSerial << EOM;
        g_poPrefs->setLicenceId( nId );

        if( nId < 2 )
            qsSpalsh += QObject::tr("DEMO");
        else
            qsSpalsh += QObject::tr("OK");
        qsSpalsh += "\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("Serial: %1\n").arg(qsSerial);
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        g_obLicenceManager.initialize();

        int     nDaysRemain = g_obLicenceManager.daysRemain();

        qsSpalsh += QObject::tr("Days remains: %1\n").arg( nDaysRemain );
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        if( nDaysRemain < cLicenceManager::EXPIRE_MAX_DAYS || g_obLicenceManager.ltLicenceType() == cLicenceManager::LTYPE_REGISTERED )
        {
            qsSpalsh += QObject::tr("\nVALIDATE YOUR APPLICATION\nWITH YOUR FRANCHISE PROVIDER\n\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

            if( nDaysRemain < 1 )
            {
                qsSpalsh += QObject::tr("The application validity has been expired.\n"
                                        "The application can be used only in DEMO mode.\n\n");
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));
            }
#ifdef __WIN32__
            Sleep(3000);
#else
            sleep( 3 );
#endif
        }

        //-------------------------------------------------------------------------------
        // End of process connection initialization
        //-------------------------------------------------------------------------------

        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

#ifdef __WIN32__

        //-------------------------------------------------------------------------------
        // If Hardware component active, process hardware initialization
        g_obLogger(cSeverity::DEBUG) << QString("HW check nID: %1 HWInstalled: %2").arg(nId).arg(g_poPrefs->isComponentHardwareInstalled()) << EOM;
        if( nId >= 2 /*&& g_poPrefs->isComponentHardwareInstalled()*/ && nDaysRemain > 0 )
        {
            qsSpalsh += QObject::tr("Checking hardware connection ...");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            g_poHardware = new CS_Communication_Serial();
            g_obLogger(cSeverity::DEBUG) << QString("COM: %1").arg(g_poPrefs->getCommunicationPort()) << EOM;
            g_poHardware->init( g_poPrefs->getCommunicationPort() );
            g_obLogger(cSeverity::DEBUG) << QString("Hardware connected: %1").arg(g_poHardware->isHardwareConnected()) << EOM;
            if( !g_poHardware->isHardwareConnected() /*|| g_obLicenceManager.isDemo()*/ )
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
        }
        else
        {
            qsSpalsh += QObject::tr("Starting application in DEMO mode.\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            g_poHardware = new CS_Communication_Demo();
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
        Sleep(2000);
#else
        sleep( 2 );
#endif

        obMainWindow.setCommunicationEnabled( g_poHardware->isHardwareConnected() );

        obSplash.finish( &obMainWindow );

        obMainWindow.showProgress();

        obMainWindow.show();
        obMainWindow.initPanels();

        if( obMainWindow.showLogIn() )
        {
            obMainWindow.startMainTimer();
            r = apMainApp->exec();
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
        qsSpalsh += QObject::tr(" FAILED\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        QString qsError = QString( e.what() );

        if( qsError.contains( "Can't connect to MySQL server on 'localhost' (10061)" ) )
        {
            qsError = QObject::tr( "Database server application is not running.\n"
                                   "Belenus application can not be started without active database server.\n"
                                   "Please start WampServer application then restart Belenus application." );
        }
        g_obLogger(e.severity()) << qsError << EOM;
    }

//    g_poServer->quit();

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " ended." << EOM;

    delete g_poPrefs;
    delete g_poDB;

    return r;
}
