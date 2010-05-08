
#include "adminClientThread.h"
#include "../framework/qtlogger.h"

extern cQTLogger g_obLogger;


AdminClientThread::AdminClientThread()
    :   Connection()
{
}



void AdminClientThread::_initialize()
{
    Packet msg(Packet::MSG_HELLO);
    msg << (int)1;

    send(msg);
}



void AdminClientThread::_handleHello(Packet &)
{
    g_obLogger << cSeverity::INFO << "[AdminClientThread::AdminClientThread] handleHello" << cQTLogger::EOM;
}
