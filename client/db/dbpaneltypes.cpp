//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpaneltypes.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpaneltypes.h"
#include "dbpanelstatuses.h"

cDBPanelTypes::cDBPanelTypes()
{
    init();
}

cDBPanelTypes::~cDBPanelTypes()
{
}

void cDBPanelTypes::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsName        = p_qsName;
    m_qsModified    = p_qsModified;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;
}

void cDBPanelTypes::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "panelTypeId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inModifiedIdx   = p_obRecord.indexOf( "modified" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPanelTypes::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelTypes WHERE panelTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Paneltype id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelTypes::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM panelTypes WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Paneltype name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelTypes::save() throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " panelTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENT_ORIGIN );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENT_ORIGIN );
*/
}

void cDBPanelTypes::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::remove" );

    if( m_uiId )
    {
        cDBPanelStatuses    obDBPanelStatuses;

        obDBPanelStatuses.remove( m_uiId );

        QString  qsQuery;

//        if( m_qsArchive != "NEW" )
//        {
            qsQuery = "DELETE FROM panelTypes ";
//        }
//        else
//        {
//            qsQuery = "UPDATE panelTypes SET active=0, archive=\"MOD\" ";
//        }
        qsQuery += QString( " WHERE panelTypeId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPanelTypes::createNew() throw()
{
    init();
}

unsigned int cDBPanelTypes::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPanelTypes::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPanelTypes::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBPanelTypes::name() const throw()
{
    return m_qsName;
}

void cDBPanelTypes::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBPanelTypes::modified() const throw()
{
    return m_qsModified;
}

bool cDBPanelTypes::active() const throw()
{
    return m_bActive;
}

void cDBPanelTypes::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPanelTypes::archive() const throw()
{
    return m_qsArchive;
}

void cDBPanelTypes::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
