//====================================================================================
//
// Belenus Kliens alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#define APPLICATION_VERSION_NUMBER  "2.3.1.0"
#define DATABASE_VERSION_NUMBER     "2.3.1.0"

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
#include "../language/language.h"
#include "db/dbuser.h"
#include "db/dbguest.h"
#include "preferences.h"
#include "communication.h"
#include "communication_demo.h"
#include "bs_connection.h"
#include "cassa.h"
#include "licenceManager.h"
#include "general.h"
#include "http.h"
#ifdef __WIN32__
    #include "communication_serial.h"
#endif
#include "communication_rfid.h"
#include "wndmain.h"

//====================================================================================

QApplication            *apMainApp;
cQTLogger                g_obLogger;
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
cLanguage                g_obLanguage;
cCommRFID               *g_poCommRFID;

// 'TO BE SOLVED' felirat, ahol még valamit meg kell oldani
// g_obLogger(cSeverity::DEBUG) << QString("") << EOM;
// cTracer obTrace( "" );

//====================================================================================
int main( int argc, char *argv[] )
{
    apMainApp = new QApplication(argc, argv);

    g_obGen.setApplication( apMainApp );
    g_obGen.initSysTrayIcon();

    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    apMainApp->setWindowIcon( QIcon(":/icons/Belenus.ico") );

    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);
    g_obLogger(cSeverity::INFO) << "Belenus Version " << APPLICATION_VERSION_NUMBER << " started." << EOM;

    g_poDB     = new cQTMySQLConnection;

    g_poPrefs  = new cPreferences();
    g_poPrefs->setVersion( APPLICATION_VERSION_NUMBER );
    g_poPrefs->setVersionDb( DATABASE_VERSION_NUMBER );
    g_poPrefs->setLangFilePrefix( "belenus_" );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );
    g_poPrefs->setApplicationPath( qsCurrentPath );

    g_obLogger(cSeverity::INFO) << "Get languages from 'language.inf'" << EOM;
    QStringList qslLanguages = g_obLanguage.getLanguages();
    g_obLogger(cSeverity::INFO) << "Available languages:" << EOM;
    for(int nLang=0;nLang<qslLanguages.count();nLang++)
    {
        g_obLogger(cSeverity::INFO) << qslLanguages.at(nLang).split("|").at(0)
                                    << " ("
                                    << qslLanguages.at(nLang).split("|").at(1)
                                    << ")"
                                    << EOM;
    }
    g_obLanguage.init( apMainApp, "belenus", "_" );

    QPixmap          obPixmap("resources/splash.png");
    QSplashScreen    obSplash( obPixmap );
    QString          qsSpalsh;

    QFont   obFont = obSplash.font();

    obFont.setBold(true);
    obSplash.setFont(obFont);
    obSplash.show();
    apMainApp->processEvents();

    qsSpalsh += QString( "V: %1\n" ).arg( APPLICATION_VERSION_NUMBER );
    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

    int r = 1;
    try
    {
        qsSpalsh += QObject::tr("Connecting to database ...");
        g_obLogger(cSeverity::INFO) << "Connecting to database ..." << EOM;
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poDB->open();

        g_obLogger(cSeverity::INFO) << "SUCCEEDED" << EOM;
        qsSpalsh += QObject::tr(" SUCCEEDED.\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("Loading settings from database ...");
        g_obLogger(cSeverity::INFO) << "Loading settings from database ..." << EOM;
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        g_poPrefs->loadSettings();
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

        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

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

            Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );
            return 0;
        }

        qsSpalsh += "\n\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

        g_obLicenceManager.initialize();

        qsSpalsh += QObject::tr("Licence serial: %1\n").arg( g_obLicenceManager.licenceSerialString() );
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        if( g_obLicenceManager.licenceID() >= 2 && g_obLicenceManager.licenceState() == cLicenceManager::LTYPE_INVALID )
        {
            qsSpalsh += "\n";
            qsSpalsh += QObject::tr( "The application's licence validity EXPIRED" );
            qsSpalsh += "\n";
            qsSpalsh += QObject::tr( "Please contact your franchise provider to activate your licence!" );
            qsSpalsh += "\n";
            qsSpalsh += "\n";
        }

        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

        //-------------------------------------------------------------------------------
        // End of process connection initialization
        //-------------------------------------------------------------------------------

        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += QObject::tr("Checking database consistency: ");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        g_poDB->executeQTQuery( QString( "UPDATE patientcardunits SET "
                                         "prepared=0, "
                                         "active=0 "
                                         "WHERE "
                                         "prepared=1 AND "
                                         "patientCardId>1 AND "
                                         "dateTimeUsed>\"2000-01-01\" " ) );

        g_poDB->executeQTQuery( QString( "UPDATE patientcardunits SET "
                                         "prepared=0 "
                                         "WHERE "
                                         "prepared=1 AND "
                                         "active=1 ") );

        g_poDB->executeQTQuery( QString( "DELETE FROM httpsendmail WHERE "
                                         "dateOfSending<\"" + QDate::currentDate().addDays( -4 ).toString( "yyyy-MM-dd" ) + "\" AND "
                                         "( mailTypeId = 1 OR mailTypeId = 2 )" ) );

        g_poDB->executeQTQuery( QString( "DELETE FROM httpsendmail WHERE recipients = \"\" " ) );

        qsSpalsh += QObject::tr("FINISHED\n");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        qsSpalsh += QObject::tr("Delete obsolete patientcard units: ");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        if( g_poPrefs->isDBAllowDeleteObsoleteUnits() )
        {
            g_poDB->executeQTQuery( QString( "DELETE FROM patientcardunits WHERE validDateTo < DATE_SUB(NOW(), INTERVAL %1 DAY) " ).arg( g_poPrefs->getObsolateUnitsDays() ) );
            qsSpalsh += QObject::tr("FINISHED\n");

            QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT ROW_COUNT()" ) );
            if( poQuery->first() )
            {
                qsSpalsh += QObject::tr("   Number of deleted units: %1").arg( poQuery->value( 0 ).toInt() );
                qsSpalsh += "\n";
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));
            }
        }
        else
        {
            qsSpalsh += QObject::tr("DISABLED\n");
        }
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        // DELETE FROM `patientcardhistories` WHERE modified < DATE_SUB(NOW(), INTERVAL 365 DAY);

        qsSpalsh += QObject::tr("Deleting old log files: ");
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        if( g_poPrefs->isLogFilesDeleteAllowed() )
        {
            int nFilesDeleted = g_obGen.deleteOldLogFiles( g_poPrefs->getDeleteLogFilesMonths() );
            qsSpalsh += QObject::tr("FINISHED with deleting %1 files").arg( nFilesDeleted );
            qsSpalsh += "\n";
        }
        else
        {
            qsSpalsh += QObject::tr("DISABLED\n");
        }
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44,75));

        qsSpalsh += "-----------------------------------------------------\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

        //-------------------------------------------------------------------------------
        // Initialize and set RFID communication if enabled
        //-------------------------------------------------------------------------------

        g_poCommRFID = NULL;
        g_poCommRFID = new cCommRFID();

        if( g_poPrefs->isRFIDEnabled() )
        {
            qsSpalsh += QObject::tr("\nRFID communication enabled\nChecking RFID connection ... ");

            Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

            g_poCommRFID->init( g_poPrefs->getRFIDComPort() );

            if( g_poCommRFID->isRFIDConnected() )
            {
                qsSpalsh += QObject::tr("CONNECTED\n\n");
            }
            else
            {
                qsSpalsh += QObject::tr("FAILED\n\n");
            }
        }
        else
        {
            qsSpalsh += QObject::tr("\nRFID communication disabled\n\n");
        }

        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

        //-------------------------------------------------------------------------------
        // If Hardware component active, process hardware initialization
        g_obLogger(cSeverity::DEBUG) << QString("HW check nID: %1 HWInstalled: %2").arg( g_obLicenceManager.licenceID() ).arg( g_poPrefs->isComponentHardwareInstalled() ) << EOM;

        if( g_obLicenceManager.licenceID() >= 2 && g_obLicenceManager.licenceState() != cLicenceManager::LTYPE_INVALID )
        {
            qsSpalsh += QObject::tr("Checking hardware connection ... ");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

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

                Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );
            }
            else
            {
                qsSpalsh += QObject::tr("CONNECTED\n");
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

                qsSpalsh += QObject::tr("Initializing hardware device ... ");
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));
                g_poHardware->setApplicationModuleCount( g_poPrefs->getPanelCount() );

                qsSpalsh += QObject::tr("FINISHED\n");
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

                qsSpalsh += QObject::tr("Caption stored in hardware: %1\n").arg( QString::fromStdString(g_poHardware->getCustomCaption()) );
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                qsSpalsh += QObject::tr("Number of hardware panels: %1\n").arg( g_poHardware->getHardwareModuleCount() );
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                qsSpalsh += QObject::tr("Checking hardware panels:\n");
                obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

                Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

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

                    Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

                }
            }
        }
        else
        {
            qsSpalsh += QObject::tr("Starting application in DEMO mode.\n");
            obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

            g_poHardware = new CS_Communication_Demo();
        }


        Sleep( g_poPrefs->getSecondsWaitOnSlpashScreen()*1000 );

        cWndMain  obMainWindow;

        obMainWindow.move( g_poPrefs->getMainWindowLeft(), g_poPrefs->getMainWindowTop() );
        obMainWindow.resize( g_poPrefs->getMainWindowWidth(), g_poPrefs->getMainWindowHeight() );

        Sleep( 5000 );

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
                                         obMainWindow.height() );
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
        g_obGen.showTrayError( qsError );
    }

//    g_poServer->quit();

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->getVersion() << " ended." << EOM;

    delete g_poPrefs;
    delete g_poDB;

    return r;
}
