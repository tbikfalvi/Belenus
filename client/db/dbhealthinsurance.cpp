//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbhealthinsurance.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbhealthinsurance.h"

cDBHealthInsurance::cDBHealthInsurance()
{
    init();
}

cDBHealthInsurance::~cDBHealthInsurance()
{
}

void cDBHealthInsurance::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const QString &p_qsCountry,
                             const QString &p_qsRegion,
                             const QString &p_qsCity,
                             const QString &p_qsZip,
                             const QString &p_qsAddress,
                             const QString &p_qsContactPerson,
                             const QString &p_qsContractId,
                             const QString &p_qsValidDateFrom,
                             const QString &p_qsValidDateTo,
                             const int p_nDiscountType,
                             const int p_nDiscount,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_qsName            = p_qsName;
    m_qsCountry         = p_qsCountry;
    m_qsRegion          = p_qsRegion;
    m_qsCity            = p_qsCity;
    m_qsZip             = p_qsZip;
    m_qsAddress         = p_qsAddress;
    m_qsContactPerson   = p_qsContactPerson;
    m_qsContractId      = p_qsContractId;
    m_qsValidDateFrom   = p_qsValidDateFrom;
    m_qsValidDateTo     = p_qsValidDateTo;
    m_nDiscountType     = p_nDiscountType;
    m_nDiscount         = p_nDiscount;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBHealthInsurance::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "healthInsuranceId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inCountryIdx        = p_obRecord.indexOf( "country" );
    int inRegionIdx         = p_obRecord.indexOf( "region" );
    int inCityIdx           = p_obRecord.indexOf( "city" );
    int inZipIdx            = p_obRecord.indexOf( "zip" );
    int inAddressIdx        = p_obRecord.indexOf( "address" );
    int inContactPersonIdx  = p_obRecord.indexOf( "contactPerson" );
    int inContractIdIdx     = p_obRecord.indexOf( "contractId" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validDateTo" );
    int inDiscountTypeIdx   = p_obRecord.indexOf( "discountType" );
    int inDiscountIdx       = p_obRecord.indexOf( "discount" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inCountryIdx ).toString(),
          p_obRecord.value( inRegionIdx ).toString(),
          p_obRecord.value( inCityIdx ).toString(),
          p_obRecord.value( inZipIdx ).toString(),
          p_obRecord.value( inAddressIdx ).toString(),
          p_obRecord.value( inContactPersonIdx ).toString(),
          p_obRecord.value( inContractIdIdx ).toString(),
          p_obRecord.value( inValidDateFromIdx ).toString(),
          p_obRecord.value( inValidDateToIdx ).toString(),
          p_obRecord.value( inDiscountTypeIdx ).toInt(),
          p_obRecord.value( inDiscountIdx ).toInt(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBHealthInsurance::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM healthInsurances WHERE healthInsuranceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "HealthInsurance id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBHealthInsurance::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM healthInsurances WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "HealthInsurance name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBHealthInsurance::save() throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::save" );
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
    qsQuery += " healthInsurances SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "country = \"%1\", " ).arg( m_qsCountry );
    qsQuery += QString( "region = \"%1\", " ).arg( m_qsRegion );
    qsQuery += QString( "city = \"%1\", " ).arg( m_qsCity );
    qsQuery += QString( "zip = \"%1\", " ).arg( m_qsZip );
    qsQuery += QString( "address = \"%1\", " ).arg( m_qsAddress );
    qsQuery += QString( "contactPerson = \"%1\", " ).arg( m_qsContactPerson );
    qsQuery += QString( "contractId = \"%1\", " ).arg( m_qsContractId );
    qsQuery += QString( "validDateFrom = \"%1\", " ).arg( m_qsValidDateFrom );
    qsQuery += QString( "validDateTo = \"%1\", " ).arg( m_qsValidDateTo );
    qsQuery += QString( "discountType = \"%1\", " ).arg( m_nDiscountType );
    qsQuery += QString( "discount = \"%1\", " ).arg( m_nDiscount );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE healthInsuranceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBHealthInsurance::remove() throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM healthInsurances ";
        }
        else
        {
            qsQuery = "UPDATE healthInsurances SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE healthInsuranceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBHealthInsurance::createNew() throw()
{
    init();
}

unsigned int cDBHealthInsurance::id() const throw()
{
    return m_uiId;
}

unsigned int cDBHealthInsurance::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBHealthInsurance::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBHealthInsurance::name() const throw()
{
    return m_qsName;
}

void cDBHealthInsurance::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::country() const throw()
{
    return m_qsCountry;
}

void cDBHealthInsurance::setCountry( const QString &p_qsCountry ) throw()
{
    m_qsCountry = p_qsCountry;
    m_qsCountry = m_qsCountry.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::region() const throw()
{
    return m_qsRegion;
}

void cDBHealthInsurance::setRegion( const QString &p_qsRegion ) throw()
{
    m_qsRegion = p_qsRegion;
    m_qsRegion = m_qsRegion.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::city() const throw()
{
    return m_qsCity;
}

void cDBHealthInsurance::setCity( const QString &p_qsCity ) throw()
{
    m_qsCity = p_qsCity;
    m_qsCity = m_qsCity.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::zip() const throw()
{
    return m_qsZip;
}

void cDBHealthInsurance::setZip( const QString &p_qsZip ) throw()
{
    m_qsZip = p_qsZip;
    m_qsZip = m_qsZip.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::address() const throw()
{
    return m_qsAddress;
}

void cDBHealthInsurance::setAddress( const QString &p_qsAddress ) throw()
{
    m_qsAddress = p_qsAddress;
    m_qsAddress = m_qsAddress.replace( QString("\""), QString("\\\"") );
}

QString cDBHealthInsurance::contactPerson() const throw()
{
    return m_qsContactPerson;
}

void cDBHealthInsurance::setContactPerson( const QString &p_qsContactPerson ) throw()
{
    m_qsContactPerson = p_qsContactPerson;
}

QString cDBHealthInsurance::contractId() const throw()
{
    return m_qsContractId;
}

void cDBHealthInsurance::setContractId( const QString &p_qsContractId ) throw()
{
    m_qsContractId = p_qsContractId;
}

QString cDBHealthInsurance::validDateFrom() const throw()
{
    return m_qsValidDateFrom;
}

void cDBHealthInsurance::setValidDateFrom( const QString &p_qsValidDateFrom ) throw()
{
    m_qsValidDateFrom = p_qsValidDateFrom;
}

QString cDBHealthInsurance::validDateTo() const throw()
{
    return m_qsValidDateTo;
}

void cDBHealthInsurance::setValidDateTo( const QString &p_qsValidDateTo ) throw()
{
    m_qsValidDateTo = p_qsValidDateTo;
}

int cDBHealthInsurance::discountType() const throw()
{
    return m_nDiscountType;
}

void cDBHealthInsurance::setDiscountType( const int p_nDiscountType ) throw()
{
    m_nDiscountType = p_nDiscountType;
}

int cDBHealthInsurance::discount() const throw()
{
    return m_nDiscount;
}

void cDBHealthInsurance::setDiscount( const int p_nDiscount ) throw()
{
    m_nDiscount = p_nDiscount;
}

bool cDBHealthInsurance::active() const throw()
{
    return m_bActive;
}

void cDBHealthInsurance::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBHealthInsurance::archive() const throw()
{
    return m_qsArchive;
}

void cDBHealthInsurance::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

