//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : cassa.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Penztart es az azzal kapcsolodo reszeket kezelo osztaly.
//====================================================================================

#include <QMessageBox>
#include <QSqlQuery>
#include <QString>

#include "cassa.h"
#include "belenus.h"
#include "db/dbcassahistory.h"
#include "db/dbledger.h"
#include "db/dbproduct.h"

//====================================================================================
cCassa::cCassa()
//====================================================================================
{
    m_pCassa                = new cDBCassa();
    m_pDenomination         = new cDBDenomination();
    m_pCassaDenomination    = new cDBCassaDenomination();

    m_bCassaEnabled         = false;
}
//====================================================================================
cCassa::~cCassa()
//====================================================================================
{
    if( m_pCassa ) delete m_pCassa;
    if( m_pDenomination ) delete m_pDenomination;
    if( m_pCassaDenomination ) delete m_pCassaDenomination;
}
//====================================================================================
bool cCassa::loadOpenCassa( unsigned int p_uiUserId )
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE userId=%1 AND licenceId=%2 AND stopDateTime=\"0000-00-00 00:00:00\" ORDER BY cassaId DESC " ).arg(p_uiUserId).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
bool cCassa::loadOpenCassa()
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE licenceId=%1 AND stopDateTime=\"0000-00-00 00:00:00\" ORDER BY cassaId DESC " ).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
bool cCassa::loadLatestCassa( unsigned int p_uiUserId )
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE userId=%1 AND licenceId=%2 ORDER BY cassaId DESC " ).arg(p_uiUserId).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
bool cCassa::loadLatestCassaWithCash()
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE currentBalance>0 AND licenceId=%1 ORDER BY cassaId DESC " ).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
void cCassa::createNew( unsigned int p_uiUserId, int p_inBalance )
//====================================================================================
{
    QSqlQuery *poQuery = NULL;

    try
    {
        m_pCassa->createNew();
        m_pCassa->setUserId( p_uiUserId );
        m_pCassa->setLicenceId( g_poPrefs->getLicenceId() );
        m_pCassa->setCurrentBalance( p_inBalance );
        m_pCassa->setStartDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
        m_pCassa->setActive( true );
        m_pCassa->save();

        poQuery = g_poDB->executeQTQuery( QString( "SELECT denominationId FROM denominations WHERE active=1" ) );
        while( poQuery->next() )
        {
            m_pCassaDenomination->createNew();
            m_pCassaDenomination->setDenominationId( poQuery->value( 0 ).toUInt() );
            m_pCassaDenomination->setCassaId( m_pCassa->id() );
            m_pCassaDenomination->setLicenceId( g_poPrefs->getLicenceId() );
            m_pCassaDenomination->save();
        }

        cDBCassaHistory obDBCassaHistory;

        obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
        obDBCassaHistory.setCassaId( m_pCassa->id() );
        obDBCassaHistory.setUserId( p_uiUserId );
        obDBCassaHistory.setPatientId( 0 );
        obDBCassaHistory.setActionValue( 0 );
        obDBCassaHistory.setActionCard( 0 );
        obDBCassaHistory.setActionCash( 0 );
        obDBCassaHistory.setActionBalance( p_inBalance );
        obDBCassaHistory.setComment( QObject::tr("Open new cassa record.") );
        obDBCassaHistory.setActive( true );
        obDBCassaHistory.save();

        setEnabled();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    if( poQuery ) delete poQuery;
}
//====================================================================================
void cCassa::cassaContinue()
//====================================================================================
{
    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( m_pCassa->userId() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Continue cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    setEnabled();
}
//====================================================================================
void cCassa::cassaContinue( unsigned int p_uiUserId )
//====================================================================================
{
/*    int inCurrentBalance = m_pCassa->currentBalance();

    m_pCassa->setStopDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    m_pCassa->save();
*/
    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( p_uiUserId );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Continue cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

//    createNew( p_uiUserId, inCurrentBalance );
}
//====================================================================================
void cCassa::cassaReOpen()
//====================================================================================
{
    m_pCassa->setStopDateTime( "" );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( m_pCassa->userId() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Reopen cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    setEnabled();
}
//====================================================================================
void cCassa::cassaReOpen( unsigned int p_uiCassaId )
//====================================================================================
{
    m_pCassa->load( p_uiCassaId );
    cassaReOpen();
}
//====================================================================================
void cCassa::cassaClose()
//====================================================================================
{
    m_pCassa->setStopDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( m_pCassa->userId() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Close cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    m_pCassa->createNew();
    m_bCassaEnabled = false;
}
//====================================================================================
unsigned int cCassa::cassaProcessPatientCardSell( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, bool p_bNewCard, int p_inPayType )
//====================================================================================
{
    QString     qsComment = p_qsComment;

    if( qsComment.length() > 0 && p_obDBShoppingCart.comment().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( p_obDBShoppingCart.comment() );

    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    if( p_bNewCard )
        obDBLedger.setLedgerTypeId( p_obDBShoppingCart.ledgerTypeId() );
    else
        obDBLedger.setLedgerTypeId( cDBLedger::LT_PC_REFILL );
    obDBLedger.setLedgerDeviceId( 0 );
    obDBLedger.setPaymentMethod( p_inPayType );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( 0 );
    obDBLedger.setPatientCardTypeId( p_obDBShoppingCart.patientCardTypeId() );
    obDBLedger.setPatientCardId( p_DBPatientCard.id() );
    obDBLedger.setPanelId( 0 );
    obDBLedger.setName( p_DBPatientCard.barcode() );
    obDBLedger.setNetPrice( p_obDBShoppingCart.itemNetPrice() );
    obDBLedger.setCard( p_obDBShoppingCart.card() );
    obDBLedger.setCash( p_obDBShoppingCart.cash() );
    obDBLedger.setVoucher( p_obDBShoppingCart.voucher() );
    obDBLedger.setDiscount( p_obDBShoppingCart.itemDiscount() );
    obDBLedger.setVatpercent( p_obDBShoppingCart.itemVAT() );
    obDBLedger.setTotalPrice( p_obDBShoppingCart.itemSumPrice() );
    obDBLedger.setComment( qsComment );
    obDBLedger.setActive( true );
    obDBLedger.save();

    unsigned int uiPatientId = p_DBPatientCard.getCardOwner();

    if( uiPatientId > 0 )
    {
        if( p_bNewCard )
            g_obGen.saveGuestActivity( uiPatientId, PATIENTHISTORY_PURCHASEDCARD, 0,  0, p_DBPatientCard.id() );
        else
            g_obGen.saveGuestActivity( uiPatientId, PATIENTHISTORY_REFILLEDCARD, 0,  0, p_DBPatientCard.id() );
    }

//    if( (p_obDBShoppingCart.cash()+p_obDBShoppingCart.voucher()) > 0 )
//    {
        cassaAddMoneyAction( p_obDBShoppingCart.cash()+p_obDBShoppingCart.voucher(), p_obDBShoppingCart.card(), obDBLedger.id(), qsComment );
//    }
/*    if( p_obDBShoppingCart.card() > 0 )
    {
        cassaAddGlobalMoneyAction( p_obDBShoppingCart.card(), obDBLedger.id(), p_qsComment );
    }*/

    return obDBLedger.id();
}
//====================================================================================
unsigned int cCassa::cassaProcessPatientCardRefill( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType )
//====================================================================================
{
    return cassaProcessPatientCardSell( p_DBPatientCard, p_obDBShoppingCart, p_qsComment, false, p_inPayType );
}
//====================================================================================
unsigned int cCassa::cassaProcessPatientCardUnitChange(  const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, bool p_bIncrease )
//====================================================================================
{
    QString     qsComment = p_qsComment;

    if( qsComment.length() > 0 && p_obDBShoppingCart.comment().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( p_obDBShoppingCart.comment() );

    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    if( p_bIncrease )
        obDBLedger.setLedgerTypeId( cDBLedger::LT_PC_UNIT_INCREASE );
    else
        obDBLedger.setLedgerTypeId( cDBLedger::LT_PC_UNIT_DECREASE );
    obDBLedger.setLedgerDeviceId( 0 );
    obDBLedger.setPaymentMethod( 0 );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( 0 );
    obDBLedger.setPatientCardTypeId( p_obDBShoppingCart.patientCardTypeId() );
    obDBLedger.setPatientCardId( p_DBPatientCard.id() );
    obDBLedger.setPanelId( 0 );
    obDBLedger.setName( p_obDBShoppingCart.itemName() );
    obDBLedger.setItemCount( p_obDBShoppingCart.itemCount() );
    obDBLedger.setNetPrice( 0 );
    obDBLedger.setCard( 0 );
    obDBLedger.setCash( 0 );
    obDBLedger.setVoucher( 0 );
    obDBLedger.setDiscount( 0 );
    obDBLedger.setVatpercent( 0 );
    obDBLedger.setTotalPrice( 0 );
    obDBLedger.setComment( qsComment );
    obDBLedger.setActive( true );
    obDBLedger.save();

    return obDBLedger.id();
}

//====================================================================================
unsigned int cCassa::cassaProcessProductStorageChange( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, bool p_bGlobalCassa )
//====================================================================================
{
    QString     qsComment = p_qsComment;

    if( qsComment.length() > 0 && p_obDBShoppingCart.comment().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( p_obDBShoppingCart.comment() );

    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( cDBLedger::LT_PROD_STORAGE_CHANGE );
    obDBLedger.setPaymentMethod( 1 );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( p_obDBShoppingCart.productId() );
    obDBLedger.setName( p_obDBShoppingCart.itemName() );
    obDBLedger.setItemCount( p_obDBShoppingCart.itemCount() );
    obDBLedger.setNetPrice( p_obDBShoppingCart.itemNetPrice() );
    obDBLedger.setCard( p_obDBShoppingCart.card() );
    obDBLedger.setCash( p_obDBShoppingCart.cash() );
    obDBLedger.setVoucher( p_obDBShoppingCart.voucher() );
    obDBLedger.setVatpercent( p_obDBShoppingCart.itemVAT() );
    obDBLedger.setTotalPrice( p_obDBShoppingCart.itemSumPrice() );
    obDBLedger.setComment( qsComment );
    obDBLedger.save();

    if( !p_bGlobalCassa )
    {
        cassaAddMoneyAction( p_obDBShoppingCart.itemSumPrice()-p_obDBShoppingCart.itemDiscount(), 0, obDBLedger.id(), qsComment );
    }
    else
    {
        cassaAddMoneyAction( p_obDBShoppingCart.itemSumPrice()-p_obDBShoppingCart.itemDiscount(), 0, obDBLedger.id(), qsComment, 0, true );
//        cassaAddGlobalMoneyAction( p_obDBShoppingCart.itemSumPrice()-p_obDBShoppingCart.itemDiscount(), obDBLedger.id(), p_qsComment );
    }

    return obDBLedger.id();
}
//====================================================================================
unsigned int cCassa::cassaProcessDeviceUse( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType, QString p_qsPanelTitle )
//====================================================================================
{
    QString     qsComment = p_qsComment;

    if( qsComment.length() > 0 && p_obDBShoppingCart.comment().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( p_obDBShoppingCart.comment() );

    cDBLedger   obDBLedger;

//    g_obLogger(cSeverity::DEBUG) << "DISCOUNT: itemNetPrice ["
//                                 << p_obDBShoppingCart.itemNetPrice()
//                                 << "]"
//                                 << " itemDiscount ["
//                                 << p_obDBShoppingCart.itemDiscount()
//                                 << "]"
//                                 << " itemSumPrice ["
//                                 << p_obDBShoppingCart.itemSumPrice()
//                                 << "]"
//                                 << EOM;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( cDBLedger::LT_DEVICE_USAGE );
    obDBLedger.setPaymentMethod( p_inPayType );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setPanelId( p_obDBShoppingCart.panelId() );
    obDBLedger.setName( p_qsPanelTitle );
    obDBLedger.setNetPrice( p_obDBShoppingCart.itemNetPrice() );
    obDBLedger.setCard( p_obDBShoppingCart.card() );
    obDBLedger.setCash( p_obDBShoppingCart.cash() );
    obDBLedger.setVoucher( p_obDBShoppingCart.voucher() );
    obDBLedger.setDiscount( p_obDBShoppingCart.itemDiscount() );
    obDBLedger.setVatpercent( g_poPrefs->getDeviceUseVAT() );
    obDBLedger.setTotalPrice( p_obDBShoppingCart.itemSumPrice() );
    obDBLedger.setComment( qsComment );
    obDBLedger.save();

    cassaAddMoneyAction( p_obDBShoppingCart.cash()+p_obDBShoppingCart.voucher(), p_obDBShoppingCart.card(), obDBLedger.id(), qsComment );

    return obDBLedger.id();
}
//====================================================================================
void cCassa::cassaProcessProductSell( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType )
//====================================================================================
{
    QString     qsComment = p_qsComment;

    if( qsComment.length() > 0 && p_obDBShoppingCart.comment().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( p_obDBShoppingCart.comment() );

    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( cDBLedger::LT_PROD_SELL );
    obDBLedger.setPaymentMethod( p_inPayType );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( p_obDBShoppingCart.productId() );
    obDBLedger.setPanelId( p_obDBShoppingCart.panelId() );
    obDBLedger.setName( p_obDBShoppingCart.itemName() );
    obDBLedger.setItemCount( p_obDBShoppingCart.itemCount() );
    obDBLedger.setNetPrice( p_obDBShoppingCart.itemNetPrice() );
    obDBLedger.setCard( p_obDBShoppingCart.card() );
    obDBLedger.setCash( p_obDBShoppingCart.cash() );
    obDBLedger.setVoucher( p_obDBShoppingCart.voucher() );
    obDBLedger.setDiscount( p_obDBShoppingCart.itemDiscount() );
    obDBLedger.setVatpercent( p_obDBShoppingCart.itemVAT() );
    obDBLedger.setTotalPrice( p_obDBShoppingCart.itemSumPrice() );
    obDBLedger.setComment( qsComment );
    obDBLedger.save();

    if( g_obGuest.id() > 0 )
    {
        g_obGen.saveGuestActivity( g_obGuest.id(), PATIENTHISTORY_PURCHASEDPRODUCT, 0, 0, 0, p_obDBShoppingCart.productId() );
    }

    cassaAddMoneyAction( p_obDBShoppingCart.cash()+p_obDBShoppingCart.voucher(), p_obDBShoppingCart.card(), obDBLedger.id(), qsComment );

    cDBProduct  obDBProduct;

    obDBProduct.load( p_obDBShoppingCart.productId() );
    obDBProduct.decreaseProductCount( p_obDBShoppingCart.itemCount() );
    obDBProduct.save();
}
//====================================================================================
void cCassa::cassaConnectLedgerWithLedgerDevice( unsigned int p_uiLedgerId, unsigned int p_uiLedgerDeviceId )
//====================================================================================
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE ledger SET ledgerDeviceId=%1 WHERE ledgerId=%2" ).arg( p_uiLedgerDeviceId ).arg( p_uiLedgerId ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}
//====================================================================================
void cCassa::cassaProcessRevokeDeviceUse( unsigned int p_uiLedgerId )
//====================================================================================
{
    try
    {
        cDBLedger   obDBLedger;

        obDBLedger.load( p_uiLedgerId );
        obDBLedger.revoke();

        cDBCassaHistory obDBCassaHistory;

        obDBCassaHistory.loadByLedger( p_uiLedgerId );
        obDBCassaHistory.revoke( obDBLedger.id() );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}
//====================================================================================
void cCassa::cassaProcessRevokeCassaAction( unsigned int p_uiCassaHistoryId )
//====================================================================================
{
    unsigned int uiLedgerId = 0;

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.load( p_uiCassaHistoryId );

    if( obDBCassaHistory.ledgerId() > 0 )
    {
        cDBLedger   obDBLedger;

        obDBLedger.load( obDBCassaHistory.ledgerId() );
        obDBLedger.revoke();
        uiLedgerId = obDBLedger.id();
    }

    obDBCassaHistory.revoke( uiLedgerId );
}
//====================================================================================
void cCassa::cassaProcessCashExpense(int p_nMoney, QString p_qsComment)
//====================================================================================
{
    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( cDBLedger::LT_CASSA_EXPENSE );
    obDBLedger.setPaymentMethod( 1 );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( 0 );
    obDBLedger.setPanelId( 0 );
    obDBLedger.setName( QObject::tr("Cassa expense") );
    obDBLedger.setItemCount( 1 );
    obDBLedger.setNetPrice( p_nMoney );
    obDBLedger.setCard( 0 );
    obDBLedger.setCash( p_nMoney );
    obDBLedger.setVoucher( 0 );
    obDBLedger.setDiscount( 0 );
    obDBLedger.setVatpercent( 0 );
    obDBLedger.setTotalPrice( p_nMoney );
    obDBLedger.setComment( p_qsComment );
    obDBLedger.save();

    cassaAddMoneyAction( p_nMoney, 0, obDBLedger.id(), p_qsComment );
}
//====================================================================================
void cCassa::cassaIncreaseMoney( int p_nMoney, QString p_qsComment )
//====================================================================================
{
    QString m_qsComment = QObject::tr("Add money to cassa.");

    if( p_qsComment.length() > 0 )
    {
        m_qsComment += " - ";
        m_qsComment += p_qsComment;
    }

    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( p_nMoney );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaDecreaseMoney( int p_nMoney, QString p_qsComment )
//====================================================================================
{
    QString m_qsComment = QObject::tr("Remove money from cassa.");

    if( p_qsComment.length() > 0 )
    {
        m_qsComment += " - ";
        m_qsComment += p_qsComment;
    }

    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()-p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( -p_nMoney );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( -p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaDecreaseMoney( unsigned int p_uiUserId, int p_nMoney, QString p_qsComment )
//====================================================================================
{
    QString m_qsComment = QObject::tr("Remove money from cassa.");

    if( p_qsComment.length() > 0 )
    {
        m_qsComment += " - ";
        m_qsComment += p_qsComment;
    }

    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()-p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( p_uiUserId );
    obDBCassaHistory.setActionValue( -p_nMoney );
    obDBCassaHistory.setActionCard( 0 );
    obDBCassaHistory.setActionCash( -p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaAddMoneyAction( int p_nCash, int p_nCard, unsigned int p_uiLedgerId, QString p_qsComment, unsigned int p_uiParentId, bool p_bGlobalCassa )
//====================================================================================
{
    unsigned int uiCassaId = m_pCassa->id();

    if( !p_bGlobalCassa )
    {
        m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nCash );
        m_pCassa->save();
    }
    else
    {
        uiCassaId = 0;
    }

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setParentId( p_uiParentId );
    obDBCassaHistory.setCassaId( uiCassaId );
    obDBCassaHistory.setLedgerId( p_uiLedgerId );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( p_nCash+p_nCard );
    obDBCassaHistory.setActionCard( p_nCard );
    obDBCassaHistory.setActionCash( p_nCash );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( p_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
/*void cCassa::cassaAddGlobalMoneyAction( int p_nMoney, unsigned int p_uiLedgerId, QString p_qsComment, unsigned int p_uiParentId )
//====================================================================================
{
    cDBCassaHistory     obDBCassaHistory;

    obDBCassaHistory.createNew();
    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setParentId( p_uiParentId );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setLedgerId( p_uiLedgerId );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( p_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}*/
//====================================================================================
void cCassa::setEnabled()
//====================================================================================
{
    g_poDB->executeQTQuery( QString( "UPDATE settings SET value='%1' WHERE identifier=\"CURRENT_CASSA_ID\" " )
                            .arg( cassaId() ) );
    m_bCassaEnabled = true;
}
//====================================================================================
void cCassa::setDisabled()
//====================================================================================
{
    m_bCassaEnabled = false;
}
//====================================================================================
bool cCassa::isCassaEnabled()
//====================================================================================
{
    return m_bCassaEnabled;
}
//====================================================================================
bool cCassa::isCassaClosed()
//====================================================================================
{
    return ( m_pCassa->id()==0 ? true : false );
}
//====================================================================================
unsigned int cCassa::cassaOwnerId()
//====================================================================================
{
    return m_pCassa->userId();
}
//====================================================================================
QString cCassa::cassaOwnerStr()
//====================================================================================
{
    cTracer obTrace( "cCassa::cassaOwnerStr" );

    cDBUser     obUser;

    obUser.load( m_pCassa->userId() );

    return obUser.realName();
}
//====================================================================================
int cCassa::cassaBalance()
//====================================================================================
{
    return m_pCassa->currentBalance();
}
//====================================================================================
int cCassa::cassaGlobalBalance()
//====================================================================================
{
    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT SUM(actionValue) AS cassaGlobalBalance FROM cassahistory WHERE cassaId=0" ) );
    poQuery->first();

    return poQuery->value( 0 ).toInt();
}
//====================================================================================
unsigned int cCassa::cassaId()
//====================================================================================
{
    return m_pCassa->id();
}
//====================================================================================
QString cCassa::cassaOpenDate()
//====================================================================================
{
    return m_pCassa->startDateTime().replace('T'," ").left(m_pCassa->startDateTime().length()-3);
}
//====================================================================================
QString cCassa::cassaCloseDate()
//====================================================================================
{
    return m_pCassa->stopDateTime().replace('T'," ").left(m_pCassa->stopDateTime().length()-3);
}
//====================================================================================
