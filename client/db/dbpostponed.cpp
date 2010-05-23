//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpostponed.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpostponed.h"

cDBPostponed::cDBPostponed()
{
    init();
}

cDBPostponed::~cDBPostponed()
{
}

void cDBPostponed::init( const unsigned int p_uiId,
                         const unsigned int p_uiPatientId,
                         const unsigned int p_uiAttendanceId ) throw()
{
    m_uiId              = p_uiId;
    m_uiPatientId       = p_uiPatientId;
    m_uiAttendanceId    = p_uiAttendanceId;
}

void cDBPostponed::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "toBeFilledId" );
    int inPatientIdIdx      = p_obRecord.indexOf( "patientId" );
    int inAttendanceIdIdx   = p_obRecord.indexOf( "attendanceId" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inPatientIdIdx ).toInt(),
          p_obRecord.value( inAttendanceIdIdx ).toInt() );
}

void cDBPostponed::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPostponed::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM toBeFilled WHERE toBeFilledId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Postponed record id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPostponed::removePatient( const unsigned int p_uiPatientId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "DELETE FROM toBeFilled WHERE patientId = %1" ).arg( p_uiPatientId ) );

    g_poPrefs->setPostponedPatients( countPostponedPatients() );
    if( poQuery ) delete poQuery;
}

void cDBPostponed::removeAttendance( const unsigned int p_uiAttendanceId ) throw( cSevException )
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "DELETE FROM toBeFilled WHERE attendanceId = %1" ).arg( p_uiAttendanceId ) );

    g_poPrefs->setPostponedAttendances( countPostponedAttendances() );
    if( poQuery ) delete poQuery;
}

void cDBPostponed::save() throw( cSevException )
{
    cTracer obTrace( "cDBPostponed::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " toBeFilled SET ";
    qsQuery += QString( "attendanceId = \"%1\", " ).arg( m_uiAttendanceId );
    qsQuery += QString( "patientId = \"%1\" " ).arg( m_uiPatientId );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE toBeFilledId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    g_poPrefs->setPostponedPatients( countPostponedPatients() );
    g_poPrefs->setPostponedAttendances( countPostponedAttendances() );
}

void cDBPostponed::createNew() throw()
{
    init();
}

unsigned int cDBPostponed::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPostponed::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBPostponed::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

unsigned int cDBPostponed::attendanceId() const throw()
{
    return m_uiAttendanceId;
}

void cDBPostponed::setAttendanceId( const unsigned int p_uiAttendanceId ) throw()
{
    m_uiAttendanceId = p_uiAttendanceId;
}

unsigned int cDBPostponed::countPostponedPatients() const throw()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(patientId) FROM toBeFilled WHERE patientId<>0" ) );

    poQuery->first();

    return poQuery->value( 0 ).toUInt();
}

unsigned int cDBPostponed::countPostponedAttendances() const throw()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(attendanceId) FROM toBeFilled WHERE attendanceId<>0" ) );

    poQuery->first();

    return poQuery->value( 0 ).toUInt();
}
