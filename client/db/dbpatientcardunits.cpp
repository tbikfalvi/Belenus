//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatientcardunits.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpatientcardunits.h"

cDBPatientcardUnit::cDBPatientcardUnit()
{
    init();
}

cDBPatientcardUnit::~cDBPatientcardUnit()
{
}

void cDBPatientcardUnit::init( const unsigned int p_uiId,
                               const unsigned int p_uiLicenceId,
                               const unsigned int p_uiPatientCardId,
                               const int p_nUnitTime,
                               const QString &p_qsValidDateFrom,
                               const QString &p_qsValidDateTo,
                               const QString &p_qsDateTime,
                               const bool p_bActive,
                               const QString &p_qsArchive)
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientCardId   = p_uiPatientCardId;
    m_nUnitTime         = p_nUnitTime;
    m_qsValidDateFrom   = p_qsValidDateFrom;
    m_qsValidDateTo     = p_qsValidDateTo;
    m_qsDateTime        = p_qsDateTime;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPatientcardUnit::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "patientCardUnitId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPatientCardIdIdx  = p_obRecord.indexOf( "patientCardId" );
    int inUnitTimeIdx       = p_obRecord.indexOf( "unitTime" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validDateTo" );
    int inDateTimeUsedIdx   = p_obRecord.indexOf( "dateTimeUsed" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientCardIdIdx ).toInt(),
          p_obRecord.value( inUnitTimeIdx ).toInt(),
          p_obRecord.value( inValidDateFromIdx ).toString(),
          p_obRecord.value( inValidDateToIdx ).toString(),
          p_obRecord.value( inDateTimeUsedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPatientcardUnit::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardUnits WHERE patientCardUnitId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard unit id not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

QStringList cDBPatientcardUnit::loadPCId( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardUnits WHERE patientCardId = %1" ).arg( p_uiId ) );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard unit id not found" );

    QStringList qslRet;

    while( poQuery->next() )
    {
        qslRet.append( poQuery->value(0).toString() );
    }

    if( poQuery ) delete poQuery;

    return qslRet;
}

void cDBPatientcardUnit::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::save" );
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
    qsQuery += " patientCardUnits SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "unitTime = \"%1\", " ).arg( m_nUnitTime );
    qsQuery += QString( "validDateFrom = \"%1\", " ).arg( m_qsValidDateFrom );
    qsQuery += QString( "validDateTo = \"%1\", " ).arg( m_qsValidDateTo );
    qsQuery += QString( "dateTimeUsed = \"%1\", " ).arg( m_qsDateTime );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardUnitId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
}

void cDBPatientcardUnit::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::remove" );

    if( m_uiId )
    {
        QString     qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM patientCardUnits ";
        }
        else
        {
            qsQuery = "UPDATE patientCardUnits SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientCardUnitId = %1" ).arg( m_uiId );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBPatientcardUnit::createNew() throw()
{
    init();
}

unsigned int cDBPatientcardUnit::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatientcardUnit::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatientcardUnit::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPatientcardUnit::patientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBPatientcardUnit::setPatientCardId( const unsigned int p_nPatientCardId ) throw()
{
    m_uiPatientCardId = p_uiPCardTypeId;
}

int cDBPatientcardUnit::unitTime() const throw()
{
    return m_nUnitTime;
}

void cDBPatientcardUnit::setUnitTime(const int p_nUnitTime) const throw()
{
    m_nUnitTime = p_nUnitTime;
}

QString cDBPatientcardUnit::validDateFrom() const throw()
{
    return m_qsValidDateFrom;
}

void cDBPatientcardUnit::setValidDateFrom( const QString &p_qsVDFrom ) throw()
{
    m_qsValidDateFrom = p_qsVDFrom;
}

QString cDBPatientcardUnit::validDateTo() const throw()
{
    return m_qsValidDateTo;
}

void cDBPatientcardUnit::setValidDateTo( const QString &p_qsVDTo ) throw()
{
    m_qsValidDateTo = p_qsVDTo;
}

QString cDBPatientcardUnit::dateTime() const throw()
{
    return m_qsDateTime;
}

void cDBPatientcardUnit::setDateTime(const QString &p_qsDateTime) throw()
{
    m_qsDateTime = p_qsDateTime;
}

bool cDBPatientcardUnit::active() const throw()
{
    return m_bActive;
}

void cDBPatientcardUnit::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPatientcardUnit::archive() const throw()
{
    return m_qsArchive;
}

void cDBPatientcardUnit::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
