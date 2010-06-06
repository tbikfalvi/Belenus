#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include "../framework/network/connection.h"



class AdminClientThread : public Connection
{
    Q_OBJECT

public:
    AdminClientThread();
    void setCredentials(QString username, QString password);
    void registerNewKey(const char* key);
    void executeSqlQuery(int, const char*);

protected:
    void _initialize();
    void _handleHello(int version);
    void _handleLogonChallenge();
    void _handleLogonOk();
    void _handleRegisterKeyResponse(Result res);

    QString _username;
    QString _password;
    bool _loggedIn;;
};


#endif // CONNECTIONTHREAD_H
