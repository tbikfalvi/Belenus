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
                               const unsigned int p_uiLedgerId,
                               const unsigned int p_uiPanelId,
                               const int p_nUnitTime,
                               const int p_nUnitPrice,
                               const QString &p_qsValidDateFrom,
                               const QString &p_qsValidDateTo,
                               const QString &p_qsDateTime,
                               const bool p_bActive,
                               const QString &p_qsArchive) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientCardId   = p_uiPatientCardId;
    m_uiLedgerId        = p_uiLedgerId;
    m_uiPanelId         = p_uiPanelId;
    m_nUnitTime         = p_nUnitTime;
    m_nUnitPrice        = p_nUnitPrice;
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
    int inLedgerIdIdx       = p_obRecord.indexOf( "ledgerId" );
    int inPanelIdIdx        = p_obRecord.indexOf( "panelId" );
    int inUnitTimeIdx       = p_obRecord.indexOf( "unitTime" );
    int inUnitPriceIdx      = p_obRecord.indexOf( "unitPrice" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validDateTo" );
    int inDateTimeUsedIdx   = p_obRecord.indexOf( "dateTimeUsed" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientCardIdIdx ).toInt(),
          p_obRecord.value( inLedgerIdIdx ).toInt(),
          p_obRecord.value( inPanelIdIdx ).toInt(),
          p_obRecord.value( inUnitTimeIdx ).toInt(),
          p_obRecord.value( inUnitPriceIdx ).toInt(),
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
        QString qsPCUnit = "";

        qsPCUnit.append( poQuery->value(0).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(2).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(3).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(4).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(5).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(6).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(7).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(8).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(9).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(10).toString() );
        qsPCUnit.append( "\t" );
        qsPCUnit.append( poQuery->value(11).toString() );
        qslRet.append( qsPCUnit );
    }

    if( poQuery ) delete poQuery;

    return qslRet;
}

void cDBPatientcardUnit::replacePatientCard(const unsigned int p_uiId) throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::replacePatientCard", QString( "oldid: %1 newid: %2" ).arg(m_uiPatientCardId).arg( p_uiId ) );

    if( m_uiPatientCardId < 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard id not set" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "UPDATE patientCardUnits SET patientCardId=%1 WHERE patientCardId=%2 AND active=1" ).arg(p_uiId).arg(m_uiPatientCardId) );
    if( poQuery ) delete poQuery;
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
    qsQuery += QString( "ledgerId = \"%1\", " ).arg( m_uiLedgerId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "unitTime = \"%1\", " ).arg( m_nUnitTime );
    qsQuery += QString( "unitPrice = \"%1\", " ).arg( m_nUnitPrice );
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

void cDBPatientcardUnit::removeLedgerUnits(const unsigned int p_uiId) throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::removeLedgerUnits", QString( "id: %1" ).arg( p_uiId ) );

    if( p_uiId > 0 )
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
        qsQuery += QString( " WHERE ledgerId = %1" ).arg( p_uiId );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBPatientcardUnit::deactivateUnits(const unsigned int p_uiId) throw( cSevException )
{
    cTracer obTrace( "cDBPatientcardUnit::deactivateUnits", QString( "id: %1" ).arg( p_uiId ) );

    if( p_uiId > 0 )
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
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( p_uiId );

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
    m_uiPatientCardId = p_nPatientCardId;
}

unsigned int cDBPatientcardUnit::ledgerId() const throw()
{
    return m_uiLedgerId;
}

void cDBPatientcardUnit::setLedgerId( const unsigned int p_nLedgerId ) throw()
{
    m_uiLedgerId = p_nLedgerId;
}

unsigned int cDBPatientcardUnit::panelId() const throw()
{
    return m_uiPanelId;
}

void cDBPatientcardUnit::setPanelId( const unsigned int p_nPanelId ) throw()
{
    m_uiPanelId = p_nPanelId;
}

int cDBPatientcardUnit::unitTime() const throw()
{
    return m_nUnitTime;
}

void cDBPatientcardUnit::setUnitTime(const int p_nUnitTime) throw()
{
    m_nUnitTime = p_nUnitTime;
}

int cDBPatientcardUnit::unitPrice() const throw()
{
    return m_nUnitPrice;
}

void cDBPatientcardUnit::setUnitPrice(const int p_nUnitPrice) throw()
{
    m_nUnitPrice = p_nUnitPrice;
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
