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

    BelenusServerConnection();
    virtual ~BelenusServerConnection();

    void setLoginKeys(QString serial, QString code2);

private:
    void _initialize();
    void _handleLogonChallenge();
    void _handleLogonOk();

private:
    QString _serial;
    QString _code2;
    bool _authenticated;
};



#endif // BS_CONNECTION_H
