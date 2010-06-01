//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
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

cDBPanelTypes::cDBPanelTypes()
{
    init();
}

cDBPanelTypes::~cDBPanelTypes()
{
}

void cDBPanelTypes::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const string &p_stName,
                             const bool p_bActive,
                             const string &p_stArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_stName        = p_stName;
    m_bActive       = p_bActive;
    m_stArchive     = p_stArchive;
}

void cDBPanelTypes::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "panelTypesId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPanelTypes::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelTypes WHERE panelTypesId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Paneltype id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelTypes::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM panelTypes WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

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

        if( m_stArchive.compare("NEW") != 0 )
        {
            m_stArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_stArchive = "NEW";
    }
    qsQuery += " panelTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "active = %1" ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPanelTypes::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelTypes::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM panelTypes ";
        }
        else
        {
            qsQuery = "UPDATE panelTypes SET active=0, archive=\"MOD\" ";
        }
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

string cDBPanelTypes::name() const throw()
{
    return m_stName;
}

void cDBPanelTypes::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

bool cDBPanelTypes::active() const throw()
{
    return m_bActive;
}

void cDBPanelTypes::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBPanelTypes::archive() const throw()
{
    return m_stArchive;
}

void cDBPanelTypes::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

