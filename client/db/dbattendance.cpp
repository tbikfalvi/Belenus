//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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
                          const QString &p_qsDate,
                          const int &p_inLength,
                          const float p_fBloodPressureStart,
                          const float p_fPulseStart,
                          const float p_fBloodPressureStop,
                          const float p_fPulseStop,
                          const QString &p_qsComment,
                          const bool p_bActive,
                          const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientId           = p_uiPatientId;
    m_qsDate                = p_qsDate;
    m_inLength              = p_inLength;
    m_fBloodPressureStart   = p_fBloodPressureStart;
    m_fPulseStart           = p_fPulseStart;
    m_fBloodPressureStop    = p_fBloodPressureStop;
    m_fPulseStop            = p_fPulseStop;
    m_qsComment             = p_qsComment;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBAttendance::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "attendanceId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPatientIdx        = p_obRecord.indexOf( "patientId" );
    int inDateIdx           = p_obRecord.indexOf( "date" );
    int inLengthIdx         = p_obRecord.indexOf( "length" );
    int inBPStartIdx        = p_obRecord.indexOf( "bloodPressureStart" );
    int inPulseStartIdx     = p_obRecord.indexOf( "pulseStart" );
    int inBPStopIdx         = p_obRecord.indexOf( "bloodPressureStop" );
    int inPulseStopIdx      = p_obRecord.indexOf( "pulseStop" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientIdx ).toInt(),
          p_obRecord.value( inDateIdx ).toString(),
          p_obRecord.value( inLengthIdx ).toInt(),
          p_obRecord.value( inBPStartIdx ).toFloat(),
          p_obRecord.value( inPulseStartIdx ).toFloat(),
          p_obRecord.value( inBPStopIdx ).toFloat(),
          p_obRecord.value( inPulseStopIdx ).toFloat(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBAttendance::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBAttendance::load", QString( "id: %1" ).arg( p_uiId ) );

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
    qsQuery += " attendance SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "date = \"%1\", " ).arg( m_qsDate );
    qsQuery += QString( "length = \"%1\", " ).arg( m_inLength );
    qsQuery += QString( "bloodPressureStart = \"%1\", " ).arg( m_fBloodPressureStart );
    qsQuery += QString( "pulseStart = \"%1\", " ).arg( m_fPulseStart );
    qsQuery += QString( "bloodPressureStop = \"%1\", " ).arg( m_fBloodPressureStop );
    qsQuery += QString( "pulseStop = \"%1\", " ).arg( m_fPulseStop );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE attendanceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_ATTENDANCE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_ATTENDANCE );
}

void cDBAttendance::remove() throw( cSevException )
{
    cTracer obTrace( "cDBAttendance::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM attendance ";
        }
        else
        {
            qsQuery = "UPDATE attendance SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE attendanceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBAttendance::createNew() throw()
{
    init();
    setDate( QDate::currentDate().toString( "yyyy-MM-dd" ) );
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

QString cDBAttendance::date() const throw()
{
    return m_qsDate;
}

void cDBAttendance::setDate( const QString &p_qsDate ) throw()
{
    m_qsDate = p_qsDate;
}

int cDBAttendance::length() const throw()
{
    return m_inLength;
}

void cDBAttendance::setLength( const int &p_inLength ) throw()
{
    m_inLength = p_inLength;
}

QString cDBAttendance::lengthStr() const throw()
{
    QTime   qtTemp( m_inLength/60, (m_inLength%60), 0, 0 );

    return qtTemp.toString( "hh:mm:ss" );
}

void cDBAttendance::setLengthStr( const QString &p_qsLength ) throw()
{
    QTime   qtTemp = QTime::fromString( p_qsLength, "hh:mm:ss" );

    m_inLength = qtTemp.hour()*60 + qtTemp.minute();
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

QString cDBAttendance::comment() const throw()
{
    return m_qsComment;
}

void cDBAttendance::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
    m_qsComment = m_qsComment.replace( QString("\""), QString("\\\"") );
}

bool cDBAttendance::active() const throw()
{
    return m_bActive;
}

void cDBAttendance::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBAttendance::archive() const throw()
{
    return m_qsArchive;
}

void cDBAttendance::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
