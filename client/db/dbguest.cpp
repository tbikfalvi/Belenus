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
                       const unsigned int p_uiGuestOriginId,
                       const unsigned int p_uiReasonToVisitId,
                       const unsigned int p_uiIllnessGroupId,
                       const unsigned int p_uiHealthInsuranceId,
                       const unsigned int p_uiCompanyId,
                       const unsigned int p_uiDoctorId,
                       const QString &p_qsDateCreated,
                       const QString &p_qsName,
                       const int p_nGender,
                       const QString &p_qsDateBirth,
                       const int p_nAgeType,
                       const bool p_bIsReturning,
                       const QString &p_qsUniqueId,
                       const QString &p_qsEmail,
                       const QString &p_qsPhone,
                       const int p_nWeight,
                       const int p_nHeight,
                       const QString &p_qsIllnesses,
                       const QString &p_qsSymptoms,
                       const QString &p_qsMedicineCurrent,
                       const QString &p_qsMedicineAllergy,
                       const bool p_bRegularCustomer,
                       const bool p_bEmployee,
                       const bool p_bService,
                       const bool p_bHealthInsurance,
                       const bool p_bCompany,
                       const bool p_bDoctorProposed,
                       const int p_inDiscountType,
                       const QString &p_qsComment,
                       const QString &p_qsModified,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
