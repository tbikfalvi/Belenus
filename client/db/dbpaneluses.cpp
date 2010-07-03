//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpaneluses.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpaneluses.h"

cDBPanelUses::cDBPanelUses()
{
    init();
}

cDBPanelUses::~cDBPanelUses()
{
}

void cDBPanelUses::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiPanelId,
                             const unsigned int p_uiUseTime,
                             const unsigned int p_uiUsePrice,
                             const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPanelId         = p_uiPanelId;
    m_uiUseTime         = p_uiUseTime;
    m_uiUsePrice        = p_uiUsePrice;
    m_stArchive         = p_stArchive;
}

void cDBPanelUses::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "panelUseId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPanelIdIdx        = p_obRecord.indexOf( "panelId" );
    int inUseTimeIdx        = p_obRecord.indexOf( "useTime" );
    int inUsePriceIdx       = p_obRecord.indexOf( "usePrice" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPanelIdIdx ).toInt(),
          p_obRecord.value( inUseTimeIdx ).toInt(),
          p_obRecord.value( inUsePriceIdx ).toInt(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPanelUses::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelUses::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelUses WHERE panelUseId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Paneluse id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelUses::save() throw( cSevException )
{
    cTracer obTrace( "cDBPanelUses::save" );
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
    qsQuery += " panelUses SET ";
    qsQuery += QString( "licenceId = %1, " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelId = %1, " ).arg( m_uiPanelId );
    qsQuery += QString( "useTime = %1, " ).arg( m_uiUseTime );
    qsQuery += QString( "usePrice = %1, " ).arg( m_uiUsePrice );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelUseId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPanelUses::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelUses::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM panelUses ";
        }
        else
        {
            qsQuery = "UPDATE panelUses SET archive=\"DEL\" ";
        }
        qsQuery += QString( " WHERE panelUseId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPanelUses::createNew() throw()
{
    init();
}

unsigned int cDBPanelUses::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPanelUses::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPanelUses::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPanelUses::panelId() const throw()
{
    return m_uiPanelId;
}

void cDBPanelUses::setPanelId( const unsigned int p_uiPanelId ) throw()
{
    m_uiPanelId = p_uiPanelId;
}

unsigned int cDBPanelUses::useTime() const throw()
{
    return m_uiUseTime;
}

void cDBPanelUses::setUseTime( const unsigned int p_uiUseTime ) throw()
{
    m_uiUseTime = p_uiUseTime;
}

unsigned int cDBPanelUses::usePrice() const throw()
{
    return m_uiUsePrice;
}

void cDBPanelUses::setUsePrice( const unsigned int p_uiUsePrice ) throw()
{
    m_uiUsePrice = p_uiUsePrice;
}

string cDBPanelUses::archive() const throw()
{
    return m_stArchive;
}

void cDBPanelUses::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

