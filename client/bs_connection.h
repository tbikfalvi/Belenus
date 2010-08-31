//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
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
        CONNECTING,
        AUTHENTICATED,      // server connection is ok
        CONNECTION_FAILED,  // eg. socket error
    };


    BelenusServerConnection();
    virtual ~BelenusServerConnection();

    void connectTo(const QString adr, int port);
    void setLoginKeys(const QString serial, const QString code2);
    ConnectionStatus getStatus() { return _status; }
    Result::ResultCode getLastResult() { return _lastResult; }
    bool isLicenseValid() { return _isLicenseValid; }        /* true if connected to server and server accepted license and key2 */

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void __connectTo(QString adr, int port);

protected slots:
    void _error(QAbstractSocket::SocketError);
    void _disconnected();
    void _connected();
    virtual void _read() { CommunicationProtocol::read(); } /* slots cannot be overloaded */
    void __connectTo_(QString, int);

protected:
    virtual void run();

    void _handleLogonChallenge();
    void _handleLogonOk();
    void _handleDisconnect(Result::ResultCode reason);

private:
    QString _serial;
    QString _code2;
    ConnectionStatus _status;
    Result::ResultCode _lastResult;
    bool _isLicenseValid;
};



#endif // BS_CONNECTION_H
