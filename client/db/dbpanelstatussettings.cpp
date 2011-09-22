//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpanelstatussettings.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpanelstatussettings.h"

cDBPanelStatusSettings::cDBPanelStatusSettings()
{
    init();
}

cDBPanelStatusSettings::~cDBPanelStatusSettings()
{
}

void cDBPanelStatusSettings::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiPanelStatusId,
                             const QString &p_qsBackgroundColor,
                             const QString &p_qsStatusFontName,
                             const int p_inStatusFontSize,
                             const QString &p_qsStatusFontColor,
                             const QString &p_qsTimerFontName,
                             const int p_inTimerFontSize,
                             const QString &p_qsTimerFontColor,
                             const QString &p_qsNextFontName,
                             const int p_inNextFontSize,
                             const QString &p_qsNextFontColor,
                             const QString &p_qsInfoFontName,
                             const int p_inInfoFontSize,
                             const QString &p_qsInfoFontColor,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPanelStatusId   = p_uiPanelStatusId;
    m_qsBackgroundColor = p_qsBackgroundColor;
    m_qsStatusFontName  = p_qsStatusFontName;
    m_inStatusFontSize  = p_inStatusFontSize;
    m_qsStatusFontColor = p_qsStatusFontColor;
    m_qsTimerFontName   = p_qsTimerFontName;
    m_inTimerFontSize   = p_inTimerFontSize;
    m_qsTimerFontColor  = p_qsTimerFontColor;
    m_qsNextFontName    = p_qsNextFontName;
    m_inNextFontSize    = p_inNextFontSize;
    m_qsNextFontColor   = p_qsNextFontColor;
    m_qsInfoFontName    = p_qsInfoFontName;
    m_inInfoFontSize    = p_inInfoFontSize;
    m_qsInfoFontColor   = p_qsInfoFontColor;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPanelStatusSettings::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "panelStatusSettingId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPanelStatusIdIdx      = p_obRecord.indexOf( "panelStatusId" );
    int inBackgroundColorIdx    = p_obRecord.indexOf( "backgroundColor" );
    int inStatusFontNameIdx     = p_obRecord.indexOf( "statusFontName" );
    int inStatusFontSizeIdx     = p_obRecord.indexOf( "statusFontSize" );
    int inStatusFontColorIdx    = p_obRecord.indexOf( "statusFontColor" );
    int inTimerFontNameIdx      = p_obRecord.indexOf( "timerFontName" );
    int inTimerFontSizeIdx      = p_obRecord.indexOf( "timerFontSize" );
    int inTimerFontColorIdx     = p_obRecord.indexOf( "timerFontColor" );
    int inNextFontNameIdx       = p_obRecord.indexOf( "nextFontName" );
    int inNextFontSizeIdx       = p_obRecord.indexOf( "nextFontSize" );
    int inNextFontColorIdx      = p_obRecord.indexOf( "nextFontColor" );
    int inInfoFontNameIdx       = p_obRecord.indexOf( "infoFontName" );
    int inInfoFontSizeIdx       = p_obRecord.indexOf( "infoFontSize" );
    int inInfoFontColorIdx      = p_obRecord.indexOf( "infoFontColor" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPanelStatusIdIdx ).toUInt(),
          p_obRecord.value( inBackgroundColorIdx ).toString(),
          p_obRecord.value( inStatusFontNameIdx ).toString(),
          p_obRecord.value( inStatusFontSizeIdx ).toInt(),
          p_obRecord.value( inStatusFontColorIdx ).toString(),
          p_obRecord.value( inTimerFontNameIdx ).toString(),
          p_obRecord.value( inTimerFontSizeIdx ).toInt(),
          p_obRecord.value( inTimerFontColorIdx ).toString(),
          p_obRecord.value( inNextFontNameIdx ).toString(),
          p_obRecord.value( inNextFontSizeIdx ).toInt(),
          p_obRecord.value( inNextFontColorIdx ).toString(),
          p_obRecord.value( inInfoFontNameIdx ).toString(),
          p_obRecord.value( inInfoFontSizeIdx ).toInt(),
          p_obRecord.value( inInfoFontColorIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPanelStatusSettings::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatusSettings::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatusSettings WHERE panelStatusSettingId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panelstatus settings id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelStatusSettings::loadStatus( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatusSettings::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatusSettings WHERE panelStatusId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panelstatus settings id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelStatusSettings::save() throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatusSettings::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive.compare("NEW") != 0 )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " panelStatusSettings SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelStatusId = \"%1\", " ).arg( m_uiPanelStatusId );
    qsQuery += QString( "backgroundColor = \"%1\", " ).arg( m_qsBackgroundColor );
    qsQuery += QString( "statusFontName = \"%1\", " ).arg( m_qsStatusFontName );
    qsQuery += QString( "statusFontSize = \"%1\", " ).arg( m_inStatusFontSize );
    qsQuery += QString( "statusFontColor = \"%1\", " ).arg( m_qsStatusFontColor );
    qsQuery += QString( "timerFontName = \"%1\", " ).arg( m_qsTimerFontName );
    qsQuery += QString( "timerFontSize = \"%1\", " ).arg( m_inTimerFontSize );
    qsQuery += QString( "timerFontColor = \"%1\", " ).arg( m_qsTimerFontColor );
    qsQuery += QString( "nextFontName = \"%1\", " ).arg( m_qsNextFontName );
    qsQuery += QString( "nextFontSize = \"%1\", " ).arg( m_inNextFontSize );
    qsQuery += QString( "nextFontColor = \"%1\", " ).arg( m_qsNextFontColor );
    qsQuery += QString( "infoFontName = \"%1\", " ).arg( m_qsInfoFontName );
    qsQuery += QString( "infoFontSize = \"%1\", " ).arg( m_inInfoFontSize );
    qsQuery += QString( "infoFontColor = \"%1\", " ).arg( m_qsInfoFontColor );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelStatusSettingId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PANEL_STATUS );
