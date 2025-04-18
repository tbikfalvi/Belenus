//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbshoppingcart.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbshoppingcart.h"

cDBShoppingCart::cDBShoppingCart()
{
    init();
}

cDBShoppingCart::~cDBShoppingCart()
{
}

void cDBShoppingCart::init( const unsigned int p_uiId,
                            const unsigned int p_uiLicenceId,
                            const unsigned int p_uiGuestId,
                            const unsigned int p_uiProductId,
                            const unsigned int p_uiPatientCardId,
                            const unsigned int p_uiPatientCardTypeId,
                            const unsigned int p_uiPanelId,
                            const unsigned int p_uiLedgerTypeId,
                            const QString &p_qsItemName,
                            const QString p_qsComment,
                            const int p_nItemCount,
                            const int p_nItemNetPrice,
                            const int p_nCard,
                            const int p_nCash,
                            const int p_nVoucher,
                            const int p_nItemVAT,
                            const int p_nItemDiscount,
                            const int p_nItemSumPrice,
                            const QString &p_qsModified,
                            const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiGuestId             = p_uiGuestId;
    m_uiProductId           = p_uiProductId;
    m_uiPatientCardId       = p_uiPatientCardId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_uiPanelId             = p_uiPanelId;
    m_uiLedgerTypeId        = p_uiLedgerTypeId;
    m_qsItemName            = p_qsItemName;
    m_qsComment             = p_qsComment;
    m_nItemCount            = p_nItemCount;
    m_nItemNetPrice         = p_nItemNetPrice;
    m_nCard                 = p_nCard;
    m_nCash                 = p_nCash;
    m_nVoucher              = p_nVoucher;
    m_nItemVAT              = p_nItemVAT;
    m_nItemDiscount         = p_nItemDiscount;
    m_nItemSumPrice         = p_nItemSumPrice;
    m_qsModified            = p_qsModified;
    m_qsArchive             = p_qsArchive;
}

void cDBShoppingCart::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "shoppingCartItemId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inGuestIdIdx            = p_obRecord.indexOf( "patientId" );
    int inProductIdIdx          = p_obRecord.indexOf( "productId" );
    int inPatientCardIdIdx      = p_obRecord.indexOf( "patientCardId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inPanelIdIdx            = p_obRecord.indexOf( "panelId" );
    int inLedgerTypeIdIdx       = p_obRecord.indexOf( "ledgerTypeId" );
    int inItemNameIdx           = p_obRecord.indexOf( "itemName" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inItemCountIdx          = p_obRecord.indexOf( "itemCount" );
    int inItemNetPriceIdx       = p_obRecord.indexOf( "itemNetPrice" );
    int inCardIdx               = p_obRecord.indexOf( "card" );
    int inCashIdx               = p_obRecord.indexOf( "cash" );
    int inVoucherIdx            = p_obRecord.indexOf( "voucher" );
    int inItemVATIdx            = p_obRecord.indexOf( "itemVAT" );
    int inItemDiscountIdx       = p_obRecord.indexOf( "discountValue" );
    int inItemSumPriceIdx       = p_obRecord.indexOf( "itemSumPrice" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inGuestIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdIdx ).toUInt(),
          p_obRecord.value( inLedgerTypeIdIdx ).toUInt(),
          p_obRecord.value( inItemNameIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inItemCountIdx ).toInt(),
          p_obRecord.value( inItemNetPriceIdx ).toInt(),
          p_obRecord.value( inCardIdx ).toInt(),
          p_obRecord.value( inCashIdx ).toInt(),
          p_obRecord.value( inVoucherIdx ).toInt(),
          p_obRecord.value( inItemVATIdx ).toInt(),
          p_obRecord.value( inItemDiscountIdx ).toInt(),
          p_obRecord.value( inItemSumPriceIdx ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBShoppingCart::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBShoppingCart::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM shoppingcartitems WHERE shoppingCartItemId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "shoppingCartItem id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBShoppingCart::save() throw( cSevException )
{
    cTracer obTrace( "cDBShoppingCart::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " shoppingcartitems SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiGuestId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "ledgerTypeId = \"%1\", " ).arg( m_uiLedgerTypeId );
    qsQuery += QString( "itemName = \"%1\", " ).arg( m_qsItemName );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment.replace( QString("\""), QString("\\\"") ) );
    qsQuery += QString( "itemCount = \"%1\", " ).arg( m_nItemCount );
    qsQuery += QString( "itemNetPrice = \"%1\", " ).arg( m_nItemNetPrice );
    qsQuery += QString( "card = \"%1\", " ).arg( m_nCard );
    qsQuery += QString( "cash = \"%1\", " ).arg( m_nCash );
    qsQuery += QString( "voucher = \"%1\", " ).arg( m_nVoucher );
    qsQuery += QString( "itemVAT = \"%1\", " ).arg( m_nItemVAT );
    qsQuery += QString( "discountValue = \"%1\", " ).arg( m_nItemDiscount );
    qsQuery += QString( "itemSumPrice = \"%1\", " ).arg( m_nItemSumPrice );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE shoppingCartItemId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_CASSA );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_CASSA );
*/
}

