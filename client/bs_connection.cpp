
#include "bs_connection.h"
#include "preferences.h"


extern cQTLogger g_obLogger;
extern cPreferences *g_poPrefs;


BelenusServerConnection::BelenusServerConnection()
    : _host(""),
      _port(0),
      _serial(""),
      _code2(""),
      _status(NOT_CONNECTED),
      _licenceResult(Result::UNKNOWN),
      _clientId(0),
      _queryIdCounter(-1)
{
    qRegisterMetaType<Result::ResultCode>("Result::ResultCode");
    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()) );
}



BelenusServerConnection::~BelenusServerConnection()
{
}



void BelenusServerConnection::setServerAddress(const QString addr, const int port)
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::setServerAddress] host=" << addr << " port=" << port << EOM;
    _host = addr;
    _port = port;
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



void BelenusServerConnection::_handleLogonResult(Result::ResultCode res, int licenceId)
{
    // licence auth ok
    if ( res == Result::OK )
        _status = AUTHENTICATED;
    _licenceResult = res;
    emit licenceStatusChanged(res, licenceId);
}



void BelenusServerConnection::run()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::run] connection thread started" << EOM;
    exec();
}



void BelenusServerConnection::connectTo()
{
    setServerAddress( g_poPrefs->getServerAddress(), g_poPrefs->getServerPort().toInt() );
    QMetaObject::invokeMethod(this, "_connectTo", Qt::QueuedConnection );
}



void BelenusServerConnection::_connectTo()
{
    if (m_socket) {
        g_obLogger(cSeverity::ERROR) << "[BelenusServerConnection::connectTo] m_socket should be closed before new connection attempt" << EOM;
        return;
    }

    if ( _host.isEmpty() || _port<1 || _port>65535 ) {
        g_obLogger(cSeverity::ERROR) << "[BelenusServerConnection::connectTo] invalid server address host=" << _host << " port=" << _port << EOM;
        return;
    }

    QTcpSocket *socket = new QTcpSocket();
    setTcpConnection(socket);

    connect( m_socket,   SIGNAL(connected()),                           this, SLOT(_connected()) );
    connect( m_socket,   SIGNAL(disconnected()),                        this, SLOT(_disconnected()) );
    connect( m_socket,   SIGNAL(error(QAbstractSocket::SocketError)),   this, SLOT(_error(QAbstractSocket::SocketError)) );
    connect( m_socket,   SIGNAL(readyRead()),                           this, SLOT(_read()) );

    m_socket->connectToHost(_host, _port);

    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::connectTo] status is CONNECTING" << EOM;
    _status = CONNECTING;
    // init licence to invalid
    _licenceResult = Result::INVALID_LICENSE_KEY;
}




void BelenusServerConnection::_connected()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::connected] entered" << EOM;
    _status = CONNECTED;
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
    _lastResult = Result::UNKNOWN;
    emit error(err);
}



void BelenusServerConnection::_disconnected()
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::disconnected] " << EOM;
    m_socket->deleteLater();
    m_socket = 0;
    _status = CONNECTION_FAILED;
    _lastResult = Result::UNKNOWN;
    emit disconnected();
}



void BelenusServerConnection::_handleDisconnect(Result::ResultCode reason)
{
    _lastResult = reason;
    // if disconnect occured because of licence reasons, store them
    if ( reason == Result::INVALID_SECOND_ID || reason == Result::INVALID_LICENSE_KEY )
        _licenceResult = reason;
}



bool BelenusServerConnection::isLicenseValid()
{
    return _licenceResult == Result::OK ;
}


int BelenusServerConnection::sendQuery(const QString query)
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::sendQuery] " << query << EOM;

    _queryIdCounterGuard.lock();
    _queryIdCounter++;
    int id = _queryIdCounter;
    _queryIdCounterGuard.unlock();
    QMetaObject::invokeMethod(this, "_sendQuery", Qt::QueuedConnection, Q_ARG(int, id), Q_ARG(QString,query) );
    return id;
}


void BelenusServerConnection::_sendQuery(int id, const QString query)
{
    sendSqlQuery(id, query.toAscii().constData());
}


void BelenusServerConnection::_handleSqlQueryResult(int queryId, SqlResult *res)
{
    g_obLogger(cSeverity::DEBUG) << "[BelenusServerConnection::_handleSqlQueryResult] id = " << queryId << EOM;
    emit queryReady(queryId, res);
}
