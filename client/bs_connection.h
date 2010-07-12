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
#include "../framework/network/connection.h"
#include "../framework/qtlogger.h"



//====================================================================================
class BelenusServerConnection : public Connection
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

    void setLoginKeys(QString serial, QString code2);
    ConnectionStatus    getStatus() { return _status; }

private:
    void _initialize();
    void _handleLogonChallenge();
    void _handleLogonOk();

private:
    QString _serial;
    QString _code2;
    ConnectionStatus _status;
};



#endif // BS_CONNECTION_H
