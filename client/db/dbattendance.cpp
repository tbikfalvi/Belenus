//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbattendance.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbattendance.h"

cDBAttendance::cDBAttendance()
{
    init();
}

cDBAttendance::~cDBAttendance()
{
}

void cDBAttendance::init( const unsigned int p_uiId,
                          const unsigned int p_uiLicenceId,
                          const unsigned int p_uiPatientId,
                          const string &p_stDate,
                          const string &p_stLength,
                          const int p_nWeight,
                          const int p_nHeight,
                          const float p_fBloodPressureStart,
                          const float p_fPulseStart,
                          const float p_fBloodPressureStop,
                          const float p_fPulseStop,
                          const string &p_stMedicineCurrent,
                          const string &p_stMedicineAllergy,
                          const string &p_stComment,
                          const string &p_stArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientId           = p_uiPatientId;
    m_stDate                = p_stDate;
    m_stLength              = p_stLength;
    m_nWeight               = p_nWeight;
    m_nHeight               = p_nHeight;
    m_fBloodPressureStart   = p_fBloodPressureStart;
    m_fPulseStart           = p_fPulseStart;
    m_fBloodPressureStop    = p_fBloodPressureStop;
    m_fPulseStop            = p_fPulseStop;
    m_stMedicineCurrent     = p_stMedicineCurrent;
    m_stMedicineAllergy     = p_stMedicineAllergy;
    m_stComment             = p_stComment;
    m_stArchive             = p_stArchive;
}

void cDBAttendance::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "attendanceId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPatientIdx        = p_obRecord.indexOf( "patientId" );
    int inDateIdx           = p_obRecord.indexOf( "date" );
    int inLengthIdx         = p_obRecord.indexOf( "length" );
    int inWeightIdx         = p_obRecord.indexOf( "weight" );
    int inHeightIdx         = p_obRecord.indexOf( "height" );
    int inBPStartIdx        = p_obRecord.indexOf( "bloodPressureStart" );
    int inPulseStartIdx     = p_obRecord.indexOf( "pulseStart" );
    int inBPStopIdx         = p_obRecord.indexOf( "bloodPressureStop" );
    int inPulseStopIdx      = p_obRecord.indexOf( "pulseStop" );
    int inMedCurrentIdx     = p_obRecord.indexOf( "medicineCurrent" );
    int inMedAllergyIdx     = p_obRecord.indexOf( "medicineAllergy" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientIdx ).toInt(),
          p_obRecord.value( inDateIdx ).toString().toStdString(),
          p_obRecord.value( inLengthIdx ).toString().toStdString(),
          p_obRecord.value( inWeightIdx ).toInt(),
          p_obRecord.value( inHeightIdx ).toInt(),
          p_obRecord.value( inBPStartIdx ).toFloat(),
          p_obRecord.value( inPulseStartIdx ).toFloat(),
          p_obRecord.value( inBPStopIdx ).toFloat(),
          p_obRecord.value( inPulseStopIdx ).toFloat(),
          p_obRecord.value( inMedCurrentIdx ).toString().toStdString(),
          p_obRecord.value( inMedAllergyIdx ).toString().toStdString(),
          p_obRecord.value( inCommentIdx ).toString().toStdString(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBAttendance::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBAttendance::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM attendance WHERE attendanceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Attendance id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBAttendance::save() throw( cSevException )
{
    cTracer obTrace( "cDBAttendance::save" );
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
    qsQuery += " attendance SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "date = \"%1\", " ).arg( QString::fromStdString( m_stDate ) );
    qsQuery += QString( "length = \"%1\", " ).arg( QString::fromStdString( m_stLength ) );
    qsQuery += QString( "weight = \"%1\", " ).arg( m_nWeight );
    qsQuery += QString( "height = \"%1\", " ).arg( m_nHeight );
    qsQuery += QString( "bloodPressureStart = \"%1\", " ).arg( m_fBloodPressureStart );
    qsQuery += QString( "pulseStart = \"%1\", " ).arg( m_fPulseStart );
    qsQuery += QString( "bloodPressureStop = \"%1\", " ).arg( m_fBloodPressureStop );
    qsQuery += QString( "pulseStop = \"%1\", " ).arg( m_fPulseStop );
    qsQuery += QString( "medicineCurrent = \"%1\", " ).arg( QString::fromStdString( m_stMedicineCurrent ) );
    qsQuery += QString( "medicineAllergy = \"%1\", " ).arg( QString::fromStdString( m_stMedicineAllergy ) );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE attendanceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBAttendance::remove() throw( cSevException )
{
    cTracer obTrace( "cDBAttendance::save" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM attendance ";
        }
        else
        {
            qsQuery = "UPDATE attendance SET active=0, archive=\"MOD\" "
        }
        qsQuery += QString( " WHERE attendanceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBAttendance::createNew() throw()
{
    init();
}

unsigned int cDBAttendance::id() const throw()
{
    return m_uiId;
}

unsigned int cDBAttendance::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBAttendance::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBAttendance::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBAttendance::setPatientId( const unsigned int p_nPatientId ) throw()
{
    m_uiPatientId = p_nPatientId;
}

string cDBAttendance::date() const throw()
{
    return m_stDate;
}

void cDBAttendance::setDate( const string &p_stDate ) throw()
{
    m_stDate = p_stDate;
}

string cDBAttendance::length() const throw()
{
    return m_stLength;
}

void cDBAttendance::setLength( const string &p_stLength ) throw()
{
    m_stLength = p_stLength;
}

int cDBAttendance::weight() const throw()
{
    return m_nWeight;
}

void cDBAttendance::setWeight( const int p_nWeight ) throw()
{
    m_nWeight = p_nWeight;
}

int cDBAttendance::height() const throw()
{
    return m_nHeight;
}

void cDBAttendance::setHeight( const int p_nHeight ) throw()
{
    m_nHeight = p_nHeight;
}

float cDBAttendance::bloodPressureStart() throw()
{
    return m_fBloodPressureStart;
}

void cDBAttendance::setBloodPressureStart( const float p_fBPStart ) throw()
{
    m_fBloodPressureStart = p_fBPStart;
}

float cDBAttendance::pulseStart() throw()
{
    return m_fPulseStart;
}

void cDBAttendance::setPulseStart( const float p_fPulseStart ) throw()
{
    m_fPulseStart = p_fPulseStart;
}

float cDBAttendance::bloodPressureStop() throw()
{
    return m_fBloodPressureStop;
}

void cDBAttendance::setBloodPressureStop( const float p_fBPStop ) throw()
{
    m_fBloodPressureStop = p_fBPStop;
}

float cDBAttendance::pulseStop() throw()
{
    return m_fPulseStop;
}

void cDBAttendance::setPulseStop( const float p_fPulseStop ) throw()
{
    m_fPulseStop = p_fPulseStop;
}

string cDBAttendance::medicineCurrent() throw()
{
    return m_stMedicineCurrent;
}

void cDBAttendance::setMedicineCurrent( const string &p_stMedicineC ) throw()
{
    m_stMedicineCurrent = p_stMedicineC;
}

string cDBAttendance::medicineAllergy() throw()
{
    return m_stMedicineAllergy;
}

void cDBAttendance::setMedicineAllergy( const string &p_stMedicineA ) throw()
{
    m_stMedicineAllergy = p_stMedicineA;
}

string cDBAttendance::comment() const throw()
{
    return m_stComment;
}

void cDBAttendance::setComment( const string &p_stComment ) throw()
{
    m_stComment = p_stComment;
}

string cDBAttendance::archive() const throw()
{
    return m_stArchive;
}

void cDBAttendance::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

