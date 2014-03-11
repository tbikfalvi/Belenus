//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbcompany.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbcompany.h"
#include "dbdiscount.h"

cDBCompany::cDBCompany()
{
    init();
}

cDBCompany::~cDBCompany()
{
}

void cDBCompany::init( const unsigned int p_uiId,
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
                             const QString &p_qsModified,
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
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBCompany::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "companyId" );
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
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    int p_nDiscountType = 0;
    int p_nDiscount     = 0;

    try
    {
        cDBDiscount obDBDiscount;

        obDBDiscount.loadCompany( p_obRecord.value( inIdIdx ).toInt() );
        if( obDBDiscount.discountValue() > 0 )
        {
            p_nDiscountType = 1;
            p_nDiscount = obDBDiscount.discountValue();
        }
        else if( obDBDiscount.discountPercent() > 0 )
        {
            p_nDiscountType = 2;
            p_nDiscount = obDBDiscount.discountPercent();
        }
    }
    catch( cSevException &e )
    {
    }

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
          p_nDiscountType,
          p_nDiscount,
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBCompany::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCompany::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM companies WHERE companyId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Company id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCompany::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBCompany::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM companies WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Company name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCompany::save() throw( cSevException )
{
    cTracer obTrace( "cDBCompany::save" );
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
    qsQuery += " companies SET ";
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
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE companyId = %1" ).arg( m_uiId );
    }
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery )
    {
        m_uiId = poQuery->lastInsertId().toUInt();
    }
    try
    {
        cDBDiscount obDBDiscount;

        obDBDiscount.loadCompany( m_uiId );
        if( m_nDiscountType == 1 )
        {
            obDBDiscount.setDiscountValue( m_nDiscount );
            obDBDiscount.setDiscountPercent( 0 );
        }
        else if( m_nDiscountType == 2 )
        {
            obDBDiscount.setDiscountValue( 0 );
            obDBDiscount.setDiscountPercent( m_nDiscount );
        }
        obDBDiscount.setName( m_qsName );
        obDBDiscount.save();
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Discount id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            cDBDiscount obDBDiscount;

            obDBDiscount.createNew();
            obDBDiscount.setLicenceId( m_uiLicenceId );
            obDBDiscount.setCompanyId( m_uiId);
            if( m_nDiscountType == 1 )
            {
                obDBDiscount.setDiscountValue( m_nDiscount );
            }
            else if( m_nDiscountType == 2 )
            {
                obDBDiscount.setDiscountPercent( m_nDiscount );
            }
            obDBDiscount.setName( m_qsName );
            obDBDiscount.save();
        }
    }
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_COMPANY );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_COMPANY );
*/
}

void cDBCompany::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCompany::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM companies ";
        }
        else
        {
            qsQuery = "UPDATE companies SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE companyId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBCompany::createNew() throw()
{
    init();
}

unsigned int cDBCompany::id() const throw()
{
    return m_uiId;
}

unsigned int cDBCompany::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBCompany::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBCompany::name() const throw()
{
    return m_qsName;
}

void cDBCompany::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::country() const throw()
{
    return m_qsCountry;
}

void cDBCompany::setCountry( const QString &p_qsCountry ) throw()
{
    m_qsCountry = p_qsCountry;
    m_qsCountry = m_qsCountry.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::region() const throw()
{
    return m_qsRegion;
}

void cDBCompany::setRegion( const QString &p_qsRegion ) throw()
{
    m_qsRegion = p_qsRegion;
    m_qsRegion = m_qsRegion.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::city() const throw()
{
    return m_qsCity;
}

void cDBCompany::setCity( const QString &p_qsCity ) throw()
{
    m_qsCity = p_qsCity;
    m_qsCity = m_qsCity.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::zip() const throw()
{
    return m_qsZip;
}

void cDBCompany::setZip( const QString &p_qsZip ) throw()
{
    m_qsZip = p_qsZip;
    m_qsZip = m_qsZip.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::address() const throw()
{
    return m_qsAddress;
}

void cDBCompany::setAddress( const QString &p_qsAddress ) throw()
{
    m_qsAddress = p_qsAddress;
    m_qsAddress = m_qsAddress.replace( QString("\""), QString("\\\"") );
}

QString cDBCompany::contactPerson() const throw()
{
    return m_qsContactPerson;
}

void cDBCompany::setContactPerson( const QString &p_qsContactPerson ) throw()
{
    m_qsContactPerson = p_qsContactPerson;
}

QString cDBCompany::contractId() const throw()
{
    return m_qsContractId;
}

void cDBCompany::setContractId( const QString &p_qsContractId ) throw()
{
    m_qsContractId = p_qsContractId;
}

QString cDBCompany::validDateFrom() const throw()
{
    return m_qsValidDateFrom;
}

void cDBCompany::setValidDateFrom( const QString &p_qsValidDateFrom ) throw()
{
    m_qsValidDateFrom = p_qsValidDateFrom;
}

QString cDBCompany::validDateTo() const throw()
{
    return m_qsValidDateTo;
}

void cDBCompany::setValidDateTo( const QString &p_qsValidDateTo ) throw()
{
    m_qsValidDateTo = p_qsValidDateTo;
}

int cDBCompany::discountType() const throw()
{
    return m_nDiscountType;
}

void cDBCompany::setDiscountType( const int p_nDiscountType ) throw()
{
    m_nDiscountType = p_nDiscountType;
}

int cDBCompany::discount() const throw()
{
    return m_nDiscount;
}

void cDBCompany::setDiscount( const int p_nDiscount ) throw()
{
    m_nDiscount = p_nDiscount;
}

QString cDBCompany::modified() const throw()
{
    return m_qsModified;
}

bool cDBCompany::active() const throw()
{
    return m_bActive;
}

void cDBCompany::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBCompany::archive() const throw()
{
    return m_qsArchive;
}

void cDBCompany::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

