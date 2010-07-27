//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================


#include <QtNetwork>
#include <QCoreApplication>
#include <QTranslator>
#include <iostream>

#include "../framework/qtlogger.h"
#include "../framework/logger/ConsoleWriter.h"
#include "../framework/logger/DatabaseWriter.h"
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

    g_obLogger(cSeverity::DEBUG) << "[Server::execute] listening on " << g_prefs.value("server/interface") << ":" << g_prefs.value("server/port") << cQTLogger::EOM;
}



void Server::connectionAvailable()
{
    g_obLogger(cSeverity::DEBUG) << "[Server::connectionAvailable] new connection" << cQTLogger::EOM;

    ServerThread *connectionThread = new ServerThread(_tcpServer.nextPendingConnection());
    connectionThread->start();
}




//====================================================================================
int main( int argc, char *argv[] )
{
    QCoreApplication  app( argc, argv );

    ConsoleWriter _writer(cSeverity::DEBUG);
    DatabaseWriter _dbWriter(cSeverity::INFO);
    _dbWriter.setDBConnection(&g_db);
    g_obLogger.attachWriter("console", &_writer);
    g_obLogger.attachWriter("db", &_dbWriter);
    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version") << " started." << cQTLogger::EOM;

    g_db.setHostName( g_prefs.value("database/host") );
    g_db.setDatabaseName( g_prefs.value("database/schema") );
    g_db.setUserName( g_prefs.value("database/username") );
    g_db.setPassword( g_prefs.value("database/password") );


    Server server;
    QTimer::singleShot(0, &server, SLOT(execute()));
    try {
        g_obLogger(cSeverity::INFO) << "Connecting to database..." << cQTLogger::EOM;
        g_db.open();

        g_obLogger(cSeverity::INFO) << "Starting app..." << cQTLogger::EOM;
        app.exec();
    } catch(cSevException e) {
        g_obLogger(cSeverity::ERROR) << "Exception: " << e.what() << cQTLogger::EOM;
    }

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version") << " ended." << cQTLogger::EOM;

    return 0;
}
//====================================================================================
