#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QThread>
#include <QHostAddress>
#include "../framework/network/CommunicationProtocol.h"



class AdminClientThread : public QThread, public CommunicationProtocol
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

    void connectTo(const QString adr, int port);
    bool isConnected() { return m_socket!=0; }
    void abort();
    void setCredentials(QString username, QString password);
    void registerNewKey(const QString key);
    void resetCode2(int clientId);
    void removeKey(int clientId);
    void queryLicenseKeys();
    void queryLogs(cSeverity::teSeverity minSeverity = cSeverity::DEBUG, int last = 50);

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void sqlResultReady(int, SqlResult*);
    void registerKeyResponse(Result::ResultCode);


protected slots:
    void _error(QAbstractSocket::SocketError);
    void _disconnected();
    void _connected();
    void _read() { CommunicationProtocol::read(); } /* slots cannot be overwritten (probably because CP does not come from QObject) */
    void _connectTo(QString, int);
    void _registerNewKey(QString key);
    void _resetCode2(int clientId);
    void _removeKey(int clientId);
    void _queryLicenseKeys();
    void _queryLogs(cSeverity::teSeverity minSeverity, int last);


protected:
    virtual void run();


    void _handleHello(int version);
    void _handleLogonChallenge();
    void _handleLogonResult(Result::ResultCode, int clientId);
    void _handleRegisterKeyResponse(Result::ResultCode res);
    void _handleSqlQueryResult(int /*queryId*/, SqlResult *);

    QString _username;
    QString _password;
    bool _loggedIn;
};


#endif // CONNECTIONTHREAD_H