//====================================================================================
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiGuestOriginId       = p_uiGuestOriginId;
    m_uiReasonToVisitId     = p_uiReasonToVisitId;
    m_uiIllnessGroupId      = p_uiIllnessGroupId;
    m_uiHealthInsuranceId   = p_uiHealthInsuranceId;
    m_uiCompanyId           = p_uiCompanyId;
    m_uiDoctorId            = p_uiDoctorId;
    m_qsDateCreated         = p_qsDateCreated;
    m_qsName                = p_qsName;
    m_nGender               = p_nGender;
    m_qsDateBirth           = p_qsDateBirth;
    m_nAgeType              = p_nAgeType;
    m_bIsReturning          = p_bIsReturning;
    m_qsUniqueId            = p_qsUniqueId;
    m_qsEmail               = p_qsEmail;
    m_qsPhone               = p_qsPhone;
    m_nWeight               = p_nWeight;
    m_nHeight               = p_nHeight;
    m_qsIllnesses           = p_qsIllnesses;
    m_qsSymptoms            = p_qsSymptoms;
    m_qsMedicineCurrent     = p_qsMedicineCurrent;
    m_qsMedicineAllergy     = p_qsMedicineAllergy;
    m_bRegularCustomer      = p_bRegularCustomer;
    m_bEmployee             = p_bEmployee;
    m_bService              = p_bService;
    m_bHealthInsurance      = p_bHealthInsurance;
    m_bCompany              = p_bCompany;
    m_bDoctorProposed       = p_bDoctorProposed;
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
    int inGuestOriginIdIdx      = p_obRecord.indexOf( "patientOriginId" );
    int inReasonToVisitIdIdx    = p_obRecord.indexOf( "reasonToVisitId" );
    int inIllnessGroupIdIdx     = p_obRecord.indexOf( "illnessGroupId" );
    int inHealthInsuranceIdIdx  = p_obRecord.indexOf( "healthInsuranceId" );
    int inCompanyIdIdx          = p_obRecord.indexOf( "companyId" );
    int inDoctorIdIdx           = p_obRecord.indexOf( "doctorId" );
    int inDateCreatedIdx        = p_obRecord.indexOf( "created" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inGenderIdx             = p_obRecord.indexOf( "gender" );
    int inDateBirthIdx          = p_obRecord.indexOf( "dateBirth" );
    int inAgeTypeIdx            = p_obRecord.indexOf( "ageType" );
    int inIsReturningIdx        = p_obRecord.indexOf( "isReturning" );
    int inUniqueIdIdx           = p_obRecord.indexOf( "uniqueId" );
    int inEmailIdx              = p_obRecord.indexOf( "email" );
    int inPhoneIdx              = p_obRecord.indexOf( "phone" );
    int inWeightIdx             = p_obRecord.indexOf( "weight" );
    int inHeightIdx             = p_obRecord.indexOf( "height" );
    int inIllnessesIdx          = p_obRecord.indexOf( "illnesses" );
    int inSymptomsIdx           = p_obRecord.indexOf( "symptoms" );
    int inMedCurrentIdx         = p_obRecord.indexOf( "medicineCurrent" );
    int inMedAllergyIdx         = p_obRecord.indexOf( "medicineAllergy" );
    int inRegularCustomerIdx    = p_obRecord.indexOf( "regularCustomer" );
    int inEmployeeIdx           = p_obRecord.indexOf( "employee" );
    int inServiceIdx            = p_obRecord.indexOf( "service" );
    int inHealthInsuranceIdx    = p_obRecord.indexOf( "healthInsurance" );
    int inCompanyIdx            = p_obRecord.indexOf( "company" );
    int inDoctorProposedIdx     = p_obRecord.indexOf( "doctorProposed" );
    int inDiscountTypeIdx       = p_obRecord.indexOf( "discountType" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inGuestOriginIdIdx ).toInt(),
          p_obRecord.value( inReasonToVisitIdIdx ).toInt(),
          p_obRecord.value( inIllnessGroupIdIdx ).toInt(),
          p_obRecord.value( inHealthInsuranceIdIdx ).toInt(),
          p_obRecord.value( inCompanyIdIdx ).toInt(),
          p_obRecord.value( inDoctorIdIdx ).toInt(),
          p_obRecord.value( inDateCreatedIdx ).toString(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inGenderIdx ).toInt(),
          p_obRecord.value( inDateBirthIdx ).toString(),
          p_obRecord.value( inAgeTypeIdx ).toInt(),
          p_obRecord.value( inIsReturningIdx ).toBool(),
          p_obRecord.value( inUniqueIdIdx ).toString(),
          p_obRecord.value( inEmailIdx ).toString(),
          p_obRecord.value( inPhoneIdx ).toString(),
          p_obRecord.value( inWeightIdx ).toInt(),
          p_obRecord.value( inHeightIdx ).toInt(),
          p_obRecord.value( inIllnessesIdx ).toString(),
          p_obRecord.value( inSymptomsIdx ).toString(),
          p_obRecord.value( inMedCurrentIdx ).toString(),
          p_obRecord.value( inMedAllergyIdx ).toString(),
          p_obRecord.value( inRegularCustomerIdx ).toBool(),
          p_obRecord.value( inEmployeeIdx ).toBool(),
          p_obRecord.value( inServiceIdx ).toBool(),
          p_obRecord.value( inHealthInsuranceIdx ).toBool(),
          p_obRecord.value( inCompanyIdx ).toBool(),
          p_obRecord.value( inDoctorProposedIdx ).toBool(),
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
        (!m_bCompany || (m_bCompany && m_uiCompanyId == 0)) &&
        (!m_bHealthInsurance || (m_bHealthInsurance && m_uiHealthInsuranceId == 0)) &&
        (!m_bDoctorProposed || (m_bDoctorProposed && m_uiDoctorId == 0)) )
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
        (m_bCompany && m_uiCompanyId > 0) ||
        (m_bHealthInsurance && m_uiHealthInsuranceId > 0 ) ||
        (m_bDoctorProposed && m_uiDoctorId > 0 ) )
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
        if( m_bHealthInsurance && m_uiHealthInsuranceId )
            m_qsQuery += QString( "healthInsuranceId=%1 OR " ).arg(m_uiHealthInsuranceId);
        if( m_bDoctorProposed && m_uiDoctorId > 0 )
            m_qsQuery += QString( "doctorId=%1 OR " ).arg(m_uiDoctorId);
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
    qsQuery += QString( "patientOriginId = \"%1\", " ).arg( m_uiGuestOriginId );
    qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( m_uiReasonToVisitId );
    qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( m_uiIllnessGroupId );
    qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( m_uiHealthInsuranceId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "doctorId = \"%1\", " ).arg( m_uiDoctorId );
    qsQuery += QString( "created = \"%1\", " ).arg( qsDateCreated );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "gender = \"%1\", " ).arg( m_nGender );
    qsQuery += QString( "dateBirth = \"%1\", " ).arg( m_qsDateBirth );
    qsQuery += QString( "ageType = \"%1\", " ).arg( m_nAgeType );
    qsQuery += QString( "isReturning = %1, " ).arg( m_bIsReturning );
    qsQuery += QString( "uniqueId = \"%1\", " ).arg( m_qsUniqueId );
    qsQuery += QString( "email = \"%1\", " ).arg( m_qsEmail );
    qsQuery += QString( "phone = \"%1\", " ).arg( m_qsPhone );
    qsQuery += QString( "weight = \"%1\", " ).arg( m_nWeight );
    qsQuery += QString( "height = \"%1\", " ).arg( m_nHeight );
    qsQuery += QString( "illnesses = \"%1\", " ).arg( m_qsIllnesses );
    qsQuery += QString( "symptoms = \"%1\", " ).arg( m_qsSymptoms );
    qsQuery += QString( "medicineCurrent = \"%1\", " ).arg( m_qsMedicineCurrent );
    qsQuery += QString( "medicineAllergy = \"%1\", " ).arg( m_qsMedicineAllergy );
    qsQuery += QString( "regularCustomer = %1, " ).arg( m_bRegularCustomer );
    qsQuery += QString( "employee = %1, " ).arg( m_bEmployee );
    qsQuery += QString( "service = %1, " ).arg( m_bService );
    qsQuery += QString( "healthInsurance = %1, " ).arg( m_bHealthInsurance );
    qsQuery += QString( "company = %1, " ).arg( m_bCompany );
    qsQuery += QString( "doctorProposed = %1, " ).arg( m_bDoctorProposed );
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
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENT );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENT );
*/
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

