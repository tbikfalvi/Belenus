#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include "../framework/network/connection.h"



class ServerThread : public Connection {
    static const int VERSION = 1;

public:
    ServerThread();
    ~ServerThread();

protected:
    void _handleHello(Packet &);

};

#endif // SERVERTHREAD_H
