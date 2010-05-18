#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include "../framework/network/connection.h"



class ServerThread : public Connection {

public:
    ServerThread();
    ~ServerThread();

protected:
    void _handleHello(int version);
    void _handleLogonResponse(const char* code1, const char* code2);
    void _handleLogonAdminResponse(const char* username, const char* password);
    void _handleRegisterKey(const char *key);

    bool _isAdmin;
};

#endif // SERVERTHREAD_H
