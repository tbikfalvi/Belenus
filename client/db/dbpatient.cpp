//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
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
                       const string &p_stName,
                       const int p_nGender,
                       const string &p_stDateBirth,
                       const string &p_stUniqueId,
                       const string &p_stCountry,
                       const string &p_stRegion,
                       const string &p_stCity,
                       const string &p_stZip,
                       const string &p_stAddress,
                       const string &p_stEmail,
                       const string &p_stPhone,
                       const string &p_stComment,
                       const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientOriginId = p_uiPatientOriginId;
    m_uiReasonToVisitId = p_uiReasonToVisitId;
    m_stName            = p_stName;
    m_nGender           = p_nGender;
    m_stDateBirth       = p_stDateBirth;
    m_stUniqueId        = p_stUniqueId;
    m_stCountry         = p_stCountry;
    m_stRegion          = p_stRegion;
    m_stCity            = p_stCity;
    m_stZip             = p_stZip;
    m_stAddress         = p_stAddress;
    m_stEmail           = p_stEmail;
    m_stPhone           = p_stPhone;
    m_stComment         = p_stComment;
    m_stArchive         = p_stArchive;
}

void cDBPatient::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientOriginIdIdx    = p_obRecord.indexOf( "patientOriginId" );
    int inReasonToVisitIdIdx    = p_obRecord.indexOf( "reasonToVisitId" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inGenderIdx             = p_obRecord.indexOf( "gender" );
    int inDateBirthIdx          = p_obRecord.indexOf( "dateBirth" );
    int inUniqueIdIdx           = p_obRecord.indexOf( "uniqueId" );
    int inCountryIdx            = p_obRecord.indexOf( "country" );
    int inRegionIdx             = p_obRecord.indexOf( "region" );
    int inCityIdx               = p_obRecord.indexOf( "city" );
    int inZipIdx                = p_obRecord.indexOf( "zip" );
    int inAddressIdx            = p_obRecord.indexOf( "address" );
    int inEmailIdx              = p_obRecord.indexOf( "email" );
    int inPhoneIdx              = p_obRecord.indexOf( "phone" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientOriginIdIdx ).toInt(),
          p_obRecord.value( inReasonToVisitIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inGenderIdx ).toInt(),
          p_obRecord.value( inDateBirthIdx ).toString().toStdString(),
          p_obRecord.value( inUniqueIdIdx ).toString().toStdString(),
          p_obRecord.value( inCountryIdx ).toString().toStdString(),
          p_obRecord.value( inRegionIdx ).toString().toStdString(),
          p_obRecord.value( inCityIdx ).toString().toStdString(),
          p_obRecord.value( inZipIdx ).toString().toStdString(),
          p_obRecord.value( inAddressIdx ).toString().toStdString(),
          p_obRecord.value( inEmailIdx ).toString().toStdString(),
          p_obRecord.value( inPhoneIdx ).toString().toStdString(),
          p_obRecord.value( inCommentIdx ).toString().toStdString(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPatient::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatient::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE patientId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patient id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatient::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatient::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patients WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patient name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatient::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatient::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_stArchive.compare("NEW") != 0 )
        {
            m_stArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_stArchive = "NEW";
    }
    qsQuery += " patients SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientOriginId = \"%1\", " ).arg( m_uiPatientOriginId );
    qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( m_uiReasonToVisitId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "gender = \"%1\", " ).arg( m_nGender );
    qsQuery += QString( "dateBirth = \"%1\", " ).arg( QString::fromStdString( m_stDateBirth ) );
    qsQuery += QString( "uniqueId = \"%1\", " ).arg( QString::fromStdString( m_stUniqueId ) );
    qsQuery += QString( "country = \"%1\", " ).arg( QString::fromStdString( m_stCountry ) );
    qsQuery += QString( "region = \"%1\", " ).arg( QString::fromStdString( m_stRegion ) );
    qsQuery += QString( "city = \"%1\", " ).arg( QString::fromStdString( m_stCity ) );
    qsQuery += QString( "zip = \"%1\", " ).arg( QString::fromStdString( m_stZip ) );
    qsQuery += QString( "address = \"%1\", " ).arg( QString::fromStdString( m_stAddress ) );
    qsQuery += QString( "email = \"%1\", " ).arg( QString::fromStdString( m_stEmail ) );
    qsQuery += QString( "phone = \"%1\", " ).arg( QString::fromStdString( m_stPhone ) );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
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

string cDBPatient::name() const throw()
{
    return m_stName;
}

void cDBPatient::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

int cDBPatient::gender() const throw()
{
    return m_nGender;
}

void cDBPatient::setGender( const int p_nGender ) throw()
{
    m_nGender = p_nGender;
}

string cDBPatient::dateBirth() const throw()
{
    return m_stDateBirth;
}

void cDBPatient::setDateBirth( const string &p_stDateBirth ) throw()
{
    m_stDateBirth = p_stDateBirth;
}

string cDBPatient::uniqueId() const throw()
{
    return m_stUniqueId;
}

void cDBPatient::setUniqueId( const string &p_stUniqueId ) throw()
{
    m_stUniqueId = p_stUniqueId;
}

string cDBPatient::country() const throw()
{
    return m_stCountry;
}

void cDBPatient::setCountry( const string &p_stCountry ) throw()
{
    m_stCountry = p_stCountry;
}

string cDBPatient::region() const throw()
{
    return m_stRegion;
}

void cDBPatient::setRegion( const string &p_stRegion ) throw()
{
    m_stRegion = p_stRegion;
}

string cDBPatient::city() const throw()
{
    return m_stCity;
}

void cDBPatient::setCity( const string &p_stCity ) throw()
{
    m_stCity = p_stCity;
}

string cDBPatient::zip() const throw()
{
    return m_stZip;
}

void cDBPatient::setZip( const string &p_stZip ) throw()
{
    m_stZip = p_stZip;
}

string cDBPatient::address() const throw()
{
    return m_stAddress;
}

void cDBPatient::setAddress( const string &p_stAddress ) throw()
{
    m_stAddress = p_stAddress;
}

string cDBPatient::email() const throw()
{
    return m_stEmail;
}

void cDBPatient::setEmail( const string &p_stEmail ) throw()
{
    m_stEmail = p_stEmail;
}

string cDBPatient::phone() const throw()
{
    return m_stPhone;
}

void cDBPatient::setPhone( const string &p_stPhone ) throw()
{
    m_stPhone = p_stPhone;
}

string cDBPatient::comment() const throw()
{
    return m_stComment;
}

void cDBPatient::setComment( const string &p_stComment ) throw()
{
    m_stComment = p_stComment;
}

string cDBPatient::archive() const throw()
{
    return m_stArchive;
}

void cDBPatient::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

