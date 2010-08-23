#ifndef _CommunicationProtocol_h
#define _CommunicationProtocol_h

#include <QByteArray>
#include <QTcpSocket>
#include "packet.h"
#include "sqlResult.h"



namespace Result {
    enum ResultCode {
        OK,
        UNKNOWN,
        INVALID_VERSION,
        INVALID_LICENSE_KEY,
        INVALID_SECOND_ID,
        AUTHENTICATION_FAILED,
        ALREADY_REGISTERED,
    };
};




class CommunicationProtocol {

public:
    static const int VERSION = 1;


    CommunicationProtocol(QTcpSocket *socket = 0);
    virtual ~CommunicationProtocol();
    void setTcpConnection(QTcpSocket *socket);
    virtual void read();

    virtual void sendHello();
    virtual void sendDisconnect(Result::ResultCode reason);
    virtual void sendLogonChallenge();
    virtual void sendLogonResponse(const char* code1, const char *code2);
    virtual void sendLogonAdminResponse(const char* username, const char* password);
    virtual void sendLogonOk();
    virtual void sendRegisterKey(const char* key);
    virtual void sendRegisterKeyResponse(Result::ResultCode result);
    virtual void sendSqlQuery(int queryId, const char *query);
    virtual void sendSqlQueryResult(int queryId, SqlResult &);

protected:
    virtual void _send(Packet &);
    virtual void _handlePacket(Packet &);
    virtual void _initialize() {};              /* called when connection established to initialize protocol */
    virtual void _handleHello(int /*version*/) {};
    virtual void _handleLogonChallenge() {};
    virtual void _handleLogonAdminResponse(const char* /*username*/, const char* /*password*/) {};
    virtual void _handleLogonResponse(const char* /*code1*/, const char* /*code2*/) {};
    virtual void _handleLogonOk() {};
    virtual void _handleDisconnect(Result::ResultCode /*reason*/) {};
    virtual void _handleRegisterKey(const char* /*key*/) {};
    virtual void _handleRegisterKeyResponse(Result::ResultCode /*result*/) {};
    virtual void _handleSqlQuery(int /*queryId*/, const char* /*query*/) {};
    virtual void _handleSqlQueryResult(int /*queryId*/, SqlResult *) {};


    QByteArray m_buffer;
    QTcpSocket *m_socket;
};



#endif