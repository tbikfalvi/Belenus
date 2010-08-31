
#include "bs_connection.h"


extern cQTLogger g_obLogger;



BelenusServerConnection::BelenusServerConnection()
    : _serial(""),
      _code2(""),
      _status(NOT_CONNECTED),
      _lastResult(Result::OK),
      _isLicenseValid(false)
{
    connect( this,       SIGNAL(finished()),                            this, SLOT(deleteLater()));
}



BelenusServerConnection::~BelenusServerConnection()
{
}



void BelenusServerConnection::setLoginKeys(const QString serial, const QString code2)
{
    _serial = QCryptographicHash::hash(serial.toAscii(), QCryptographicHash::Sha1).toHex();
    _code2 = code2;
}



void BelenusServerConnection::_handleLogonChallenge()
{
    sendLogonResponse(_serial.toStdString().c_str(), _code2.toStdString().c_str());
}



void BelenusServerConnection::_handleLogonOk()
{
    _status = AUTHENTICATED;
    _isLicenseValid = true;
}



void BelenusServerConnection::run()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::run] connection thread started" << EOM;

    exec();
}



void BelenusServerConnection::connectTo(const QHostAddress addr, int port)
{
    if (m_socket) {
        g_obLogger(cSeverity::ERROR) << "[BelenusServerConnection::connectTo] m_socket should be closed before new connection attempt" << EOM;
        return;
    }

    QTcpSocket *socket = new QTcpSocket();
    setTcpConnection(socket);

    connect( m_socket,   SIGNAL(connected()),                           this, SLOT(_connected()) );
    connect( m_socket,   SIGNAL(disconnected()),                        this, SLOT(_disconnected()) );
    connect( m_socket,   SIGNAL(error(QAbstractSocket::SocketError)),   this, SLOT(_error(QAbstractSocket::SocketError)) );
    connect( m_socket,   SIGNAL(readyRead()),                           this, SLOT(_read()) );

    m_socket->connectToHost(addr, port);
    m_socket->moveToThread(this);
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::connectTo] status is CONNECTING" << EOM;
    _status = CONNECTING;
    _isLicenseValid = false;
}




void BelenusServerConnection::_connected()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::connected] entered" << EOM;
    sendHello();

    emit connected();
}



void BelenusServerConnection::_error(QAbstractSocket::SocketError err)
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

    g_obLogger(cSeverity::INFO) << "[BelenusServerConnection::_error] " << err << ": " << errStr << EOM;
    m_socket->deleteLater();
    m_socket = 0;
    _status = CONNECTION_FAILED;
    emit error(err);
}



void BelenusServerConnection::_disconnected()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::disconnected] " << EOM;
    m_socket->deleteLater();
    m_socket = 0;
    _status = CONNECTION_FAILED;
    emit disconnected();
}



void BelenusServerConnection::_handleDisconnect(Result::ResultCode reason)
{
    _lastResult = reason;
    _status = CONNECTION_FAILED;
}