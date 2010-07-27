
#include "connection.h"
#include "../qtlogger.h"


extern cQTLogger g_obLogger;


Connection::Connection()
    :   QThread(),
        _socket(0),
        _port(0)
{
}



Connection::~Connection()
{
    mutex.lock();
    if ( _socket )
        delete _socket;
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}



void Connection::connectTo(QTcpSocket *s)
{
    g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] incoming from " << s->peerAddress().toString() << ":" << s->peerPort() << cQTLogger::EOM;
    QMutexLocker(&mutex);
    _socket = s;
    if (!isRunning())
        start();
}



void Connection::connectTo(QHostAddress addr, qint16 port)
{
    g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] connecting to " << addr.toString() << ":" << port << cQTLogger::EOM;
    QMutexLocker(&mutex);
    _addr = addr;
    _port = port;
    if (!isRunning())
        start();
}



void Connection::abortConnection()
{
    if ( _socket )
        _socket->abort();
}



/*
 * Slots
 */
void Connection::_disconnected()
{
    _socket = 0;
    emit disconnected();
}



void Connection::_connected()
{
    g_obLogger(cSeverity::DEBUG) << "[Connection::_connected] connect signal received" << cQTLogger::EOM;
    emit connected();
}






void Connection::_error(QAbstractSocket::SocketError s)
{
    quit();
    _socket = 0;
    emit error(s);
}



/*
 * public methods
 */
void Connection::run()
{
    g_obLogger(cSeverity::DEBUG) << "[Connection::run] thread started" << cQTLogger::EOM;
    if ( !_socket && _port ) {
        _socket = new QTcpSocket();
        _socket->connectToHost(_addr, _port);
    }

    connect( _socket,   SIGNAL(connected()),                          this, SLOT(_connected()) );
    connect( _socket,   SIGNAL(readyRead()),                          this, SLOT(_recv()) );
    connect( _socket,   SIGNAL(disconnected()),                       this, SLOT(_disconnected()) );
    connect( _socket,   SIGNAL(error(QAbstractSocket::SocketError)),  this, SLOT(_error(QAbstractSocket::SocketError)) );
    connect( this,      SIGNAL(finished()),                           this, SLOT(deleteLater()));

    _initialize();
    exec();
}






bool Connection::isConnected()
{
    return _socket && (_socket->state()==QAbstractSocket::ConnectedState || _socket->state()==QAbstractSocket::BoundState);
}



