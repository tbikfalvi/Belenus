//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbaddress.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbaddress.h"

cDBAddress::cDBAddress()
{
    init();
}

cDBAddress::~cDBAddress()
{
}

void cDBAddress::init( const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       const unsigned int p_uiPatientId,
                       const unsigned int p_uiPublicPlaceId,
                       const QString &p_qsName,
                       const QString &p_qsCountry,
                       const QString &p_qsRegion,
                       const QString &p_qsCity,
                       const QString &p_qsZip,
                       const QString &p_qsStreet,
                       const QString &p_qsStreetNumber,
                       const QString &p_qsFloor,
                       const QString &p_qsDoor,
                       const bool p_bPrimaryAddress,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientId       = p_uiPatientId;
    m_uiPublicPlaceId   = p_uiPublicPlaceId;
    m_qsName            = p_qsName;
    m_qsCountry         = p_qsCountry;
    m_qsRegion          = p_qsRegion;
    m_qsCity            = p_qsCity;
    m_qsZip             = p_qsZip;
    m_qsStreet          = p_qsStreet;
    m_qsStreetNumber    = p_qsStreetNumber;
    m_qsFloor           = p_qsFloor;
    m_qsDoor            = p_qsDoor;
    m_bPrimaryAddress   = p_bPrimaryAddress;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBAddress::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "addressId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPatientIdIdx      = p_obRecord.indexOf( "patientId" );
    int inPublicPlaceIdIdx  = p_obRecord.indexOf( "publicPlaceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inCountryIdx        = p_obRecord.indexOf( "country" );
    int inRegionIdx         = p_obRecord.indexOf( "region" );
    int inCityIdx           = p_obRecord.indexOf( "city" );
    int inZipIdx            = p_obRecord.indexOf( "zip" );
    int inStreetIdx         = p_obRecord.indexOf( "street" );
    int inStreetNumberIdx   = p_obRecord.indexOf( "streetNumber" );
    int inFloorIdx          = p_obRecord.indexOf( "floor" );
    int inDoorIdx           = p_obRecord.indexOf( "door" );
    int inPrimaryAddressIdx = p_obRecord.indexOf( "primaryAddress" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientIdIdx ).toInt(),
          p_obRecord.value( inPublicPlaceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inCountryIdx ).toString(),
          p_obRecord.value( inRegionIdx ).toString(),
          p_obRecord.value( inCityIdx ).toString(),
          p_obRecord.value( inZipIdx ).toString(),
          p_obRecord.value( inStreetIdx ).toString(),
          p_obRecord.value( inStreetNumberIdx ).toString(),
          p_obRecord.value( inFloorIdx ).toString(),
          p_obRecord.value( inDoorIdx ).toString(),
          p_obRecord.value( inPrimaryAddressIdx ).toBool(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBAddress::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBAddress::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM address WHERE addressId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Address id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAddress::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBAddress::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM address WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Address name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAddress::loadPatientSelected( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBAddress::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM address WHERE patientId = %1 AND primaryAddress=1" ).arg( p_uiId ) );
    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Primary address not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAddress::save() throw( cSevException )
{
    cTracer obTrace( "cDBAddress::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive != "NEW" )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " address SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "country = \"%1\", " ).arg( m_qsCountry );
    qsQuery += QString( "region = \"%1\", " ).arg( m_qsRegion );
    qsQuery += QString( "city = \"%1\", " ).arg( m_qsCity );
    qsQuery += QString( "zip = \"%1\", " ).arg( m_qsZip );
    qsQuery += QString( "street = \"%1\", " ).arg( m_qsStreet );
    qsQuery += QString( "publicPlaceId = \"%1\", " ).arg( m_uiPublicPlaceId );
    qsQuery += QString( "streetNumber = \"%1\", " ).arg( m_qsStreetNumber );
    qsQuery += QString( "floor = \"%1\", " ).arg( m_qsFloor );
    qsQuery += QString( "door = \"%1\", " ).arg( m_qsDoor );
    qsQuery += QString( "primaryAddress = %1, " ).arg( m_bPrimaryAddress );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE addressId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBAddress::remove( const unsigned int p_uiPatientId ) throw( cSevException )
{
    cTracer obTrace( "cDBAddress::remove" );

    if( m_uiId || p_uiPatientId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM address ";
        }
        else
        {
            qsQuery = "UPDATE address SET active=0, archive=\"MOD\" ";
        }
        if( p_uiPatientId == 0 )
            qsQuery += QString( " WHERE addressId = %1" ).arg( m_uiId );
        else
            qsQuery += QString( " WHERE patientId = %1" ).arg( p_uiPatientId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBAddress::removePatient( const unsigned int p_uiPatientId ) throw( cSevException )
{
    remove( p_uiPatientId );
}

void cDBAddress::createNew() throw()
{
    init();
}

unsigned int cDBAddress::id() const throw()
{
    return m_uiId;
}

unsigned int cDBAddress::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBAddress::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBAddress::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBAddress::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

unsigned int cDBAddress::publicPlaceId() const throw()
{
    return m_uiPublicPlaceId;
}

void cDBAddress::setPublicPlaceId( const unsigned int p_uiPublicPlace ) throw()
{
    m_uiPublicPlaceId = p_uiPublicPlace;
}

QString cDBAddress::name() const throw()
{
    return m_qsName;
}

void cDBAddress::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::country() const throw()
{
    return m_qsCountry;
}

void cDBAddress::setCountry( const QString &p_qsCountry ) throw()
{
    m_qsCountry = p_qsCountry;
    m_qsCountry = m_qsCountry.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::region() const throw()
{
    return m_qsRegion;
}

void cDBAddress::setRegion( const QString &p_qsRegion ) throw()
{
    m_qsRegion = p_qsRegion;
    m_qsRegion = m_qsRegion.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::city() const throw()
{
    return m_qsCity;
}

void cDBAddress::setCity( const QString &p_qsCity ) throw()
{
    m_qsCity = p_qsCity;
    m_qsCity = m_qsCity.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::zip() const throw()
{
    return m_qsZip;
}

void cDBAddress::setZip( const QString &p_qsZip ) throw()
{
    m_qsZip = p_qsZip;
    m_qsZip = m_qsZip.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::street() const throw()
{
    return m_qsStreet;
}

void cDBAddress::setStreet( const QString &p_qsStreet ) throw()
{
    m_qsStreet = p_qsStreet;
    m_qsStreet = m_qsStreet.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::streetNumber() const throw()
{
    return m_qsStreetNumber;
}

void cDBAddress::setStreetNumber( const QString &p_qsStreetNumber ) throw()
{
    m_qsStreetNumber = p_qsStreetNumber;
    m_qsStreetNumber = m_qsStreetNumber.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::floor() const throw()
{
    return m_qsFloor;
}

void cDBAddress::setFloor( const QString &p_qsFloor ) throw()
{
    m_qsFloor = p_qsFloor;
    m_qsFloor = m_qsFloor.replace( QString("\""), QString("\\\"") );
}

QString cDBAddress::door() const throw()
{
    return m_qsDoor;
}

void cDBAddress::setDoor( const QString &p_qsDoor ) throw()
{
    m_qsDoor = p_qsDoor;
    m_qsDoor = m_qsDoor.replace( QString("\""), QString("\\\"") );
}

bool cDBAddress::primaryAddress() const throw()
{
    return m_bPrimaryAddress;
}

void cDBAddress::setPrimaryAddress( const bool p_bPrimaryAddress ) throw()
{
    m_bPrimaryAddress = p_bPrimaryAddress;
}

bool cDBAddress::active() const throw()
{
    return m_bActive;
}

void cDBAddress::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBAddress::archive() const throw()
{
    return m_qsArchive;
}

void cDBAddress::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

