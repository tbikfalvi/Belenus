
#include <QCryptographicHash>
#include "adminClientThread.h"
#include "../framework/qtlogger.h"

extern cQTLogger g_obLogger;


AdminClientThread::AdminClientThread()
    :   CommunicationProtocol(),
        _loggedIn(false)
{
    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()));

    qRegisterMetaType<cSeverity::teSeverity>("cSeverity::teSeverity");
    qRegisterMetaType<Result::ResultCode>("Result::ResultCode");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}



void AdminClientThread::setCredentials(QString username, QString password)
{
    _username = username;
    _password = password;
}



void AdminClientThread::_handleHello(int version)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleHello] server version is " << version << EOM;
}



void AdminClientThread::_handleLogonChallenge()
{
    sendLogonAdminResponse(_username.toStdString().c_str(), QString(QCryptographicHash::hash(_password.toAscii(), QCryptographicHash::Sha1).toHex()).toStdString().c_str() );
}



void AdminClientThread::_handleRegisterKeyResponse(Result::ResultCode res)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleRegisterKeyResult] result is " << res << EOM;
    emit registerKeyResponse(res);
}



void AdminClientThread::_handleLogonResult(Result::ResultCode res, int /*clientId*/)
{
    if ( res == Result::OK ) {
        g_obLogger(cSeverity::INFO) << "[AdminClientThread::_handleLogonResult] logged in successfully" << EOM;
        _loggedIn = true;
    } else
        g_obLogger(cSeverity::INFO) << "[AdminClientThread::_handleLogonResult] res = " << res << EOM;
}



void AdminClientThread::registerNewKey(const QString key)
{
    QMetaObject::invokeMethod(this, "_registerNewKey", Qt::QueuedConnection, Q_ARG(QString, key) );
}



void AdminClientThread::_registerNewKey(QString key)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_registerNewKey] key to register is " << key << EOM;
    if ( !_loggedIn ) {
        g_obLogger(cSeverity::ERROR) << "Not logged in yet" << EOM;
        return;
    }

    sendRegisterKey(key.toStdString().c_str());
}



void AdminClientThread::queryLicenseKeys()
{
    QMetaObject::invokeMethod(this, "_queryLicenseKeys", Qt::QueuedConnection );
}



void AdminClientThread::_queryLicenseKeys()
{
    if ( !_loggedIn ) {
        g_obLogger(cSeverity::ERROR) << "Not logged in yet" << EOM;
        return;
    }

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::queryLicenseKeys] entered " << EOM;
    sendSqlQuery(Q_GET_KEYS, "SELECT clientId, code1, code2, dateCreated, lastLogin, licenceId, serial, country, region, city, zip, address, studio, contact, active FROM clients LEFT JOIN licences ON clients.code1=SHA1(serial)");
}



void AdminClientThread::resetCode2(int clientId)
{
    QMetaObject::invokeMethod(this, "_resetCode2", Qt::QueuedConnection, Q_ARG(int,clientId) );
}



void AdminClientThread::_resetCode2(int clientId)
{
    if ( !_loggedIn ) {
        g_obLogger(cSeverity::ERROR) << "Not logged in yet" << EOM;
        return;
    }

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::resetCode2] reseting code2 of client " << clientId << EOM;
    sendSqlQuery(Q_RESET_CODE2, QString("UPDATE clients SET code2=NULL WHERE clientId=%1").arg(clientId).toStdString().c_str());
}



void AdminClientThread::queryLogs(cSeverity::teSeverity minSeverity, int last)
{
    QMetaObject::invokeMethod(this, "_queryLogs", Qt::QueuedConnection, Q_ARG(cSeverity::teSeverity,minSeverity), Q_ARG(int,last) );
}



