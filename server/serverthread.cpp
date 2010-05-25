
#include "../framework/qtlogger.h"
#include "../framework/qtmysqlconnection.h"
#include "serverthread.h"


extern cQTLogger g_obLogger;
extern cQTMySQLConnection g_db;



ServerThread::ServerThread() :
        Connection(),
        _isAdmin(false),
        _isAuthenticated(false)
{
    g_obLogger(cSeverity::INFO) << "[ServerThread::ServerThread] initialized" << cQTLogger::EOM;
}



ServerThread::~ServerThread()
{
}


void ServerThread::_handleHello(int version)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleHello] called" << cQTLogger::EOM;

    _sendHello(VERSION);

    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client version is " << version << cQTLogger::EOM;
    if ( version != VERSION ) {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client has invalid version" << cQTLogger::EOM;
        _sendDisconnect(RESULT_INVALID_VERSION);
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] version ok. sending logon challenge to client" << cQTLogger::EOM;
        _sendLogonChallenge();
    }
}



void ServerThread::_handleLogonResponse(const char* code1, const char* code2)
{
    int clientId;

    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonResponse] code1=" << code1 << " code2=" << code2 << cQTLogger::EOM;

    QSqlQuery *q = g_db.executeQTQuery(QString("SELECT clientId, code2 FROM clients WHERE code1='%1' AND active=1 LIMIT 2;").arg(code1));
    if ( q->size()==1 ) {
        clientId = q->value(0).toInt();
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonResponse] clientId is " << clientId << cQTLogger::EOM;

        // if code2 is null, user not logined yet, so store its key for next use
        // otherwise check if code2 matches with the stored one
        if ( !q->value(1).isNull() && QString::compare(q->value(1).toString(), code2, Qt::CaseInsensitive)!=0 ) {
            _sendDisconnect(RESULT_INVALID_SECOND_ID);
        } else {
            // update lastlogin and set code2
            QSqlQuery *r = g_db.executeQTQuery(QString("UPDATE clients SET code2='%1', lastLogin=NOW() WHERE clientId='%2'").arg(code2).arg(clientId));
            delete r;

            _sendLogonOk();
            _isAuthenticated = true;
        }
    } else {
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonResponse] no active client has been found" << cQTLogger::EOM;
        _sendDisconnect(RESULT_INVALID_LICENSE_KEY);
    }

    delete q;
}



void ServerThread::_handleLogonAdminResponse(const char* username, const char* password)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleLogonAdminResponse] entering" << cQTLogger::EOM;

    if ( QString::compare(username, "root")!=0 || QString::compare(password, "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922")!=0 ) {
        _sendDisconnect(RESULT_AUTHENTICATION_FAILED);
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonAdminResponse] admin authenticated" << cQTLogger::EOM;
        _sendLogonOk();
        _isAdmin = true;
        _isAuthenticated = true;
    }
}



void ServerThread::_handleRegisterKey(const char* newKey)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleRegisterKey] requesting to register new key: " << newKey << cQTLogger::EOM;
    if ( !_isAdmin ) {
        g_obLogger(cSeverity::ERROR) << "[ServerThread::_handleRegisterKey] requesting new key(" << newKey << ") registration while user is not admin! Disconnecting." << cQTLogger::EOM;
        _sendDisconnect(RESULT_AUTHENTICATION_FAILED);
        return;
    }

    // check if there is no key already registered and insert
    QSqlQuery *q = g_db.executeQTQuery(QString("SELECT clientId FROM clients WHERE code1=SHA1('%1') LIMIT 1;").arg(newKey) );
    if ( q->size()!=0 ) {
        g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleRegisterKey] code alread registered" << cQTLogger::EOM;

        _sendRegisterKeyResponse(RESULT_ALREADY_REGISTERED);
    } else {
        QSqlQuery *r = g_db.executeQTQuery( QString("INSERT INTO clients(code1, dateCreated, lastLogin) VALUES(SHA1('%1'),Now(), Now());").arg(newKey) );
        if ( !r->numRowsAffected() ) {
            g_obLogger(cSeverity::ERROR) << "[ServerThread::_handleRegisterKey] new key was not inserted" << cQTLogger::EOM;
            _sendRegisterKeyResponse(RESULT_UNKOWN);
        } else {
            // key inserted
            _sendRegisterKeyResponse(RESULT_OK);
        }

        delete r;
    }

    delete q;
}



