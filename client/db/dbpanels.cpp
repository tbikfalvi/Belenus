//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpanels.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpanels.h"

cDBPanel::cDBPanel()
{
    init();
}

cDBPanel::~cDBPanel()
{
}

void cDBPanel::init( const unsigned int p_uiId,
                     const unsigned int p_uiLicenceId,
                     const unsigned int p_uiPanelTypeId,
                     const unsigned int p_uiPanelGroupId,
                     const QString &p_qsTitle,
                     const unsigned int p_uiWorkTime,
                     const unsigned int p_uiMaxWorkTime,
                     const unsigned int p_uiCleanTime,
                     const unsigned int p_uiMaxCleanTime,
                     const QString &p_qsModified,
                     const bool p_bActive,
                     const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPanelTypeId     = p_uiPanelTypeId;
    m_uiPanelGroupId    = p_uiPanelGroupId;
    m_qsTitle           = p_qsTitle;
    m_uiWorkTime        = p_uiWorkTime;
    m_uiMaxWorkTime     = p_uiMaxWorkTime;
    m_uiCleanTime       = p_uiCleanTime;
    m_uiMaxCleanTime    = p_uiMaxCleanTime;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPanel::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "panelId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPanelTypeIdIdx    = p_obRecord.indexOf( "panelTypeId" );
    int inPanelGroupIdIdx   = p_obRecord.indexOf( "panelGroupId" );
    int inTitleIdx          = p_obRecord.indexOf( "title" );
    int inWorkTimeIdx       = p_obRecord.indexOf( "workTime" );
    int inMaxWorkTimeIdx    = p_obRecord.indexOf( "maxWorkTime" );
    int inCleanTimeIdx      = p_obRecord.indexOf( "cleanTime" );
    int inMaxCleanTimeIdx   = p_obRecord.indexOf( "maxCleanTime" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPanelTypeIdIdx ).toUInt(),
          p_obRecord.value( inPanelGroupIdIdx ).toUInt(),
          p_obRecord.value( inTitleIdx ).toString(),
          p_obRecord.value( inWorkTimeIdx ).toUInt(),
          p_obRecord.value( inMaxWorkTimeIdx ).toUInt(),
          p_obRecord.value( inCleanTimeIdx ).toUInt(),
          p_obRecord.value( inMaxCleanTimeIdx ).toUInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPanel::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanel::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panels WHERE panelId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panel id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanel::save() throw( cSevException )
{
    cTracer obTrace( "cDBPanel::save" );
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
    qsQuery += " panels SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelTypeId = \"%1\", " ).arg( m_uiPanelTypeId );
    qsQuery += QString( "panelGroupId = \"%1\", " ).arg( m_uiPanelGroupId );
    qsQuery += QString( "title = \"%1\", " ).arg( m_qsTitle );
    qsQuery += QString( "workTime = \"%1\", " ).arg( m_uiWorkTime );
    qsQuery += QString( "maxWorkTime = \"%1\", " ).arg( m_uiMaxWorkTime );
    qsQuery += QString( "cleanTime = \"%1\", " ).arg( m_uiCleanTime );
    qsQuery += QString( "maxCleanTime = \"%1\", " ).arg( m_uiMaxCleanTime );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PANEL );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PANEL );
*/
}

void cDBPanel::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanel::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM panels ";
        }
        else
        {
            qsQuery = "UPDATE panels SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE panelId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPanel::createNew() throw()
{
    init();
}

unsigned int cDBPanel::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPanel::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPanel::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPanel::panelTypeId() const throw()
{
    return m_uiPanelTypeId;
}

void cDBPanel::setPanelTypeId( const unsigned int p_uiPanelTypeId ) throw()
{
    m_uiPanelTypeId = p_uiPanelTypeId;
}

unsigned int cDBPanel::panelGroupId() const throw()
{
    return m_uiPanelGroupId;
}

void cDBPanel::setPanelGroupId( const unsigned int p_uiPanelGroupId ) throw()
{
    m_uiPanelGroupId = p_uiPanelGroupId;
}

QString cDBPanel::title() const throw()
{
    return m_qsTitle;
}

void cDBPanel::setTitle( const QString &p_qsTitle ) throw()
{
    m_qsTitle = p_qsTitle;
    m_qsTitle = m_qsTitle.replace( QString("\""), QString("\\\"") );
}

unsigned int cDBPanel::workTime() const throw()
{
    return m_uiWorkTime;
}

void cDBPanel::setWorkTime( const unsigned int p_uiWorkTime ) throw()
{
    m_uiWorkTime = p_uiWorkTime;
}

unsigned int cDBPanel::maxWorkTime() const throw()
{
    return m_uiMaxWorkTime;
}

void cDBPanel::setMaxWorkTime( const unsigned int p_uiMaxWorkTime ) throw()
{
    m_uiMaxWorkTime = p_uiMaxWorkTime;
}

unsigned int cDBPanel::cleanTime() const throw()
{
    return m_uiCleanTime;
}

void cDBPanel::setCleanTime( const unsigned int p_uiCleanTime ) throw()
{
    m_uiCleanTime = p_uiCleanTime;
}

unsigned int cDBPanel::maxCleanTime() const throw()
{
    return m_uiMaxCleanTime;
}

void cDBPanel::setMaxCleanTime( const unsigned int p_uiMaxCleanTime ) throw()
{
    m_uiMaxCleanTime = p_uiMaxCleanTime;
}

QString cDBPanel::modified() const throw()
{
    return m_qsModified;
}

bool cDBPanel::active() const throw()
{
    return m_bActive;
}

void cDBPanel::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPanel::archive() const throw()
{
    return m_qsArchive;
}

void cDBPanel::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

