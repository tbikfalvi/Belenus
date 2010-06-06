
#include "adminClientThread.h"
#include "../framework/qtlogger.h"

extern cQTLogger g_obLogger;


AdminClientThread::AdminClientThread()
    :   Connection(),
        _loggedIn(false)
{
}



void AdminClientThread::setCredentials(QString username, QString password)
{
    _username = username;
    _password = password;
}



void AdminClientThread::_initialize()
{
    _sendHello(VERSION);
}



void AdminClientThread::_handleHello(int version)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleHello] server version is " << version << cQTLogger::EOM;
}



void AdminClientThread::_handleLogonChallenge()
{
    _sendLogonAdminResponse(_username.toStdString().c_str(), QString(QCryptographicHash::hash(_password.toAscii(), QCryptographicHash::Sha1).toHex()).toStdString().c_str() );
}



void AdminClientThread::_handleRegisterKeyResponse(Result res)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleRegisterKeyResult] result is " << res << cQTLogger::EOM;
}



void AdminClientThread::_handleLogonOk()
{
    _loggedIn = true;
}



void AdminClientThread::registerNewKey(const char *key)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::registerNewKey] key to register is " << key << cQTLogger::EOM;
    if ( !_loggedIn )
        return;

    _sendRegisterKey(key);
}



void AdminClientThread::executeSqlQuery(int id, const char *q)
{
    _sendSqlQuery(id, q);
}
