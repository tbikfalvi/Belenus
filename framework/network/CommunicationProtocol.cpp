#include <QHostAddress>

#include "../qtlogger.h"
#include "CommunicationProtocol.h"


extern cQTLogger g_obLogger;




CommunicationProtocol::CommunicationProtocol(QTcpSocket *socket)
    : m_socket(0)
{
    setTcpConnection(socket);
}



CommunicationProtocol::~CommunicationProtocol()
{
}



void CommunicationProtocol::setTcpConnection(QTcpSocket *socket)
{
    if ( !socket )
        return;

    m_socket = socket;
    if ( m_socket->state()==QAbstractSocket::ConnectedState )
        g_obLogger(cSeverity::INFO) << "Connection from "<< m_socket->peerAddress().toString() << ":" << m_socket->peerPort();
}



void CommunicationProtocol::read()
{
    if ( m_socket->bytesAvailable() <= 0 )
        return;

    qint64 size = m_buffer.size();
    m_buffer.resize( size + m_socket->bytesAvailable() );
    qint64 bytesRead = m_socket->read( m_buffer.data() + size, m_socket->bytesAvailable() );

    if ( bytesRead <= 0 )
        return;

    try {
        while ( Packet *p = Packet::createPacket(m_buffer) ) {
            g_obLogger(cSeverity::DEBUG) << "[CommunicationProtocol::read] received " << p->getPacketName() << ": "<< p->dump() << EOM;
            _handlePacket(*p);
            delete p;
        }
    } catch ( ProtocolException &e ) {
        g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::read] exception: " << e.what() << EOM;
        sendDisconnect(Result::UNKNOWN);
    }
}



void CommunicationProtocol::_handlePacket(Packet &packet)
{
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

            case Packet::MSG_LOGON_RESULT: {
                    int licenceId;
                    Result::ResultCode r;
                    packet >> r >> licenceId;
                    _handleLogonResult(r, licenceId);
                } break;

            case Packet::MSG_DISCONNECT: {
                    Result::ResultCode r;
                    packet >> r;
                    _handleDisconnect(r);
                } break;

            case Packet::MSG_REGISTER_LICENSE_KEY: {
                    char* key;
                    packet >> key;
                    _handleRegisterKey(key);
                } break;

            case Packet::MSG_REGISTER_LICENSE_KEY_RESPONSE: {
                    Result::ResultCode r;
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
                g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::_handlePacket] packet unhandled:  " << packet.getId() << EOM;
                break;
        }
    } catch (cSevException e) {
        g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::_handlePacket] exception caught("<<e.what()<<"). Closing connection." << EOM;
        sendDisconnect(Result::UNKNOWN);
    }
}



void CommunicationProtocol::_send(Packet &p)
{
    qint64 writtenBytes = m_socket->write(p.getRawPacket());
    g_obLogger(cSeverity::DEBUG) << "[CommunicationProtocol::send] sending "<< p.getPacketName() << "(" << p.getId() << ")" << ". " << p.dump() << ". Bytes written " << writtenBytes << EOM;
}



void CommunicationProtocol::sendHello()
{
    Packet p(Packet::MSG_HELLO);
    p << VERSION;
    _send(p);
}



void CommunicationProtocol::sendDisconnect(Result::ResultCode reason)
{
    Packet p(Packet::MSG_DISCONNECT);
    p << reason;
    _send(p);
    m_socket->disconnectFromHost();
}



void CommunicationProtocol::sendLogonChallenge()
{
    Packet p(Packet::MSG_LOGON_CHALLENGE);
    _send(p);
}



void CommunicationProtocol::sendLogonResponse(const char* code1, const char *code2)
{
    Packet p(Packet::MSG_LOGON_RESPONSE);
    p << code1 << code2;
    _send(p);
}



void CommunicationProtocol::sendLogonAdminResponse(const char* username, const char* password)
{
    Packet p(Packet::MSG_LOGON_ADMIN_RESPONSE);
    p << username << password;
    _send(p);
}



void CommunicationProtocol::sendLogonResult(Result::ResultCode res, const int licenceId)
{
    Packet p(Packet::MSG_LOGON_RESULT);
    p << res << licenceId;
    _send(p);
}



void CommunicationProtocol::sendRegisterKey(const char *key)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY);
    p << key;
    _send(p);
}



void CommunicationProtocol::sendRegisterKeyResponse(Result::ResultCode reason)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY_RESPONSE);
    p << reason;
    _send(p);
}



void CommunicationProtocol::sendSqlQuery(int queryId, const char *query)
{
    Packet p(Packet::MSG_SQL_QUERY);
    p << queryId << query;
    _send(p);
}



void CommunicationProtocol::sendSqlQueryResult(int queryId, SqlResult &b)
{
    Packet p(Packet::MSG_SQL_RESULT);
    p << queryId << b.isValid();
    p << b.toStringStream();

    _send(p);
}

