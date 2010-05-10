#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include "../framework/network/connection.h"



class ServerThread : public Connection {

public:
    ServerThread();
    ~ServerThread();

protected:
    void _handleHello(Packet &);
    void _handleLogonResponse(Packet &);
    void _handleLogonAdminResponse(Packet &);

};

#endif // SERVERTHREAD_H
