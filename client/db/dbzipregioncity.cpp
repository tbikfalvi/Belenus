//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbzipregioncity.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <qmessagebox.h>
#include "belenus.h"
#include "dbzipregioncity.h"

cDBZipRegionCity::cDBZipRegionCity()
{
    init();
}

cDBZipRegionCity::~cDBZipRegionCity()
{
}

void cDBZipRegionCity::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsZip,
                             const QString &p_qsRegion,
                             const QString &p_qsCity,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsZip         = p_qsZip;
    m_qsRegion      = p_qsRegion;
    m_qsCity        = p_qsCity;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;
}

void cDBZipRegionCity::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "zipRegionCityId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inZipIdx        = p_obRecord.indexOf( "zip" );
    int inRegionIdx     = p_obRecord.indexOf( "region" );
    int inCityIdx       = p_obRecord.indexOf( "city" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inZipIdx ).toString(),
          p_obRecord.value( inRegionIdx ).toString(),
          p_obRecord.value( inCityIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBZipRegionCity::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBZipRegionCity::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM zipRegionCity WHERE zipRegionCityId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "ZipRegionCity id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBZipRegionCity::load( const QString &p_qsZip ) throw( cSevException )
{
    cTracer obTrace( "cDBZipRegionCity::load", QString("zip: \"%1\"").arg(p_qsZip) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM zipRegionCity WHERE zip = \"" + p_qsZip + "\"" );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "ZipRegionCity zip not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBZipRegionCity::loadCity( const QString &p_qsCity ) throw( cSevException )
{
    cTracer obTrace( "cDBZipRegionCity::load", QString("city: \"%1\"").arg(p_qsCity) );

    QString qsCity = p_qsCity;

    qsCity.replace( "*", "\%" );

    QString qsQuery = QString("SELECT * FROM zipRegionCity WHERE city LIKE \"%1\"").arg(qsCity);

    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "ZipRegionCity city not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBZipRegionCity::save() throw( cSevException )
{
    cTracer obTrace( "cDBZipRegionCity::save" );
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
    qsQuery += " zipRegionCity SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "zip = \"%1\", " ).arg( m_qsZip );
    qsQuery += QString( "region = \"%1\", " ).arg( m_qsRegion );
    qsQuery += QString( "city = \"%1\", " ).arg( m_qsCity );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientOriginId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBZipRegionCity::remove() throw( cSevException )
{
    cTracer obTrace( "cDBZipRegionCity::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM zipRegionCity ";
        }
        else
        {
            qsQuery = "UPDATE zipRegionCity SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE zipRegionCityId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBZipRegionCity::createNew() throw()
{
    init();
}

unsigned int cDBZipRegionCity::id() const throw()
{
    return m_uiId;
}

unsigned int cDBZipRegionCity::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBZipRegionCity::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBZipRegionCity::zip() const throw()
{
    return m_qsZip;
}

void cDBZipRegionCity::setZip( const QString &p_qsZip ) throw()
{
    m_qsZip = p_qsZip;
}

QString cDBZipRegionCity::region() const throw()
{
    return m_qsRegion;
}

void cDBZipRegionCity::setRegion( const QString &p_qsRegion ) throw()
{
    m_qsRegion = p_qsRegion;
}

QString cDBZipRegionCity::city() const throw()
{
    return m_qsCity;
}

void cDBZipRegionCity::setCity( const QString &p_qsCity ) throw()
{
    m_qsCity = p_qsCity;
}

bool cDBZipRegionCity::active() const throw()
{
    return m_bActive;
}

void cDBZipRegionCity::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBZipRegionCity::archive() const throw()
{
    return m_qsArchive;
}

void cDBZipRegionCity::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

