//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbguest.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <QMessageBox>
#include "belenus.h"
#include "dbguest.h"

//====================================================================================
cDBGuest::cDBGuest()
//====================================================================================
{
    init();
}
//====================================================================================
cDBGuest::~cDBGuest()
//====================================================================================
{
}
//====================================================================================
void cDBGuest::init( const unsigned int p_uiId,
                     const unsigned int p_uiLicenceId,
                     const unsigned int p_uiCompanyId,
                     const QString &p_qsDateCreated,
                     const QString &p_qsName,
                     const int p_nGender,
                     const int p_nAgeType,
                     const bool p_bIsReturning,
                     const QString &p_qsUniqueId,
                     const QString &p_qsEmail,
                     const bool p_bRegularCustomer,
                     const bool p_bEmployee,
                     const bool p_bService,
                     const bool p_bCompany,
                     const int p_inDiscountType,
                     const QString p_qsMembership,
                     const QString p_qsDateBirth,
                     const QString p_qsAddress,
                     const unsigned int p_uiSkinTypeId,
                     const QString p_qsMobile,
                     const QString &p_qsComment,
                     const int p_inLoyaltyPoints,
                     const QString &p_qsModified,
                     const bool p_bActive,
                     const QString &p_qsArchive ) throw()
//====================================================================================
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiCompanyId           = p_uiCompanyId;
    m_qsDateCreated         = p_qsDateCreated;
    m_qsName                = p_qsName;
    m_nGender               = p_nGender;
    m_nAgeType              = p_nAgeType;
    m_bIsReturning          = p_bIsReturning;
    m_qsUniqueId            = p_qsUniqueId;
    m_qsEmail               = p_qsEmail;
    m_bRegularCustomer      = p_bRegularCustomer;
    m_bEmployee             = p_bEmployee;
    m_bService              = p_bService;
    m_bCompany              = p_bCompany;
    m_inDiscountType        = p_inDiscountType;
    m_qsMembership          = p_qsMembership;
    m_qsDateBirth           = p_qsDateBirth;
    m_qsAddress             = p_qsAddress;
    m_uiSkinTypeId          = p_uiSkinTypeId;
    m_qsMobile              = p_qsMobile;
    m_qsComment             = p_qsComment;
    m_inLoyaltyPoints       = p_inLoyaltyPoints;
    m_qsModified            = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}
