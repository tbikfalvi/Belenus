//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_main.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo forras allomanya. Kornyezeti valtozok beolvasasa, adatbazis
// kapcsolat kiepitese, fo dialogus ablak megnyitasa
//====================================================================================

#include <QtNetwork>
#include <QApplication>
#include <QTranslator>
#include <iostream>

#include "../framework/qtlogger.h"
#include "preferences.h"
#include "serverthread.h"
#include "main.h"

cQTLogger             g_obLogger;
Preferences          *g_poPrefs;






Server::Server()
{
    g_obLogger(cSeverity::DEBUG) << "[Server::Server] constructed" << cQTLogger::EOM;
}



Server::~Server()
{
    _tcpServer.close();
    g_obLogger(cSeverity::DEBUG) << "[Server::~Server] finished" << cQTLogger::EOM;
}



void Server::execute()
{
    connect(&_tcpServer, SIGNAL(newConnection()), this, SLOT(connectionAvailable()));


    if ( !_tcpServer.listen( QHostAddress(g_poPrefs->value("server/interface")), g_poPrefs->value("server/port").toInt() ) )
        throw cSevException(cSeverity::ERROR, "Unable to start listener");

    g_obLogger(cSeverity::DEBUG) << "[Server::execute] listening on " << g_poPrefs->value("server/interface").toStdString() << ":" << g_poPrefs->value("server/port").toStdString() << cQTLogger::EOM;
}



void Server::connectionAvailable()
{
    g_obLogger(cSeverity::DEBUG) << "[Server::connectionAvailable] new connection" << cQTLogger::EOM;

    ServerThread *connectionThread = new ServerThread();
    connectionThread->connectTo(_tcpServer.nextPendingConnection());
}




//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication  app( argc, argv );

    g_poPrefs  = new ServerPreferences();
    g_obLogger.setMinSeverityLevels(cSeverity::MAX, cSeverity::MIN, cSeverity::MIN );
    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->value("version").toStdString() << " started." << cQTLogger::EOM;

    Server server;
    QTimer::singleShot(0, &server, SLOT(execute()));
    try {
        app.exec();
    } catch(cSevException e) {
        g_obLogger(cSeverity::ERROR) << "Exception caught " << e.what() << cQTLogger::EOM;
    }

    // cout << "Enter ! to quit";
    // while ( getchar()!='!' ) ;

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_poPrefs->value("version").toStdString() << " ended." << cQTLogger::EOM;

    delete g_poPrefs;

    return 0;
}
//====================================================================================
