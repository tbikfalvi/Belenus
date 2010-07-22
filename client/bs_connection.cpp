
#include "bs_connection.h"


extern cQTLogger* g_obLogger;



BelenusServerConnection::BelenusServerConnection()
    : _serial(""),
      _code2(""),
      _status(NOT_CONNECTED)
{
}



BelenusServerConnection::~BelenusServerConnection()
{
}



void BelenusServerConnection::setLoginKeys(QString serial, QString code2)
{
    _serial = serial;
    _code2 = code2;
}



void BelenusServerConnection::_initialize()
{
    _status = CONNECTING;
    _sendHello();
}



void BelenusServerConnection::_handleLogonChallenge()
{
    _sendLogonResponse(_serial.toStdString().c_str(), _code2.toStdString().c_str());
}



void BelenusServerConnection::_handleLogonOk()
{
    _status = AUTHENTICATED;
}
