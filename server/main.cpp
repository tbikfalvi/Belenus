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
#include <QCoreApplication>
#include <QTranslator>
#include <iostream>

#include "qtlogger.h"
#include "../framework/qtmysqlconnection.h"
#include "preferences.h"
#include "serverthread.h"
#include "main.h"

cQTLogger            g_obLogger;
ServerPreferences    g_prefs;
cQTMySQLConnection   g_db;






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


    if ( !_tcpServer.listen( QHostAddress(g_prefs.value("server/interface")), g_prefs.value("server/port").toInt() ) )
        throw cSevException(cSeverity::ERROR, "Unable to start listener");

    g_obLogger(cSeverity::DEBUG) << "[Server::execute] listening on " << g_prefs.value("server/interface").toStdString() << ":" << g_prefs.value("server/port").toStdString() << cQTLogger::EOM;
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
    QCoreApplication  app( argc, argv );

    g_obLogger.setMinSeverityLevels(cSeverity::INFO);
    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version").toStdString() << " started." << cQTLogger::EOM;

    g_db.setHostName( g_prefs.value("database/host") );
    g_db.setDatabaseName( g_prefs.value("database/schema") );
    g_db.setUserName( g_prefs.value("database/username") );
    g_db.setPassword( g_prefs.value("database/password") );


    Server server;
    QTimer::singleShot(0, &server, SLOT(execute()));
    try {
        g_obLogger(cSeverity::INFO) << "Connecting to database..." << cQTLogger::EOM;
        g_db.open();

//        QSqlQuery *r = g_db.executeQTQuery("SELECT * FROM logs");
//        g_obLogger(cSeverity::DEBUG) << "Query is " << r->isActive() << " rows=" << r->size() << cQTLogger::EOM;
//
//        SqlResult sres;
//        sres.copy(r);
//        g_obLogger(cSeverity::DEBUG) << "CSV is\n" << sres.toStringStream().toStdString() << cQTLogger::EOM;
//
//        SqlResult sres2;
//        sres2.fromStringStream( sres.toStringStream() );
//        g_obLogger(cSeverity::DEBUG) << "CSV2 is\n" << sres2.toStringStream().toStdString() << cQTLogger::EOM;

        g_obLogger(cSeverity::INFO) << "Starting app..." << cQTLogger::EOM;
        app.exec();
    } catch(cSevException e) {
        g_obLogger(cSeverity::ERROR) << "Exception: " << e.what() << cQTLogger::EOM;
    }

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version").toStdString() << " ended." << cQTLogger::EOM;

    return 0;
}
//====================================================================================
