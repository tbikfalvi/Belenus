//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbwaitlist.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbwaitlist.h"

cDBWaitlist::cDBWaitlist()
{
    init();
}

cDBWaitlist::~cDBWaitlist()
{
}

void cDBWaitlist::init(const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       const unsigned int p_uiPatientCardId,
                       const unsigned int p_uiLedgerId,
                       const unsigned int p_uiPanelTypeId,
                       const unsigned int p_uiPayType,
                       const QString &p_qsBarcode,
                       const QString &p_qsUnitIds,
                       const int p_nLengthCash,
                       const int p_nLengthCard,
                       const unsigned int p_uiUseTime,
                       const unsigned int p_uiUsePrice,
                       const QString &p_qsComment ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPatientCardId   = p_uiPatientCardId;
    m_uiLedgerId        = p_uiLedgerId;
    m_uiPanelTypeId     = p_uiPanelTypeId;
    m_uiPayType         = p_uiPayType;
    m_qsBarcode         = p_qsBarcode;
    m_qsUnitIds         = p_qsUnitIds;
    m_nLengthCash       = p_nLengthCash;
    m_nLengthCard       = p_nLengthCard;
    m_uiUseTime         = p_uiUseTime;
    m_uiUsePrice        = p_uiUsePrice;
    m_qsComment         = p_qsComment;
}

void cDBWaitlist::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "waitlistId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inPatientCardIdIdx  = p_obRecord.indexOf( "patientCardId" );
    int inLedgerIdIdx       = p_obRecord.indexOf( "ledgerId" );
    int inPanelTypeIdIdx    = p_obRecord.indexOf( "panelTypeId" );
    int inPayTypeIdx= p_obRecord.indexOf( "paymentMethodId" );
    int inBarcodeIdx        = p_obRecord.indexOf( "barcode" );
    int inUnitIdsIdx        = p_obRecord.indexOf( "unitIds" );
    int inLengthCashIdx     = p_obRecord.indexOf( "lengthCash" );
    int inLengthCardIdx     = p_obRecord.indexOf( "lengthCard" );
    int inUseTimeIdx        = p_obRecord.indexOf( "useTime" );
    int inUsePriceIdx       = p_obRecord.indexOf( "usePrice" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardIdIdx ).toUInt(),
          p_obRecord.value( inLedgerIdIdx ).toUInt(),
          p_obRecord.value( inPanelTypeIdIdx ).toUInt(),
          p_obRecord.value( inPayTypeIdx ).toUInt(),
          p_obRecord.value( inBarcodeIdx ).toString(),
          p_obRecord.value( inUnitIdsIdx ).toString(),
          p_obRecord.value( inLengthCashIdx ).toInt(),
          p_obRecord.value( inLengthCardIdx ).toInt(),
          p_obRecord.value( inUseTimeIdx ).toUInt(),
          p_obRecord.value( inUsePriceIdx ).toUInt(),
          p_obRecord.value( inCommentIdx ).toString() );
}

void cDBWaitlist::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBWaitlist::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM waitlist WHERE waitlistId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Waitlist id not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBWaitlist::save() throw( cSevException )
{
    cTracer obTrace( "cDBWaitlist::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " waitlist SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "ledgerId = \"%1\", " ).arg( m_uiLedgerId );
    qsQuery += QString( "panelTypeId = \"%1\", " ).arg( m_uiPanelTypeId );
    qsQuery += QString( "payType = \"%1\", " ).arg( m_uiPayType );
    qsQuery += QString( "barcode = \"%1\", " ).arg( m_qsBarcode );
    qsQuery += QString( "unitIds = \"%1\", " ).arg( m_qsUnitIds );
    qsQuery += QString( "lengthCash = \"%1\", " ).arg( m_nLengthCash );
    qsQuery += QString( "lengthCard = \"%1\", " ).arg( m_nLengthCard );
    qsQuery += QString( "useTime = \"%1\", " ).arg( m_uiUseTime );
    qsQuery += QString( "usePrice = \"%1\", " ).arg( m_uiUsePrice );
    qsQuery += QString( "comment = \"%1\" " ).arg( m_qsComment );

    if( m_uiId )
    {
        qsQuery += QString( " WHERE waitlistId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
}

void cDBWaitlist::remove() throw( cSevException )
{
    cTracer obTrace( "cDBWaitlist::remove" );

    if( m_uiId )
    {
        QString     qsQuery;

        qsQuery = "DELETE FROM waitlist ";
        qsQuery += QString( " WHERE waitlistId = \"%1\" " ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBWaitlist::createNew() throw()
{
    init();
}

unsigned int cDBWaitlist::id() const throw()
{
    return m_uiId;
}

unsigned int cDBWaitlist::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBWaitlist::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBWaitlist::PatientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBWaitlist::setPatientCardId( const unsigned int p_uiPatientCardId ) throw()
{
    m_uiPatientCardId = p_uiPatientCardId;
}

unsigned int cDBWaitlist::LedgerId() const throw()
{
    return m_uiLedgerId;
}

void cDBWaitlist::setLedgerId( const unsigned int p_uiLedgerId ) throw()
{
    m_uiLedgerId = p_uiLedgerId;
}

unsigned int cDBWaitlist::PanelTypeId() const throw()
{
    return m_uiPanelTypeId;
}

void cDBWaitlist::setPanelTypeId( const unsigned int p_uiPanelTypeId ) throw()
{
    m_uiPanelTypeId = p_uiPanelTypeId;
}

unsigned int cDBWaitlist::PayType() const throw()
{
    return m_uiPayType;
}

void cDBWaitlist::setPayType( const unsigned int p_uiPayType ) throw()
{
    m_uiPayType = p_uiPayType;
}

QString cDBWaitlist::Barcode() const throw()
{
    return m_qsBarcode;
}

void cDBWaitlist::setBarcode( const QString &p_qsBarcode ) throw()
{
    m_qsBarcode = p_qsBarcode;
}

QString cDBWaitlist::UnitIds() const throw()
{
    return m_qsUnitIds;
}

void cDBWaitlist::setUnitIds( const QString &p_qsUnitIds ) throw()
{
    m_qsUnitIds = p_qsUnitIds;
}

int cDBWaitlist::LengthCash() const throw()
{
    return m_nLengthCash;
}

void cDBWaitlist::setLengthCash( const int p_nLengthCash ) throw()
{
    m_nLengthCash = p_nLengthCash;
}

int cDBWaitlist::LengthCard() const throw()
{
    return m_nLengthCard;
}

void cDBWaitlist::setLengthCard( const int p_nLengthCard ) throw()
{
    m_nLengthCard = p_nLengthCard;
}

unsigned int cDBWaitlist::UseTime() const throw()
{
    return m_uiUseTime;
}

void cDBWaitlist::setUseTime( const unsigned int p_uiUseTime ) throw()
{
    m_uiUseTime = p_uiUseTime;
}

unsigned int cDBWaitlist::UsePrice() const throw()
{
    return m_uiUsePrice;
}

void cDBWaitlist::setUsePrice( const unsigned int p_uiUsePrice ) throw()
{
    m_uiUsePrice = p_uiUsePrice;
}

QString cDBWaitlist::Comment() const throw()
{
    return m_qsComment;
}

void cDBWaitlist::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

