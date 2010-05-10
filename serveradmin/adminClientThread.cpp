
#include "adminClientThread.h"
#include "../framework/qtlogger.h"

extern cQTLogger g_obLogger;


AdminClientThread::AdminClientThread()
    :   Connection()
{
}



void AdminClientThread::setCredentials(QString username, QString password)
{
    _username = username;
    _password = password;
}



void AdminClientThread::_initialize()
{
    Packet msg(Packet::MSG_HELLO);
    msg << (int) VERSION;

    send(msg);
}



void AdminClientThread::_handleHello(Packet &packet)
{
    _assertSize( 4, packet );
}



void AdminClientThread::_handleLogonChallenge(Packet &packet)
{
    _assertSize( 0, packet );

    Packet p(Packet::MSG_LOGON_ADMIN_RESPONSE);
    p << _username.toStdString() << _password.toStdString();
    send(p);
}
