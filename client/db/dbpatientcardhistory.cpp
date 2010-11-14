//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatientcardhistory.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpatientcardhistory.h"

cDBPatientCardHistory::cDBPatientCardHistory()
{
    init();
}

cDBPatientCardHistory::~cDBPatientCardHistory()
{
}

void cDBPatientCardHistory::init( const unsigned int p_uiId,
                                  const unsigned int p_uiLicenceId,
                                  const unsigned int p_uiPatientCardId,
                                  const QString p_qsDateTime,
                                  const int p_nUnits,
                                  const QString p_qsTime,
                                  const QString &p_qsModified,
                                  const bool p_bActive,
                                  const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientCardId       = p_uiPatientCardId;
    m_qsDateTime            = p_qsDateTime;
    m_nUnits                = p_nUnits;
    m_qsTime                = p_qsTime;
    m_qsModified        = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBPatientCardHistory::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientCardHistoryId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientCardIdIdx      = p_obRecord.indexOf( "patientCardId" );
    int inDateTimeIdx           = p_obRecord.indexOf( "dateTimeUsed" );
    int inUnitsIdx              = p_obRecord.indexOf( "units" );
    int inTimeIdx               = p_obRecord.indexOf( "time" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardIdIdx ).toUInt(),
          p_obRecord.value( inDateTimeIdx ).toString(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inTimeIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPatientCardHistory::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardHistory::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardHistories WHERE patientCardHistoryId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "PatientcardHistory id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCardHistory::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardHistory::save" );
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
    qsQuery += " patientCardHistories SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
    qsQuery += QString( "time = \"%1\", " ).arg( m_qsTime );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardHistoryId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENTCARD_HISTORY );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENTCARD_HISTORY );
}

void cDBPatientCardHistory::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardHistory::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM patientCardHistories ";
        }
        else
        {
            qsQuery = "UPDATE patientCardHistories SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientCardHistoryId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPatientCardHistory::createNew() throw()
{
    init();
}

unsigned int cDBPatientCardHistory::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatientCardHistory::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatientCardHistory::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPatientCardHistory::patientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBPatientCardHistory::setPatientCardId( const unsigned int p_uiPCardTypeId ) throw()
{
    m_uiPatientCardId = p_uiPCardTypeId;
}

QString cDBPatientCardHistory::dateTime() const throw()
{
    return m_qsDateTime;
}

void cDBPatientCardHistory::setDateTime( const QString &p_qsDateTime ) throw()
{
    m_qsDateTime = p_qsDateTime;
}

int cDBPatientCardHistory::units() const throw()
{
    return m_nUnits;
}

void cDBPatientCardHistory::setUnits( const int p_nUnits ) throw()
{
    m_nUnits = p_nUnits;
}

QString cDBPatientCardHistory::time() const throw()
{
    return m_qsTime;
}

void cDBPatientCardHistory::setTime( const QString &p_qsTime ) throw()
{
    m_qsTime = p_qsTime;
}

QString cDBPatientCardHistory::modified() const throw()
{
    return m_qsModified;
}

bool cDBPatientCardHistory::active() const throw()
{
    return m_bActive;
}

void cDBPatientCardHistory::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPatientCardHistory::archive() const throw()
{
    return m_qsArchive;
}

void cDBPatientCardHistory::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
