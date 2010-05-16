
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
    _socket = 0;
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

    try {
        while ( Packet *p = Packet::createPacket(_buffer) ) {
            g_obLogger(cSeverity::DEBUG) << "[Connection::_recv] received " << p->getPacketName() << ": "<< p->dump().toStdString() << cQTLogger::EOM;
            _handlePacket(*p);
            delete p;
        }
    } catch ( ProtocolException &e ) {
        g_obLogger(cSeverity::DEBUG) << "[Connection::_recv] exception caught: " << e.what() << cQTLogger::EOM;
        _socket->disconnectFromHost();
    }
}



void Connection::_handlePacket(Packet &packet)
{
    if ( !_allowedPackets.empty() && !_allowedPackets.contains(packet.getId()) ) {
        g_obLogger(cSeverity::ERROR) << "[Connection::_handlePacket] packet is not in allowed list" << cQTLogger::EOM;
        throw ProtocolException(cSeverity::ERROR, PROTOCOL_UNEXPECTED_PACKET, packet.getPacketName());
        return; /* unhandled */
    }

    switch ( packet.getId() ) {
        case Packet::MSG_HELLO:
                _handleHello(packet);
            break;
        case Packet::MSG_VERSION_MISMATCH:
                _handleVersionMismatch(packet);
            break;
        case Packet::MSG_LOGON_CHALLENGE:
                _handleLogonChallenge(packet);
            break;
        case Packet::MSG_LOGON_ADMIN_RESPONSE:
                _handleLogonAdminResponse(packet);
            break;
        case Packet::MSG_LOGON_RESPONSE:
                _handleLogonResponse(packet);
            break;
        case Packet::MSG_LOGON_OK:
                _handleLogonOk(packet);
            break;
        case Packet::MSG_DISCONNECT:
                _handleDisconnect(packet);
            break;
        default:
            g_obLogger << cSeverity::DEBUG << "[Connection::_handlePacket] packet unhandled. " << cQTLogger::EOM;
            break;
    }
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
    if ( !_socket )
        throw 1; // exceptions todo

    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()));
    _initialize();
    exec();
}



void Connection::send(Packet &p)
{
    qint64 writtenBytes = _socket->write(p.getRawPacket());
    g_obLogger << cSeverity::INFO << "[Connection::send] sending "<< p.getPacketName() << "(" << p.getId() << ")" << ". " << p.dump().toStdString() << ". Bytes written " << writtenBytes << cQTLogger::EOM;
}



bool Connection::isConnected()
{
    return _socket && (_socket->state()==QAbstractSocket::ConnectedState || _socket->state()==QAbstractSocket::BoundState);
}



void Connection::_assertSize(unsigned int size, Packet &p, AssertType type)
{
    if ( (type==EXACT && p.getLength() != size) || ( p.getLength()<size) ) {
        QString msg;
        msg = QString("for %3 length is %1, it should be %2").arg(p.getLength()).arg(size).arg(p.getPacketName());
        g_obLogger << cSeverity::ERROR << "[Connection::_assertSize] for packet " << p.getPacketName() << " length should be "<< (type==EXACT?"exactly ":"minimum ") << size << ", but " << p.getLength() << " bytes received. " << cQTLogger::EOM;
        throw ProtocolException(cSeverity::ERROR, PROTOCOL_PACKET_SIZE_MISMATCH, msg.toStdString() );
    }
}
