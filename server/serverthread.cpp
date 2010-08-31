#include <QHostAddress>
#include "../framework/qtlogger.h"
#include "../framework/qtmysqlconnection.h"
#include "serverthread.h"


extern cQTLogger g_obLogger;
extern cQTMySQLConnection g_db;



ServerThread::ServerThread(QTcpSocket *tcpSocket) :
        CommunicationProtocol(tcpSocket),
        _isAuthenticated(false),
        _isAdmin(false)
{
    g_obLogger(cSeverity::INFO) << "[ServerThread::ServerThread] initialized" << EOM;

    connect( this,       SIGNAL(finished()),                            this, SLOT(deleteLater()));
}



ServerThread::~ServerThread()
{
}



void ServerThread::read()
{
    g_obLogger(cSeverity::DEBUG) << "[serverthread::read]" << EOM;
    CommunicationProtocol::read();
}



void ServerThread::_handleHello(int version)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleHello] called" << EOM;

    sendHello();

    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client version is " << version << EOM;
    if ( version != VERSION ) {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client has invalid version" << EOM;
        sendDisconnect(Result::INVALID_VERSION);
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] version ok. sending logon challenge to client" << EOM;
        sendLogonChallenge();
    }
}



void ServerThread::_handleLogonResponse(const char* code1, const char* code2)
{
    int clientId;

    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonResponse] code1=" << code1 << " code2=" << code2 << EOM;

    QSqlQuery *q = g_db.executeQTQuery(QString("SELECT clientId, code2 FROM clients WHERE code1='%1' LIMIT 2;").arg(code1));
    if ( q->size()==1 ) {
        q->first();
        clientId = q->value(0).toInt();
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonResponse] clientId is " << clientId << EOM;

        // if code2 is null (or empty), user not logined yet, so store its key for next use
        // otherwise check if code2 matches with the stored one
        if ( !(q->value(1).isNull() || q->value(1).toString().isEmpty()) && QString::compare(q->value(1).toString(), code2, Qt::CaseInsensitive)!=0 ) {
            sendDisconnect(Result::INVALID_SECOND_ID);
        } else {
            // update lastlogin and set code2
            QSqlQuery *r = g_db.executeQTQuery(QString("UPDATE clients SET code2='%1', lastLogin=NOW() WHERE clientId='%2'").arg(code2).arg(clientId));
            delete r;

            sendLogonOk();
            _isAuthenticated = true;
        }
    } else {
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonResponse] no active client has been found" << EOM;
        sendDisconnect(Result::INVALID_LICENSE_KEY);
    }

    delete q;
}



void ServerThread::_handleLogonAdminResponse(const char* username, const char* password)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonAdminResponse] entering" << EOM;

    if ( QString::compare(username, "root")!=0 || QString::compare(password, "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922")!=0 ) {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonAdminResponse] admin authentication failed" << EOM;
        sendDisconnect(Result::AUTHENTICATION_FAILED);
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonAdminResponse] admin authenticated" << EOM;
        sendLogonOk();
        _isAdmin = true;
        _isAuthenticated = true;
    }
}



void ServerThread::_handleRegisterKey(const char* newKey)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleRegisterKey] requesting to register new key: " << newKey << EOM;
    if ( !_isAdmin ) {
        g_obLogger(cSeverity::ERROR) << "[ServerThread::_handleRegisterKey] requesting new key(" << newKey << ") registration while user is not admin! Disconnecting." << EOM;
        sendDisconnect(Result::AUTHENTICATION_FAILED);
        return;
    }

    // check if there is no key already registered and insert
    QSqlQuery *q = g_db.executeQTQuery(QString("SELECT clientId FROM clients WHERE code1=SHA1('%1') LIMIT 1;").arg(newKey) );
    if ( q->size()!=0 ) {
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleRegisterKey] code alread registered" << EOM;

        sendRegisterKeyResponse(Result::ALREADY_REGISTERED);
    } else {
        QSqlQuery *r = g_db.executeQTQuery( QString("INSERT INTO clients(code1, dateCreated, lastLogin) VALUES(SHA1('%1'),Now(), Now());").arg(newKey) );
        if ( !r->numRowsAffected() ) {
            g_obLogger(cSeverity::ERROR) << "[ServerThread::_handleRegisterKey] new key was not inserted" << EOM;
            sendRegisterKeyResponse(Result::UNKNOWN);
        } else {
            // key inserted
            sendRegisterKeyResponse(Result::OK);
        }

        delete r;
    }

    delete q;
}



void ServerThread::_handleSqlQuery(int queryId, const char *query)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleSqlQuery] entered. QueryId="<<queryId<<" query=" << query << EOM;

    SqlResult result;

    try {
        QSqlQuery *q = g_db.executeQTQuery(query);
        if (!q)
            throw cSevException(cSeverity::ERROR, "Memory error during query");

        result.copy(q);
        delete q;

    } catch(cSevException e) {
        g_obLogger(cSeverity::ERROR) << "[ServerThread::_handleSqlQuery] error at queryId="<<queryId<<" query=" << query << ". error:" << e.what() << EOM;
        result.setValid(false);
    }

    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleSqlQuery] valid=" << result.isValid() << EOM;
    sendSqlQueryResult(queryId, result);
}


void ServerThread::run()
{
    if ( !m_socket )
        return;

    g_obLogger(cSeverity::DEBUG) << "[ServerThread::run] entered" << EOM;

    m_socket->setParent(this);

    connect( m_socket,   SIGNAL(disconnected()),                        this, SLOT(disconnected()) );
    connect( m_socket,   SIGNAL(error(QAbstractSocket::SocketError)),   this, SLOT(error(QAbstractSocket::SocketError)) );
    connect( m_socket,   SIGNAL(readyRead()),                           this, SLOT(read()) );

    exec(); // start event processing loop
}



void ServerThread::error(QAbstractSocket::SocketError err)
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

    g_obLogger(cSeverity::ERROR) << "[Serverthread::error] (" << err <<") " << errStr << EOM;
}



void ServerThread::disconnected()
{
    g_obLogger(cSeverity::INFO) << "[Serverthread::disconnected] " << m_socket->peerAddress().toString() << ":" << m_socket->peerPort() << EOM;
}