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

#include "belenus.h"
#include "dbmirror.h"

//====================================================================================

extern cQTLogger                g_obLogger;
extern BelenusServerConnection *g_poServer;

//====================================================================================
cDBMirror::cDBMirror()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::cDBMirror] called" << EOM;

    m_bGetGlobalData    = 0;
    m_inProcessCount    = 0;
}
//====================================================================================
cDBMirror::~cDBMirror()
//====================================================================================
{

}
//====================================================================================
// Creates the qt-connection to the serverconnection object.
// Should be called after the global g_poServer is allocated.
//====================================================================================
void cDBMirror::initialize()
//====================================================================================
{
    connect(g_poServer, SIGNAL(queryReady(int, SqlResult*)), this, SLOT(queryReady(int, SqlResult*)), Qt::QueuedConnection);
}
//====================================================================================
// Start database syncronization. Get data linked to licenceId 0
//====================================================================================
void cDBMirror::start()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] start called" << EOM;
    if ( g_poServer->getStatus()!=BelenusServerConnection::AUTHENTICATED )
    {
        g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] not connected to server" << EOM;
        return;
    }

    m_bGetGlobalData = true;
//    _qId = g_poServer->sendQuery("SELECT lastLogin FROM clients;");
}
//====================================================================================
void cDBMirror::queryReady( int id, SqlResult *r )
//====================================================================================
{
    //g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] id=" << id << " result has " << r->rowCount() << " rows." << EOM;

    if( id == _qId )
    {
        switch( m_inProcessCount )
        {
            case MIRROR_UPDATE_LICENCE_DATA:
                if( r->isValid() )
                {
                    g_poDB->executeQTQuery( QString( "UPDATE licences SET archive=\"ARC\" WHERE licenceId=%1" ).arg(g_poPrefs->getLicenceId()) );
                }
                break;

            default:
                break;
        }

        if( r->rowCount() > 0 )
        {
            g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] data[0,0] = '" << r->index(0,0).data().toString() << "'" << EOM;
        }
        delete r;
    }
}
//====================================================================================
void cDBMirror::updateLicenceData()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::updateLicenceData]" << EOM;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences WHERE licenceId=%1" ).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        QString qsQuery = "";

        qsQuery += QString( "UPDATE licences SET " );
        qsQuery += QString( "country = \"%1\", " ).arg( poQuery->value( 2 ).toString() );
        qsQuery += QString( "region = \"%1\", " ).arg( poQuery->value( 3 ).toString() );
        qsQuery += QString( "city = \"%1\", " ).arg( poQuery->value( 4 ).toString() );
        qsQuery += QString( "zip = \"%1\", " ).arg( poQuery->value( 5 ).toString() );
        qsQuery += QString( "address = \"%1\", " ).arg( poQuery->value( 6 ).toString() );
        qsQuery += QString( "studio = \"%1\", " ).arg( poQuery->value( 7 ).toString() );
        qsQuery += QString( "contact = \"%1\", " ).arg( poQuery->value( 8 ).toString() );
        qsQuery += QString( "lastValidated = \"%1\" " ).arg( poQuery->value( 9 ).toString() );
        qsQuery += QString( "WHERE licenceId = %1" ).arg( poQuery->value( 0 ).toString() );

        m_inProcessCount = MIRROR_UPDATE_LICENCE_DATA;

        _qId = g_poServer->sendQuery( qsQuery );
    }
}
//====================================================================================
