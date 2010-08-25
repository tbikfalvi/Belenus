#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QThread>
#include <QHostAddress>
#include "../framework/network/CommunicationProtocol.h"



class AdminClientThread : public QThread, CommunicationProtocol
{
    Q_OBJECT

public:
    enum QueryIds {
        Q_GET_KEYS,
        Q_RESET_CODE2,
        Q_REMOVE_LICENSE_KEY,
        Q_GET_LOGS,
    };


    AdminClientThread();
    void connectTo(const QHostAddress adr, int port);
    bool isConnected() { return m_socket!=0; }
    void abort();

    void setCredentials(QString username, QString password);
    void registerNewKey(const char* key);
    void queryLicenseKeys();
    void resetCode2(int clientId);
    void queryLogs(cSeverity::teSeverity minSeverity = cSeverity::DEBUG, int last = 50);
    void removeKey(int clientId);

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void sqlResultReady(int, SqlResult*);

protected slots:
    void _error(QAbstractSocket::SocketError);
    void _disconnected();
    void _connected();
    virtual void _read() { CommunicationProtocol::read(); } /* slots cannot be overwritten */

protected:
    virtual void run();


    void _initialize();
    void _handleHello(int version);
    void _handleLogonChallenge();
    void _handleLogonOk();
    void _handleRegisterKeyResponse(Result::ResultCode res);
    void _handleSqlQueryResult(int /*queryId*/, SqlResult *);

    QString _username;
    QString _password;
    bool _loggedIn;
};


#endif // CONNECTIONTHREAD_H
