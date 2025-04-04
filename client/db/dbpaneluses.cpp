//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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

#include <QMessageBox>

#include "belenus.h"
#include "dbpaneluses.h"

cDBPanelUses::cDBPanelUses()
{
    init();
}

cDBPanelUses::~cDBPanelUses()
{
}

void cDBPanelUses::init(const unsigned int p_uiId,
                         const unsigned int p_uiLicenceId,
                         const QString &p_qsPanelIds,
                         const QString &p_qsName,
                         const unsigned int p_uiUseTime,
                         const unsigned int p_uiUsePrice,
                         const QString &p_qsModified,
                         const bool p_bActive,
                         const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_qsPanelIds        = p_qsPanelIds;
    m_qsName            = p_qsName;
    m_uiUseTime         = p_uiUseTime;
    m_uiUsePrice        = p_uiUsePrice;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPanelUses::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "panelUseId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPanelIdsIdx       = p_obRecord.indexOf( "panelIds" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inUseTimeIdx        = p_obRecord.indexOf( "useTime" );
    int inUsePriceIdx       = p_obRecord.indexOf( "usePrice" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdsIdx ).toString(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inUseTimeIdx ).toUInt(),
          p_obRecord.value( inUsePriceIdx ).toUInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPanelUses::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelUses::load", QString( "id: %1" ).arg( p_uiId ) );

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
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " panelUses SET ";
    qsQuery += QString( "licenceId = %1, " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelIds = \"%1\", " ).arg( m_qsPanelIds );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "useTime = %1, " ).arg( m_uiUseTime );
    qsQuery += QString( "usePrice = %1, " ).arg( m_uiUsePrice );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
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
        g_poDB->executeQTQuery( QString( "DELETE FROM panelUses WHERE panelUseId = %1" ).arg( m_uiId ) );
    }
}

bool cDBPanelUses::isPanelUseExists() throw()
{
    bool bRet = false;

/*    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelUses WHERE panelId=%1 AND useTime=%2 AND usePrice=%3" ).arg( m_uiPanelId ).arg(m_uiUseTime).arg(m_uiUsePrice) );

    if( poQuery->size() > 0 )
        bRet = true;
*/
    return bRet;
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

QString cDBPanelUses::panelIds() const throw()
{
    return m_qsPanelIds;
}

void cDBPanelUses::setPanelIds(const QString &p_qsPanelIds ) throw()
{
    m_qsPanelIds = p_qsPanelIds;
}

QString cDBPanelUses::name() const throw()
{
    return m_qsName;
}

void cDBPanelUses::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
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

QString cDBPanelUses::modified() const throw()
{
    return m_qsModified;
}

bool cDBPanelUses::active() const throw()
{
    return m_bActive;
}

void cDBPanelUses::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPanelUses::archive() const throw()
{
    return m_qsArchive;
}

void cDBPanelUses::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

