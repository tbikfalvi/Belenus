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

#include <QDateTime>

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
#include "dbpaneltypes.h"
#include "dbpanelstatuses.h"
#include "dbpanels.h"
#include "dbpaneluses.h"
//#include "dbattendanceschedule.h"
#include "dbdenomination.h"
//#include "dbproducttype"
//#include "dbproduct"
#include "dbdiscount.h"
#include "dbzipregioncity.h"
#include "dbaddress.h"
#include "dbcassa.h"
#include "dbcassahistory.h"
#include "dbcassadenomination.h"
#include "dbledgerdevice.h"
#include "dbledger.h"

//====================================================================================

extern cQTLogger                g_obLogger;
extern BelenusServerConnection *g_poServer;

//====================================================================================
cDBMirror::cDBMirror()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::cDBMirror] called" << EOM;

    m_inProcessCount            = 0;
    m_bAcquireGlobalData        = false;
    m_bGlobalDataChanged        = false;
    m_bSyncAllTable             = false;
    m_bSyncExit                 = false;
    m_uiDbModificationLevel     = 0;
    m_uiCurrentId               = 0;
    m_uiGlobalDataChanged       = 0;
    m_bGlobalDataSynchronize    = false;
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

    if( g_poServer->getStatus() != BelenusServerConnection::AUTHENTICATED )
    {
        g_obLogger(cSeverity::DEBUG) << "[cDBMirror::start] not connected to server" << EOM;
        return false;
    }

    m_bAcquireGlobalData = true;
    requestGlobalDataTimestamp();

    return true;
}
//====================================================================================
void cDBMirror::requestGlobalDataTimestamp()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::requestGlobalDataTimestamp]" << EOM;

    QString qsQuery = QString( "SELECT value FROM settings WHERE identifier=\"GLOBAL_DATA_UPDATED\" ");

    m_inProcessCount = MIRROR_GET_GLOBAL_TIMESTAMP;

    _qId = g_poServer->sendQuery( qsQuery );
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

        if( poQuery->value( 11 ).toString().compare("NEW") == 0 )
        {
            qsQuery += QString( "INSERT INTO licences SET " );
            qsQuery += QString( "licenceId = %1, " ).arg( poQuery->value( 0 ).toString() );
            qsQuery += QString( "serial = \"%1\", " ).arg( poQuery->value( 1 ).toString() );
        }
        else
        {
            qsQuery += QString( "UPDATE licences SET " );
        }
        qsQuery += QString( "country = \"%1\", " ).arg( poQuery->value( 2 ).toString() );
        qsQuery += QString( "region = \"%1\", " ).arg( poQuery->value( 3 ).toString() );
        qsQuery += QString( "city = \"%1\", " ).arg( poQuery->value( 4 ).toString() );
        qsQuery += QString( "zip = \"%1\", " ).arg( poQuery->value( 5 ).toString() );
        qsQuery += QString( "address = \"%1\", " ).arg( poQuery->value( 6 ).toString() );
        qsQuery += QString( "studio = \"%1\", " ).arg( poQuery->value( 7 ).toString() );
        qsQuery += QString( "contact = \"%1\", " ).arg( poQuery->value( 8 ).toString() );
        qsQuery += QString( "lastValidated = \"%1\" " ).arg( poQuery->value( 9 ).toString() );
        if( poQuery->value( 11 ).toString().compare("NEW") == 0 )
        {
            qsQuery += QString( ", active = 1, archive = \"ARC\" " );
        }
        else
        {
            qsQuery += QString( "WHERE licenceId = %1" ).arg( poQuery->value( 0 ).toString() );
        }

        m_inProcessCount = MIRROR_UPDATE_LICENCE_DATA;

        _qId = g_poServer->sendQuery( qsQuery );
    }
}
//====================================================================================
void cDBMirror::queryReady( int id, SqlResult *p_sqlResult )
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] id=" << id << " affected " << p_sqlResult->affectedRecords() << " rows." << EOM;

    if( id == _qId )
    {
        if( p_sqlResult->rowCount() > 0 )
        {
            g_obLogger(cSeverity::DEBUG) << "[cDBMirror::queryReady] data[0,0] = '" << p_sqlResult->index(0,0).data().toString() << "'" << EOM;
        }

        switch( m_inProcessCount )
        {
            case MIRROR_GET_GLOBAL_TIMESTAMP:
                if( p_sqlResult->isValid() )
                {
                    _compareGlobalDataTimestamp( p_sqlResult->index(0,0).data().toString() );
                }
                break;
            case MIRROR_UPDATE_LICENCE_DATA:
            {
                if( p_sqlResult->isValid() > 0 )
                {
                    g_poDB->executeQTQuery( QString( "UPDATE licences SET archive=\"ARC\" WHERE licenceId=%1" ).arg(g_poPrefs->getLicenceId()) );
                    m_inProcessCount = 0;
                }
                break;
            }
            case MIRROR_GET_GLOBAL_PATIENTORIGIN:
                if( p_sqlResult->isValid() )
                    _processPatientOriginGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_REASONTOVISIT:
                if( p_sqlResult->isValid() )
                    _processReasonToVisitGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_ILLNESSGROUPS:
                if( p_sqlResult->isValid() )
                    _processIllnessGroupsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PUBLICPLACES:
                if( p_sqlResult->isValid() )
                    _processPublicPlacesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_HEALTHINSURANCES:
                if( p_sqlResult->isValid() )
                    _processHealthInsurancesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_COMPANIES:
                if( p_sqlResult->isValid() )
                    _processCompaniesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_DOCTORTYPES:
                if( p_sqlResult->isValid() )
                    _processDoctorTypesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_DOCTORS:
                if( p_sqlResult->isValid() )
                    _processDoctorsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PATIENTS:
                if( p_sqlResult->isValid() )
                    _processPatientsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PATIENTCARDTYPES:
                if( p_sqlResult->isValid() )
                    _processPatientCardTypesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PATIENTCARDS:
                if( p_sqlResult->isValid() )
                    _processPatientCardsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_LEDGERTYPES:
                if( p_sqlResult->isValid() )
                    _processLedgerTypesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PRODUCTTYPES:
                if( p_sqlResult->isValid() )
                    _processProductTypesGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PRODUCTS:
                if( p_sqlResult->isValid() )
                    _processProductsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_DISCOUNTS:
                if( p_sqlResult->isValid() )
                    _processDiscountsGlobals( p_sqlResult );
                break;
            case MIRROR_GET_GLOBAL_PAYMENTMETHODS:
                if( p_sqlResult->isValid() )
                    _processPaymentMethodsGlobals( p_sqlResult );
                break;
            case MIRROR_SYNC_DB_USER:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordUserSynchronized();
                else
                    _synchronizeUserTable();
                break;
            case MIRROR_SYNC_DB_PATIENTORIGIN:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientOriginSynchronized();
                else
                    _synchronizePatientOriginTable();
                break;
            case MIRROR_SYNC_DB_REASONTOVISIT:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordReasonToVisitSynchronized();
                else
                    _synchronizeReasonToVisit();
                break;
            case MIRROR_SYNC_DB_ILLNESSGROUP:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordIllnessSynchronized();
                else
                    _synchronizeIllnessGroup();
                break;
            case MIRROR_SYNC_DB_PUBLICPLACES:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPublicPlacesSynchronized();
                else
                    _synchronizePublicPlaces();
                break;
            case MIRROR_SYNC_DB_HEALTHINSURANCE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordHealthInsuranceSynchronized();
                else
                    _synchronizeHealthInsurance();
                break;
            case MIRROR_SYNC_DB_COMPANY:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordCompanySynchronized();
                else
                    _synchronizeCompany();
                break;
            case MIRROR_SYNC_DB_DOCTORTYPE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordDoctorTypeSynchronized();
                else
                    _synchronizeDoctorType();
                break;
            case MIRROR_SYNC_DB_DOCTOR:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordDoctorSynchronized();
                else
                    _synchronizeDoctor();
                break;
            case MIRROR_SYNC_DB_DOCTORSCHEDULE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordDoctorScheduleSynchronized();
                else
                    _synchronizeDoctorSchedule();
                break;
            case MIRROR_SYNC_DB_PATIENT:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientSynchronized();
                else
                    _synchronizePatient();
                break;
            case MIRROR_SYNC_DB_ATTENDANCE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordAttendanceSynchronized();
                else
                    _synchronizeAttendance();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDTYPE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientcardTypeSynchronized();
                else
                    _synchronizePatientcardType();
                break;
            case MIRROR_SYNC_DB_PATIENTCARD:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientcardSynchronized();
                else
                    _synchronizePatientcard();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDCONNECT:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientcardConnectSynchronized();
                else
                    _synchronizePatientcardConnect();
                break;
            case MIRROR_SYNC_DB_PATIENTCARDHISTORY:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPatientcardHistorySynchronized();
                else
                    _synchronizePatientcardHistory();
                break;
            case MIRROR_SYNC_DB_PANELTYPE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPanelTypeSynchronized();
                else
                    _synchronizePanelType();
                break;
            case MIRROR_SYNC_DB_PANELSTATUS:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPanelStatusSynchronized();
                else
                    _synchronizePanelStatus();
                break;
            case MIRROR_SYNC_DB_PANEL:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPanelSynchronized();
                else
                    _synchronizePanel();
                break;
            case MIRROR_SYNC_DB_PANELUSE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordPanelUseSynchronized();
                else
                    _synchronizePanelUse();
                break;
            case MIRROR_SYNC_DB_ATTENDANCESCHEDULE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordAttendanceScheduleSynchronized();
                else
                    _synchronizeAttendanceSchedule();
                break;
            case MIRROR_SYNC_DB_DENOMINATION:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordDenominationSynchronized();
                else
                    _synchronizeDenomination();
                break;
            case MIRROR_SYNC_DB_PRODUCTTYPE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordProductTypeSynchronized();
                else
                    _synchronizeProductType();
                break;
            case MIRROR_SYNC_DB_PRODUCT:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordProductSynchronized();
                else
                    _synchronizeProduct();
                break;
            case MIRROR_SYNC_DB_DISCOUNT:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordDiscountSynchronized();
                else
                    _synchronizeDiscount();
                break;
            case MIRROR_SYNC_DB_ZIPREGIONCITY:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordZipRegionCitySynchronized();
                else
                    _synchronizeZipRegionCity();
                break;
            case MIRROR_SYNC_DB_ADDRESS:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordAddressSynchronized();
                else
                    _synchronizeAddress();
                break;
            case MIRROR_SYNC_DB_CASSA:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordCassaSynchronized();
                else
                    _synchronizeCassa();
                break;
            case MIRROR_SYNC_DB_CASSAHISTORY:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordCassaHistorySynchronized();
                else
                    _synchronizeCassaHistory();
                break;
            case MIRROR_SYNC_DB_CASSADENOMINATION:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordCassaDenominationSynchronized();
                else
                    _synchronizeCassaDenomination();
                break;
            case MIRROR_SYNC_DB_LEDGERDEVICE:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordLedgerDeviceSynchronized();
                else
                    _synchronizeLedgerDevice();
                break;
            case MIRROR_SYNC_DB_LEDGER:
                if( p_sqlResult->isValid() && p_sqlResult->affectedRecords() > 0 )
                    _recordLedgerSynchronized();
                else
                    _synchronizeLedger();
                break;
            default:
                break;
        }
        delete p_sqlResult;
    }
}
//====================================================================================
void cDBMirror::updateSynchronizationLevel( unsigned int p_uiSyncLevel )
//====================================================================================
{
    m_uiDbModificationLevel |= p_uiSyncLevel;
}
//====================================================================================
void cDBMirror::_compareGlobalDataTimestamp( const QString &p_qsGlobalTimestamp )
//====================================================================================
{
    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"GLOBAL_DATA_UPDATED\" ") );
    poQuery->first();

    QDateTime   qdServer = QDateTime::fromString( p_qsGlobalTimestamp, "yyyy-MM-dd hh:mm:ss" );
    QDateTime   qdClient = QDateTime::fromString( poQuery->value( 0 ).toString(), "yyyy-MM-dd hh:mm:ss" );

    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::_compareGlobalDataTimestamp] Server: " << qdServer.toString("yyyy-MM-dd hh:mm:ss") << EOM;
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::_compareGlobalDataTimestamp] Client: " << qdClient.toString("yyyy-MM-dd hh:mm:ss") << EOM;
    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::_compareGlobalDataTimestamp] Diff: " << qdClient.secsTo( qdServer ) << EOM;

    if( qdClient.secsTo( qdServer ) > 0 )
    {
        m_bGlobalDataChanged = true;
        m_qsServerTimestamp = p_qsGlobalTimestamp;
    }
    else
    {
        m_bGlobalDataChanged = false;
        m_qsServerTimestamp = poQuery->value( 0 ).toString();
    }
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
    if( (m_uiGlobalSyncLevel&p_uiSyncLevel) > 0 )
    {
        _updateGlobalTimestampOnServer();
    }
    m_uiGlobalSyncLevel &= ~p_uiSyncLevel;
}
//====================================================================================
void cDBMirror::_updateGlobalTimestampOnServer()
//====================================================================================
{
    QString qsQuery = QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"GLOBAL_DATA_UPDATED\" ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    m_inProcessCount = MIRROR_SET_GLOBAL_TIMESTAMP_ON_SERVER;

    _qId = g_poServer->sendQuery( qsQuery );
}
//====================================================================================
bool cDBMirror::checkSyncLevel( unsigned int p_uiSyncLevel )
//====================================================================================
{
//    g_obLogger(cSeverity::DEBUG) << "[cDBMirror::checkSyncLevel] p_uiSyncLevel: " << QString::number(p_uiSyncLevel) << " - m_uiDbModificationLevel: " << QString::number(m_uiDbModificationLevel) << EOM;

    return ((m_uiDbModificationLevel&p_uiSyncLevel)>0?true:false);
}
//====================================================================================
bool cDBMirror::checkGlobalData( unsigned int p_uiSyncLevel )
//====================================================================================
{
    return ((m_uiGlobalDataChanged&p_uiSyncLevel)>0?true:false);
}
//====================================================================================
bool cDBMirror::checkIsGlobalDataDownloadInProgress()
//====================================================================================
{
    return m_bAcquireGlobalData;
}
//====================================================================================
bool cDBMirror::checkSynchronizationFinished()
//====================================================================================
{
    return ( ((m_uiDbModificationLevel == 0 && m_uiGlobalSyncLevel == 0) || !m_bProcessSucceeded) ? true : false );
}
//====================================================================================
bool cDBMirror::checkIsSynchronizationNeeded()
//====================================================================================
{
    QSqlQuery   *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_USER );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientOrigin WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PATIENT_ORIGIN );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM reasonToVisit WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_REASON_TO_VISIT );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM illnessGroups WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_ILLNESS_GROUP );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM publicPlaces WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PUBLIC_PLACES );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM healthInsurances WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_HEALTH_INSURANCE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM companies WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_COMPANY );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctors WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_DOCTOR );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PATIENT );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM attendance WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_ATTENDANCE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypes WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PATIENTCARD_TYPE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PATIENTCARD );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardHistories WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PATIENTCARD_HISTORY );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelTypes WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PANEL_TYPE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PANEL_STATUS );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panels WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PANEL );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelUses WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_PANEL_USE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_DISCOUNT );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM zipRegionCity WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_ZIP_REGION_CITY );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM address WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_ADDRESS );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_CASSA );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_CASSA_HISTORY );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledgerDevice WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_LEDGER_DEVICE );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledger WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );
    if( poQuery->first() )
        updateSynchronizationLevel( DB_LEDGER );

    return (m_uiDbModificationLevel>0?true:false);
}
//====================================================================================
bool cDBMirror::checkIsGlobalDataModifiedOnServer()
//====================================================================================
{
    return m_bGlobalDataChanged;
}
//====================================================================================
//====================================================================================
//====================================================================================
//====================================================================================
//
//         H E R E  C O M E S  T H E  G L O B A L  D A T A  D O W N L O A D
//
//====================================================================================
//====================================================================================
//====================================================================================
//====================================================================================
void cDBMirror::acquirePatientOriginGlobals()
//====================================================================================
{
    m_inProcessCount        = MIRROR_GET_GLOBAL_PATIENTORIGIN;
    m_bProcessSucceeded     = true;
    m_inCountOfTries        = 0;
    m_uiCurrentId           = 0;
    m_uiGlobalDataChanged   = 0;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM patientOrigin WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPatientOriginGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientOrigin WHERE patientOriginId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE patientOrigin SET ";
        }
        else
        {
            qsQuery = "INSERT INTO patientOrigin SET ";
            qsQuery += QString( "patientOriginId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientOriginId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PATIENT_ORIGIN;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireReasonToVisitGlobals();
}
//====================================================================================
void cDBMirror::acquireReasonToVisitGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_REASONTOVISIT;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM reasonToVisit WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processReasonToVisitGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM reasonToVisit WHERE reasonToVisitId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE reasonToVisit SET ";
        }
        else
        {
            qsQuery = "INSERT INTO reasonToVisit SET ";
            qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "reasonToVisitId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_REASON_TO_VISIT;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireIllnessGroupsGlobals();
}
//====================================================================================
void cDBMirror::acquireIllnessGroupsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_ILLNESSGROUPS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM illnessGroups WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processIllnessGroupsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM illnessGroups WHERE illnessGroupId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE illnessGroups SET ";
        }
        else
        {
            qsQuery = "INSERT INTO illnessGroups SET ";
            qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "illnessGroupId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_ILLNESS_GROUP;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquirePublicPlacesGlobals();
}
//====================================================================================
void cDBMirror::acquirePublicPlacesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PUBLICPLACES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM publicPlaces WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPublicPlacesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM publicPlaces WHERE publicPlaceId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE publicPlaces SET ";
        }
        else
        {
            qsQuery = "INSERT INTO publicPlaces SET ";
            qsQuery += QString( "publicPlaceId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "publicPlaceId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PUBLIC_PLACES;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireHealthInsurancesGlobals();
}
//====================================================================================
void cDBMirror::acquireHealthInsurancesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_HEALTHINSURANCES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM healthInsurances WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processHealthInsurancesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM healthInsurances WHERE healthInsuranceId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,12).data().toString();
            if( qsServerTS.compare( poQuery->value( 12 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE healthInsurances SET ";
        }
        else
        {
            qsQuery = "INSERT INTO healthInsurances SET ";
            qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "country = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "region = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "city = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toString() );
        qsQuery += QString( "zip = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toString() );
        qsQuery += QString( "address = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toString() );
        qsQuery += QString( "contactPerson = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "contractId = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toString() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toString() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_sqlResult->index(i,11).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,12).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,13).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "healthInsuranceId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_HEALTH_INSURANCE;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireCompaniesGlobals();
}
//====================================================================================
void cDBMirror::acquireCompaniesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_COMPANIES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM companies WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processCompaniesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM companies WHERE companyId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,12).data().toString();
            if( qsServerTS.compare( poQuery->value( 12 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE companies SET ";
        }
        else
        {
            qsQuery = "INSERT INTO companies SET ";
            qsQuery += QString( "companyId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "country = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "region = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "city = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toString() );
        qsQuery += QString( "zip = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toString() );
        qsQuery += QString( "address = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toString() );
        qsQuery += QString( "contactPerson = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "contractId = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toString() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toString() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_sqlResult->index(i,11).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,12).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,13).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "companyId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_COMPANY;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireDoctorTypesGlobals();
}
//====================================================================================
void cDBMirror::acquireDoctorTypesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_DOCTORTYPES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM doctorTypes WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processDoctorTypesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctorTypes WHERE doctorTypeId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE doctorTypes SET ";
        }
        else
        {
            qsQuery = "INSERT INTO doctorTypes SET ";
            qsQuery += QString( "doctorTypeId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "doctorTypeId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_DOCTOR_TYPE;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireDoctorsGlobals();
}
//====================================================================================
void cDBMirror::acquireDoctorsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_DOCTORS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM doctors WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processDoctorsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctors WHERE doctorId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,6).data().toString();
            if( qsServerTS.compare( poQuery->value( 6 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE doctors SET ";
        }
        else
        {
            qsQuery = "INSERT INTO doctors SET ";
            qsQuery += QString( "doctorId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "doctorTypeId = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toUInt() );
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "doctorLicence = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "data = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,7).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "doctorId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_DOCTOR;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquirePatientsGlobals();
}
//====================================================================================
void cDBMirror::acquirePatientsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PATIENTS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM patients WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPatientsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE patientIdId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,29).data().toString();
            if( qsServerTS.compare( poQuery->value( 29 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE patients SET ";
        }
        else
        {
            qsQuery = "INSERT INTO patients SET ";
            qsQuery += QString( "patientId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "patientOriginId = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toUInt() );
        qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toUInt() );
        qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toUInt() );
        qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toUInt() );
        qsQuery += QString( "companyId = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toUInt() );
        qsQuery += QString( "doctorId = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toUInt() );
        qsQuery += QString( "created = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toString() );
        qsQuery += QString( "gender = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toString() );
        qsQuery += QString( "dateBirth = \"%1\", " ).arg( p_sqlResult->index(i,11).data().toString() );
        qsQuery += QString( "uniqueId = \"%1\", " ).arg( p_sqlResult->index(i,12).data().toString() );
        qsQuery += QString( "email = \"%1\", " ).arg( p_sqlResult->index(i,13).data().toString() );
        qsQuery += QString( "phone = \"%1\", " ).arg( p_sqlResult->index(i,14).data().toString() );
        qsQuery += QString( "weight = \"%1\", " ).arg( p_sqlResult->index(i,15).data().toInt() );
        qsQuery += QString( "height = \"%1\", " ).arg( p_sqlResult->index(i,16).data().toInt() );
        qsQuery += QString( "illnesses = \"%1\", " ).arg( p_sqlResult->index(i,17).data().toString() );
        qsQuery += QString( "symptoms = \"%1\", " ).arg( p_sqlResult->index(i,18).data().toString() );
        qsQuery += QString( "medicineCurrent = \"%1\", " ).arg( p_sqlResult->index(i,19).data().toString() );
        qsQuery += QString( "medicineAllergy = \"%1\", " ).arg( p_sqlResult->index(i,20).data().toString() );
        qsQuery += QString( "regularCustomer = %1, " ).arg( p_sqlResult->index(i,21).data().toBool() );
        qsQuery += QString( "employee = %1, " ).arg( p_sqlResult->index(i,22).data().toBool() );
        qsQuery += QString( "service = %1, " ).arg( p_sqlResult->index(i,23).data().toBool() );
        qsQuery += QString( "healthInsurance = %1, " ).arg( p_sqlResult->index(i,24).data().toBool() );
        qsQuery += QString( "company = %1, " ).arg( p_sqlResult->index(i,25).data().toBool() );
        qsQuery += QString( "doctorProposed = %1, " ).arg( p_sqlResult->index(i,26).data().toBool() );
        qsQuery += QString( "discountType = %1, " ).arg( p_sqlResult->index(i,27).data().toInt() );
        qsQuery += QString( "comment = \"%1\", " ).arg( p_sqlResult->index(i,28).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,29).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,30).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PATIENT;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquirePatientCardTypesGlobals();
}
//====================================================================================
void cDBMirror::acquirePatientCardTypesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PATIENTCARDTYPES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM patientCardTypes WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPatientCardTypesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypes WHERE patientCardTypeId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,10).data().toString();
            if( qsServerTS.compare( poQuery->value( 10 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE patientCardTypes SET ";
        }
        else
        {
            qsQuery = "INSERT INTO patientCardTypes SET ";
            qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "price = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "vatpercent = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toInt() );
        qsQuery += QString( "units = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toInt() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toString() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toString() );
        qsQuery += QString( "validDays = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "unitTime = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toInt() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,11).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientCardTypeId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PATIENTCARD_TYPE;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquirePatientCardsGlobals();
}
//====================================================================================
void cDBMirror::acquirePatientCardsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PATIENTCARDS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM patientCards WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPatientCardsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientCardId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,11).data().toString();
            if( qsServerTS.compare( poQuery->value( 11 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE patientCards SET ";
        }
        else
        {
            qsQuery = "INSERT INTO patientCards SET ";
            qsQuery += QString( "patientCardId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toUInt() );
        qsQuery += QString( "patientId = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toUInt() );
        qsQuery += QString( "barcode = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "comment = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toString() );
        qsQuery += QString( "units = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toInt() );
        qsQuery += QString( "timeLeft = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toUInt() );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toString() );
        qsQuery += QString( "pincode = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,11).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,12).data().toString() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "patientCardId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PATIENTCARD;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireLedgerTypesGlobals();
}
//====================================================================================
void cDBMirror::acquireLedgerTypesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_LEDGERTYPES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM ledgerTypes WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processLedgerTypesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledgerTypes WHERE ledgerTypeId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE ledgerTypes SET ";
        }
        else
        {
            qsQuery = "INSERT INTO ledgerTypes SET ";
            qsQuery += QString( "ledgerTypeId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toBool() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "ledgerTypeId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_LEDGER;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireProductTypesGlobals();
}
//====================================================================================
void cDBMirror::acquireProductTypesGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PRODUCTTYPES;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM productTypes WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processProductTypesGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productTypes WHERE productTypeId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE productTypes SET ";
        }
        else
        {
            qsQuery = "INSERT INTO productTypes SET ";
            qsQuery += QString( "productTypeId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "productTypeId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PRODUCT_TYPE;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireProductsGlobals();
}
//====================================================================================
void cDBMirror::acquireProductsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PRODUCTS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM products WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processProductsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM products WHERE productId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,6).data().toString();
            if( qsServerTS.compare( poQuery->value( 6 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE products SET ";
        }
        else
        {
            qsQuery = "INSERT INTO products SET ";
            qsQuery += QString( "productId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "productTypeId = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toUInt() );
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "netPrice = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "vatpercent = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,7).data().toString() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "productId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_PRODUCT;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquireDiscountsGlobals();
}
//====================================================================================
void cDBMirror::acquireDiscountsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_DISCOUNTS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM discounts WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processDiscountsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE discountId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,11).data().toString();
            if( qsServerTS.compare( poQuery->value( 11 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE discounts SET ";
        }
        else
        {
            qsQuery = "INSERT INTO discounts SET ";
            qsQuery += QString( "discountId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toUInt() );
        qsQuery += QString( "companyId = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toUInt() );
        qsQuery += QString( "doctorId = \"%1\", " ).arg( p_sqlResult->index(i,4).data().toUInt() );
        qsQuery += QString( "regularCustomer = \"%1\", " ).arg( p_sqlResult->index(i,5).data().toBool() );
        qsQuery += QString( "employee = \"%1\", " ).arg( p_sqlResult->index(i,6).data().toBool() );
        qsQuery += QString( "service = \"%1\", " ).arg( p_sqlResult->index(i,7).data().toBool() );
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,8).data().toString() );
        qsQuery += QString( "discountValue = \"%1\", " ).arg( p_sqlResult->index(i,9).data().toInt() );
        qsQuery += QString( "discountPercent = \"%1\", " ).arg( p_sqlResult->index(i,10).data().toInt() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,11).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,12).data().toString() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "discountId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_DISCOUNT;

        if( poQuery ) delete poQuery;
    }

    if( m_bProcessSucceeded )
        acquirePaymentMethodsGlobals();
}
//====================================================================================
void cDBMirror::acquirePaymentMethodsGlobals()
//====================================================================================
{
    m_inProcessCount = MIRROR_GET_GLOBAL_PAYMENTMETHODS;

    _qId = g_poServer->sendQuery( QString("SELECT * FROM paymentMethods WHERE licenceId=0 AND active=1") );
}
//====================================================================================
void cDBMirror::_processPaymentMethodsGlobals( SqlResult *p_sqlResult )
//====================================================================================
{
    for( int i=0; i<p_sqlResult->rowCount();i++ )
    {
        QSqlQuery  *poQuery = NULL;
        QString     qsQuery = "";
        QString     qsServerTS = "";

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM paymentMethods WHERE paymentMethodId=%1 AND licenceId=0 " ).arg(p_sqlResult->index(i,0).data().toUInt()) );

        if( poQuery->first() )
        {
            qsServerTS = p_sqlResult->index(i,3).data().toString();
            if( qsServerTS.compare( poQuery->value( 3 ).toString() ) == 0 )
                continue;
        }

        if( poQuery->first() )
        {
            qsQuery = "UPDATE paymentMethods SET ";
        }
        else
        {
            qsQuery = "INSERT INTO paymentMethods SET ";
            qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += QString( "licenceId = \"0\", " );
        }
        qsQuery += QString( "name = \"%1\", " ).arg( p_sqlResult->index(i,2).data().toString() );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_sqlResult->index(i,3).data().toString() );
        qsQuery += QString( "active = %1, " ).arg( p_sqlResult->index(i,4).data().toString() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( poQuery->first() )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "paymentMethodId = \"%1\" " ).arg( p_sqlResult->index(i,0).data().toUInt() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"0\" " );
        }

        poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery->numRowsAffected() != 1 )
        {
            m_bProcessSucceeded     = false;
            m_bAcquireGlobalData    = false;
        }

        m_uiGlobalDataChanged |= DB_CASSA;

        if( poQuery ) delete poQuery;
    }

    _processGlobalsFinished();
}
//====================================================================================
void cDBMirror::_processGlobalsFinished()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"GLOBAL_DATA_UPDATED\" ").arg(m_qsServerTimestamp) );
    m_bAcquireGlobalData = false;
    m_bGlobalDataChanged = false;
}
//====================================================================================
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
void cDBMirror::synchronizeAllTable()
//====================================================================================
{
    m_bSyncAllTable     = true;
    m_bProcessSucceeded = true;
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    if( m_uiGlobalSyncLevel > 0 )
    {
        m_bGlobalDataSynchronize = true;
    }
    else
    {
        m_bGlobalDataSynchronize = false;
    }

    synchronizeUserTable();
}
//====================================================================================
void cDBMirror::synchronizeExit()
//====================================================================================
{
    m_bSyncExit = true;
}
//====================================================================================
void cDBMirror::synchronizeUserTable()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeUserTable();
}
//====================================================================================
void cDBMirror::_synchronizeUserTable( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        if( m_uiCurrentId == poQuery->value( 0 ).toUInt() )
            m_inCountOfTries++;
        else
            m_inCountOfTries = 0;

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
        qsQuery += QString( "comment = \"%1\", " ).arg(  obUser.comment() );
        qsQuery += QString( "modified = \"%1\", " ).arg( obUser.modified() );
        qsQuery += QString( "active = %1, " ).arg(  obUser.active() );
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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePatientOriginTable();
}
//====================================================================================
void cDBMirror::_synchronizePatientOriginTable( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeReasonToVisit();
}
//====================================================================================
void cDBMirror::_synchronizeReasonToVisit( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeIllnessGroup();
}
//====================================================================================
void cDBMirror::_synchronizeIllnessGroup( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePublicPlaces();
}
//====================================================================================
void cDBMirror::_synchronizePublicPlaces( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeHealthInsurance();
}
//====================================================================================
void cDBMirror::_synchronizeHealthInsurance( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeCompany();
}
//====================================================================================
void cDBMirror::_synchronizeCompany( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

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
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeDoctor();
}
//====================================================================================
void cDBMirror::_synchronizeDoctor( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

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
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePatient();
}
//====================================================================================
void cDBMirror::_synchronizePatient( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeAttendance();
}
//====================================================================================
void cDBMirror::_synchronizeAttendance( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePatientcardType();
}
//====================================================================================
void cDBMirror::_synchronizePatientcardType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePatientcard();
}
//====================================================================================
void cDBMirror::_synchronizePatientcard( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

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
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePatientcardHistory();
}
//====================================================================================
void cDBMirror::_synchronizePatientcardHistory( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
        else
            _synchronizeDatabaseFinished();
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePanelType();
}
//====================================================================================
void cDBMirror::_synchronizePanelType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelTypes WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPanelTypes       obPanelTypes;

        obPanelTypes.load( m_uiCurrentId );

        if( obPanelTypes.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO panelTypes SET ";
            qsQuery += QString( "panelTypeId = \"%1\", " ).arg( obPanelTypes.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPanelTypes.licenceId() );
        }
        else if( obPanelTypes.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE panelTypes SET ";
        }
        qsQuery += QString( "name = \"%1\", " ).arg( obPanelTypes.name() );
        qsQuery += QString( "active = %1, " ).arg( obPanelTypes.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPanelTypes.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "panelTypeId = \"%1\" " ).arg( obPanelTypes.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPanelTypes.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PANELTYPE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePanelStatus();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordPanelTypeSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE panelTypes SET archive=\"ARC\" WHERE panelTypeId=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelType();
}
//====================================================================================
void cDBMirror::synchronizePanelStatus()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePanelStatus();
}
//====================================================================================
void cDBMirror::_synchronizePanelStatus( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPanelStatuses    obPanelStatuses;

        obPanelStatuses.load( m_uiCurrentId );

        if( obPanelStatuses.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO panelStatuses SET ";
            qsQuery += QString( "panelStatusId = \"%1\", " ).arg( obPanelStatuses.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPanelStatuses.licenceId() );
        }
        else if( obPanelStatuses.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE panelStatuses SET ";
        }
        qsQuery += QString( "panelTypeId = %1, " ).arg( obPanelStatuses.panelTypeId() );
        qsQuery += QString( "seqNumber = %1, " ).arg( obPanelStatuses.sequenceNumber() );
        qsQuery += QString( "name = \"%1\", " ).arg( obPanelStatuses.name() );
        qsQuery += QString( "length = %1, " ).arg( obPanelStatuses.length() );
        qsQuery += QString( "activateCmd = %1, " ).arg( obPanelStatuses.activateCommand() );
        qsQuery += QString( "active = %1, " ).arg( obPanelStatuses.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPanelStatuses.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "panelStatusId = \"%1\" " ).arg( obPanelStatuses.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPanelStatuses.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PANELSTATUS;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePanel();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordPanelStatusSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE panelStatuses SET archive=\"ARC\" WHERE panelStatusId=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelStatus();
}
//====================================================================================
void cDBMirror::synchronizePanel()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePanel();
}
//====================================================================================
void cDBMirror::_synchronizePanel( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panels WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPanel            obPanel;

        obPanel.load( m_uiCurrentId );

        if( obPanel.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO panels SET ";
            qsQuery += QString( "panelId = \"%1\", " ).arg( obPanel.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPanel.licenceId() );
        }
        else if( obPanel.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE panels SET ";
        }
        qsQuery += QString( "panelTypeId = \"%1\", " ).arg( obPanel.panelTypeId() );
        qsQuery += QString( "title = \"%1\", " ).arg( obPanel.title() );
        qsQuery += QString( "workTime = \"%1\", " ).arg( obPanel.workTime() );
        qsQuery += QString( "maxWorkTime = \"%1\", " ).arg( obPanel.maxWorkTime() );
        qsQuery += QString( "active = %1, " ).arg( obPanel.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPanel.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "panelId = \"%1\" " ).arg( obPanel.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPanel.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PANEL;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizePanelUse();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordPanelSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE panels SET archive=\"ARC\" WHERE panelId=%1" ).arg(m_uiCurrentId) );

    _synchronizePanel();
}
//====================================================================================
void cDBMirror::synchronizePanelUse()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizePanelUse();
}
//====================================================================================
void cDBMirror::_synchronizePanelUse( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelUses WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBPanelUses        obPanelUses;

        obPanelUses.load( m_uiCurrentId );

        if( obPanelUses.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO panelUses SET ";
            qsQuery += QString( "panelUseId = \"%1\", " ).arg( obPanelUses.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obPanelUses.licenceId() );
        }
        else if( obPanelUses.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE panelUses SET ";
        }
        qsQuery += QString( "panelId = %1, " ).arg( obPanelUses.panelId() );
        qsQuery += QString( "name = \"%1\", " ).arg( obPanelUses.name() );
        qsQuery += QString( "useTime = %1, " ).arg( obPanelUses.useTime() );
        qsQuery += QString( "usePrice = %1, " ).arg( obPanelUses.usePrice() );
        qsQuery += QString( "active = %1, " ).arg( obPanelUses.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obPanelUses.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "panelUseId = \"%1\" " ).arg( obPanelUses.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obPanelUses.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_PANELUSE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeAttendanceSchedule();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordPanelUseSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE panelUses SET archive=\"ARC\" WHERE panelUseId=%1" ).arg(m_uiCurrentId) );

    _synchronizePanelUse();
}
//====================================================================================
void cDBMirror::synchronizeAttendanceSchedule()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeAttendanceSchedule();
}
//====================================================================================
void cDBMirror::_synchronizeAttendanceSchedule( unsigned int p_uiSyncLevel )
//====================================================================================
{
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeDenomination();
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
            synchronizeDenomination();
        else
            _synchronizeDatabaseFinished();
    }
