
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
    _sendHello();
}



void AdminClientThread::_handleHello(int version)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleHello] server version is " << version << EOM;
}



void AdminClientThread::_handleLogonChallenge()
{
    _sendLogonAdminResponse(_username.toStdString().c_str(), QString(QCryptographicHash::hash(_password.toAscii(), QCryptographicHash::Sha1).toHex()).toStdString().c_str() );
}



void AdminClientThread::_handleRegisterKeyResponse(Result res)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleRegisterKeyResult] result is " << res << EOM;
    queryLicenseKeys();
}



void AdminClientThread::_handleLogonOk()
{
    _loggedIn = true;
}



void AdminClientThread::registerNewKey(const char *key)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::registerNewKey] key to register is " << key << EOM;
    if ( !_loggedIn )
        return;

    _sendRegisterKey(key);
}



void AdminClientThread::queryLicenseKeys()
{
    if ( !_loggedIn )
        return;

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::queryLicenseKeys] entered " << EOM;
    _sendSqlQuery(Q_GET_KEYS, "SELECT clientId, code1, code2, dateCreated, lastLogin, licenceId, serial, country, region, city, zip, address, studio, contact, active FROM clients LEFT JOIN licences ON clients.code1=SHA1(serial)");
}



void AdminClientThread::resetCode2(int clientId)
{
    if ( !_loggedIn )
        return;

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::resetCode2] reseting code2 of client " << clientId << EOM;
    _sendSqlQuery(Q_RESET_CODE2, QString("UPDATE clients SET code2=NULL WHERE clientId=%1").arg(clientId).toStdString().c_str());
}



void AdminClientThread::queryLogs(cSeverity::teSeverity minSeverity, int last)
{
    if ( !_loggedIn )
        return;

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::queryLogs] getting logs from server sev=" << (int)minSeverity << " last=" << last << EOM;
    _sendSqlQuery(Q_GET_LOGS, QString("SELECT * FROM logs WHERE severity<=%1 ORDER BY date DESC LIMIT %2").arg(minSeverity).arg(last).toStdString().c_str());
}



void AdminClientThread::removeKey(int clientId)
{
    if ( !_loggedIn )
        return;

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::removeKey] removing license key for client " << clientId << EOM;
    _sendSqlQuery(Q_RESET_CODE2, QString("DELETE FROM clients WHERE clientID=%1").arg(clientId).toStdString().c_str());
}
