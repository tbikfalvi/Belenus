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
#include "dbreasontovisit.h"

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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
void cDBMirror::updateGlobalSyncLevel( unsigned int p_uiSyncLevel )
//====================================================================================
{
    m_uiGlobalSyncLevel |= p_uiSyncLevel;
}
//====================================================================================
void cDBMirror::_globalDataSynchronized( unsigned int p_uiSyncLevel )
//====================================================================================
{
    m_uiGlobalSyncLevel &= ~p_uiSyncLevel;
}
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientOrigin WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
void cDBMirror::synchronizeReasonToVisit()
//====================================================================================
{
    _synchronizeReasonToVisit();
}
//====================================================================================
void cDBMirror::_synchronizeReasonToVisit( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM reasonToVisit WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBReasonToVisit    obReasonToVisit;

        obReasonToVisit.load( m_uiCurrentId );

        if( obReasonToVisit.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO reasonToVisit SET ";
            qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( obReasonToVisit.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obReasonToVisit.licenceId() );
        }
        else if( obReasonToVisit.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patientOrigin SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obReasonToVisit.name() );
        qsQuery += QString( "active = %1, " ).arg( obReasonToVisit.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obReasonToVisit.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "reasonToVisitId = \"%1\" " ).arg( obReasonToVisit.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obReasonToVisit.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_REASONTOVISIT;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeIllnessGroup();
    }
}
//====================================================================================
void cDBMirror::_recordReasonToVisitSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE reasonToVisit SET archive=\"ARC\" WHERE reasonToVisitId=%1" ).arg(m_uiCurrentId) );

    _synchronizeReasonToVisit();
}
//====================================================================================
void cDBMirror::synchronizeIllnessGroup()
//====================================================================================
{
    _synchronizeIllnessGroup();
}
//====================================================================================
void cDBMirror::_synchronizeIllnessGroup( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordIllnessSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePublicPlaces()
//====================================================================================
{
    _synchronizePublicPlaces();
}
//====================================================================================
void cDBMirror::_synchronizePublicPlaces( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPublicPlacesSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeHealthInsurance()
//====================================================================================
{
    _synchronizeHealthInsurance();
}
//====================================================================================
void cDBMirror::_synchronizeHealthInsurance( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordHealthInsuranceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeCompany()
//====================================================================================
{
    _synchronizeCompany();
}
//====================================================================================
void cDBMirror::_synchronizeCompany( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
/*        cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordCompanySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeDoctorType()
//====================================================================================
{
    _synchronizeDoctorType();
}
//====================================================================================
void cDBMirror::_synchronizeDoctorType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordDoctorTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeDoctor()
//====================================================================================
{
    _synchronizeDoctor();
}
//====================================================================================
void cDBMirror::_synchronizeDoctor( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordDoctorSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeDoctorSchedule()
//====================================================================================
{
    _synchronizeDoctorSchedule();
}
//====================================================================================
void cDBMirror::_synchronizeDoctorSchedule( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordDoctorScheduleSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePatient()
//====================================================================================
{
    _synchronizePatient();
}
//====================================================================================
void cDBMirror::_synchronizePatient( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPatientSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeAttendance()
//====================================================================================
{
    _synchronizeAttendance();
}
//====================================================================================
void cDBMirror::_synchronizeAttendance( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordAttendanceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePatientcardType()
//====================================================================================
{
    _synchronizePatientcardType();
}
//====================================================================================
void cDBMirror::_synchronizePatientcardType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePatientcard()
//====================================================================================
{
    _synchronizePatientcard();
}
//====================================================================================
void cDBMirror::_synchronizePatientcard( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePatientcardConnect()
//====================================================================================
{
    _synchronizePatientcardConnect();
}
//====================================================================================
void cDBMirror::_synchronizePatientcardConnect( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardConnectSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePatientcardHistory()
//====================================================================================
{
    _synchronizePatientcardHistory();
}
//====================================================================================
void cDBMirror::_synchronizePatientcardHistory( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardHistorySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePanelType()
//====================================================================================
{
    _synchronizePanelType();
}
//====================================================================================
void cDBMirror::_synchronizePanelType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPanelTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePanelStatus()
//====================================================================================
{
    _synchronizePanelStatus();
}
//====================================================================================
void cDBMirror::_synchronizePanelStatus( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPanelStatusSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePanel()
//====================================================================================
{
    _synchronizePanel();
}
//====================================================================================
void cDBMirror::_synchronizePanel( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPanelSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizePanelUse()
//====================================================================================
{
    _synchronizePanelUse();
}
//====================================================================================
void cDBMirror::_synchronizePanelUse( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordPanelUseSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeAttendanceSchedule()
//====================================================================================
{
    _synchronizeAttendanceSchedule();
}
//====================================================================================
void cDBMirror::_synchronizeAttendanceSchedule( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordAttendanceScheduleSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeDenomination()
//====================================================================================
{
    _synchronizeDenomination();
}
//====================================================================================
void cDBMirror::_synchronizeDenomination( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordDenominationSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeProductType()
//====================================================================================
{
    _synchronizeProductType();
}
//====================================================================================
void cDBMirror::_synchronizeProductType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordProductTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeProduct()
//====================================================================================
{
    _synchronizeProduct();
}
//====================================================================================
void cDBMirror::_synchronizeProduct( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordProductSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeDiscount()
//====================================================================================
{
    _synchronizeZipRegionCity();
}
//====================================================================================
void cDBMirror::_synchronizeDiscount( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordDiscountSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeZipRegionCity()
//====================================================================================
{
    _synchronizeZipRegionCity();
}
//====================================================================================
void cDBMirror::_synchronizeZipRegionCity( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordZipRegionCitySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeAddress()
//====================================================================================
{
    _synchronizeAddress();
}
//====================================================================================
void cDBMirror::_synchronizeAddress( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordAddressSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeCassa()
//====================================================================================
{
    _synchronizeCassa();
}
//====================================================================================
void cDBMirror::_synchronizeCassa( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordCassaSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeCassaHistory()
//====================================================================================
{
    _synchronizeCassaHistory();
}
//====================================================================================
void cDBMirror::_synchronizeCassaHistory( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordCassaHistorySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeCassaDenomination()
//====================================================================================
{
    _synchronizeCassaDenomination();
}
//====================================================================================
void cDBMirror::_synchronizeCassaDenomination( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordCassaDenominationSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeLedgerDevice()
//====================================================================================
{
    _synchronizeLedgerDevice();
}
//====================================================================================
void cDBMirror::_synchronizeLedgerDevice( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        /*cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordLedgerDeviceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
void cDBMirror::synchronizeLedger()
//====================================================================================
{
    _synchronizeLedger();
}
//====================================================================================
void cDBMirror::_synchronizeLedger( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM XXXXX WHERE archive<>\"ARC\" AND licenceId > 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
         /*cDB_ClassName_   ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO _TABLENAME_ SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE _TABLENAME_ SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;*/

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        //if( m_bSyncAllTable )
            //synchronizeYYY;
    }
}
//====================================================================================
void cDBMirror::_recordLedgerSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE XXXX SET archive=\"ARC\" WHERE xxxxId=%1" ).arg(m_uiCurrentId) );

    //_synchronizeXXXXX();
}
//====================================================================================
