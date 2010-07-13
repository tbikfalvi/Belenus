
#include "connection.h"
#ifdef SERVER
#  include "../../server/qtlogger.h"
#else
#  include "../framework/qtlogger.h"
#endif


extern cQTLogger g_obLogger;


Connection::Connection()
    :   QThread(),
        _socket(0),
        _port(0)
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
    g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] connecting to " << addr.toString().toStdString() << ":" << port << cQTLogger::EOM;
    _addr = addr;
    _port = port;
    start();
    g_obLogger(cSeverity::DEBUG) << "[Connection::connectTo] connecting..." << cQTLogger::EOM;
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
    g_obLogger(cSeverity::DEBUG) << "[Connection::_connected] connect signal received" << cQTLogger::EOM;
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

    try {

        switch ( packet.getId() ) {

            case Packet::MSG_HELLO: {
                    int version;
                    packet >> version;
                    _handleHello(version);
                } break;

            case Packet::MSG_LOGON_CHALLENGE: {
                _handleLogonChallenge();
                } break;

            case Packet::MSG_LOGON_ADMIN_RESPONSE: {
                    char *p1, *p2;
                    packet >> p1 >> p2;
                    _handleLogonAdminResponse(p1, p2);
                } break;

            case Packet::MSG_LOGON_RESPONSE: {
                    char *u, *p;
                    packet >> u >> p;
                    _handleLogonResponse(u, p);
                } break;

            case Packet::MSG_LOGON_OK: {
                    _handleLogonOk();
                } break;

            case Packet::MSG_DISCONNECT: {
                    Result r;
                    packet >> r;
                    _handleDisconnect(r);
                } break;

            case Packet::MSG_REGISTER_LICENSE_KEY: {
                    char* key;
                    packet >> key;
                    _handleRegisterKey(key);
                } break;

            case Packet::MSG_REGISTER_LICENSE_KEY_RESPONSE: {
                    Result r;
                    packet >> r;
                    _handleRegisterKeyResponse(r);
                } break;

            case Packet::MSG_SQL_QUERY: {
                    int id;
                    char *u;
                    packet >> id >> u;
                    _handleSqlQuery(id,u);
                } break;

            case Packet::MSG_SQL_RESULT: {
                    int id;
                    bool status;
                    char* str;
                    packet >> id >> status >> str;
                    SqlResult *s = new SqlResult();
                    s->fromStringStream(str);
                    s->setValid(status);
                    _handleSqlQueryResult(id, s);
                } break;

            default:
                g_obLogger << cSeverity::ERROR << "[Connection::_handlePacket] packet unhandled:  " << packet.getId() << cQTLogger::EOM;
                break;
        }
    } catch (cSevException e) {
        g_obLogger(cSeverity::ERROR) << "[Connection::_handlePacket] exception caught("<<e.what()<<"). Closing connection." << cQTLogger::EOM;
        _sendDisconnect(RESULT_UNKOWN);
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
    g_obLogger(cSeverity::DEBUG) << "[Connection::run] thread started" << cQTLogger::EOM;
    if ( !_socket && _port ) {
        _socket = new QTcpSocket();
        _connectSignalsToSocket();
        _socket->connectToHost(_addr, _port);
    }

    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()));
    _initialize();
    exec();
}



void Connection::send(Packet &p)
{
    qint64 writtenBytes = _socket->write(p.getRawPacket());
    g_obLogger(cSeverity::DEBUG) << "[Connection::send] sending "<< p.getPacketName() << "(" << p.getId() << ")" << ". " << p.dump().toStdString() << ". Bytes written " << writtenBytes << cQTLogger::EOM;
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



void Connection::_sendHello()
{
    Packet p(Packet::MSG_HELLO);
    p << VERSION;
    send(p);
}



void Connection::_sendDisconnect(Result reason)
{
    Packet p(Packet::MSG_DISCONNECT);
    p << reason;
    send(p);
    _socket->disconnectFromHost();
}



void Connection::_sendLogonChallenge()
{
    Packet p(Packet::MSG_LOGON_CHALLENGE);
    send(p);
}



void Connection::_sendLogonResponse(const char* code1, const char *code2)
{
    Packet p(Packet::MSG_LOGON_RESPONSE);
    p << code1 << code2;
    send(p);
}



void Connection::_sendLogonAdminResponse(const char* username, const char* password)
{
    Packet p(Packet::MSG_LOGON_ADMIN_RESPONSE);
    p << username << password;
    send(p);
}



void Connection::_sendLogonOk()
{
    Packet p(Packet::MSG_LOGON_OK);
    send(p);
}



void Connection::_sendRegisterKey(const char *key)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY);
    p << key;
    send(p);
}



void Connection::_sendRegisterKeyResponse(Result reason)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY_RESPONSE);
    p << reason;
    send(p);
}



void Connection::_sendSqlQuery(int queryId, const char *query)
{
    Packet p(Packet::MSG_SQL_QUERY);
    p << queryId << query;
    send(p);
}



void Connection::_sendSqlQueryResult(int queryId, SqlResult &b)
{
    Packet p(Packet::MSG_SQL_RESULT);
    p << queryId << b.isValid();
    p << b.toStringStream();

    send(p);
}



void Connection::_handleSqlQueryResult(int queryId, SqlResult *res)
{
    emit sqlResultReady(queryId, res);
}
