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
                       const QString &p_qsComment,
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
    m_qsComment             = p_qsComment;
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
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
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
          p_obRecord.value( inCommentIdx ).toString(),
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

    if( poQuery->size() != 1 )
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

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Guest name not found" );

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
int cDBGuest::getDiscountPrice( const int p_inPriceTotal ) throw( cSevException )
//====================================================================================
{
    if( !m_bRegularCustomer &&
        !m_bEmployee &&
        !m_bService &&
        (!m_bCompany || (m_bCompany && m_uiCompanyId == 0)) )
    {
        return p_inPriceTotal;
    }

    QSqlQuery   *poQuery;
    QString      m_qsQuery = "";
    int          m_inMax = 0;
    int          m_inValueIdx = 0;

    m_qsQuery += QString( "SELECT * FROM discounts WHERE " );

    if( m_inDiscountType == 1 )
    {
        m_qsQuery += QString( "discountValue>0 " );
        m_inValueIdx = 9;
    }
    else if( m_inDiscountType == 2 )
    {
        m_qsQuery += QString( "discountPercent>0 " );
        m_inValueIdx = 10;
    }
    if( m_bRegularCustomer ||
        m_bEmployee ||
        m_bService ||
        (m_bCompany && m_uiCompanyId > 0) )
    {
        m_qsQuery += QString( "AND ( " );
        if( m_bRegularCustomer )
            m_qsQuery += QString( "regularCustomer>0 OR " );
        if( m_bEmployee )
            m_qsQuery += QString( "employee>0 OR " );
        if( m_bService )
            m_qsQuery += QString( "service>0 OR " );
        if( m_bCompany && m_uiCompanyId )
            m_qsQuery += QString( "companyId=%1 OR " ).arg(m_uiCompanyId);
        m_qsQuery.chop(3);
        m_qsQuery += QString( ") " );
    }
    m_qsQuery += QString( "AND active=1" );

    poQuery = g_poDB->executeQTQuery( m_qsQuery );
    while( poQuery->next() )
    {
        if( m_inMax < poQuery->value( m_inValueIdx ).toInt() )
        {
            m_inMax = poQuery->value( m_inValueIdx ).toInt();
        }
    }
    if( poQuery ) delete poQuery;

    int inRet = 0;
    if( m_inDiscountType == 1 )
    {
        inRet = p_inPriceTotal - m_inMax;
    }
    else if( m_inDiscountType == 2 )
    {
        inRet = p_inPriceTotal - ((p_inPriceTotal/100)*m_inMax);
    }
    else
    {
        inRet = p_inPriceTotal;
    }

    return inRet;
}
//====================================================================================
void cDBGuest::save() throw( cSevException )
//====================================================================================
{
    cTracer obTrace( "cDBGuest::save" );
    QString  qsQuery;
    QString qsDateCreated = "";

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive != "NEW" )
        {
            m_qsArchive = "MOD";
        }
        qsDateCreated = m_qsDateCreated;
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
        qsDateCreated = QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") );
    }
    qsQuery += " patients SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "created = \"%1\", " ).arg( qsDateCreated );
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
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
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
    m_bIsReturning = m_bIsReturning;
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

QString cDBGuest::comment() const throw()
{
    return m_qsComment;
}

void cDBGuest::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
    m_qsComment = m_qsComment.replace( QString("\""), QString("\\\"") );
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

