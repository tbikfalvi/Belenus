//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbdiscount.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbdiscount.h"

cDBDiscount::cDBDiscount()
{
    init();
}

cDBDiscount::~cDBDiscount()
{
}

void cDBDiscount::init( const unsigned int p_uiId,
                        const unsigned int p_uiLicenceId,
                        const unsigned int p_uiHealthInsuranceId,
                        const unsigned int p_uiCompanyId,
                        const unsigned int p_uiDoctorId,
                        const bool p_bRegularCustomer,
                        const bool p_bEmployee,
                        const bool p_bService,
                        const QString &p_qsName,
                        const int p_inDiscountValue,
                        const int p_inDiscountPercent,
                        const bool p_bActive,
                        const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiHealthInsuranceId   = p_uiHealthInsuranceId;
    m_uiCompanyId           = p_uiCompanyId;
    m_uiDoctorId            = p_uiDoctorId;
    m_bRegularCustomer      = p_bRegularCustomer;
    m_bEmployee             = p_bEmployee;
    m_bService              = p_bService;
    m_qsName                = p_qsName;
    m_inDiscountValue       = p_inDiscountValue;
    m_inDiscountPercent     = p_inDiscountPercent;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBDiscount::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "discountId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inHealthInsuranceIdIdx  = p_obRecord.indexOf( "healthInsuranceId" );
    int inCompanyIdIdx          = p_obRecord.indexOf( "companyId" );
    int inDoctorIdIdx           = p_obRecord.indexOf( "doctorId" );
    int inRegularCustomerIdx    = p_obRecord.indexOf( "regularCustomer" );
    int inEmpoyeeIdx            = p_obRecord.indexOf( "employee" );
    int inServiceIdx            = p_obRecord.indexOf( "service" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inDiscountValueIdx      = p_obRecord.indexOf( "discountValue" );
    int inDiscountPercentIdx    = p_obRecord.indexOf( "discountPercent" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inHealthInsuranceIdIdx ).toUInt(),
          p_obRecord.value( inCompanyIdIdx ).toUInt(),
          p_obRecord.value( inDoctorIdIdx ).toUInt(),
          p_obRecord.value( inRegularCustomerIdx ).toBool(),
          p_obRecord.value( inEmpoyeeIdx ).toBool(),
          p_obRecord.value( inServiceIdx ).toBool(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inDiscountValueIdx ).toInt(),
          p_obRecord.value( inDiscountPercentIdx ).toInt(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBDiscount::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE discountId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDiscount::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM discount WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDiscount::loadHealthInsurance( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::loadHealthInsurance", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE healthInsuranceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDiscount::loadCompany( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::loadCompany", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE companyId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDiscount::save() throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::save" );
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
    qsQuery += " discounts SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( m_uiHealthInsuranceId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "doctorId = \"%1\", " ).arg( m_uiDoctorId );
    qsQuery += QString( "regularCustomer = \"%1\", " ).arg( m_bRegularCustomer );
    qsQuery += QString( "employee = \"%1\", " ).arg( m_bEmployee );
    qsQuery += QString( "service = \"%1\", " ).arg( m_bService );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "discountValue = \"%1\", " ).arg( m_inDiscountValue );
    qsQuery += QString( "discountPercent = \"%1\", " ).arg( m_inDiscountPercent );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE discountId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBDiscount::remove() throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM discount ";
        }
        else
        {
            qsQuery = "UPDATE discount SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE discountId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBDiscount::createNew() throw()
{
    init();
}

unsigned int cDBDiscount::id() const throw()
{
    return m_uiId;
}

unsigned int cDBDiscount::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBDiscount::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBDiscount::healthInsuranceId() const throw()
{
    return m_uiHealthInsuranceId;
}

void cDBDiscount::setHealthInsuranceId( const unsigned int p_uiHealthInsuranceId ) throw()
{
    m_uiHealthInsuranceId = p_uiHealthInsuranceId;
}

unsigned int cDBDiscount::companyId() const throw()
{
    return m_uiCompanyId;
}

void cDBDiscount::setCompanyId( const unsigned int p_uiCompanyId ) throw()
{
    m_uiCompanyId = p_uiCompanyId;
}

unsigned int cDBDiscount::doctorId() const throw()
{
    return m_uiDoctorId;
}

void cDBDiscount::setDoctorId( const unsigned int p_uiDoctorId ) throw()
{
    m_uiDoctorId = p_uiDoctorId;
}

bool cDBDiscount::regularCustomer() const throw()
{
    return m_bRegularCustomer;
}

void cDBDiscount::setRegularCustomer( const bool p_bRegularCustomer ) throw()
{
    m_bRegularCustomer = p_bRegularCustomer;
}

bool cDBDiscount::employee() const throw()
{
    return m_bEmployee;
}

void cDBDiscount::setEmployee( const bool p_bEmployee ) throw()
{
    m_bEmployee = p_bEmployee;
}

bool cDBDiscount::service() const throw()
{
    return m_bService;
}

void cDBDiscount::setService( const bool p_bService ) throw()
{
    m_bService = p_bService;
}

QString cDBDiscount::name() const throw()
{
    return m_qsName;
}

void cDBDiscount::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

int cDBDiscount::discountValue() const throw()
{
    return m_inDiscountValue;
}

void cDBDiscount::setDiscountValue( const int p_nDiscountValue ) throw()
{
    m_inDiscountValue = p_nDiscountValue;
}

int cDBDiscount::discountPercent() const throw()
{
    return m_inDiscountPercent;
}

void cDBDiscount::setDiscountPercent( const int p_nDiscountPercent ) throw()
{
    m_inDiscountPercent = p_nDiscountPercent;
}

bool cDBDiscount::active() const throw()
{
    return m_bActive;
}

void cDBDiscount::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBDiscount::archive() const throw()
{
    return m_qsArchive;
}

void cDBDiscount::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

