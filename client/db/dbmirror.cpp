//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbmirror.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================


//====================================================================================

#include "dbmirror.h"


extern cQTLogger g_obLogger;
extern BelenusServerConnection *g_poServer;



//====================================================================================
cDBMirror::cDBMirror()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::cDBMirror] called" << EOM;
}
//====================================================================================
cDBMirror::~cDBMirror()
//====================================================================================
{

}
//====================================================================================


/**
 * initialize()
 * Creates the qt-connection to the serverconnection object.
 * Should be called after the global g_poServer is allocated.
 */
void cDBMirror::initialize()
{
    connect(g_poServer, SIGNAL(queryReady(int, SqlResult*)), this, SLOT(queryReady(int, SqlResult*)), Qt::QueuedConnection);
}



void cDBMirror::start()
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] start called" << EOM;
    if ( g_poServer->getStatus()!=BelenusServerConnection::AUTHENTICATED ) {
        g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] not connected to server" << EOM;
        return;
    }

    _qId = g_poServer->sendQuery("SELECT lastLogin FROM clients;");
}


void cDBMirror::queryReady(int id, SqlResult *r)
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] id=" << id << " result has " << r->rowCount() << " rows." << EOM;
    if ( id==_qId ) {
        if ( r->rowCount()>0 )
            g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] data[0,0] = '" << r->index(0,0).data().toString() << "'" << EOM;
        delete r;
    }
}