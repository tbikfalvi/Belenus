//====================================================================================
//
// Belenus Server Admin alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_connection.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Interneten kereszuli kapcsolat a BelenusServer alkalmazassal
//====================================================================================

#ifndef BS_CONNECTION_H
#define BS_CONNECTION_H

//====================================================================================

#include <QString>
#include <QtNetwork>
#include <QThread>
#include <QHostAddress>
#include "../framework/network/CommunicationProtocol.h"
#include "../framework/qtlogger.h"



//====================================================================================
class BelenusServerConnection : public QThread, public CommunicationProtocol
{
    Q_OBJECT

public:
    enum ConnectionStatus {
        NOT_CONNECTED,
        CONNECTION_FAILED,  // eg. socket error
        CONNECTING,
        CONNECTED,          // connected but not authenticated
        AUTHENTICATED,      // server connection is ok
    };


    BelenusServerConnection();
    virtual ~BelenusServerConnection();

    void setServerAddress(const QString addr, const int port);
    void connectTo();
    ConnectionStatus getStatus() { return _status; }
    bool isConnected() { return _status==CONNECTED || _status==AUTHENTICATED; }

    /* licence management */
    void setLoginKeys(const QString serial, const QString code2);
    Result::ResultCode getLicenceResult() { return _licenceResult; }    // can return UNKNOWN, OK, INVALID_LICENCE_KEY or INVALID_SECOND_ID
    bool isLicenseValid();                                              // true if connected to server and server accepted license and key2 */
    int getClientId();                                                  // if lic auth succeeded, returns the clientId stored FROM server

    int sendQuery(const QString query);

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void licenceStatusChanged(Result::ResultCode, int clientId);
    void queryReady(int id, SqlResult*);

protected slots:
    void _error(QAbstractSocket::SocketError);
    void _disconnected();
    void _connected();
    virtual void _read() { CommunicationProtocol::read(); } /* slots cannot be overloaded */
    void _connectTo();
    void _sendQuery(int id, const QString query);

protected:
    virtual void run();

    void _handleLogonChallenge();
    void _handleLogonResult(Result::ResultCode res, int licenceId);
    void _handleDisconnect(Result::ResultCode reason);
    void _handleSqlQueryResult(int /*queryId*/, SqlResult *);

private:
    QString _host;
    int _port;
    QString _serial;
    QString _code2;
    ConnectionStatus _status;
    Result::ResultCode _licenceResult;
    Result::ResultCode _lastResult;
    bool _isLicenseValid;
    int _clientId;
    int _queryIdCounter;
    QMutex _queryIdCounterGuard;
};



#endif // BS_CONNECTION_H
