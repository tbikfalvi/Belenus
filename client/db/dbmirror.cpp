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
#include "dbuser.h"
#include "dbpatientorigin.h"

//====================================================================================

extern cQTLogger                g_obLogger;
extern BelenusServerConnection *g_poServer;

//====================================================================================
cDBMirror::cDBMirror()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::cDBMirror] called" << EOM;

    m_bGetGlobalData        = false;
    m_inProcessCount        = 0;
    m_bSyncAllTable         = false;
    m_uiDbModificationLevel = 0;
    m_uiCurrentId           = 0;
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
bool cDBMirror::start()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] start called" << EOM;

    if ( g_poServer->getStatus()!=BelenusServerConnection::AUTHENTICATED )
    {
        g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] not connected to server" << EOM;
        return false;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
        updateSynchronizationLevel( DB_USER );

    m_bGetGlobalData = true;

    return true;
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
            {
                if( r->isValid() )
                {
                    g_poDB->executeQTQuery( QString( "UPDATE licences SET archive=\"ARC\" WHERE licenceId=%1" ).arg(g_poPrefs->getLicenceId()) );
                    m_inProcessCount = 0;
                }
                break;
            }

            case MIRROR_SYNC_DB_USER:
                _recordUserSynchronized();
                break;

            case MIRROR_SYNC_DB_PATIENTORIGIN:
                _recordPatientOriginSynchronized();
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
void cDBMirror::updateSynchronizationLevel( unsigned int p_uiSyncLevel )
//====================================================================================
{
    m_uiDbModificationLevel |= p_uiSyncLevel;
}
//====================================================================================
void cDBMirror::_tableSynchronized( unsigned int p_uiSyncLevel )
//====================================================================================
{
    m_uiDbModificationLevel &= ~p_uiSyncLevel;
}
//====================================================================================
//
//  User table synchronization
//
//====================================================================================
void cDBMirror::synchronizeUserTable()
//====================================================================================
{
    _synchronizeUserTable();
}
//====================================================================================
void cDBMirror::_synchronizeUserTable( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString     qsQuery = "";
        cDBUser     obUser;

        obUser.load( m_uiCurrentId );

        if( obUser.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO users SET ";
            qsQuery += QString( "userId = \"%1\", " ).arg( obUser.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obUser.licenceId() );
        }
        else if( obUser.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE users SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg(  obUser.name() );
        qsQuery += QString( "realName = \"%1\", " ).arg(  obUser.realName() );
        qsQuery += QString( "password = \"%1\", " ).arg(  obUser.password() );
        qsQuery += QString( "accgroup = %1, " ).arg(  obUser.group() );
        qsQuery += QString( "active = %1, " ).arg(  obUser.active() );
        qsQuery += QString( "comment = \"%1\", " ).arg(  obUser.comment() );
        qsQuery += QString( "archive = \"ARC\"" );
        if( obUser.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "userId = \"%1\" " ).arg( obUser.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obUser.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_USER;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatientOriginTable();
    }
}
//====================================================================================
void cDBMirror::_recordUserSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE users SET archive=\"ARC\" WHERE userId=%1" ).arg(m_uiCurrentId) );

    _synchronizeUserTable();
}
//====================================================================================
//
//  Patient origin table synchronization
//
//====================================================================================
void cDBMirror::synchronizePatientOriginTable()
//====================================================================================
{
    _synchronizePatientOriginTable();
}
//====================================================================================
void cDBMirror::_synchronizePatientOriginTable( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientOrigin WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPatientOrigin    obPatientOrigin;

        obPatientOrigin.load( m_uiCurrentId );

        if( obPatientOrigin.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO patientOrigin SET ";
            qsQuery += QString( "patientOriginId = \"%1\", " ).arg( obPatientOrigin.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPatientOrigin.licenceId() );
        }
        else if( obPatientOrigin.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patientOrigin SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obPatientOrigin.name() );
        qsQuery += QString( "active = %1, " ).arg( obPatientOrigin.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPatientOrigin.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientOriginId = \"%1\" " ).arg( obPatientOrigin.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPatientOrigin.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PATIENTORIGIN;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeReasonToVisit();
    }
}
//====================================================================================
void cDBMirror::_recordPatientOriginSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE patientOrigin SET archive=\"ARC\" WHERE patientOriginId=%1" ).arg(m_uiCurrentId) );

    _synchronizePatientOriginTable();
}
//====================================================================================
//
//  Reason to visit table synchronization
//
//====================================================================================
void cDBMirror::synchronizeReasonToVisit()
//====================================================================================
{
//    _synchronizeReasonToVisit();
}
//====================================================================================
