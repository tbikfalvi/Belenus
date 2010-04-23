#include <QApplication>
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QTranslator>
#include <QSplashScreen>

#include "../framework/qtlogger.h"
#include "../framework/qtmysqlconnection.h"
#include "dbuser.h"
#include "preferences.h"
#include "communication.h"
#include "communication_demo.h"
#include "bs_connection.h"
#ifdef __WIN32__
    #include "communication_serial.h"
#endif
#include "wndmain.h"

cQTLogger                g_obLogger;
cQTMySQLConnection      *g_poDB;
cDBUser                  g_obUser;
cPreferences            *g_poPrefs;
CS_Communication        *g_poHardware;
BelenusServerConnection *g_poServer;

int main( int argc, char *argv[] )
{
    QApplication     apMainApp( argc, argv );
//    QTcpSocket      *m_ptcpSocket;

    QPixmap          obPixmap("resources/splash.jpg");
    QSplashScreen    obSplash( obPixmap );
    QString          qsSpalsh;

    QFont   obFont = obSplash.font();

    obFont.setBold(true);
    obSplash.setFont(obFont);
    obSplash.show();
    apMainApp.processEvents();

    qsSpalsh += "Connecting to database ...";
    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

    g_poDB     = new cQTMySQLConnection;

    qsSpalsh += " CONNECTED.\n";
    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

    qsSpalsh += "Loading application settings ...";
    obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

    g_poPrefs  = new cPreferences( QString::fromAscii( "./belenus.ini" ) );
    g_poPrefs->setVersion( "0.0.1" );
    g_poPrefs->setLangFilePrefix( "belenus_" );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

    int r = 1;
    try
    {
        g_poDB->open();
        g_obLogger.setDBConnection( g_poDB );

        g_obLogger << cSeverity::INFO;
        g_obLogger << "Belenus Version " << g_poPrefs->getVersion().toStdString() << " started.";
        g_obLogger << cQTLogger::EOM;

        if( g_poPrefs->getLang() != "uk" )
        {
            QTranslator obTrans;
            QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
            g_obLogger << cSeverity::INFO;
            obTrans.load( qsTransFile );
            apMainApp.installTranslator( &obTrans );
        }

        g_poPrefs->loadDBSettings();

        qsSpalsh += " LOADED\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        qsSpalsh += "Connecting to Belenus server\n";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

//        m_ptcpSocket = new QTcpSocket();
//        g_poServer   = new BelenusServerConnection( m_ptcpSocket );

//        g_poServer->start();

        /*g_poServer->init( g_poPrefs->getClientSerial(),
                          g_poPrefs->getServerAddress(),
                          g_poPrefs->getServerPort() );*/

        qsSpalsh += "Waiting for response from Belenus server ";
        obSplash.showMessage(qsSpalsh,Qt::AlignLeft,QColor(59,44, 75));

        /*int nCount = 0;
        while( g_poServer->isConnectionActive() )
        {
            QString qsTemp;
            qsTemp.fill('.',++nCount/100);
            obSplash.showMessage(qsSpalsh+qsTemp,Qt::AlignLeft,QColor(59,44, 75));
            if( nCount > 399 ) nCount = 0;
        }*/

#ifdef __WIN32__
        g_poHardware = new CS_Communication_Serial();
        if( !g_poHardware->IsHardwareConnected() /*|| !g_poServer->isSerialValid()*/ )
        {
            delete g_poHardware;
            g_poHardware = new CS_Communication_Demo();
        }
#else
        g_poHardware = new CS_Communication_Demo();
#endif

        cWndMain  obMainWindow;

        obMainWindow.move( g_poPrefs->getMainWindowLeft(), g_poPrefs->getMainWindowTop() );
        obMainWindow.resize( g_poPrefs->getMainWindowWidth(), g_poPrefs->getMainWindowHeight() );
        obMainWindow.show();
        obSplash.finish( &obMainWindow );

        if( obMainWindow.showLogIn() )
        {
            obMainWindow.initPanels();
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
        g_obLogger << e.severity();
        g_obLogger << e.what();
        g_obLogger << cQTLogger::EOM;
    }

//    g_poServer->quit();

    g_obLogger << cSeverity::INFO;
    g_obLogger << "Belenus Version " << g_poPrefs->getVersion().toStdString() << " ended.";
    g_obLogger << cQTLogger::EOM;

    //delete m_ptcpSocket;
    //delete g_poServer;
    delete g_poPrefs;
    delete g_poDB;

    return r;
}