//====================================================================================
void cDBGuest::init( const QSqlRecord &p_obRecord ) throw()
//====================================================================================
{
    int inIdIdx                 = p_obRecord.indexOf( "patientId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inCompanyIdIdx          = p_obRecord.indexOf( "companyId" );
    int inDateCreatedIdx        = p_obRecord.indexOf( "created" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inGenderIdx             = p_obRecord.indexOf( "gender" );
    int inAgeTypeIdx            = p_obRecord.indexOf( "ageType" );
    int inIsReturningIdx        = p_obRecord.indexOf( "isReturning" );
    int inUniqueIdIdx           = p_obRecord.indexOf( "uniqueId" );
    int inEmailIdx              = p_obRecord.indexOf( "email" );
    int inRegularCustomerIdx    = p_obRecord.indexOf( "regularCustomer" );
    int inEmployeeIdx           = p_obRecord.indexOf( "employee" );
    int inServiceIdx            = p_obRecord.indexOf( "service" );
    int inCompanyIdx            = p_obRecord.indexOf( "company" );
    int inDiscountTypeIdx       = p_obRecord.indexOf( "discountType" );
    int inMembershipIdx         = p_obRecord.indexOf( "membership" );
    int inDateBirthIdx          = p_obRecord.indexOf( "dateBirth" );
    int inAddressIdx            = p_obRecord.indexOf( "address" );
    int inSkinTypeIdIdx         = p_obRecord.indexOf( "skinTypeId" );
    int inMobileIdx             = p_obRecord.indexOf( "mobile" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inLoyaltyPoints         = p_obRecord.indexOf( "loyaltyPoints" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inCompanyIdIdx ).toInt(),
          p_obRecord.value( inDateCreatedIdx ).toString(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inGenderIdx ).toInt(),
          p_obRecord.value( inAgeTypeIdx ).toInt(),
          p_obRecord.value( inIsReturningIdx ).toBool(),
          p_obRecord.value( inUniqueIdIdx ).toString(),
          p_obRecord.value( inEmailIdx ).toString(),
          p_obRecord.value( inRegularCustomerIdx ).toBool(),
          p_obRecord.value( inEmployeeIdx ).toBool(),
          p_obRecord.value( inServiceIdx ).toBool(),
          p_obRecord.value( inCompanyIdx ).toBool(),
          p_obRecord.value( inDiscountTypeIdx ).toInt(),
          p_obRecord.value( inMembershipIdx ).toString(),
          p_obRecord.value( inDateBirthIdx ).toString(),
          p_obRecord.value( inAddressIdx ).toString(),
          p_obRecord.value( inSkinTypeIdIdx ).toUInt(),
          p_obRecord.value( inMobileIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inLoyaltyPoints ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}
//====================================================================================
void cDBGuest::load( const unsigned int p_uiId ) throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE patientId = %1" ).arg( p_uiId ) );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Guest id not found" );

    poQuery->first();
    init( poQuery->record() );
}
//====================================================================================
void cDBGuest::load( const QString &p_qsName ) throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Guest name not found" );

    poQuery->first();
    init( poQuery->record() );
}
//====================================================================================
void cDBGuest::load( const QString &p_qsName, const QString &p_qsEmail ) throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::load", QString("name: \"%1\" email: \"%2\"").arg(p_qsName).arg(p_qsEmail) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name = \"" + p_qsName + "\" AND email = \"" + p_qsEmail + "\" " );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Guest name/email not found" );
    else if( poQuery->size() > 1 )
        throw cSevException( cSeverity::ERROR, "More than one guest found" );

    poQuery->first();
    init( poQuery->record() );
}
//====================================================================================
unsigned int cDBGuest::getGuestCount( const QString &p_qsName ) throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name LIKE '\%" + p_qsName + "\%'" );

    if( poQuery->size() == 1 )
    {
        poQuery->first();
        init( poQuery->record() );
    }

    return poQuery->size();
}
//====================================================================================
int cDBGuest::getDiscountedPrice( const int p_inPriceTotal ) throw( cSevException )
//====================================================================================
{
    QSqlQuery   *poQuery;
    QString      qsQuery        = "";
    QString      qsCondition    = "";

    if( m_bRegularCustomer )
        qsCondition += QString( "regularCustomer>0 OR " );
    if( m_bEmployee )
        qsCondition += QString( "employee>0 OR " );
    if( m_bService )
        qsCondition += QString( "service>0 OR " );
    if( m_bCompany && m_uiCompanyId )
        qsCondition += QString( "companyId=%1 OR " ).arg(m_uiCompanyId);
    if( m_uiId>0 )
        qsCondition += QString( "patientId=%1" ).arg(m_uiId);
    else
        qsCondition += QString( "patientId>0" );

    qsQuery = QString( "SELECT * FROM discounts WHERE (%1) AND active=1" ).arg(qsCondition);
    poQuery = g_poDB->executeQTQuery( qsQuery );

    int inDiscountedPrice   = p_inPriceTotal;
    int inBestValue         = 0;

    while( poQuery->next() )
    {
        if( poQuery->value( 12 ).toInt() > 0 )
        {
            inBestValue = p_inPriceTotal - poQuery->value( 12 ).toInt()/100;
        }
        else if( poQuery->value( 13 ).toInt() > 0 )
        {
            inBestValue = p_inPriceTotal - ((p_inPriceTotal/100)*poQuery->value( 13 ).toInt());
        }

        if( inBestValue < inDiscountedPrice )
        {
            inDiscountedPrice = inBestValue;
        }
        g_obLogger(cSeverity::DEBUG) << "PriceTotal: ["
                                     << p_inPriceTotal
                                     << "] Discount1 ["
                                     << poQuery->value( 12 ).toInt()/100
                                     << "] Discount2 ["
                                     << poQuery->value( 13 ).toInt()
                                     << "] BestValue ["
                                     << inBestValue
                                     << "] DiscountedPrice ["
                                     << inDiscountedPrice
                                     << "]"
                                     << EOM;
    }
    if( poQuery ) delete poQuery;

    return inDiscountedPrice;
}
//====================================================================================
void cDBGuest::save() throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::save" );
    QString  qsQuery;

    g_obLogger(cSeverity::DEBUG) << "Created " << m_qsDateCreated << EOM;
    if( m_qsDateCreated.compare( "0000-00-00 00:00:00" ) == 0 || m_qsDateCreated.length() < 1 )
    {
        m_qsDateCreated = QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") );
    }

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
    qsQuery += " patients SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "created = \"%1\", " ).arg( m_qsDateCreated );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "gender = \"%1\", " ).arg( m_nGender );
    qsQuery += QString( "ageType = \"%1\", " ).arg( m_nAgeType );
    qsQuery += QString( "isReturning = %1, " ).arg( m_bIsReturning );
    qsQuery += QString( "uniqueId = \"%1\", " ).arg( m_qsUniqueId );
    qsQuery += QString( "email = \"%1\", " ).arg( m_qsEmail );
    qsQuery += QString( "regularCustomer = %1, " ).arg( m_bRegularCustomer );
    qsQuery += QString( "employee = %1, " ).arg( m_bEmployee );
    qsQuery += QString( "service = %1, " ).arg( m_bService );
    qsQuery += QString( "company = %1, " ).arg( m_bCompany );
    qsQuery += QString( "discountType = %1, " ).arg( m_inDiscountType );
    qsQuery += QString( "membership = \"%1\", " ).arg( m_qsMembership );
    qsQuery += QString( "dateBirth = \"%1\", " ).arg( m_qsDateBirth );
    qsQuery += QString( "address = \"%1\", " ).arg( m_qsAddress );
    qsQuery += QString( "skinTypeId = %1, " ).arg( m_uiSkinTypeId );
    qsQuery += QString( "mobile = \"%1\", " ).arg( m_qsMobile );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment.replace( QString("\""), QString("\\\"") ) );
    qsQuery += QString( "loyaltyPoints = %1, " ).arg( m_inLoyaltyPoints );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}
