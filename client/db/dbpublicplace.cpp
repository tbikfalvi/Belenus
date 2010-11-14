//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpublicplace.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpublicplace.h"

cDBPublicPlace::cDBPublicPlace()
{
    init();
}

cDBPublicPlace::~cDBPublicPlace()
{
}

void cDBPublicPlace::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsName        = p_qsName;
    m_qsModified        = p_qsModified;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;
}

void cDBPublicPlace::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "publicPlaceId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPublicPlace::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPublicPlace::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM publicPlaces WHERE publicPlaceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Publicplace id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPublicPlace::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPublicPlace::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM publicPlaces WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Publicplace name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPublicPlace::save() throw( cSevException )
{
    cTracer obTrace( "cDBPublicPlace::save" );
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
    qsQuery += " publicPlaces SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE publicPlaceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PUBLIC_PLACES );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PUBLIC_PLACES );
}

void cDBPublicPlace::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPublicPlace::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM publicPlaces ";
        }
        else
        {
            qsQuery = "UPDATE publicPlaces SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE publicPlaceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPublicPlace::createNew() throw()
{
    init();
}

unsigned int cDBPublicPlace::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPublicPlace::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPublicPlace::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBPublicPlace::name() const throw()
{
    return m_qsName;
}

void cDBPublicPlace::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBPublicPlace::modified() const throw()
{
    return m_qsModified;
}

bool cDBPublicPlace::active() const throw()
{
    return m_bActive;
}

void cDBPublicPlace::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPublicPlace::archive() const throw()
{
    return m_qsArchive;
}

void cDBPublicPlace::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

