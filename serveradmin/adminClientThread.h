#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include "../framework/network/connection.h"



class AdminClientThread : public Connection
{
public:
    AdminClientThread();

protected:
    void _initialize();
    void _handleHello(Packet&);
};


#endif // CONNECTIONTHREAD_H
