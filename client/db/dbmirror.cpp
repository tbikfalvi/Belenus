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
#include "dbillnessgroup.h"
#include "dbpublicplace.h"
#include "dbhealthinsurance.h"
#include "dbcompany.h"
//#include "dbdoctortype.h"
#include "dbdoctor.h"
//#include "dbdoctorschedule.h"
#include "dbpatient.h"
#include "dbattendance.h"
#include "dbpatientcardtype.h"
#include "dbpatientcard.h"
//#include "dbpatientcardconnect.h"
#include "dbpatientcardhistory.h"

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
            case MIRROR_SYNC_DB_REASONTOVISIT:
                _recordReasonToVisitSynchronized();
                break;
            case MIRROR_SYNC_DB_ILLNESSGROUP:
                _recordIllnessSynchronized();
                break;
            case MIRROR_SYNC_DB_PUBLICPLACES:
                _recordPublicPlacesSynchronized();
                break;
            case MIRROR_SYNC_DB_HEALTHINSURANCE:
                _recordHealthInsuranceSynchronized();
                break;
            case MIRROR_SYNC_DB_COMPANY:
                _recordCompanySynchronized();
                break;
            case MIRROR_SYNC_DB_DOCTORTYPE:
                _recordDoctorTypeSynchronized();
                break;
            case MIRROR_SYNC_DB_DOCTOR:
                _recordDoctorSynchronized();
                break;
            case MIRROR_SYNC_DB_DOCTORSCHEDULE:
                _recordDoctorScheduleSynchronized();
                break;
            case MIRROR_SYNC_DB_PATIENT:
                _recordPatientSynchronized();
                break;
            case MIRROR_SYNC_DB_ATTENDANCE:
                _recordAttendanceSynchronized();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDTYPE:
                _recordPatientcardTypeSynchronized();
                break;
            case MIRROR_SYNC_DB_PATIENTCARD:
                _recordPatientcardSynchronized();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDCONNECT:
                _recordPatientcardConnectSynchronized();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDHISTORY:
                _recordPatientcardHistorySynchronized();
                break;
            case MIRROR_SYNC_DB_PANELTYPE:
                _recordPanelTypeSynchronized();
                break;
            case MIRROR_SYNC_DB_PANELSTATUS:
                _recordPanelStatusSynchronized();
                break;
            case MIRROR_SYNC_DB_PANEL:
                _recordPanelSynchronized();
                break;
            case MIRROR_SYNC_DB_PANELUSE:
                _recordPanelUseSynchronized();
                break;
            case MIRROR_SYNC_DB_ATTENDANCESCHEDULE:
                _recordAttendanceScheduleSynchronized();
                break;
            case MIRROR_SYNC_DB_DENOMINATION:
                _recordDenominationSynchronized();
                break;
            case MIRROR_SYNC_DB_PRODUCTTYPE:
                _recordProductTypeSynchronized();
                break;
            case MIRROR_SYNC_DB_PRODUCT:
                _recordProductSynchronized();
                break;
            case MIRROR_SYNC_DB_DISCOUNT:
                _recordDiscountSynchronized();
                break;
            case MIRROR_SYNC_DB_ZIPREGIONCITY:
                _recordZipRegionCitySynchronized();
                break;
            case MIRROR_SYNC_DB_ADDRESS:
                _recordAddressSynchronized();
                break;
            case MIRROR_SYNC_DB_CASSA:
                _recordCassaSynchronized();
                break;
            case MIRROR_SYNC_DB_CASSAHISTORY:
                _recordCassaHistorySynchronized();
                break;
            case MIRROR_SYNC_DB_CASSADENOMINATION:
                _recordCassaDenominationSynchronized();
                break;
            case MIRROR_SYNC_DB_LEDGERDEVICE:
                _recordLedgerDeviceSynchronized();
                break;
            case MIRROR_SYNC_DB_LEDGER:
                _recordLedgerSynchronized();
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
bool cDBMirror::checkSynchronizationFinished()
//====================================================================================
{
    bool bRet = false;

    if( m_uiDbModificationLevel == 0 && m_uiGlobalSyncLevel == 0 )
    {
        bRet = true;

        m_bGetGlobalData    = false;
        m_inProcessCount    = 0;
        m_bSyncAllTable     = false;
        m_uiCurrentId       = 0;
    }

    return bRet;
}
//====================================================================================
//====================================================================================
//====================================================================================
//
//           H E R E  C O M E S  T H E  S Y N C H R O N I Z A T I O N
//
//====================================================================================
//====================================================================================
//====================================================================================
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM illnessGroups WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBIllnessGroup    obIllnessGroup;

        obIllnessGroup.load( m_uiCurrentId );

        if( obIllnessGroup.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO illnessGroups SET ";
            qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( obIllnessGroup.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obIllnessGroup.licenceId() );
        }
        else if( obIllnessGroup.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE illnessGroups SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obIllnessGroup.name() );
        qsQuery += QString( "active = %1, " ).arg( obIllnessGroup.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obIllnessGroup.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "illnessGroupId = \"%1\" " ).arg( obIllnessGroup.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obIllnessGroup.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_ILLNESSGROUP;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePublicPlaces();
    }
}
//====================================================================================
void cDBMirror::_recordIllnessSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE illnessGroups SET archive=\"ARC\" WHERE illnessGroupId=%1" ).arg(m_uiCurrentId) );

    _synchronizeIllnessGroup();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM publicPlaces WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPublicPlace      obPublicPlace;

        obPublicPlace.load( m_uiCurrentId );

        if( obPublicPlace.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO publicPlaces SET ";
            qsQuery += QString( "publicPlaceId = \"%1\", " ).arg( obPublicPlace.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPublicPlace.licenceId() );
        }
        else if( obPublicPlace.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE publicPlaces SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obPublicPlace.name() );
        qsQuery += QString( "active = %1, " ).arg( obPublicPlace.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPublicPlace.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "publicPlaceId = \"%1\" " ).arg( obPublicPlace.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPublicPlace.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PUBLICPLACES;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeHealthInsurance();
    }
}
//====================================================================================
void cDBMirror::_recordPublicPlacesSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE publicPlaces SET archive=\"ARC\" WHERE publicPlaceId=%1" ).arg(m_uiCurrentId) );

    _synchronizePublicPlaces();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM healthInsurances WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBHealthInsurance  obHealthInsurance;

        obHealthInsurance.load( m_uiCurrentId );

        if( obHealthInsurance.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO healthInsurances SET ";
            qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( obHealthInsurance.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obHealthInsurance.licenceId() );
        }
        else if( obHealthInsurance.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE healthInsurances SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obHealthInsurance.name() );
        qsQuery += QString( "country = \"%1\", " ).arg( obHealthInsurance.country() );
        qsQuery += QString( "region = \"%1\", " ).arg( obHealthInsurance.region() );
        qsQuery += QString( "city = \"%1\", " ).arg( obHealthInsurance.city() );
        qsQuery += QString( "zip = \"%1\", " ).arg( obHealthInsurance.zip() );
        qsQuery += QString( "address = \"%1\", " ).arg( obHealthInsurance.address() );
        qsQuery += QString( "contactPerson = \"%1\", " ).arg( obHealthInsurance.contactPerson() );
        qsQuery += QString( "contractId = \"%1\", " ).arg( obHealthInsurance.contractId() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( obHealthInsurance.validDateFrom() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( obHealthInsurance.validDateTo() );
        qsQuery += QString( "active = %1, " ).arg( obHealthInsurance.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obHealthInsurance.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "healthInsuranceId = \"%1\" " ).arg( obHealthInsurance.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obHealthInsurance.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_HEALTHINSURANCE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeCompany();
    }
}
//====================================================================================
void cDBMirror::_recordHealthInsuranceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE healthInsurances SET archive=\"ARC\" WHERE healthInsuranceId=%1" ).arg(m_uiCurrentId) );

    _synchronizeHealthInsurance();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM companies WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBCompany          obCompany;

        obCompany.load( m_uiCurrentId );

        if( obCompany.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO companies SET ";
            qsQuery += QString( "companyId = \"%1\", " ).arg( obCompany.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obCompany.licenceId() );
        }
        else if( obCompany.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE companies SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obCompany.name() );
        qsQuery += QString( "country = \"%1\", " ).arg( obCompany.country() );
        qsQuery += QString( "region = \"%1\", " ).arg( obCompany.region() );
        qsQuery += QString( "city = \"%1\", " ).arg( obCompany.city() );
        qsQuery += QString( "zip = \"%1\", " ).arg( obCompany.zip() );
        qsQuery += QString( "address = \"%1\", " ).arg( obCompany.address() );
        qsQuery += QString( "contactPerson = \"%1\", " ).arg( obCompany.contactPerson() );
        qsQuery += QString( "contractId = \"%1\", " ).arg( obCompany.contractId() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( obCompany.validDateFrom() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( obCompany.validDateTo() );
        qsQuery += QString( "active = %1, " ).arg( obCompany.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obCompany.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "companyId = \"%1\" " ).arg( obCompany.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obCompany.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_COMPANY;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeDoctorType();
    }
}
//====================================================================================
void cDBMirror::_recordCompanySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE companies SET archive=\"ARC\" WHERE companyId=%1" ).arg(m_uiCurrentId) );

    _synchronizeCompany();
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
    // This table includes non changeable records
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeDoctor();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctorTypes WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDB_ClassName_    ob_ClassName_;

        ob_ClassName_.load( m_uiCurrentId );

        if( ob_ClassName_.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO doctorTypes SET ";
            qsQuery += QString( "doctorTypeId = \"%1\", " ).arg( ob_ClassName_.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( ob_ClassName_.licenceId() );
        }
        else if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE doctorTypes SET ";
        }
        //_NEED_TABLE_SPECIFIC_FIELDS_HERE_
        qsQuery += QString( "active = %1, " ).arg( ob_ClassName_.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( ob_ClassName_.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "doctorTypeId = \"%1\" " ).arg( ob_ClassName_.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( ob_ClassName_.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_DOCTORTYPE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeDoctor();
    }
*/
}
//====================================================================================
void cDBMirror::_recordDoctorTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE doctorTypes SET archive=\"ARC\" WHERE doctorTypeId=%1" ).arg(m_uiCurrentId) );

    _synchronizeDoctorType();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctors WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBDoctor           obDoctor;

        obDoctor.load( m_uiCurrentId );

        if( obDoctor.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO doctors SET ";
            qsQuery += QString( "doctorId = \"%1\", " ).arg( obDoctor.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obDoctor.licenceId() );
        }
        else if( obDoctor.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE doctors SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obDoctor.name() );
        qsQuery += QString( "doctorLicence = \"%1\", " ).arg( obDoctor.licence() );
        qsQuery += QString( "data = \"%1\", " ).arg( obDoctor.data() );
        qsQuery += QString( "active = %1, " ).arg( obDoctor.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obDoctor.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "doctorId = \"%1\" " ).arg( obDoctor.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obDoctor.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_DOCTOR;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeDoctorSchedule();
    }
}
//====================================================================================
void cDBMirror::_recordDoctorSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE doctors SET archive=\"ARC\" WHERE doctorId=%1" ).arg(m_uiCurrentId) );

    _synchronizeDoctor();
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
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizePatient();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDB_ClassName_    ob_ClassName_;

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

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatient();
    }
