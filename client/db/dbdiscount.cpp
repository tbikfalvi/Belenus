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
                        const unsigned int p_uiGuestId,
                        const unsigned int p_uiCompanyId,
                        const unsigned int p_uiPaymentMethodId,
                        const unsigned int p_uiProductId,
                        const bool p_bRegularCustomer,
                        const bool p_bEmployee,
                        const bool p_bService,
                        const QString &p_qsName,
                        const int p_inDiscountValue,
                        const int p_inDiscountPercent,
                        const QString &p_qsModified,
                        const bool p_bActive,
                        const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiGuestId             = p_uiGuestId;
    m_uiCompanyId           = p_uiCompanyId;
    m_uiPaymentMethodId       = p_uiPaymentMethodId;
    m_uiProductId           = p_uiProductId;
    m_bRegularCustomer      = p_bRegularCustomer;
    m_bEmployee             = p_bEmployee;
    m_bService              = p_bService;
    m_qsName                = p_qsName;
    m_inDiscountValue       = p_inDiscountValue;
    m_inDiscountPercent     = p_inDiscountPercent;
    m_qsModified            = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBDiscount::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "discountId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inGuestIdIdx            = p_obRecord.indexOf( "patientId" );
    int inCompanyIdIdx          = p_obRecord.indexOf( "companyId" );
    int inPaymentMethodIdIdx      = p_obRecord.indexOf( "paymentMethodId" );
    int inProductIdIdx          = p_obRecord.indexOf( "productId" );
    int inRegularCustomerIdx    = p_obRecord.indexOf( "regularCustomer" );
    int inEmpoyeeIdx            = p_obRecord.indexOf( "employee" );
    int inServiceIdx            = p_obRecord.indexOf( "service" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inDiscountValueIdx      = p_obRecord.indexOf( "discountValue" );
    int inDiscountPercentIdx    = p_obRecord.indexOf( "discountPercent" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inGuestIdIdx ).toUInt(),
          p_obRecord.value( inCompanyIdIdx ).toUInt(),
          p_obRecord.value( inPaymentMethodIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inRegularCustomerIdx ).toBool(),
          p_obRecord.value( inEmpoyeeIdx ).toBool(),
          p_obRecord.value( inServiceIdx ).toBool(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inDiscountValueIdx ).toInt(),
          p_obRecord.value( inDiscountPercentIdx ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
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

void cDBDiscount::loadGuest( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::loadGuest", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE patientId = %1" ).arg( p_uiId ) );

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

void cDBDiscount::loadPaymentMethod( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::loadPaymentMethod", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE paymentMethodId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDiscount::loadProduct( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::loadProduct", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE productId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Discount id not found" );

    poQuery->first();
    init( poQuery->record() );
}

bool cDBDiscount::isRegularCustomerExists() throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE regularCustomer > 0 AND discountId<>%1" ).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isEmployeeExists() throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE employee > 0 AND discountId<>%1" ).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isServiceExists() throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE service > 0 AND discountId<>%1" ).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isGuestExists( const unsigned int p_uiId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE patientId=%1 AND discountId<>%2" ).arg(p_uiId).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isCompanyExists( const unsigned int p_uiId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE companyId=%1 AND discountId<>%2" ).arg(p_uiId).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isPaymentMethodExists( const unsigned int p_uiId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE paymentMethodId=%1 AND discountId<>%2" ).arg(p_uiId).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isProductExists( const unsigned int p_uiId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE productId=%1 AND discountId<>%2" ).arg(p_uiId).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBDiscount::isCouponExists(const QString &p_qsName) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM discounts WHERE patientId=0 AND companyId=0 AND paymentMethodId=0 AND productId=0 AND regularCustomer=0 AND employee=0 AND service=0 AND name=\"%1\" AND discountId<>%2" ).arg(p_qsName).arg(m_uiId) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

void cDBDiscount::save() throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::save" );
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
    qsQuery += " discounts SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiGuestId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( m_uiPaymentMethodId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "regularCustomer = \"%1\", " ).arg( m_bRegularCustomer );
    qsQuery += QString( "employee = \"%1\", " ).arg( m_bEmployee );
    qsQuery += QString( "service = \"%1\", " ).arg( m_bService );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "discountValue = \"%1\", " ).arg( m_inDiscountValue );
    qsQuery += QString( "discountPercent = \"%1\", " ).arg( m_inDiscountPercent );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE discountId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_DISCOUNT );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_DISCOUNT );
*/
}

void cDBDiscount::remove() throw( cSevException )
{
    cTracer obTrace( "cDBDiscount::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM discounts ";
        }
        else
        {
            qsQuery = "UPDATE discounts SET active=0, archive=\"MOD\" ";
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

unsigned int cDBDiscount::guestId() const throw()
{
    return m_uiGuestId;
}

void cDBDiscount::setGuestId( const unsigned int p_uiGuestId ) throw()
{
    m_uiGuestId = p_uiGuestId;
}

unsigned int cDBDiscount::companyId() const throw()
{
    return m_uiCompanyId;
}

void cDBDiscount::setCompanyId( const unsigned int p_uiCompanyId ) throw()
{
    m_uiCompanyId = p_uiCompanyId;
}

unsigned int cDBDiscount::paymentMethodId() const throw()
{
    return m_uiPaymentMethodId;
}

void cDBDiscount::setPaymentMethodId( const unsigned int p_uiPaymentMethodId ) throw()
{
    m_uiPaymentMethodId = p_uiPaymentMethodId;
}

unsigned int cDBDiscount::productId() const throw()
{
    return m_uiProductId;
}

void cDBDiscount::setProductId( const unsigned int p_uiProductId ) throw()
{
    m_uiProductId = p_uiProductId;
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

QString cDBDiscount::modified() const throw()
{
    return m_qsModified;
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

int cDBDiscount::discountedValue( int p_inValue )
{
    int nRet = p_inValue;

    if( m_inDiscountValue > 0 )
    {
        nRet -= m_inDiscountValue;
    }
    else
    {
        nRet -= (p_inValue / 100 * m_inDiscountPercent);
    }

    g_obLogger(cSeverity::DEBUG) << "Value [" << p_inValue << "] discount [" << p_inValue-nRet  << "] discounted[" << nRet << "]" << EOM;

    return nRet;
}

int cDBDiscount::discount(int p_inValue)
{
    return p_inValue - discountedValue( p_inValue );
}

