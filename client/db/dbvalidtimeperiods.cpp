//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbvalidtimeperiods.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbvalidtimeperiods.h"

cDBValidTimePeriod::cDBValidTimePeriod()
{
    init();
}

cDBValidTimePeriod::~cDBValidTimePeriod()
{
}

void cDBValidTimePeriod::init( const unsigned int p_uiId,
                               const unsigned int p_uiLicenceId,
                               const unsigned int p_uiPatientCardTypeId,
                               const int p_nValidWeekDays,
                               const QString &p_qsStartTime,
                               const QString &p_qsStopTime,
                               const QString &p_qsModified,
                               const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_nValidWeekDays        = p_nValidWeekDays;
    m_qsStartTime           = p_qsStartTime;
    m_qsStopTime            = p_qsStopTime;
    m_qsModified            = p_qsModified;
    m_qsArchive             = p_qsArchive;
}

void cDBValidTimePeriod::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientCardTypeEnabledId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inValidWeekDaysIdx      = p_obRecord.indexOf( "validWeekDays" );
    int inStartTimeIdx          = p_obRecord.indexOf( "start" );
    int inStopTimeIdx           = p_obRecord.indexOf( "stop" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inValidWeekDaysIdx ).toInt(),
          p_obRecord.value( inStartTimeIdx ).toString(),
          p_obRecord.value( inStopTimeIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBValidTimePeriod::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBValidTimePeriod::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypeEnabled WHERE patientCardTypeEnabledId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Timeperiod id not found" );

    poQuery->first();
    init( poQuery->record() );
}

QStringList cDBValidTimePeriod::loadPeriods( const unsigned int p_uiId ) const throw( cSevException )
{
    cTracer obTrace( "cDBValidTimePeriod::loadPeriods", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypeEnabled WHERE patientCardTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Timeperiods not found" );

    QStringList qslPeriods;

    while( poQuery->next() )
    {
        qslPeriods << QString( "%1 => %2" ).arg(poQuery->value(3).toString().left(5)).arg(poQuery->value(4).toString().left(5));
    }

    return qslPeriods;
}

void cDBValidTimePeriod::save() throw( cSevException )
{
    cTracer obTrace( "cDBValidTimePeriod::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive.compare("NEW") != 0 )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " patientCardTypeEnabled SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "validWeekDays = \"%1\", " ).arg( m_nValidWeekDays );
    qsQuery += QString( "start = \"%1\", " ).arg( m_qsStartTime );
    qsQuery += QString( "stop = \"%1\", " ).arg( m_qsStopTime );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardTypeEnabledId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

//    if( m_uiId > 0 && m_uiLicenceId != 1 )
//        g_obDBMirror.updateSynchronizationLevel( DB_VALIDTIMEPERIOD );
//    if( m_uiId > 0 && m_uiLicenceId == 0 )
//        g_obDBMirror.updateGlobalSyncLevel( DB_VALIDTIMEPERIOD );
}

void cDBValidTimePeriod::saveList(const QStringList &m_qslPeriods) throw( cSevException )
{
    if( m_qslPeriods.count() == 0 )
        return;

    removePatienCardTypes( m_uiPatientCardTypeId );

    for( int i=0; i<m_qslPeriods.count(); i++ )
    {
        m_uiId          = 0;
        m_qsStartTime   = m_qslPeriods.at(i).left(5);
        m_qsStopTime    = m_qslPeriods.at(i).right(5);

        save();
    }
}

void cDBValidTimePeriod::remove() throw( cSevException )
{
    cTracer obTrace( "cDBValidTimePeriod::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        qsQuery = "DELETE FROM patientCardTypeEnabled ";
        qsQuery += QString( " WHERE patientCardTypeEnabledId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBValidTimePeriod::removePatienCardTypes( const unsigned int p_uiId ) throw( cSevException )
{
    QString  qsQuery;

    qsQuery = "DELETE FROM patientCardTypeEnabled ";
    qsQuery += QString( " WHERE patientCardTypeId = %1" ).arg( p_uiId );

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( poQuery ) delete poQuery;
}

void cDBValidTimePeriod::createNew() throw()
{
    init();
}

unsigned int cDBValidTimePeriod::id() const throw()
{
    return m_uiId;
}

unsigned int cDBValidTimePeriod::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBValidTimePeriod::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBValidTimePeriod::patientCardTypeId() const throw()
{
    return m_uiPatientCardTypeId;
}

void cDBValidTimePeriod::setPatientCardTypeId( const unsigned int p_uiPCardTypeId ) throw()
{
    m_uiPatientCardTypeId = p_uiPCardTypeId;
}

int cDBValidTimePeriod::validWeekDays() const throw()
{
    return m_nValidWeekDays;
}

void cDBValidTimePeriod::setValidWeekDays( const int p_nValidWeekDays ) throw()
{
    m_nValidWeekDays = p_nValidWeekDays;
}

QString cDBValidTimePeriod::startTime() const throw()
{
    return m_qsStartTime;
}

void cDBValidTimePeriod::setStartTime( const QString &p_qsStartTime ) throw()
{
    m_qsStartTime = p_qsStartTime;
}

QString cDBValidTimePeriod::stopTime() const throw()
{
    return m_qsStopTime;
}

void cDBValidTimePeriod::setStopTime( const QString &p_qsStopTime ) throw()
{
    m_qsStopTime = p_qsStopTime;
}

QString cDBValidTimePeriod::modified() const throw()
{
    return m_qsModified;
}

QString cDBValidTimePeriod::archive() const throw()
{
    return m_qsArchive;
}

void cDBValidTimePeriod::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

