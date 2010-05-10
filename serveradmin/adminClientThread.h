#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include "../framework/network/connection.h"



class AdminClientThread : public Connection
{
public:
    AdminClientThread();
    void setCredentials(QString username, QString password);

protected:
    void _initialize();
    void _handleHello(Packet&);
    void _handleLogonChallenge(Packet &);

    QString _username;
    QString _password;
};


#endif // CONNECTIONTHREAD_H
