
#include "../framework/qtlogger.h"
#include "serverthread.h"


extern cQTLogger g_obLogger;




ServerThread::ServerThread() :
        Connection()
{
    g_obLogger(cSeverity::INFO) << "[ServerThread::ServerThread] initialized" << cQTLogger::EOM;
}



ServerThread::~ServerThread()
{
}


void ServerThread::_handleHello(Packet &packet)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleHello] called" << cQTLogger::EOM;

    Packet _p(Packet::MSG_HELLO);
    _p << (int) VERSION;
    send(_p);

    int version = 0;
    packet >> version;
    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client version is " << version << cQTLogger::EOM;
    if ( version != VERSION ) {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client has invalid version" << cQTLogger::EOM;
        Packet p(Packet::MSG_VERSION_MISMATCH);
        send(p);
        _socket->disconnectFromHost();
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] version ok. sending logon challenge to client" << cQTLogger::EOM;
        Packet p(Packet::MSG_LOGON_CHALLENGE);
        send(p);

    }
}



void ServerThread::_handleLogonResponse(Packet &packet)
{
    _assertSize(4, packet, MINIMUM);
}



void ServerThread::_handleLogonAdminResponse(Packet &packet)
{
    _assertSize(2, packet, MINIMUM);
    char *username = 0, *password = 0;

    packet >> username >> password;
    g_obLogger(cSeverity::INFO) << "[ServerThread::_handleLogonAdminResponse] username=" << username << " password=" << password << cQTLogger::EOM;

    Packet p(Packet::MSG_LOGON_OK);
    send(p);
}
