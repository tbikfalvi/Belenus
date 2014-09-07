//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbadvertisements.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <QDateTime>

#include "../client/belenus.h"
#include "dbadvertisements.h"

cDBAdvertisements::cDBAdvertisements()
{
    init();
}

cDBAdvertisements::~cDBAdvertisements()
{
}

void cDBAdvertisements::init(const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const QString &p_qsCaption,
                             const QString &p_qsBackgroundColor,
                             const QString &p_qsPath,
                             const QString &p_qsFilenames,
                             const unsigned int p_uiTimerLength,
                             const bool p_bTransparent,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsName        = p_qsName;
    m_qsCaption     = p_qsCaption;
    m_qsBackgroundColor = p_qsBackgroundColor;
    m_qsPath        = p_qsPath;
    m_qsFilenames   = p_qsFilenames;
    m_nTimerLength  = p_uiTimerLength;
    m_bTransparent  = p_bTransparent;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;
}

void cDBAdvertisements::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "advertisementId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inCaptionIdx       = p_obRecord.indexOf( "caption" );
    int inBackgroundColor   = p_obRecord.indexOf( "backgroundColor" );
    int inPathIdx       = p_obRecord.indexOf( "path" );
    int inFilenamesIdx       = p_obRecord.indexOf( "fileNames" );
    int inTimerLengthIdx       = p_obRecord.indexOf( "timer" );
    int inTransparentIdx       = p_obRecord.indexOf( "transparent" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inCaptionIdx ).toString(),
          p_obRecord.value( inBackgroundColor ).toString(),
          p_obRecord.value( inPathIdx ).toString(),
          p_obRecord.value( inFilenamesIdx ).toString(),
          p_obRecord.value( inTimerLengthIdx ).toInt(),
          p_obRecord.value( inTransparentIdx ).toBool(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBAdvertisements::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBAdvertisements::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM advertisements WHERE advertisementId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Advertisement id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAdvertisements::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBAdvertisements::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM advertisements WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Advertisement name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAdvertisements::save() throw( cSevException )
{
    cTracer obTrace( "cDBAdvertisements::save" );
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
    qsQuery += " advertisements SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "caption = \"%1\", " ).arg( m_qsCaption );
    qsQuery += QString( "backgroundColor = \"%1\", " ).arg( m_qsBackgroundColor );
    qsQuery += QString( "path = \"%1\", " ).arg( m_qsPath );
    qsQuery += QString( "fileNames = \"%1\", " ).arg( m_qsFilenames );
    qsQuery += QString( "timer = \"%1\", " ).arg( m_nTimerLength );
    qsQuery += QString( "transparent = %1, " ).arg( m_bTransparent );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE advertisementId = %1" ).arg( m_uiId );
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

void cDBAdvertisements::remove() throw( cSevException )
{
    cTracer obTrace( "cDBAdvertisements::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

//        if( m_qsArchive != "NEW" )
//        {
            qsQuery = "DELETE FROM advertisements ";
//        }
//        else
//        {
//            qsQuery = "UPDATE advertisements SET active=0, archive=\"MOD\" ";
//        }
        qsQuery += QString( " WHERE advertisementId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBAdvertisements::createNew() throw()
{
    init();
}

unsigned int cDBAdvertisements::id() const throw()
{
    return m_uiId;
}

unsigned int cDBAdvertisements::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBAdvertisements::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBAdvertisements::name() const throw()
{
    return m_qsName;
}
void cDBAdvertisements::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
}
QString cDBAdvertisements::caption() const throw()
{
    return m_qsCaption;
}
void cDBAdvertisements::setCaption( const QString &p_qsCaption ) throw()
{
    m_qsCaption = p_qsCaption;
}
QString cDBAdvertisements::backgroundColor() const throw()
{
    return m_qsBackgroundColor;
}
void cDBAdvertisements::setBackgroundColor(const QString &p_qsColor) throw()
{
    m_qsBackgroundColor = p_qsColor;
}
QString cDBAdvertisements::path() const throw()
{
    return m_qsPath;
}
void cDBAdvertisements::setPath( const QString &p_qsPath ) throw()
{
    m_qsPath = p_qsPath;
}
QString cDBAdvertisements::filenames() const throw()
{
    return m_qsFilenames;
}
void cDBAdvertisements::setFilenames( const QString &p_qsFilenames ) throw()
{
    m_qsFilenames = p_qsFilenames;
}
unsigned int cDBAdvertisements::timerLength() const throw()
{
    return m_nTimerLength;
}
void cDBAdvertisements::setTimerLength( const unsigned int p_nTimerLength ) throw()
{
    m_nTimerLength = p_nTimerLength;
}
bool cDBAdvertisements::transparent() const throw()
{
    return m_bTransparent;
}
void cDBAdvertisements::setTransparent( const bool p_bTransparent ) throw()
{
    m_bTransparent = p_bTransparent;
}

bool cDBAdvertisements::active() const throw()
{
    return m_bActive;
}

void cDBAdvertisements::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBAdvertisements::archive() const throw()
{
    return m_qsArchive;
}

void cDBAdvertisements::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
