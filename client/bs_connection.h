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
class BelenusServerConnection : public QThread, CommunicationProtocol
{
    Q_OBJECT

public:
    enum ConnectionStatus {
        NOT_CONNECTED,
        CONNECTING,
        AUTHENTICATED,      // server connection is ok
        LICENSE_FAILED,     // connected to server but license authentication failed
        CONNECTION_FAILED,  // eg. socket error
    };


    BelenusServerConnection();
    virtual ~BelenusServerConnection();

    void connectTo(const QHostAddress adr, int port);

    void setLoginKeys(QString serial, QString code2);
    ConnectionStatus    getStatus() { return _status; }


signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

protected slots:
    void _error(QAbstractSocket::SocketError);
    void _disconnected();
    void _connected();
    virtual void _read() { CommunicationProtocol::read(); } /* slots cannot be overloaded */

protected:
    virtual void run();

    void _handleLogonChallenge();
    void _handleLogonOk();

private:
    QString _serial;
    QString _code2;
    ConnectionStatus _status;
};



#endif // BS_CONNECTION_H
