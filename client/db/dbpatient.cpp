//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatient.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <QMessageBox>
#include "belenus.h"
#include "dbpatient.h"

cDBPatient::cDBPatient()
{
    init();
}

cDBPatient::~cDBPatient()
{
}

void cDBPatient::init( const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       const unsigned int p_uiPatientOriginId,
                       const unsigned int p_uiReasonToVisitId,
                       const unsigned int p_uiIllnessGroupId,
                       const unsigned int p_uiHealthInsuranceId,
                       const unsigned int p_uiCompanyId,
                       const unsigned int p_uiDoctorId,
                       const QString &p_qsDateCreated,
                       const QString &p_qsName,
                       const int p_nGender,
                       const QString &p_qsDateBirth,
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
                       const QString &p_qsComment,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientOriginId     = p_uiPatientOriginId;
    m_uiReasonToVisitId     = p_uiReasonToVisitId;
    m_uiIllnessGroupId      = p_uiIllnessGroupId;
    m_uiHealthInsuranceId   = p_uiHealthInsuranceId;
    m_uiCompanyId           = p_uiCompanyId;
    m_uiDoctorId            = p_uiDoctorId;
    m_qsDateCreated         = p_qsDateCreated;
    m_qsName                = p_qsName;
    m_nGender               = p_nGender;
    m_qsDateBirth           = p_qsDateBirth;
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
    m_qsComment             = p_qsComment;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBPatient::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientOriginIdIdx    = p_obRecord.indexOf( "patientOriginId" );
    int inReasonToVisitIdIdx    = p_obRecord.indexOf( "reasonToVisitId" );
    int inIllnessGroupIdIdx     = p_obRecord.indexOf( "illnessGroupId" );
    int inHealthInsuranceIdIdx  = p_obRecord.indexOf( "healthInsuranceId" );
    int inCompanyIdIdx          = p_obRecord.indexOf( "companyId" );
    int inDoctorIdIdx           = p_obRecord.indexOf( "doctorId" );
    int inDateCreatedIdx        = p_obRecord.indexOf( "created" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inGenderIdx             = p_obRecord.indexOf( "gender" );
    int inDateBirthIdx          = p_obRecord.indexOf( "dateBirth" );
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
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientOriginIdIdx ).toInt(),
          p_obRecord.value( inReasonToVisitIdIdx ).toInt(),
          p_obRecord.value( inIllnessGroupIdIdx ).toInt(),
          p_obRecord.value( inHealthInsuranceIdIdx ).toInt(),
          p_obRecord.value( inCompanyIdIdx ).toInt(),
          p_obRecord.value( inDoctorIdIdx ).toInt(),
          p_obRecord.value( inDateCreatedIdx ).toString(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inGenderIdx ).toInt(),
          p_obRecord.value( inDateBirthIdx ).toString(),
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
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPatient::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatient::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE patientId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patient id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatient::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatient::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patient name not found" );

    poQuery->first();
    init( poQuery->record() );
}

unsigned int cDBPatient::getPatientCount( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatient::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name LIKE '\%" + p_qsName + "\%'" );

    if( poQuery->size() == 1 )
    {
        poQuery->first();
        init( poQuery->record() );
    }

    return poQuery->size();
}

void cDBPatient::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatient::save" );
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
    qsQuery += QString( "patientOriginId = \"%1\", " ).arg( m_uiPatientOriginId );
    qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( m_uiReasonToVisitId );
    qsQuery += QString( "illnessGroupId = \"%1\", " ).arg( m_uiIllnessGroupId );
    qsQuery += QString( "healthInsuranceId = \"%1\", " ).arg( m_uiHealthInsuranceId );
    qsQuery += QString( "companyId = \"%1\", " ).arg( m_uiCompanyId );
    qsQuery += QString( "doctorId = \"%1\", " ).arg( m_uiDoctorId );
    qsQuery += QString( "created = \"%1\", " ).arg( qsDateCreated );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "gender = \"%1\", " ).arg( m_nGender );
    qsQuery += QString( "dateBirth = \"%1\", " ).arg( m_qsDateBirth );
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
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
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

void cDBPatient::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatient::remove" );

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

void cDBPatient::createNew() throw()
{
    init();
}

unsigned int cDBPatient::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatient::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatient::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPatient::patientOriginId() const throw()
{
    return m_uiPatientOriginId;
}

void cDBPatient::setPatientOriginId( const unsigned int p_nPatientOriginId ) throw()
{
    m_uiPatientOriginId = p_nPatientOriginId;
}

unsigned int cDBPatient::reasonToVisitId() const throw()
{
    return m_uiReasonToVisitId;
}

void cDBPatient::setReasonToVisitId( const unsigned int p_nReasonToVisitId ) throw()
{
    m_uiReasonToVisitId = p_nReasonToVisitId;
}

unsigned int cDBPatient::illnessGroupId() const throw()
{
    return m_uiIllnessGroupId;
}

void cDBPatient::setIllnessGroupId( const unsigned int p_uiIllnessGroupId ) throw()
{
    m_uiIllnessGroupId = p_uiIllnessGroupId;
}

