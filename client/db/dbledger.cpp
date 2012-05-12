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
                      const unsigned int p_uiLedgerDeviceId,
                      const unsigned int p_uiPaymentMethodId,
                      const unsigned int p_uiUserId,
                      const unsigned int p_uiProductId,
                      const unsigned int p_uiPatientCardTypeId,
                      const unsigned int p_uiPatientCardId,
                      const unsigned int p_uiPanelId,
                      const QString &p_qsName,
                      const int p_nNetPrice,
                      const int p_inDiscount,
                      const int p_nVatpercent,
                      const int p_nTotalPrice,
                      const QString &p_qsLedgerTime,
                      const QString &p_qsComment,
                      const QString &p_qsModified,
                      const bool p_bActive,
                      const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiLedgerTypeId        = p_uiLedgerTypeId;
    m_uiLedgerDeviceId      = p_uiLedgerDeviceId;
    m_uiPaymentMethod       = p_uiPaymentMethodId;
    m_uiUserId              = p_uiUserId;
    m_uiProductId           = p_uiProductId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_uiPatientCardId       = p_uiPatientCardId;
    m_uiPanelId             = p_uiPanelId;
    m_qsName                = p_qsName;
    m_nNetPrice             = p_nNetPrice;
    m_inDiscount            = p_inDiscount;
    m_nVatpercent           = p_nVatpercent;
    m_nTotalPrice           = p_nTotalPrice;
    m_qsLedgerTime          = p_qsLedgerTime;
    m_qsComment             = p_qsComment;
    m_qsModified            = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBLedger::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "ledgerId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inLedgerTypeIdIdx       = p_obRecord.indexOf( "ledgerTypeId" );
    int inLedgerDeviceIdIdx     = p_obRecord.indexOf( "ledgerDeviceId" );
    int inPaymentMethodIdx      = p_obRecord.indexOf( "paymentMethodId" );
    int inUserIdIdx             = p_obRecord.indexOf( "userId" );
    int inProductIdIdx          = p_obRecord.indexOf( "productId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inPatientCardIdIdx      = p_obRecord.indexOf( "patientCardId" );
    int inPanelIdIdx            = p_obRecord.indexOf( "panelId" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inNetPriceIdx           = p_obRecord.indexOf( "netPrice" );
    int inDiscountIdx           = p_obRecord.indexOf( "discount" );
    int inVatpercentIdx         = p_obRecord.indexOf( "vatpercent" );
    int inTotalPriceIdx         = p_obRecord.indexOf( "totalPrice" );
    int inLedgerTimeIdx         = p_obRecord.indexOf( "ledgerTime" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inLedgerTypeIdIdx ).toUInt(),
          p_obRecord.value( inLedgerDeviceIdIdx ).toUInt(),
          p_obRecord.value( inPaymentMethodIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inNetPriceIdx ).toInt(),
          p_obRecord.value( inDiscountIdx ).toInt(),
          p_obRecord.value( inVatpercentIdx ).toInt(),
          p_obRecord.value( inTotalPriceIdx ).toInt(),
          p_obRecord.value( inLedgerTimeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
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

    m_nTotalPrice = m_nNetPrice + (m_nNetPrice/100)*m_nVatpercent;

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
    qsQuery += QString( "ledgerDeviceId = \"%1\", " ).arg( m_uiLedgerDeviceId );
    qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( m_uiPaymentMethod );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "netPrice = \"%1\", " ).arg( m_nNetPrice );
    qsQuery += QString( "discount = \"%1\", " ).arg( m_inDiscount );
    qsQuery += QString( "vatpercent = \"%1\", " ).arg( m_nVatpercent );
    qsQuery += QString( "totalPrice = \"%1\", " ).arg( m_nTotalPrice );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE ledgerId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_LEDGER );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_LEDGER );
*/
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

void cDBLedger::revoke() throw( cSevException )
{
    m_uiId = 0;
    setComment( QObject::tr("Revoking action: %1").arg(comment()) );
    setNetPrice( netPrice()*(-1) );
    save();
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

unsigned int cDBLedger::ledgerDeviceId() const throw()
{
    return m_uiLedgerDeviceId;
}

void cDBLedger::setLedgerDeviceId( const unsigned int p_uiLedgerDeviceId ) throw()
{
    m_uiLedgerDeviceId = p_uiLedgerDeviceId;
}

unsigned int cDBLedger::paymentMethod() const throw()
{
    return m_uiPaymentMethod;
}

void cDBLedger::setPaymentMethod( const unsigned int p_nPaymentMethod ) throw()
{
    m_uiPaymentMethod = p_nPaymentMethod;
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

unsigned int cDBLedger::patientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBLedger::setPatientCardId( const unsigned int p_nPatientCardId ) throw()
{
    m_uiPatientCardId = p_nPatientCardId;
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
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

int cDBLedger::netPrice() const throw()
{
    return m_nNetPrice;
}

void cDBLedger::setNetPrice( const int p_nNetPrice ) throw()
{
    m_nNetPrice = p_nNetPrice;
}

int cDBLedger::discount() const throw()
{
    return m_inDiscount;
}

void cDBLedger::setDiscount( const int p_inDiscount ) throw()
{
    m_inDiscount = p_inDiscount;
}

int cDBLedger::vatpercent() const throw()
{
    return m_nVatpercent;
}

void cDBLedger::setVatpercent( const int p_nVatpercent ) throw()
{
    m_nVatpercent = p_nVatpercent;
}

int cDBLedger::totalPrice() const throw()
{
    return m_nTotalPrice;
}

void cDBLedger::setTotalPrice( const int p_nTotalPrice ) throw()
{
    m_nTotalPrice = p_nTotalPrice;
    g_obLogger( cSeverity::INFO ) << "m_nTotalPrice: " << m_nTotalPrice << " p_nTotalPrice: " << p_nTotalPrice << EOM;
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
    m_qsComment = m_qsComment.replace( QString("\""), QString("\\\"") );
}

QString cDBLedger::modified() const throw()
{
    return m_qsModified;
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

