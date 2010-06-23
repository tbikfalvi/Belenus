
#include "bs_connection.h"


extern cQTLogger* g_obLogger;



BelenusServerConnection::BelenusServerConnection()
    : _serial(""),
      _code2(""),
      _authenticated(false)
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
    _sendHello();
}



void BelenusServerConnection::_handleLogonChallenge()
{
    _sendLogonResponse(_serial.toStdString().c_str(), _code2.toStdString().c_str());
}



void BelenusServerConnection::_handleLogonOk()
{
    _authenticated = true;

}