void cDBShoppingCart::remove() throw( cSevException )
{
    cTracer obTrace( "cDBShoppingCart::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

/*        if( m_qsArchive == "NEW" )
        {*/
            qsQuery = "DELETE FROM shoppingcartitems ";
/*        }
        else
        {
            qsQuery = "UPDATE shoppingcartitems SET archive=\"MOD\" ";
        }*/
        qsQuery += QString( " WHERE shoppingCartItemId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBShoppingCart::createNew() throw()
{
    init();
}

unsigned int cDBShoppingCart::id() const throw()
{
    return m_uiId;
}

unsigned int cDBShoppingCart::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBShoppingCart::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBShoppingCart::guestId() const throw()
{
    return m_uiGuestId;
}

void cDBShoppingCart::setGuestId( const unsigned int p_uiGuestId ) throw()
{
    m_uiGuestId = p_uiGuestId;
}

unsigned int cDBShoppingCart::productId() const throw()
{
    return m_uiProductId;
}

void cDBShoppingCart::setProductId( const unsigned int p_uiProductId ) throw()
{
    m_uiProductId = p_uiProductId;
}

unsigned int cDBShoppingCart::patientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBShoppingCart::setPatientCardId( const unsigned int p_uiPatientCardId ) throw()
{
    m_uiPatientCardId = p_uiPatientCardId;
}

unsigned int cDBShoppingCart::patientCardTypeId() const throw()
{
    return m_uiPatientCardTypeId;
}

void cDBShoppingCart::setPatientCardTypeId( const unsigned int p_uiPatientCardTypeId ) throw()
{
    m_uiPatientCardTypeId = p_uiPatientCardTypeId;
}

unsigned int cDBShoppingCart::panelId() const throw()
{
    return m_uiPanelId;
}

void cDBShoppingCart::setPanelId( const unsigned int p_uiPanelId ) throw()
{
    m_uiPanelId = p_uiPanelId;
}

unsigned int cDBShoppingCart::ledgerTypeId() const throw()
{
    return m_uiLedgerTypeId;
}

void cDBShoppingCart::setLedgerTypeId( const unsigned int p_uiLedgerTypeId ) throw()
{
    m_uiLedgerTypeId = p_uiLedgerTypeId;
}

QString cDBShoppingCart::itemName() const throw()
{
    return m_qsItemName;
}

void cDBShoppingCart::setItemName(const QString &p_qsItemName) throw()
{
    m_qsItemName = p_qsItemName;
}

QString cDBShoppingCart::comment() const throw()
{
    return m_qsComment;
}

void cDBShoppingCart::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

int cDBShoppingCart::itemCount() const throw()
{
    return m_nItemCount;
}

void cDBShoppingCart::setItemCount( const int p_nItemCount ) throw()
{
    m_nItemCount = p_nItemCount;
}

int cDBShoppingCart::itemNetPrice() const throw()
{
    return m_nItemNetPrice;
}

void cDBShoppingCart::setItemNetPrice( const int p_nItemNetPrice ) throw()
{
    m_nItemNetPrice = p_nItemNetPrice;
}

int cDBShoppingCart::card() const throw()
{
    return m_nCard;
}

void cDBShoppingCart::setCard( const int p_nCard ) throw()
{
    m_nCard = p_nCard;
}

int cDBShoppingCart::cash() const throw()
{
    return m_nCash;
}

void cDBShoppingCart::setCash( const int p_nCash ) throw()
{
    m_nCash = p_nCash;
}

int cDBShoppingCart::voucher() const throw()
{
    return m_nVoucher;
}

void cDBShoppingCart::setVoucher( const int p_nVoucher ) throw()
{
    m_nVoucher = p_nVoucher;
}

int cDBShoppingCart::itemVAT() const throw()
{
    return m_nItemVAT;
}

void cDBShoppingCart::setItemVAT( const int p_nItemVAT ) throw()
{
    m_nItemVAT = p_nItemVAT;
}

int cDBShoppingCart::itemDiscount() const throw()
{
    return m_nItemDiscount;
}

void cDBShoppingCart::setItemDiscount( const int p_nItemDiscount ) throw()
{
    m_nItemDiscount = p_nItemDiscount;
}

int cDBShoppingCart::itemSumPrice() const throw()
{
    return m_nItemSumPrice;
}

void cDBShoppingCart::setItemSumPrice( const int p_nItemSumPrice ) throw()
{
    m_nItemSumPrice = p_nItemSumPrice;
}

QString cDBShoppingCart::modified() const throw()
{
    return m_qsModified;
}

QString cDBShoppingCart::archive() const throw()
{
    return m_qsArchive;
}

void cDBShoppingCart::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