*/
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeDenomination();
}
//====================================================================================
void cDBMirror::_synchronizeDenomination( unsigned int p_uiSyncLevel )
//====================================================================================
{
    // This table includes non changeable records
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeProductType();
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
            synchronizeProductType();
        else
            _synchronizeDatabaseFinished();
    }
*/
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeProductType();
}
//====================================================================================
void cDBMirror::_synchronizeProductType( unsigned int p_uiSyncLevel )
//====================================================================================
{
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeProduct();
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
            synchronizeProduct();
        else
            _synchronizeDatabaseFinished();
    }
*/
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeProduct();
}
//====================================================================================
void cDBMirror::_synchronizeProduct( unsigned int p_uiSyncLevel )
//====================================================================================
{
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeDiscount();
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

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
            synchronizeDiscount();
        else
            _synchronizeDatabaseFinished();
    }
*/
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
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeDiscount();
}
//====================================================================================
void cDBMirror::_synchronizeDiscount( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBDiscount         obDiscount;

        obDiscount.load( m_uiCurrentId );

        if( obDiscount.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO discounts SET ";
            qsQuery += QString( "discountId = \"%1\", " ).arg( obDiscount.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obDiscount.licenceId() );
        }
        else if( obDiscount.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE discounts SET ";
        }
        qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( obDiscount.healthInsuranceId() );
        qsQuery += QString( "companyId = \"%1\", " ).arg( obDiscount.companyId() );
        qsQuery += QString( "doctorId = \"%1\", " ).arg( obDiscount.doctorId() );
        qsQuery += QString( "regularCustomer = \"%1\", " ).arg( obDiscount.regularCustomer() );
        qsQuery += QString( "employee = \"%1\", " ).arg( obDiscount.employee() );
        qsQuery += QString( "service = \"%1\", " ).arg( obDiscount.service() );
        qsQuery += QString( "name = \"%1\", " ).arg( obDiscount.name() );
        qsQuery += QString( "discountValue = \"%1\", " ).arg( obDiscount.discountValue() );
        qsQuery += QString( "discountPercent = \"%1\", " ).arg( obDiscount.discountPercent() );
        qsQuery += QString( "active = %1, " ).arg( obDiscount.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obDiscount.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "discountId = \"%1\" " ).arg( obDiscount.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obDiscount.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_DISCOUNT;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeZipRegionCity();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordDiscountSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE discounts SET archive=\"ARC\" WHERE discountId=%1" ).arg(m_uiCurrentId) );

    _synchronizeDiscount();
}
//====================================================================================
void cDBMirror::synchronizeZipRegionCity()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeZipRegionCity();
}
//====================================================================================
void cDBMirror::_synchronizeZipRegionCity( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM zipRegionCity WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBZipRegionCity    obZipRegionCity;

        obZipRegionCity.load( m_uiCurrentId );

        if( obZipRegionCity.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO zipRegionCity SET ";
            qsQuery += QString( "zipRegionCityId = \"%1\", " ).arg( obZipRegionCity.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obZipRegionCity.licenceId() );
        }
        else if( obZipRegionCity.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE zipRegionCity SET ";
        }
        qsQuery += QString( "zip = \"%1\", " ).arg( obZipRegionCity.zip() );
        qsQuery += QString( "region = \"%1\", " ).arg( obZipRegionCity.region() );
        qsQuery += QString( "city = \"%1\", " ).arg( obZipRegionCity.city() );
        qsQuery += QString( "active = %1, " ).arg( obZipRegionCity.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obZipRegionCity.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "zipRegionCityId = \"%1\" " ).arg( obZipRegionCity.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obZipRegionCity.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_ZIPREGIONCITY;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeAddress();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordZipRegionCitySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE zipRegionCity SET archive=\"ARC\" WHERE zipRegionCityId=%1" ).arg(m_uiCurrentId) );

    _synchronizeZipRegionCity();
}
//====================================================================================
void cDBMirror::synchronizeAddress()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeAddress();
}
//====================================================================================
void cDBMirror::_synchronizeAddress( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM address WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBAddress          obAddress;

        obAddress.load( m_uiCurrentId );

        if( obAddress.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO address SET ";
            qsQuery += QString( "addressId = \"%1\", " ).arg( obAddress.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obAddress.licenceId() );
        }
        else if( obAddress.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE address SET ";
        }
        qsQuery += QString( "patientId = \"%1\", " ).arg( obAddress.patientId() );
        qsQuery += QString( "name = \"%1\", " ).arg( obAddress.name() );
        qsQuery += QString( "country = \"%1\", " ).arg( obAddress.country() );
        qsQuery += QString( "region = \"%1\", " ).arg( obAddress.region() );
        qsQuery += QString( "city = \"%1\", " ).arg( obAddress.city() );
        qsQuery += QString( "zip = \"%1\", " ).arg( obAddress.zip() );
        qsQuery += QString( "street = \"%1\", " ).arg( obAddress.street() );
        qsQuery += QString( "publicPlaceId = \"%1\", " ).arg( obAddress.publicPlaceId() );
        qsQuery += QString( "streetNumber = \"%1\", " ).arg( obAddress.streetNumber() );
        qsQuery += QString( "floor = \"%1\", " ).arg( obAddress.floor() );
        qsQuery += QString( "door = \"%1\", " ).arg( obAddress.door() );
        qsQuery += QString( "primaryAddress = %1, " ).arg( obAddress.primaryAddress() );
        qsQuery += QString( "active = %1, " ).arg( obAddress.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obAddress.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "addressId = \"%1\" " ).arg( obAddress.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obAddress.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_ADDRESS;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeCassa();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordAddressSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE address SET archive=\"ARC\" WHERE addressId=%1" ).arg(m_uiCurrentId) );

    _synchronizeAddress();
}
//====================================================================================
void cDBMirror::synchronizeCassa()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeCassa();
}
//====================================================================================
void cDBMirror::_synchronizeCassa( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBCassa            obCassa;

        obCassa.load( m_uiCurrentId );

        if( obCassa.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO cassa SET ";
            qsQuery += QString( "cassaId = \"%1\", " ).arg( obCassa.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obCassa.licenceId() );
        }
        else if( obCassa.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE cassa SET ";
        }
        qsQuery += QString( "userId = \"%1\", " ).arg( obCassa.userId() );
        qsQuery += QString( "currentBalance = \"%1\", " ).arg( obCassa.currentBalance() );
        qsQuery += QString( "startDateTime = \"%1\", " ).arg( obCassa.startDateTime() );
        qsQuery += QString( "stopDateTime = \"%1\", " ).arg( obCassa.stopDateTime() );
        qsQuery += QString( "active = %1, " ).arg( obCassa.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obCassa.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "cassaId = \"%1\" " ).arg( obCassa.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obCassa.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_CASSA;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeCassaHistory();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordCassaSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE cassa SET archive=\"ARC\" WHERE cassaId=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassa();
}
//====================================================================================
void cDBMirror::synchronizeCassaHistory()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeCassaHistory();
}
//====================================================================================
void cDBMirror::_synchronizeCassaHistory( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBCassaHistory     obCassaHistory;

        obCassaHistory.load( m_uiCurrentId );

        if( obCassaHistory.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO cassaHistory SET ";
            qsQuery += QString( "cassaHistoryId = \"%1\", " ).arg( obCassaHistory.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obCassaHistory.licenceId() );
        }
        else if( obCassaHistory.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE cassaHistory SET ";
        }
        qsQuery += QString( "cassaId = \"%1\", " ).arg( obCassaHistory.cassaId() );
        qsQuery += QString( "userId = \"%1\", " ).arg( obCassaHistory.userId() );
        qsQuery += QString( "actionValue = \"%1\", " ).arg( obCassaHistory.actionValue() );
        qsQuery += QString( "actionBalance = \"%1\", " ).arg( obCassaHistory.actionBalance() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obCassaHistory.comment() );
        qsQuery += QString( "active = %1, " ).arg( obCassaHistory.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obCassaHistory.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "cassaHistoryId = \"%1\" " ).arg( obCassaHistory.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obCassaHistory.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_CASSAHISTORY;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeCassaDenomination();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordCassaHistorySynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE cassaHistory SET archive=\"ARC\" WHERE cassaHistoryId=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassaHistory();
}
//====================================================================================
void cDBMirror::synchronizeCassaDenomination()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeCassaDenomination();
}
//====================================================================================
void cDBMirror::_synchronizeCassaDenomination( unsigned int p_uiSyncLevel )
//====================================================================================
{
    // This table is not in use
    // Currently no need to synchronize it with server
    _tableSynchronized( p_uiSyncLevel );
    _globalDataSynchronized( p_uiSyncLevel );
    if( m_bSyncAllTable )
        synchronizeLedgerDevice();
    else
        _synchronizeDatabaseFinished();
/*
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaDenominations WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString                 qsQuery = "";
        cDBCassaDenomination    obCassaDenomination;

        obCassaDenomination.load( m_uiCurrentId );

        if( obCassaDenomination.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO cassaDenominations SET ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\", " ).arg( obCassaDenomination.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obCassaDenomination.licenceId() );
        }
        else if( obCassaDenomination.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE cassaDenominations SET ";
        }
        qsQuery += QString( "denominationId = \"%1\", " ).arg( m_uiDenominationId );
        qsQuery += QString( "cassaId = \"%1\", " ).arg( m_uiCassaId );
        qsQuery += QString( "active = %1, " ).arg( obCassaDenomination.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obCassaDenomination.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "_TABLERECORD_Id = \"%1\" " ).arg( obCassaDenomination.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obCassaDenomination.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_CASSADENOMINATION;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeLedgerDevice();
        else
            _synchronizeDatabaseFinished();
    }
*/
}
//====================================================================================
void cDBMirror::_recordCassaDenominationSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE cassaDenominations SET archive=\"ARC\" WHERE _TABLERECORD_Id=%1" ).arg(m_uiCurrentId) );

    _synchronizeCassaDenomination();
}
//====================================================================================
void cDBMirror::synchronizeLedgerDevice()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeLedgerDevice();
}
//====================================================================================
void cDBMirror::_synchronizeLedgerDevice( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledgerDevice WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBLedgerDevice     obLedgerDevice;

        obLedgerDevice.load( m_uiCurrentId );

        if( obLedgerDevice.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO ledgerDevice SET ";
            qsQuery += QString( "ledgerDeviceId = \"%1\", " ).arg( obLedgerDevice.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obLedgerDevice.licenceId() );
        }
        else if( obLedgerDevice.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE ledgerDevice SET ";
        }
        qsQuery += QString( "userId = \"%1\", " ).arg( obLedgerDevice.userId() );
        qsQuery += QString( "panelId = \"%1\", " ).arg( obLedgerDevice.panelId() );
        qsQuery += QString( "patientId = \"%1\", " ).arg( obLedgerDevice.patientId() );
        qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( obLedgerDevice.paymentMethod() );
        qsQuery += QString( "units = \"%1\", " ).arg( obLedgerDevice.units() );
        qsQuery += QString( "cash = \"%1\", " ).arg( obLedgerDevice.cash() );
        qsQuery += QString( "timeReal = \"%1\", " ).arg( obLedgerDevice.timeReal() );
        qsQuery += QString( "timeLeft = \"%1\", " ).arg( obLedgerDevice.timeLeft() );
        qsQuery += QString( "timeCard = \"%1\", " ).arg( obLedgerDevice.timeCard() );
        qsQuery += QString( "timeCash = \"%1\", " ).arg( obLedgerDevice.timeCash() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obLedgerDevice.comment() );
        qsQuery += QString( "active = %1, " ).arg( obLedgerDevice.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obLedgerDevice.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "ledgerDeviceId = \"%1\" " ).arg( obLedgerDevice.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obLedgerDevice.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_LEDGERDEVICE;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );
        if( m_bSyncAllTable )
            synchronizeLedger();
        else
            _synchronizeDatabaseFinished();
    }
}
//====================================================================================
void cDBMirror::_recordLedgerDeviceSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE ledgerDevice SET archive=\"ARC\" WHERE ledgerDeviceId=%1" ).arg(m_uiCurrentId) );

    _synchronizeLedgerDevice();
}
//====================================================================================
void cDBMirror::synchronizeLedger()
//====================================================================================
{
    m_inCountOfTries    = 0;
    m_uiCurrentId       = 0;

    _synchronizeLedger();
}
//====================================================================================
void cDBMirror::_synchronizeLedger( unsigned int p_uiSyncLevel )
//====================================================================================
{
    if( m_uiDbModificationLevel > 0 && m_uiDbModificationLevel < p_uiSyncLevel )
    {
        _synchronizeDatabaseFinished();
        return;
    }

    if( m_inCountOfTries > 4 )
    {
        m_bProcessSucceeded = false;
        _synchronizeDatabaseFinished();
        return;
    }

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledger WHERE archive<>\"ARC\" AND licenceId <> 1 " ) );

    if( poQuery->first() )
    {
        m_uiCurrentId = poQuery->value( 0 ).toUInt();

        QString             qsQuery = "";
        cDBLedger           obLedger;

        obLedger.load( m_uiCurrentId );

        if( obLedger.archive().compare( "NEW" ) == 0 )
        {
            qsQuery = "INSERT INTO ledger SET ";
            qsQuery += QString( "ledgerId = \"%1\", " ).arg( obLedger.id() );
            qsQuery += QString( "licenceId = \"%1\", " ).arg( obLedger.licenceId() );
        }
        else if( obLedger.archive().compare( "MOD" ) == 0 )
        {
            qsQuery = "UPDATE ledger SET ";
        }
        qsQuery += QString( "ledgerTypeId = \"%1\", " ).arg( obLedger.ledgerTypeId() );
        qsQuery += QString( "ledgerDeviceId = \"%1\", " ).arg( obLedger.ledgerDeviceId() );
        qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( obLedger.paymentMethod() );
        qsQuery += QString( "userId = \"%1\", " ).arg( obLedger.userId() );
        qsQuery += QString( "productId = \"%1\", " ).arg( obLedger.productId() );
        qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( obLedger.patientCardTypeId() );
        qsQuery += QString( "patientCardId = \"%1\", " ).arg( obLedger.patientCardId() );
        qsQuery += QString( "panelId = \"%1\", " ).arg( obLedger.panelId() );
        qsQuery += QString( "name = \"%1\", " ).arg( obLedger.name() );
        qsQuery += QString( "netPrice = \"%1\", " ).arg( obLedger.netPrice() );
        qsQuery += QString( "discount = \"%1\", " ).arg( obLedger.discount() );
        qsQuery += QString( "vatpercent = \"%1\", " ).arg( obLedger.vatpercent() );
        qsQuery += QString( "totalPrice = \"%1\", " ).arg( obLedger.totalPrice() );
        qsQuery += QString( "comment = \"%1\", " ).arg( obLedger.comment() );
        qsQuery += QString( "active = %1, " ).arg( obLedger.active() );
        qsQuery += QString( "archive = \"ARC\" " );
        if( obLedger.archive().compare( "MOD" ) == 0 )
        {
            qsQuery += "WHERE ";
            qsQuery += QString( "ledgerId = \"%1\" " ).arg( obLedger.id() );
            qsQuery += "AND ";
            qsQuery += QString( "licenceId = \"%1\" " ).arg( obLedger.licenceId() );
        }

        m_inProcessCount = MIRROR_SYNC_DB_LEDGER;

        _qId = g_poServer->sendQuery( qsQuery );
    }
    else
    {
        _tableSynchronized( p_uiSyncLevel );
        _globalDataSynchronized( p_uiSyncLevel );

        _synchronizeDatabaseFinished();
//        if( m_bSyncAllTable )
//            synchronize_NEXT_TABLE_();
    }
}
//====================================================================================
void cDBMirror::_recordLedgerSynchronized()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE ledger SET archive=\"ARC\" WHERE ledgerId=%1" ).arg(m_uiCurrentId) );

    _synchronizeLedger();
}
//====================================================================================
void cDBMirror::_synchronizeDatabaseFinished()
//====================================================================================
{
    m_inProcessCount    = 0;
    m_bSyncAllTable     = false;
    m_bSyncExit         = false;
    m_uiCurrentId       = 0;
    m_inCountOfTries    = 0;
}
//====================================================================================
