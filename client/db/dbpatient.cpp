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
                       const QString &p_qsName,
                       const int p_nGender,
                       const QString &p_qsDateBirth,
                       const QString &p_qsUniqueId,
                       const QString &p_qsCountry,
                       const QString &p_qsRegion,
                       const QString &p_qsCity,
                       const QString &p_qsZip,
                       const QString &p_qsAddress,
                       const QString &p_qsEmail,
                       const QString &p_qsPhone,
                       const QString &p_qsComment,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientOriginId = p_uiPatientOriginId;
    m_uiReasonToVisitId = p_uiReasonToVisitId;
    m_qsName            = p_qsName;
    m_nGender           = p_nGender;
    m_qsDateBirth       = p_qsDateBirth;
    m_qsUniqueId        = p_qsUniqueId;
    m_qsCountry         = p_qsCountry;
    m_qsRegion          = p_qsRegion;
    m_qsCity            = p_qsCity;
    m_qsZip             = p_qsZip;
    m_qsAddress         = p_qsAddress;
    m_qsEmail           = p_qsEmail;
    m_qsPhone           = p_qsPhone;
    m_qsComment         = p_qsComment;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
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
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientOriginIdIdx ).toInt(),
          p_obRecord.value( inReasonToVisitIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inGenderIdx ).toInt(),
          p_obRecord.value( inDateBirthIdx ).toString(),
          p_obRecord.value( inUniqueIdIdx ).toString(),
          p_obRecord.value( inCountryIdx ).toString(),
          p_obRecord.value( inRegionIdx ).toString(),
          p_obRecord.value( inCityIdx ).toString(),
          p_obRecord.value( inZipIdx ).toString(),
          p_obRecord.value( inAddressIdx ).toString(),
          p_obRecord.value( inEmailIdx ).toString(),
          p_obRecord.value( inPhoneIdx ).toString(),
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
    qsQuery += " patients SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientOriginId = \"%1\", " ).arg( m_uiPatientOriginId );
    qsQuery += QString( "reasonToVisitId = \"%1\", " ).arg( m_uiReasonToVisitId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "gender = \"%1\", " ).arg( m_nGender );
    qsQuery += QString( "dateBirth = \"%1\", " ).arg( m_qsDateBirth );
    qsQuery += QString( "uniqueId = \"%1\", " ).arg( m_qsUniqueId );
    qsQuery += QString( "country = \"%1\", " ).arg( m_qsCountry );
    qsQuery += QString( "region = \"%1\", " ).arg( m_qsRegion );
    qsQuery += QString( "city = \"%1\", " ).arg( m_qsCity );
    qsQuery += QString( "zip = \"%1\", " ).arg( m_qsZip );
    qsQuery += QString( "address = \"%1\", " ).arg( m_qsAddress );
    qsQuery += QString( "email = \"%1\", " ).arg( m_qsEmail );
    qsQuery += QString( "phone = \"%1\", " ).arg( m_qsPhone );
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

QString cDBPatient::country() const throw()
{
    return m_qsCountry;
}

void cDBPatient::setCountry( const QString &p_qsCountry ) throw()
{
    m_qsCountry = p_qsCountry;
}

QString cDBPatient::region() const throw()
{
    return m_qsRegion;
}

void cDBPatient::setRegion( const QString &p_qsRegion ) throw()
{
    m_qsRegion = p_qsRegion;
}

QString cDBPatient::city() const throw()
{
    return m_qsCity;
}

void cDBPatient::setCity( const QString &p_qsCity ) throw()
{
    m_qsCity = p_qsCity;
}

QString cDBPatient::zip() const throw()
{
    return m_qsZip;
}

void cDBPatient::setZip( const QString &p_qsZip ) throw()
{
    m_qsZip = p_qsZip;
}

QString cDBPatient::address() const throw()
{
    return m_qsAddress;
}

void cDBPatient::setAddress( const QString &p_qsAddress ) throw()
{
    m_qsAddress = p_qsAddress;
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

