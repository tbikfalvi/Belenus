//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbledger.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================
#include <QMessageBox>
#include "belenus.h"
#include "dbledger.h"

cDBLedger::cDBLedger()
{
    init();
}

cDBLedger::~cDBLedger()
{
}

void cDBLedger::init( const unsigned int p_uiId,
                      const unsigned int p_uiLicenceId,
                      const unsigned int p_uiLedgerTypeId,
                      const unsigned int p_uiUserId,
                      const unsigned int p_uiProductId,
                      const unsigned int p_uiPatientCardTypeId,
                      const unsigned int p_uiPanelId,
                      const QString &p_qsName,
                      const int p_nNetPrice,
                      const int p_nVatpercent,
                      const QString &p_qsLedgerTime,
                      const QString &p_qsComment,
                      const bool p_bActive,
                      const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiLedgerTypeId        = p_uiLedgerTypeId;
    m_uiUserId              = p_uiUserId;
    m_uiProductId           = p_uiProductId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_uiPanelId             = p_uiPanelId;
    m_qsName                = p_qsName;
    m_nNetPrice             = p_nNetPrice;
    m_nVatpercent           = p_nVatpercent;
    m_qsLedgerTime          = p_qsLedgerTime;
    m_qsComment             = p_qsComment;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBLedger::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "ledgerId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inLedgerTypeIdIdx       = p_obRecord.indexOf( "ledgerTypeId" );
    int inUserIdIdx             = p_obRecord.indexOf( "userId" );
    int inProductIdIdx          = p_obRecord.indexOf( "productId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inPanelIdIdx            = p_obRecord.indexOf( "panelId" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inNetPriceIdx           = p_obRecord.indexOf( "netPrice" );
    int inVatpercentIdx         = p_obRecord.indexOf( "vatpercent" );
    int inLedgerTimeIdx         = p_obRecord.indexOf( "ledgerTime" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inLedgerTypeIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inNetPriceIdx ).toInt(),
          p_obRecord.value( inVatpercentIdx ).toInt(),
          p_obRecord.value( inLedgerTimeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBLedger::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBLedger::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledger WHERE ledgerId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Ledger id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBLedger::save() throw( cSevException )
{
    cTracer obTrace( "cDBLedger::save" );
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
    qsQuery += " ledger SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "ledgerTypeId = \"%1\", " ).arg( m_uiLedgerTypeId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "netPrice = \"%1\", " ).arg( m_nNetPrice );
    qsQuery += QString( "vatpercent = \"%1\", " ).arg( m_nVatpercent );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE ledgerId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBLedger::remove() throw( cSevException )
{
    cTracer obTrace( "cDBLedger::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM ledger ";
        }
        else
        {
            qsQuery = "UPDATE ledger SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE ledgerId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBLedger::createNew() throw()
{
    init();
}

unsigned int cDBLedger::id() const throw()
{
    return m_uiId;
}

unsigned int cDBLedger::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBLedger::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBLedger::ledgerTypeId() const throw()
{
    return m_uiLedgerTypeId;
}

void cDBLedger::setLedgerTypeId( const unsigned int p_uiLedgerTypeId ) throw()
{
    m_uiLedgerTypeId = p_uiLedgerTypeId;
}

unsigned int cDBLedger::userId() const throw()
{
    return m_uiUserId;
}

void cDBLedger::setUserId( const unsigned int p_nUserId ) throw()
{
    m_uiUserId = p_nUserId;
}

unsigned int cDBLedger::productId() const throw()
{
    return m_uiProductId;
}

void cDBLedger::setProductId( const unsigned int p_nProductId ) throw()
{
    m_uiProductId = p_nProductId;
}

unsigned int cDBLedger::patientCardTypeId() const throw()
{
    return m_uiPatientCardTypeId;
}

void cDBLedger::setPatientCardTypeId( const unsigned int p_nPatientCardTypeId ) throw()
{
    m_uiPatientCardTypeId = p_nPatientCardTypeId;
}

unsigned int cDBLedger::panelId() const throw()
{
    return m_uiPanelId;
}

void cDBLedger::setPanelId( const unsigned int p_nPanelId ) throw()
{
    m_uiPanelId = p_nPanelId;
}

QString cDBLedger::name() const throw()
{
    return m_qsName;
}

void cDBLedger::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
}

int cDBLedger::netPrice() const throw()
{
    return m_nNetPrice;
}

void cDBLedger::setNetPrice( const int p_nNetPrice ) throw()
{
    m_nNetPrice = p_nNetPrice;
}

int cDBLedger::vatpercent() const throw()
{
    return m_nVatpercent;
}

void cDBLedger::setVatpercent( const int p_nVatpercent ) throw()
{
    m_nVatpercent = p_nVatpercent;
}

QString cDBLedger::ledgerTime() const throw()
{
    return m_qsLedgerTime;
}

void cDBLedger::setLedgerTime( const QString &p_qsLedgerTime ) throw()
{
    m_qsLedgerTime = p_qsLedgerTime;
}

QString cDBLedger::comment() const throw()
{
    return m_qsComment;
}

void cDBLedger::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

bool cDBLedger::active() const throw()
{
    return m_bActive;
}

void cDBLedger::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBLedger::archive() const throw()
{
    return m_qsArchive;
}

void cDBLedger::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

