

#include "connection.h"
#include "../qtlogger.h"

extern cQTLogger g_obLogger;


Connection::Connection()
    :   QThread()
{
}



Connection::~Connection()
{
    if ( _socket )
        delete _socket;
}



void Connection::_connectSignalsToSocket()
{
    connect( _socket, SIGNAL(connected()),      this, SLOT(_connected()) );
    connect( _socket, SIGNAL(readyRead()),      this, SLOT(_recv())         );
    connect( _socket, SIGNAL(disconnected()),   this, SLOT(_disconnected()) );
    connect( _socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_error(QAbstractSocket::SocketError)) );
}



void Connection::connectTo(QTcpSocket *s)
{
    if ( !isFinished() ) {
        g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] incoming from " << s->peerAddress().toString().toStdString() << ":" << s->peerPort() << cQTLogger::EOM;

        _socket = s;
        _connectSignalsToSocket();
        start();
    }
}



void Connection::connectTo(QHostAddress addr, qint16 port)
{
    g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] incoming from " << addr.toString().toStdString() << ":" << port << cQTLogger::EOM;
    _socket = new QTcpSocket();
    _connectSignalsToSocket();
    _socket->connectToHost(addr, port);
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
    quit();
    emit disconnected();
}



void Connection::_connected()
{
    start();
    emit connected();
}



void Connection::_recv()
{
    qint64 size = _buffer.size();
    _buffer.resize( size + _socket->bytesAvailable() );
    qint64 bytesRead = _socket->read( _buffer.data() + size, _socket->bytesAvailable() );
    if ( bytesRead<=0 )
        return;

    while ( Packet *p = Packet::createPacket(_buffer) ) {
        g_obLogger(cSeverity::DEBUG) << "[Connection::_recv] packet received " << p->getId() << cQTLogger::EOM;
        _handlePacket(*p);
        delete p;
    }
}



void Connection::_handlePacket(Packet &packet)
{
    if ( !_allowedPackets.empty() && !_allowedPackets.contains(packet.getId()) ) {
        /* TODO: protocol error */
        g_obLogger(cSeverity::DEBUG) << "[Connection::_handlePacket] packet is not in allowed list" << cQTLogger::EOM;
        return; /* unhandled */
    }

    switch ( packet.getId() ) {
        case Packet::MSG_HELLO:
                _handleHello(packet);
            break;
        default:
            g_obLogger << cSeverity::DEBUG << "[Connection::_handlePacket] packet unhandled. " << cQTLogger::EOM;
            break;
    }
}



void Connection::_error(QAbstractSocket::SocketError s)
{
    quit();
    emit error(s);
}



/*
 * public methods
 */
void Connection::run()
{
    if ( !_socket )
        throw 1; // exceptions todo

    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()));
    _initialize();
    exec();
}



void Connection::send(Packet &p)
{
    qint64 writtenBytes = _socket->write(p.getPacket());
    printf("%d", p.getId());
    g_obLogger << cSeverity::INFO << "[Connection::send] sending packet #"<< p.getId() << ". Bytes written " << writtenBytes << cQTLogger::EOM;
}



bool Connection::isConnected()
{
    return _socket && (_socket->state()==QAbstractSocket::ConnectedState || _socket->state()==QAbstractSocket::BoundState);
}

