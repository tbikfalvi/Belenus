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
        obDBCassaHistory.setActionBalance( p_inBalance );
        obDBCassaHistory.setComment( QObject::tr("Open new cassa record.") );
        obDBCassaHistory.setActive( true );
        obDBCassaHistory.save();

        setEnabled();
    }
    catch( cSevException &e )
    {
            g_obLogger(e.severity()) << e.what() << EOM;
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
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( 0 );
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
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Close cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaProcessPatientCardSell( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_DBShoppingCart, QString p_qsComment, bool p_bNewCard, int p_inPayType )
//====================================================================================
{
    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    if( p_bNewCard )
        obDBLedger.setLedgerTypeId( 2 );
    else
        obDBLedger.setLedgerTypeId( 3 );
    obDBLedger.setLedgerDeviceId( 0 );
    obDBLedger.setPaymentMethod( p_inPayType );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( 0 );
    obDBLedger.setPatientCardTypeId( p_DBPatientCard.patientCardTypeId() );
    obDBLedger.setPatientCardId( p_DBPatientCard.id() );
    obDBLedger.setPanelId( 0 );
    obDBLedger.setName( p_DBPatientCard.barcode() );
    obDBLedger.setNetPrice( p_DBShoppingCart.itemNetPrice() );
    obDBLedger.setDiscount( p_DBShoppingCart.itemDiscount() );
    obDBLedger.setVatpercent( p_DBShoppingCart.itemVAT() );
    obDBLedger.setComment( qsComment );
    obDBLedger.setActive( true );
    obDBLedger.save();

    if( p_inPayType == 1 /*PAY_CASH*/ )
    {
        cassaAddMoneyAction( p_DBShoppingCart.itemSumPrice(), obDBLedger.id(), qsComment );
    }
    else
    {
        cassaAddGlobalMoneyAction( p_DBShoppingCart.itemSumPrice(), obDBLedger.id(), qsComment );
    }
}
//====================================================================================
void cCassa::cassaProcessPatientCardRefill( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_DBShoppingCart, QString p_qsComment, int p_inPayType )
//====================================================================================
{
    cassaProcessPatientCardSell( p_DBPatientCard, p_DBShoppingCart, p_qsComment, false, p_inPayType );
}
//====================================================================================
void cCassa::cassaProcessProductStorageChange( const cDBShoppingCart &p_DBShoppingCart, QString p_qsComment, bool p_bGlobalCassa )
//====================================================================================
{
    cDBLedger   obDBLedger;

    obDBLedger.createNew();
    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( 5 );
    obDBLedger.setPaymentMethod( 1 );
    obDBLedger.setUserId( 0 );
    obDBLedger.setProductId( p_DBShoppingCart.productId() );
    obDBLedger.setName( p_DBShoppingCart.itemName() );
    obDBLedger.setItemCount( p_DBShoppingCart.itemCount() );
    obDBLedger.setNetPrice( p_DBShoppingCart.itemNetPrice() );
    obDBLedger.setVatpercent( p_DBShoppingCart.itemVAT() );
    obDBLedger.setTotalPrice( p_DBShoppingCart.itemSumPrice() );
    obDBLedger.setComment( p_qsComment );
    obDBLedger.save();

    if( !p_bGlobalCassa )
    {
        cassaAddMoneyAction( p_DBShoppingCart.itemSumPrice(), obDBLedger.id(), qsComment );
    }
    else
    {
        cassaAddGlobalMoneyAction( p_DBShoppingCart.itemSumPrice(), obDBLedger.id(), qsComment );
    }
}
//====================================================================================
void cCassa::cassaProcessDeviceUsePayed()
//====================================================================================
{
    cDBLedger   obDBLedger;

    obDBLedger.createNew();


    g_obCassa.cassaAddMoneyAction( inPriceTotal, qsComment );

    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
    obDBLedger.setLedgerTypeId( 1 );
    obDBLedger.setLedgerDeviceId( 0 );
    obDBLedger.setPaymentMethod( p_nPaymentType );
    obDBLedger.setUserId( g_obUser.id() );
    obDBLedger.setProductId( 0 );
    obDBLedger.setPatientCardTypeId( 0 );
    obDBLedger.setPatientCardId( 0 );
    obDBLedger.setPanelId( p_obDBShoppingCart.panelId() );
    obDBLedger.setName( mdiPanels->getPanelCaption(p_obDBShoppingCart.panelId()) );
    obDBLedger.setNetPrice( p_obDBShoppingCart.itemNetPrice() );
    obDBLedger.setDiscount( p_obDBShoppingCart.itemDiscount() );
    obDBLedger.setVatpercent( g_poPrefs->getDeviceUseVAT() );
    obDBLedger.setComment( p_qsComment );
    obDBLedger.setActive( true );
    obDBLedger.save();
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
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaAddMoneyAction( int p_nMoney, unsigned int p_uiLedgerId, QString p_qsComment, unsigned int p_uiParentId )
//====================================================================================
{
    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

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
}
//====================================================================================
void cCassa::cassaAddGlobalMoneyAction( int p_nMoney, unsigned int p_uiLedgerId, QString p_qsComment, unsigned int p_uiParentId )
//====================================================================================
{
    cDBCassaHistory     obDBCassaHistory;

    obDBCassaHistory.createNew();
    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setParentId( p_uiParentId );
    obDBCassaHistory.setCassaId( 0 );
    obDBCassaHistory.setLedgerId( p_uiLedgerId );
    obDBCassaHistory.setUserId( 0 );
    obDBCassaHistory.setPatientId( 0 );
    obDBCassaHistory.setActionValue( p_nMoney );
    obDBCassaHistory.setActionBalance( cassaGlobalBalance()+p_nMoney );
    obDBCassaHistory.setComment( p_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::setEnabled()
//====================================================================================
{
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
QString cCassa::cassaOwnerStr()
//====================================================================================
{
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