unsigned int cDBGuest::patientOriginId() const throw()
{
    return m_uiGuestOriginId;
}

void cDBGuest::setGuestOriginId( const unsigned int p_nGuestOriginId ) throw()
{
    m_uiGuestOriginId = p_nGuestOriginId;
}

unsigned int cDBGuest::reasonToVisitId() const throw()
{
    return m_uiReasonToVisitId;
}

void cDBGuest::setReasonToVisitId( const unsigned int p_nReasonToVisitId ) throw()
{
    m_uiReasonToVisitId = p_nReasonToVisitId;
}

unsigned int cDBGuest::illnessGroupId() const throw()
{
    return m_uiIllnessGroupId;
}

void cDBGuest::setIllnessGroupId( const unsigned int p_uiIllnessGroupId ) throw()
{
    m_uiIllnessGroupId = p_uiIllnessGroupId;
}

unsigned int cDBGuest::healthInsuranceId() const throw()
{
    return m_uiHealthInsuranceId;
}

void cDBGuest::setHealthInsuranceId( const unsigned int p_nHealthInsuranceId ) throw()
{
    m_uiHealthInsuranceId = p_nHealthInsuranceId;
}

unsigned int cDBGuest::companyId() const throw()
{
    return m_uiCompanyId;
}

void cDBGuest::setCompanyId( const unsigned int p_nCompanyId ) throw()
{
    m_uiCompanyId = p_nCompanyId;
}

unsigned int cDBGuest::doctorId() const throw()
{
    return m_uiDoctorId;
}

void cDBGuest::setDoctorId( const unsigned int p_nDoctorId ) throw()
{
    m_uiDoctorId = p_nDoctorId;
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

QString cDBGuest::dateBirth() const throw()
{
    return m_qsDateBirth;
}

void cDBGuest::setDateBirth( const QString &p_qsDateBirth ) throw()
{
    m_qsDateBirth = p_qsDateBirth;
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

QString cDBGuest::phone() const throw()
{
    return m_qsPhone;
}

void cDBGuest::setPhone( const QString &p_qsPhone ) throw()
{
    m_qsPhone = p_qsPhone;
    m_qsPhone = m_qsPhone.replace( QString("\""), QString("\\\"") );
}

int cDBGuest::weight() const throw()
{
    return m_nWeight;
}

void cDBGuest::setWeight( const int p_nWeight ) throw()
{
    m_nWeight = p_nWeight;
}

int cDBGuest::height() const throw()
{
    return m_nHeight;
}

void cDBGuest::setHeight( const int p_nHeight ) throw()
{
    m_nHeight = p_nHeight;
}

QString cDBGuest::illnesses() throw()
{
    return m_qsIllnesses;
}

void cDBGuest::setIllnesses( const QString &p_qsIllnesses ) throw()
{
    m_qsIllnesses = p_qsIllnesses;
    m_qsIllnesses = m_qsIllnesses.replace( QString("\""), QString("\\\"") );
}

QString cDBGuest::symptoms() throw()
{
    return m_qsSymptoms;
}

void cDBGuest::setSymptoms( const QString &p_qsSymptoms ) throw()
{
    m_qsSymptoms = p_qsSymptoms;
    m_qsSymptoms = m_qsSymptoms.replace( QString("\""), QString("\\\"") );
}

QString cDBGuest::medicineCurrent() throw()
{
    return m_qsMedicineCurrent;
}

void cDBGuest::setMedicineCurrent( const QString &p_qsMedicineC ) throw()
{
    m_qsMedicineCurrent = p_qsMedicineC;
    m_qsMedicineCurrent = m_qsMedicineCurrent.replace( QString("\""), QString("\\\"") );
}

QString cDBGuest::medicineAllergy() throw()
{
    return m_qsMedicineAllergy;
}

void cDBGuest::setMedicineAllergy( const QString &p_qsMedicineA ) throw()
{
    m_qsMedicineAllergy = p_qsMedicineA;
    m_qsMedicineAllergy = m_qsMedicineAllergy.replace( QString("\""), QString("\\\"") );
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

bool cDBGuest::healthInsurance() const throw()
{
    return m_bHealthInsurance;
}

void cDBGuest::setHealthInsurance( const bool p_bHealthInsurance ) throw()
{
    m_bHealthInsurance = p_bHealthInsurance;
}

bool cDBGuest::company() const throw()
{
    return m_bCompany;
}

void cDBGuest::setCompany( const bool p_bCompany ) throw()
{
    m_bCompany = p_bCompany;
}

bool cDBGuest::doctorProposed() const throw()
{
    return m_bDoctorProposed;
}

void cDBGuest::setDoctorProposed( const bool p_bDoctorProposed ) throw()
{
    m_bDoctorProposed = p_bDoctorProposed;
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