unsigned int cDBPatient::healthInsuranceId() const throw()
{
    return m_uiHealthInsuranceId;
}

void cDBPatient::setHealthInsuranceId( const unsigned int p_nHealthInsuranceId ) throw()
{
    m_uiHealthInsuranceId = p_nHealthInsuranceId;
}

unsigned int cDBPatient::companyId() const throw()
{
    return m_uiCompanyId;
}

void cDBPatient::setCompanyId( const unsigned int p_nCompanyId ) throw()
{
    m_uiCompanyId = p_nCompanyId;
}

unsigned int cDBPatient::doctorId() const throw()
{
    return m_uiDoctorId;
}

void cDBPatient::setDoctorId( const unsigned int p_nDoctorId ) throw()
{
    m_uiDoctorId = p_nDoctorId;
}

QString cDBPatient::dateCreated() const throw()
{
    return m_qsDateCreated;
}

QString cDBPatient::name() const throw()
{
    return m_qsName;
}

void cDBPatient::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
}

int cDBPatient::gender() const throw()
{
    return m_nGender;
}

void cDBPatient::setGender( const int p_nGender ) throw()
{
    m_nGender = p_nGender;
}

QString cDBPatient::dateBirth() const throw()
{
    return m_qsDateBirth;
}

void cDBPatient::setDateBirth( const QString &p_qsDateBirth ) throw()
{
    m_qsDateBirth = p_qsDateBirth;
}

QString cDBPatient::uniqueId() const throw()
{
    return m_qsUniqueId;
}

void cDBPatient::setUniqueId( const QString &p_qsUniqueId ) throw()
{
    m_qsUniqueId = p_qsUniqueId;
}

QString cDBPatient::email() const throw()
{
    return m_qsEmail;
}

void cDBPatient::setEmail( const QString &p_qsEmail ) throw()
{
    m_qsEmail = p_qsEmail;
}

QString cDBPatient::phone() const throw()
{
    return m_qsPhone;
}

void cDBPatient::setPhone( const QString &p_qsPhone ) throw()
{
    m_qsPhone = p_qsPhone;
}

int cDBPatient::weight() const throw()
{
    return m_nWeight;
}

void cDBPatient::setWeight( const int p_nWeight ) throw()
{
    m_nWeight = p_nWeight;
}

int cDBPatient::height() const throw()
{
    return m_nHeight;
}

void cDBPatient::setHeight( const int p_nHeight ) throw()
{
    m_nHeight = p_nHeight;
}

QString cDBPatient::illnesses() throw()
{
    return m_qsIllnesses;
}

void cDBPatient::setIllnesses( const QString &p_qsIllnesses ) throw()
{
    m_qsIllnesses = p_qsIllnesses;
}

QString cDBPatient::symptoms() throw()
{
    return m_qsSymptoms;
}

void cDBPatient::setSymptoms( const QString &p_qsSymptoms ) throw()
{
    m_qsSymptoms = p_qsSymptoms;
}

QString cDBPatient::medicineCurrent() throw()
{
    return m_qsMedicineCurrent;
}

void cDBPatient::setMedicineCurrent( const QString &p_qsMedicineC ) throw()
{
    m_qsMedicineCurrent = p_qsMedicineC;
}

QString cDBPatient::medicineAllergy() throw()
{
    return m_qsMedicineAllergy;
}

void cDBPatient::setMedicineAllergy( const QString &p_qsMedicineA ) throw()
{
    m_qsMedicineAllergy = p_qsMedicineA;
}

bool cDBPatient::regularCustomer() const throw()
{
    return m_bRegularCustomer;
}

void cDBPatient::setRegularCustomer( const bool p_bRegularCustomer ) throw()
{
    m_bRegularCustomer = p_bRegularCustomer;
}

bool cDBPatient::employee() const throw()
{
    return m_bEmployee;
}

void cDBPatient::setEmployee( const bool p_bEmployee ) throw()
{
    m_bEmployee = p_bEmployee;
}

bool cDBPatient::service() const throw()
{
    return m_bService;
}

void cDBPatient::setService( const bool p_bService ) throw()
{
    m_bService = p_bService;
}

bool cDBPatient::healthInsurance() const throw()
{
    return m_bHealthInsurance;
}

void cDBPatient::setHealthInsurance( const bool p_bHealthInsurance ) throw()
{
    m_bHealthInsurance = p_bHealthInsurance;
}

bool cDBPatient::company() const throw()
{
    return m_bCompany;
}

void cDBPatient::setCompany( const bool p_bCompany ) throw()
{
    m_bCompany = p_bCompany;
}

bool cDBPatient::doctorProposed() const throw()
{
    return m_bDoctorProposed;
}

void cDBPatient::setDoctorProposed( const bool p_bDoctorProposed ) throw()
{
    m_bDoctorProposed = p_bDoctorProposed;
}

QString cDBPatient::comment() const throw()
{
    return m_qsComment;
}

void cDBPatient::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

bool cDBPatient::active() const throw()
{
    return m_bActive;
}

void cDBPatient::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPatient::archive() const throw()
{
    return m_qsArchive;
}

void cDBPatient::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