*/
}
//====================================================================================
void cDBMirror::_recordDoctorScheduleSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeDoctorSchedule();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPatient          obPatient;

        obPatient.load( m_uiCurrentId );

        if( obPatient.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO patients SET ";
            qsQuery += QString( "patientId = \"%1\", " ).arg( obPatient.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPatient.licenceId() );
        }
        else if( obPatient.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patients SET ";
        }
        qsQuery += QString( "patientOriginId = \"%1\", " ).arg( obPatient.patientOriginId() );
        qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( obPatient.reasonToVisitId() );
        qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( obPatient.illnessGroupId() );
        qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( obPatient.healthInsuranceId() );
        qsQuery += QString( "companyId = \"%1\", " ).arg( obPatient.companyId() );
        qsQuery += QString( "doctorId = \"%1\", " ).arg( obPatient.doctorId() );
        qsQuery += QString( "created = \"%1\", " ).arg( obPatient.dateCreated() );
        qsQuery += QString( "name = \"%1\", " ).arg( obPatient.name() );
        qsQuery += QString( "gender = \"%1\", " ).arg( obPatient.gender() );
        qsQuery += QString( "dateBirth = \"%1\", " ).arg( obPatient.dateBirth() );
        qsQuery += QString( "uniqueId = \"%1\", " ).arg( obPatient.uniqueId() );
        qsQuery += QString( "email = \"%1\", " ).arg( obPatient.email() );
        qsQuery += QString( "phone = \"%1\", " ).arg( obPatient.phone() );
        qsQuery += QString( "weight = \"%1\", " ).arg( obPatient.weight() );
        qsQuery += QString( "height = \"%1\", " ).arg( obPatient.height() );
        qsQuery += QString( "illnesses = \"%1\", " ).arg( obPatient.illnesses() );
        qsQuery += QString( "symptoms = \"%1\", " ).arg( obPatient.symptoms() );
        qsQuery += QString( "medicineCurrent = \"%1\", " ).arg( obPatient.medicineCurrent() );
        qsQuery += QString( "medicineAllergy = \"%1\", " ).arg( obPatient.medicineAllergy() );
        qsQuery += QString( "regularCustomer = %1, " ).arg( obPatient.regularCustomer() );
        qsQuery += QString( "employee = %1, " ).arg( obPatient.employee() );
        qsQuery += QString( "service = %1, " ).arg( obPatient.service() );
        qsQuery += QString( "healthInsurance = %1, " ).arg( obPatient.healthInsurance() );
        qsQuery += QString( "company = %1, " ).arg( obPatient.company() );
        qsQuery += QString( "doctorProposed = %1, " ).arg( obPatient.doctorProposed() );
        qsQuery += QString( "discountType = %1, " ).arg( obPatient.discountType() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obPatient.comment() );
        qsQuery += QString( "active = %1, " ).arg( obPatient.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPatient.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientId = \"%1\" " ).arg( obPatient.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPatient.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PATIENT;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeAttendance();
    }
}
//====================================================================================
void cDBMirror::_recordPatientSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE patients SET archive=\"ARC\" WHERE patientId=%1" ).arg(m_uiCurrentId) );

    _synchronizePatient();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM attendance WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBAttendance       obAttendance;

        obAttendance.load( m_uiCurrentId );

        if( obAttendance.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO attendance SET ";
            qsQuery += QString( "attendanceId = \"%1\", " ).arg( obAttendance.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obAttendance.licenceId() );
        }
        else if( obAttendance.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE attendance SET ";
        }
        qsQuery += QString( "patientId = \"%1\", " ).arg( obAttendance.patientId() );
        qsQuery += QString( "date = \"%1\", " ).arg( obAttendance.date() );
        qsQuery += QString( "length = \"%1\", " ).arg( obAttendance.length() );
        qsQuery += QString( "bloodPressureStart = \"%1\", " ).arg( obAttendance.bloodPressureStart() );
        qsQuery += QString( "pulseStart = \"%1\", " ).arg( obAttendance.pulseStart() );
        qsQuery += QString( "bloodPressureStop = \"%1\", " ).arg( obAttendance.bloodPressureStop() );
        qsQuery += QString( "pulseStop = \"%1\", " ).arg( obAttendance.pulseStop() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obAttendance.comment() );
        qsQuery += QString( "active = %1, " ).arg( obAttendance.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obAttendance.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "attendanceId = \"%1\" " ).arg( obAttendance.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obAttendance.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_ATTENDANCE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatientcardType();
    }
}
//====================================================================================
void cDBMirror::_recordAttendanceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE attendance SET archive=\"ARC\" WHERE attendanceId=%1" ).arg(m_uiCurrentId) );

    _synchronizeAttendance();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypes WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPatientCardType  obPatientCardType;

        obPatientCardType.load( m_uiCurrentId );

        if( obPatientCardType.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO patientCardTypes SET ";
            qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( obPatientCardType.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPatientCardType.licenceId() );
        }
        else if( obPatientCardType.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patientCardTypes SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obPatientCardType.name() );
        qsQuery += QString( "price = \"%1\", " ).arg( obPatientCardType.price() );
        qsQuery += QString( "vatpercent = \"%1\", " ).arg( obPatientCardType.vatpercent() );
        qsQuery += QString( "units = \"%1\", " ).arg( obPatientCardType.units() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( obPatientCardType.validDateFrom() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( obPatientCardType.validDateTo() );
        qsQuery += QString( "validDays = \"%1\", " ).arg( obPatientCardType.validDays() );
        qsQuery += QString( "unitTime = \"%1\", " ).arg( obPatientCardType.unitTime() );
        qsQuery += QString( "active = %1, " ).arg( obPatientCardType.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPatientCardType.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientCardTypeId = \"%1\" " ).arg( obPatientCardType.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPatientCardType.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PATIENTCARDTYPE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatientcard();
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE patientCardTypes SET archive=\"ARC\" WHERE patientCardTypeId=%1" ).arg(m_uiCurrentId) );

    _synchronizePatientcardType();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPatientCard      obPatientCard;

        obPatientCard.load( m_uiCurrentId );

        if( obPatientCard.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO patientCards SET ";
            qsQuery += QString( "patientCardId = \"%1\", " ).arg( obPatientCard.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPatientCard.licenceId() );
        }
        else if( obPatientCard.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patientCards SET ";
        }
        qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( obPatientCard.patientCardTypeId() );
        qsQuery += QString( "patientId = \"%1\", " ).arg( obPatientCard.patientId() );
        qsQuery += QString( "barcode = \"%1\", " ).arg( obPatientCard.barcode() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obPatientCard.comment() );
        qsQuery += QString( "units = \"%1\", " ).arg( obPatientCard.units() );
        qsQuery += QString( "timeLeft = \"%1\", " ).arg( obPatientCard.timeLeft() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( obPatientCard.validDateFrom() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( obPatientCard.validDateTo() );
        qsQuery += QString( "pincode = \"%1\", " ).arg( obPatientCard.pincode() );
        qsQuery += QString( "active = %1, " ).arg( obPatientCard.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPatientCard.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientCardId = \"%1\" " ).arg( obPatientCard.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPatientCard.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PATIENTCARD;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatientcardConnect();
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE patientCards SET archive=\"ARC\" WHERE patientCardId=%1" ).arg(m_uiCurrentId) );

    _synchronizePatientcard();
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
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizePatientcardHistory();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
        return;

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDB_ClassName_    ob_ClassName_;

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

        m_inProcessCount = MIRROR_SYNC_DB_TABLENAME;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePatientcardHistory();
    }
*/
}
//====================================================================================
void cDBMirror::_recordPatientcardConnectSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizePatientcardConnect();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardHistories WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString                 qsQuery = "";
        cDBPatientCardHistory   obPatientCardHistory;

        obPatientCardHistory.load( m_uiCurrentId );

        if( obPatientCardHistory.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO patientCardHistories SET ";
            qsQuery += QString( "patientCardHistoryId = \"%1\", " ).arg( obPatientCardHistory.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPatientCardHistory.licenceId() );
        }
        else if( obPatientCardHistory.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE patientCardHistories SET ";
        }
        qsQuery += QString( "patientCardId = \"%1\", " ).arg( obPatientCardHistory.patientCardId() );
        qsQuery += QString( "units = \"%1\", " ).arg( obPatientCardHistory.units() );
        qsQuery += QString( "time = \"%1\", " ).arg( obPatientCardHistory.time() );
        qsQuery += QString( "active = %1, " ).arg( obPatientCardHistory.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPatientCardHistory.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientCardHistoryId = \"%1\" " ).arg( obPatientCardHistory.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPatientCardHistory.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PATIENTCARDHISTORY;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePanelType();
    }
}
//====================================================================================
void cDBMirror::_recordPatientcardHistorySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE patientCardHistories SET archive=\"ARC\" WHERE patientCardHistoryId=%1" ).arg(m_uiCurrentId) );

    _synchronizePatientcardHistory();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizePanelStatus();
    }
}
//====================================================================================
void cDBMirror::_recordPanelTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelType();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizePanel();
    }
}
//====================================================================================
void cDBMirror::_recordPanelStatusSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelStatus();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizePanelUse();
    }
}
//====================================================================================
void cDBMirror::_recordPanelSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizePanel();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeAttendanceSchedule();
    }
}
//====================================================================================
void cDBMirror::_recordPanelUseSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelUse();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeDenomination();
    }
}
//====================================================================================
void cDBMirror::_recordAttendanceScheduleSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeAttendanceSchedule();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeProductType();
    }
}
//====================================================================================
void cDBMirror::_recordDenominationSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeDenomination();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeProduct();
    }
}
//====================================================================================
void cDBMirror::_recordProductTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeProductType();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeDiscount();
    }
}
//====================================================================================
void cDBMirror::_recordProductSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeProduct();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeZipRegionCity();
    }
}
//====================================================================================
void cDBMirror::_recordDiscountSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeDiscount();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeAddress();
    }
}
//====================================================================================
void cDBMirror::_recordZipRegionCitySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeZipRegionCity();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeCassa();
    }
}
//====================================================================================
void cDBMirror::_recordAddressSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeAddress();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeCassaHistory();
    }
}
//====================================================================================
void cDBMirror::_recordCassaSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassa();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeCassaDenomination();
    }
}
//====================================================================================
void cDBMirror::_recordCassaHistorySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassaHistory();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeLedgerDevice();
    }
}
//====================================================================================
void cDBMirror::_recordCassaDenominationSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassaDenomination();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            synchronizeLedger();
    }
}
//====================================================================================
void cDBMirror::_recordLedgerDeviceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeLedgerDevice();
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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM _TABLENAME_ WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

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
        if( m_bSyncAllTable )
            checkSynchronizationFinished();
    }
}
//====================================================================================
void cDBMirror::_recordLedgerSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE _TABLENAME_ SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeLedger();
}
//====================================================================================
