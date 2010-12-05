//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QtNetwork>
#include <QCoreApplication>
#include <QTranslator>
#include <iostream>

//====================================================================================

#include "../framework/qtlogger.h"
#include "../framework/logger/ConsoleWriter.h"
#include "../framework/logger/DatabaseWriter.h"
#include "../framework/logger/FileWriter.h"
#include "../framework/qtmysqlconnection.h"
#include "preferences.h"
#include "serverthread.h"
#include "main.h"
#include "sigc.h"

//====================================================================================

cQTLogger            g_obLogger;
ServerPreferences    g_prefs;
cQTMySQLConnection   g_db;
cDebugger            g_obDebugger;

//====================================================================================
//====================================================================================
//
//
//
//====================================================================================
//====================================================================================
Server::Server()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[Server::Server] constructed" << EOM;
}
//====================================================================================
Server::~Server()
//====================================================================================
{
    _tcpServer.close();
    g_obLogger(cSeverity::DEBUG) << "[Server::~Server] finished" << EOM;
}
//====================================================================================
void Server::execute()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[Server::execute] called" << EOM;
    connect(&_tcpServer, SIGNAL(newConnection()), this, SLOT(connectionAvailable()));

    if ( !_tcpServer.listen( QHostAddress(g_prefs.value("server/interface")), g_prefs.value("server/port").toInt() ) )
        throw cSevException(cSeverity::ERROR, "Unable to start listener");

    g_obLogger(cSeverity::DEBUG) << "[Server::execute] listening on " << g_prefs.value("server/interface") << ":" << g_prefs.value("server/port") << EOM;
}
//====================================================================================
void Server::connectionAvailable()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[Server::connectionAvailable] new connection" << EOM;

    QTcpSocket *socket = _tcpServer.nextPendingConnection();

    ServerThread *connectionThread = new ServerThread();
    socket->setParent(0);
    socket->moveToThread(connectionThread);
    connectionThread->moveToThread(connectionThread);
    connectionThread->setTcpConnection(socket);
    connectionThread->start();

}
//====================================================================================
void sigc_handler(int)
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[Server] SIGC caught" << EOM;
    QCoreApplication::exit();
}
//====================================================================================
//====================================================================================
//
//
//
//====================================================================================
//====================================================================================
int main( int argc, char *argv[] )
//====================================================================================
{
    g_obDebugger.dLog( "Starting Belenus Server" );

        g_obDebugger.dLog( "[installSIGCHandler]" );
        installSIGCHandler();
        QCoreApplication  app( argc, argv );

        g_obDebugger.dLog( "<Loading settings from ini file>" );
        g_prefs.loadFile();

        //g_obDebugger.dLog( "<Creating console logger>" );
        //ConsoleWriter *_writer = new ConsoleWriter( static_cast<cSeverity::teSeverity>(g_prefs.value("loglevel/console").toInt()) );
        g_obDebugger.dLog( "<Creating database logger>" );
        DatabaseWriter *_dbWriter = new DatabaseWriter( static_cast<cSeverity::teSeverity>(g_prefs.value("loglevel/db").toInt()) );
        g_obDebugger.dLog( "<Creating file logger>" );
        FileWriter *_fWriter = new FileWriter( g_prefs.value("log/file"), static_cast<cSeverity::teSeverity>(g_prefs.value("loglevel/file").toInt()) );

        g_obDebugger.dLog( "[_dbWriter->setDBConnection]" );
        _dbWriter->setDBConnection(&g_db);
        //g_obDebugger.dLog( "<Adding consol writer to logger>" );
        //g_obLogger.attachWriter("console", _writer);
        g_obDebugger.dLog( "<Adding database writer to logger>" );
        g_obLogger.attachWriter("db", _dbWriter);
        g_obDebugger.dLog( "<Adding file writer to logger>" );
        g_obLogger.attachWriter("file", _fWriter);

        g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version") << " started." << EOM;
        g_obDebugger.dLog( QString("Belenus Version %1 started.").arg(g_prefs.value("version")) );

        g_obDebugger.dLog( "" );
        g_db.setHostName( g_prefs.value("database/host") );
        g_db.setDatabaseName( g_prefs.value("database/schema") );
        g_db.setUserName( g_prefs.value("database/username") );
        g_db.setPassword( g_prefs.value("database/password") );

        g_obDebugger.dLog( "<Creating Server object>" );
        Server server;

        g_obDebugger.dLog( "[QTimer::singleShot]" );
        QTimer::singleShot(0, &server, SLOT(execute()));

    try
    {
        g_obLogger(cSeverity::INFO) << "Connecting to database..." << EOM;
        g_obDebugger.dLog( "Connecting to database..." );
        g_db.open();

        g_obLogger(cSeverity::INFO) << "Starting app..." << EOM;
        g_obDebugger.dLog( "Starting app..." );
        app.exec();
    }
    catch(cSevException e)
    {
        g_obLogger(cSeverity::ERROR) << "Exception: " << e.what() << EOM;
        g_obDebugger.dLog( QString( "Exception: " ).arg(e.what()) );
    }

    g_obDebugger.dLog( "[g_obLogger.detachWriter]" );
    g_obLogger.detachWriter("db");

    g_obDebugger.dLog( "<Saving settings to ini file>" );
    g_prefs.saveFile();

    g_obLogger(cSeverity::INFO) << "Belenus Version " << g_prefs.value("version") << " ended." << EOM;
    g_obDebugger.dLog( QString("Belenus Version %1 ended.\n").arg(g_prefs.value("version")) );

    return 0;
}
//====================================================================================
//
// Debugger class for easy debug server application process
//
//====================================================================================
cDebugger::cDebugger()
//====================================================================================
{
}
//====================================================================================
cDebugger::~cDebugger()
//====================================================================================
{
}
//====================================================================================
void cDebugger::dLog( QString p_qsLog )
//====================================================================================
{
    FILE *file;

    file = fopen( "debug.log", "a+t");
    if( file != NULL )
    {
        fputs( QString("%1 %2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(p_qsLog).toStdString().c_str(), file );
        fclose( file );
    }
}
//====================================================================================