*/
}

void cDBPanelStatusSettings::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatusSettings::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM panelStatusSettings ";
        }
        else
        {
            qsQuery = "UPDATE panelStatusSettings SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE panelStatusSettingId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPanelStatusSettings::createNew() throw()
{
    init();
}

unsigned int cDBPanelStatusSettings::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPanelStatusSettings::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPanelStatusSettings::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPanelStatusSettings::panelStatusId() const throw()
{
    return m_uiPanelStatusId;
}

void cDBPanelStatusSettings::setPanelStatusId( const unsigned int p_uiPanelStatusId ) throw()
{
    m_uiPanelStatusId = p_uiPanelStatusId;
}

QString cDBPanelStatusSettings::backgroundColor() const throw()
{
    return m_qsBackgroundColor;
}

void cDBPanelStatusSettings::setBackgroundColor( const QString &p_qsBackgroundColor ) throw()
{
    m_qsBackgroundColor = p_qsBackgroundColor;
}

QString cDBPanelStatusSettings::statusFontName() const throw()
{
    return m_qsStatusFontName;
}
void cDBPanelStatusSettings::setStatusFontName( const QString &p_qsStatusFontName ) throw()
{
    m_qsStatusFontName = p_qsStatusFontName;
}
int cDBPanelStatusSettings::statusFontSize() const throw()
{
    return m_inStatusFontSize;
}
void cDBPanelStatusSettings::setStatusFontSize( const int p_inStatusFontSize ) throw()
{
    m_inStatusFontSize = p_inStatusFontSize;
}
QString cDBPanelStatusSettings::statusFontColor() const throw()
{
    return m_qsStatusFontColor;
}
void cDBPanelStatusSettings::setStatusFontColor( const QString &p_qsStatusFontColor ) throw()
{
    m_qsStatusFontColor = p_qsStatusFontColor;
}
QString cDBPanelStatusSettings::timerFontName() const throw()
{
    return m_qsTimerFontName;
}
void cDBPanelStatusSettings::setTimerFontName( const QString &p_qsTimerFontName ) throw()
{
    m_qsTimerFontName = p_qsTimerFontName;
}
int cDBPanelStatusSettings::timerFontSize() const throw()
{
    return m_inTimerFontSize;
}
void cDBPanelStatusSettings::setTimerFontSize( const int p_inTimerFontSize ) throw()
{
    m_inTimerFontSize = p_inTimerFontSize;
}
QString cDBPanelStatusSettings::timerFontColor() const throw()
{
    return m_qsTimerFontColor;
}
void cDBPanelStatusSettings::setTimerFontColor( const QString &p_qsTimerFontColor ) throw()
{
    m_qsTimerFontColor = p_qsTimerFontColor;
}
QString cDBPanelStatusSettings::nextFontName() const throw()
{
    return m_qsNextFontName;
}
void cDBPanelStatusSettings::setNextFontName( const QString &p_qsNextFontName ) throw()
{
    m_qsNextFontName = p_qsNextFontName;
}
int cDBPanelStatusSettings::nextFontSize() const throw()
{
    return m_inNextFontSize;
}
void cDBPanelStatusSettings::setNextFontSize( const int p_inNextFontSize ) throw()
{
    m_inNextFontSize = p_inNextFontSize;
}
QString cDBPanelStatusSettings::nextFontColor() const throw()
{
    return m_qsNextFontColor;
}
void cDBPanelStatusSettings::setNextFontColor( const QString &p_qsNextFontColor ) throw()
{
    m_qsNextFontColor = p_qsNextFontColor;
}
QString cDBPanelStatusSettings::infoFontName() const throw()
{
    return m_qsInfoFontName;
}
void cDBPanelStatusSettings::setInfoFontName( const QString &p_qsInfoFontName ) throw()
{
    m_qsInfoFontName = p_qsInfoFontName;
}
int cDBPanelStatusSettings::infoFontSize() const throw()
{
    return m_inInfoFontSize;
}
void cDBPanelStatusSettings::setInfoFontSize( const int p_inInfoFontSize ) throw()
{
    m_inInfoFontSize = p_inInfoFontSize;
}
QString cDBPanelStatusSettings::infoFontColor() const throw()
{
    return m_qsInfoFontColor;
}
void cDBPanelStatusSettings::setInfoFontColor( const QString &p_qsInfoFontColor ) throw()
{
    m_qsInfoFontColor = p_qsInfoFontColor;
}

QString cDBPanelStatusSettings::modified() const throw()
{
    return m_qsModified;
}

bool cDBPanelStatusSettings::active() const throw()
{
    return m_bActive;
}

void cDBPanelStatusSettings::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPanelStatusSettings::archive() const throw()
{
    return m_qsArchive;
}

void cDBPanelStatusSettings::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

