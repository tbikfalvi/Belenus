//============================================================================================================================
//
// Liszensz manager
//
//============================================================================================================================

#include <QSettings>
#include <QDateTime>

//----------------------------------------------------------------------------------------------------------------------------

#include "../framework/qtframework.h"
#include "licenceManager.h"
#include "preferences.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"
#include "general.h"

//----------------------------------------------------------------------------------------------------------------------------

extern cPreferences        *g_poPrefs;
extern cQTMySQLConnection  *g_poDB;
extern cQTLogger            g_obLogger;
extern cGeneral             g_obGen;

//============================================================================================================================
cLicenceManager::cLicenceManager()
//----------------------------------------------------------------------------------------------------------------------------
{
    m_uiLicenceId       = 1;
    m_qsLicenceString   = "BLNS_SERIAL_DEMO";
    m_qdLastValidated   = QDate::currentDate();
    m_qsState           = "DEMO";
    m_LicenceType       = cLicenceManager::LTYPE_DEMO;
}

//============================================================================================================================
cLicenceManager::~cLicenceManager()
//----------------------------------------------------------------------------------------------------------------------------
{
}

//============================================================================================================================
void cLicenceManager::initialize()
//----------------------------------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::INFO) << "Licence initialization started" << EOM;

    QSqlQuery   *poQuery = NULL;

    try
    {
        // Get the actually used licence data
        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_uiLicenceId       = poQuery->value( 0 ).toUInt();
            m_qsLicenceString   = poQuery->value( 1 ).toString();
            m_qdLastValidated   = poQuery->value( 9 ).toDate();
            m_qsState           = poQuery->value( 10 ).toString();
        }

        g_poPrefs->setLicenceId( m_uiLicenceId );

        g_obLogger(cSeverity::INFO) << "Initialization finished successfully" << EOM;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

//============================================================================================================================
unsigned int cLicenceManager::licenceID()
//----------------------------------------------------------------------------------------------------------------------------
{
    return m_uiLicenceId;
}

//============================================================================================================================
QString cLicenceManager::licenceSerialString()
//----------------------------------------------------------------------------------------------------------------------------
{
    return m_qsLicenceString;
}

//============================================================================================================================
cLicenceManager::licenceType cLicenceManager::licenceState()
//----------------------------------------------------------------------------------------------------------------------------
{
    if( m_qsState.compare( "VALIDATED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_VALIDATED;
    }
    else if( m_qsState.compare( "UNVALIDATED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_UNVALIDATED;
    }
    else if( m_qsState.compare( "UNREGISTERED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_UNREGISTERED;
    }
    else if( m_qsState.compare( "INVALID" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_INVALID;
    }
    else
    {
        m_LicenceType = cLicenceManager::LTYPE_DEMO;
    }

    return m_LicenceType;
}

//============================================================================================================================
cLicenceManager::licenceType cLicenceManager::checkLicenceState()
//----------------------------------------------------------------------------------------------------------------------------
{
    QSqlQuery   *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT type FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_qsState = poQuery->value( 0 ).toString();
        }
        g_obLogger(cSeverity::DEBUG) << "Licence current state: " << m_qsState << EOM;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return licenceState();
}

//============================================================================================================================
void cLicenceManager::deactivate()
//----------------------------------------------------------------------------------------------------------------------------
{
    try
    {
        m_qsState = "INVALID";
        m_LicenceType = cLicenceManager::LTYPE_INVALID;

        g_poDB->executeQTQuery( QString( "UPDATE licences SET type='%1' WHERE licenceId = %2 " ).arg( m_qsState ).arg( m_uiLicenceId ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

//============================================================================================================================
void cLicenceManager::sysadminActivateLicence()
//----------------------------------------------------------------------------------------------------------------------------
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE licences SET "
                                         "lastValidated=\"%1\", "
                                         "type=\"VALIDATED\", "
                                         "active= 1, "
                                         "archive= \"MOD\" "
                                         "WHERE licenceId=%2" )
                                        .arg( QDate::currentDate().toString("yyyy-MM-dd") )
                                        .arg( m_uiLicenceId ) );

        g_obGen.showTrayInfo( tr( "Manual licence key activation succeeded." ) );
        g_obLogger(cSeverity::INFO) << "System admin manually validated the licence key" << EOM;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}



