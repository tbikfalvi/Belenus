
#include "CommunicationProtocol.h"


CommunicationProtocol::CommunicationProtocol(QTcpSocket *socket)
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

    connect( m_socket,   SIGNAL(readyRead()),   this, SLOT(read()) );
}



void CommunicationProtocol::read()
{
    if ( m_socket->bytesAvailable() <= 0 )
        return;

    qint64 size = m_buffer.size();
    m_buffer.resize( size + m_socket->bytesAvailable() );
    qint64 bytesRead = m_socket->read( m_buffer.data() + size, _socket->bytesAvailable() );

    if ( bytesRead <= 0 )
        return;

    try {
        while ( Packet *p = Packet::createPacket(m_buffer) ) {
            g_obLogger(cSeverity::DEBUG) << "[CommunicationProtocol::read] received " << p->getPacketName() << ": "<< p->dump() << cQTLogger::EOM;
            _handlePacket(*p);
            delete p;
        }
    } catch ( ProtocolException &e ) {
        g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::read] exception: " << e.what() << cQTLogger::EOM;
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
                g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::_handlePacket] packet unhandled:  " << packet.getId() << cQTLogger::EOM;
                break;
        }
    } catch (cSevException e) {
        g_obLogger(cSeverity::ERROR) << "[CommunicationProtocol::_handlePacket] exception caught("<<e.what()<<"). Closing connection." << cQTLogger::EOM;
        _sendDisconnect(Result::UNKOWN);
    }
}



void CommunicationProtocol::send(Packet &p)
{
    qint64 writtenBytes = m_socket->write(p.getRawPacket());
    g_obLogger(cSeverity::DEBUG) << "[CommunicationProtocol::send] sending "<< p.getPacketName() << "(" << p.getId() << ")" << ". " << p.dump() << ". Bytes written " << writtenBytes << cQTLogger::EOM;
}



void CommunicationProtocol::_sendHello()
{
    Packet p(Packet::MSG_HELLO);
    p << VERSION;
    send(p);
}



void CommunicationProtocol::_sendDisconnect(Result reason)
{
    Packet p(Packet::MSG_DISCONNECT);
    p << reason;
    send(p);
    _socket->disconnectFromHost();
}



void CommunicationProtocol::_sendLogonChallenge()
{
    Packet p(Packet::MSG_LOGON_CHALLENGE);
    send(p);
}



void CommunicationProtocol::_sendLogonResponse(const char* code1, const char *code2)
{
    Packet p(Packet::MSG_LOGON_RESPONSE);
    p << code1 << code2;
    send(p);
}



void CommunicationProtocol::_sendLogonAdminResponse(const char* username, const char* password)
{
    Packet p(Packet::MSG_LOGON_ADMIN_RESPONSE);
    p << username << password;
    send(p);
}



void CommunicationProtocol::_sendLogonOk()
{
    Packet p(Packet::MSG_LOGON_OK);
    send(p);
}



void CommunicationProtocol::_sendRegisterKey(const char *key)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY);
    p << key;
    send(p);
}



void CommunicationProtocol::_sendRegisterKeyResponse(Result reason)
{
    Packet p(Packet::MSG_REGISTER_LICENSE_KEY_RESPONSE);
    p << reason;
    send(p);
}



void CommunicationProtocol::_sendSqlQuery(int queryId, const char *query)
{
    Packet p(Packet::MSG_SQL_QUERY);
    p << queryId << query;
    send(p);
}



void CommunicationProtocol::_sendSqlQueryResult(int queryId, SqlResult &b)
{
    Packet p(Packet::MSG_SQL_RESULT);
    p << queryId << b.isValid();
    p << b.toStringStream();

    send(p);
}