//====================================================================================
void cDBGuest::remove() throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM patients ";
        }
        else
        {
            qsQuery = "UPDATE patients SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBGuest::createNew() throw()
{
    init();
}

unsigned int cDBGuest::id() const throw()
{
    return m_uiId;
}

unsigned int cDBGuest::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBGuest::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBGuest::companyId() const throw()
{
    return m_uiCompanyId;
}

void cDBGuest::setCompanyId( const unsigned int p_nCompanyId ) throw()
{
    m_uiCompanyId = p_nCompanyId;
}

QString cDBGuest::dateCreated() const throw()
{
    return m_qsDateCreated;
}

QString cDBGuest::name() const throw()
{
    return m_qsName;
}

void cDBGuest::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

int cDBGuest::gender() const throw()
{
    return m_nGender;
}

void cDBGuest::setGender( const int p_nGender ) throw()
{
    m_nGender = p_nGender;
}

int cDBGuest::ageType() const throw()
{
    return m_nAgeType;
}

void cDBGuest::setAgeType( const int p_nAgeType ) throw()
{
    m_nAgeType = p_nAgeType;
}

bool cDBGuest::isReturning() const throw()
{
    return m_bIsReturning;
}

void cDBGuest::setIsReturning( const bool p_bIsReturning ) throw()
{
    m_bIsReturning = p_bIsReturning;
}

QString cDBGuest::uniqueId() const throw()
{
    return m_qsUniqueId;
}

void cDBGuest::setUniqueId( const QString &p_qsUniqueId ) throw()
{
    m_qsUniqueId = p_qsUniqueId;
    m_qsUniqueId = m_qsUniqueId.replace( QString("\""), QString("\\\"") );
}

QString cDBGuest::email() const throw()
{
    return m_qsEmail;
}

void cDBGuest::setEmail( const QString &p_qsEmail ) throw()
{
    m_qsEmail = p_qsEmail;
    m_qsEmail = m_qsEmail.replace( QString("\""), QString("\\\"") );
}

bool cDBGuest::regularCustomer() const throw()
{
    return m_bRegularCustomer;
}

void cDBGuest::setRegularCustomer( const bool p_bRegularCustomer ) throw()
{
    m_bRegularCustomer = p_bRegularCustomer;
}

bool cDBGuest::employee() const throw()
{
    return m_bEmployee;
}

void cDBGuest::setEmployee( const bool p_bEmployee ) throw()
{
    m_bEmployee = p_bEmployee;
}

bool cDBGuest::service() const throw()
{
    return m_bService;
}

void cDBGuest::setService( const bool p_bService ) throw()
{
    m_bService = p_bService;
}

bool cDBGuest::company() const throw()
{
    return m_bCompany;
}

void cDBGuest::setCompany( const bool p_bCompany ) throw()
{
    m_bCompany = p_bCompany;
}

int cDBGuest::discountType() const throw()
{
    return m_inDiscountType;
}

void cDBGuest::setDiscountType( const int p_inDiscountType ) throw()
{
    m_inDiscountType = p_inDiscountType;
}

QString cDBGuest::membership() const throw()
{
    return m_qsMembership;
}

void cDBGuest::setMembership( const QString p_qsMembership ) throw()
{
    m_qsMembership = p_qsMembership;
}

QString cDBGuest::dateBirth() const throw()
{
    return m_qsDateBirth;
}

void cDBGuest::setDateBirth( const QString p_qsDateBirth ) throw()
{
    m_qsDateBirth = p_qsDateBirth;
}

QString cDBGuest::address() const throw()
{
    return m_qsAddress;
}

void cDBGuest::setAddress( const QString p_qsAddress ) throw()
{
    m_qsAddress = p_qsAddress;
}

unsigned int cDBGuest::skinTypeId() const throw()
{
    return m_uiSkinTypeId;
}

void cDBGuest::setSkinTypeId( const unsigned int p_nSkinTypeId ) throw()
{
    m_uiSkinTypeId = p_nSkinTypeId;
}

QString cDBGuest::mobile() const throw()
{
    return m_qsMobile;
}

void cDBGuest::setMobile( const QString p_qsMobile ) throw()
{
    m_qsMobile = p_qsMobile;
}

QString cDBGuest::comment() const throw()
{
    return m_qsComment;
}

void cDBGuest::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

int cDBGuest::loyaltyPoints() const throw()
{
    return m_inLoyaltyPoints;
}

void cDBGuest::setLoyaltyPoints( const int p_inLoyaltyPoints ) throw()
{
    m_inLoyaltyPoints = p_inLoyaltyPoints;
}

QString cDBGuest::modified() const throw()
{
    return m_qsModified;
}

bool cDBGuest::active() const throw()
{
    return m_bActive;
}

void cDBGuest::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBGuest::archive() const throw()
{
    return m_qsArchive;
}

void cDBGuest::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

