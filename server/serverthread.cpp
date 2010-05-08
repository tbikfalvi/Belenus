
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


void ServerThread::_handleHello(Packet &)
{
    g_obLogger(cSeverity::DEBUG) << "[ServerThread::_handleHello] called" << cQTLogger::EOM;

    Packet _p(Packet::MSG_HELLO);
    _p << (int) VERSION;
    send(_p);

    int version = 0;
    // _p >> version;
    if ( version!=VERSION ) {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] client has invalid version " << version << cQTLogger::EOM;
        Packet p(Packet::MSG_VERSION_MISMATCH);
        send(p);
    } else {
        g_obLogger(cSeverity::INFO) << "[ServerThread::_handleHello] sending logon challenge to client" << version << cQTLogger::EOM;
        Packet p(Packet::MSG_LOGON_CHALLENGE);
        send(p);
    }
}