void AdminClientThread::_queryLogs(cSeverity::teSeverity minSeverity, int last)
{
    if ( !_loggedIn ) {
        g_obLogger(cSeverity::ERROR) << "Not logged in yet" << EOM;
        return;
    }

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::queryLogs] getting logs from server sev=" << (int)minSeverity << " last=" << last << EOM;
    sendSqlQuery(Q_GET_LOGS, QString("SELECT * FROM logs WHERE severity<=%1 ORDER BY date DESC LIMIT %2").arg(minSeverity).arg(last).toStdString().c_str());
}



void AdminClientThread::removeKey(int clientId)
{
    QMetaObject::invokeMethod(this, "_removeKey", Qt::QueuedConnection, Q_ARG(int,clientId) );
}



void AdminClientThread::_removeKey(int clientId)
{
    if ( !_loggedIn ) {
        g_obLogger(cSeverity::ERROR) << "Not logged in yet" << EOM;
        return;
    }

    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_removeKey] removing license key for client " << clientId << EOM;
    sendSqlQuery(Q_REMOVE_LICENSE_KEY, QString("DELETE FROM clients WHERE clientID=%1").arg(clientId).toStdString().c_str());
}



void AdminClientThread::run()
{
    exec();
}



void AdminClientThread::connectTo(const QString addr, int port)
{
    QMetaObject::invokeMethod(this, "_connectTo", Qt::QueuedConnection, Q_ARG(QString,addr), Q_ARG(int, port) );
}


/*
  this is called only the thread of adminclientthread.
 */
void AdminClientThread::_connectTo(const QString addr, int port)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::connectTo] entering. connect to " << addr << ":" << port << EOM;

    if (m_socket) {
        g_obLogger(cSeverity::ERROR) << "[AdminClientThread::connectTo] m_socket should be closed before new connection attempt" << EOM;
        return;
    }

    QTcpSocket *socket = new QTcpSocket();
    setTcpConnection(socket);

    connect( m_socket,   SIGNAL(connected()),                           SLOT(_connected()) );
    connect( m_socket,   SIGNAL(disconnected()),                        SLOT(_disconnected()) );
    connect( m_socket,   SIGNAL(error(QAbstractSocket::SocketError)),   SLOT(_error(QAbstractSocket::SocketError)) );
    connect( m_socket,   SIGNAL(readyRead()),                           SLOT(_read()) );

    m_socket->connectToHost(addr, port);
}



void AdminClientThread::_connected()
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::connected] entered" << EOM;
    sendHello();

    emit connected();
}



void AdminClientThread::_error(QAbstractSocket::SocketError err)
{
    QString errStr;
    switch (err) {
        case 0: errStr = "Connection refused"; break;
        case 1: errStr = "Remote host closed the connection"; break;
        case 2: errStr = "Host not found"; break;
        case 3: errStr = "Socket access error"; break;
        case 4: errStr = "Socket resource error"; break;
        case 5: errStr = "Socket timeout"; break;
        case 7: errStr = "Network error"; break;
        default:errStr = "Unkown error"; break;
    }

    g_obLogger(cSeverity::WARNING) << "[AdminClientThread::_error] " << err << ": " << errStr << EOM;
    m_socket->deleteLater();
    m_socket = 0;
    emit error(err);
}



void AdminClientThread::_disconnected()
{
    g_obLogger(cSeverity::WARNING) << "[AdminClientThread::disconnected] " << EOM;
    m_socket->deleteLater();
    m_socket = 0;

    emit disconnected();
}



void AdminClientThread::_handleSqlQueryResult(int queryId, SqlResult *res)
{
    g_obLogger(cSeverity::DEBUG) << "[AdminClientThread::_handleSqlQueryResult] result for queryid "<< queryId << " has " << res->rowCount() << " rows. Affected=" << res->affectedRecords() << EOM;
    emit sqlResultReady(queryId, res);
}



void AdminClientThread::abort()
{
    if (m_socket) {
        m_socket->deleteLater();
        m_socket = 0;
        m_socket->abort();
    }
}
