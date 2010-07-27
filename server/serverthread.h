#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include "../framework/network/CommunicationProtocol.h"



class ServerThread : public QThread, public CommunicationProtocol {

public:
    ServerThread(QTcpSocket*);
    ~ServerThread();

protected slots:
    void error(QAbstractSocket::SocketError);
    void disconnected();

protected:
    virtual void run();
    void _handleHello(int version);
    void _handleLogonResponse(const char* code1, const char* code2);
    void _handleLogonAdminResponse(const char* username, const char* password);
    void _handleRegisterKey(const char *key);
    void _handleSqlQuery(int queryid, const char* query);

    bool _isAuthenticated;
    bool _isAdmin;
};

#endif // SERVERTHREAD_H
