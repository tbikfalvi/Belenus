#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"
#include "db/dbdiscount.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent, cDBShoppingCart *p_poShoppingCart ) : QDialog( p_poParent )
{
    m_bActionPayment = false;

    setupUi( this );

    setWindowTitle( tr("Cassa action") );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

    pbPayCash->setIcon( QIcon("./resources/40x40_paywithcash.png") );
    pbPayCard->setIcon( QIcon("./resources/40x40_paywithcard.png") );
    pbPayOther->setIcon( QIcon("./resources/40x40_question.png") );

    pbCalculateCash->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbCalculateCard->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbCalculateVoucher->setIcon( QIcon("./resources/40x40_refresh.png") );

    pbComment->setIcon( QIcon("./resources/40x40_edit.png") );
    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbShoppingCart->setIcon( QIcon("./resources/40x40_shoppingcart.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    gbComment->setVisible( false );
    teComment->setEnabled( false );

    m_bShoppingCart = false;

    int nMoney = 0;

    if( p_poShoppingCart )
    {
        m_poShoppingCart = p_poShoppingCart;
        nMoney = m_poShoppingCart->itemSumPrice();
    }

    g_obLogger(cSeverity::DEBUG) << "nMoney: [" << nMoney << "]" << EOM;

    if( nMoney > 0 )
    {
        cCurrency   cPrice( nMoney );

        g_obLogger(cSeverity::DEBUG) << "[" << cPrice.currencyValue() << "]" << EOM;

        ledAmountToPay->setText( cPrice.currencyStringSeparator() );
        ledAmountToPay->setEnabled( false );
    }

    cmbPaymentType->addItem( tr("<Not selected>"), 0 );
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT paymentMethodId, name FROM paymentmethods WHERE active=1 AND archive<>\"DEL\" AND paymentMethodId>2" ) );
    while( poQuery->next() )
    {
        cmbPaymentType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    if( cmbCoupon )
    {
        cmbCoupon->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT discountId, name FROM discounts WHERE patientId=0 AND companyId=0 AND paymentMethodId=0 AND productId=0 AND regularCustomer=0 AND employee=0 AND service=0 AND active=1 AND archive<>\"DEL\" " ) );

        while( poQuery->next() )
        {
            cmbCoupon->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        }
    }

    m_nHeightSmall  = 334;
    m_nHeightBig    = 450;

    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );

    pbPayCash->setFocus();
}

cDlgCassaAction::~cDlgCassaAction()
{
}

void cDlgCassaAction::setPayWithCash()
{
    on_pbPayCash_clicked();
}

void cDlgCassaAction::setPayWithCreditcard()
{
    on_pbPayCard_clicked();
}

void cDlgCassaAction::actionPayment()
{
    m_bActionPayment = true;
    pbShoppingCart->setEnabled( false );
    frmCoupon->setVisible( false );
    frmCoupon->setEnabled( false );

    m_nHeightSmall  = 308;
    m_nHeightBig    = 424;
    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );
}

QString cDlgCassaAction::cassaResult( int *p_nPayType, bool *p_bShoppingCart, unsigned int *p_uiCouponId )
{
    if( pbPayCash->isChecked() ) *p_nPayType = cDlgCassaAction::PAY_CASH;
    else if( pbPayCard->isChecked() ) *p_nPayType = cDlgCassaAction::PAY_CREDITCARD;
    else *p_nPayType = cDlgCassaAction::PAY_OTHER;

    *p_bShoppingCart    = m_bShoppingCart;
    *p_uiCouponId       = cmbCoupon->itemData( cmbCoupon->currentIndex() ).toUInt();

    return ledAmountToPay->text().remove( QChar(',') );
}

void cDlgCassaAction::on_pbComment_clicked()
{
    if( pbComment->text().compare( tr("Comment >>") ) == 0 )
    {
        gbComment->setVisible( true );
        teComment->setEnabled( true );
        pbComment->setText( tr("Comment <<") );
        setMinimumHeight( m_nHeightBig );
        setMaximumHeight( m_nHeightBig );
        teComment->setFocus();
    }
    else
    {
        gbComment->setVisible( false );
        teComment->setEnabled( false );
        pbComment->setText( tr("Comment >>") );
        setMinimumHeight( m_nHeightSmall );
        setMaximumHeight( m_nHeightSmall );
    }
}

void cDlgCassaAction::on_pbPayCash_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: bold; color: blue;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: normal;}" );

    cmbPaymentType->setEnabled( false );

    pbPayCard->setChecked( false );
    pbPayOther->setChecked( false );
    pbPayCash->setChecked( true );

    cCurrency   cToPay( ledAmountToPay->text() );

    ledCashGiven->setText( cToPay.currencyStringSeparator() );
    ledCardGiven->setText( "0" );
    ledVoucherGiven->setText( "0" );

    ledCashGiven->setFocus();
    ledCashGiven->selectAll();

    _updateButtons();
}

void cDlgCassaAction::on_pbPayCard_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: bold; color: blue;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: normal;}" );

    cmbPaymentType->setEnabled( false );

    pbPayCash->setChecked( false );
    pbPayOther->setChecked( false );
    pbPayCard->setChecked( true );

    cCurrency   cToPay( ledAmountToPay->text() );

    ledCashGiven->setText( "0" );
    ledCardGiven->setText( cToPay.currencyStringSeparator() );
    ledVoucherGiven->setText( "0" );

    ledCardGiven->setFocus();
    ledCardGiven->selectAll();

    _updateButtons();
}

void cDlgCassaAction::on_pbPayOther_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: bold; color: blue;}" );

    cmbPaymentType->setEnabled( true );

    pbPayCash->setChecked( false );
    pbPayCard->setChecked( false );
    pbPayOther->setChecked( true );

    cCurrency   cToPay( ledAmountToPay->text() );

    ledCashGiven->setText( "0" );
    ledCardGiven->setText( cToPay.currencyStringSeparator() );
    ledVoucherGiven->setText( "0" );

    cmbPaymentType->setFocus();

    _updateButtons();
}

void cDlgCassaAction::on_ledCashGiven_textChanged(const QString&)
{
    if( !pbPayCash->isChecked() )
    {
        cCurrency   cToPay( ledAmountToPay->text() );
        cCurrency   cCash( ledCashGiven->text() );
        cCurrency   cVoucher( ledVoucherGiven->text() );

        int inSum = cToPay.currencyValue().toInt() - cCash.currencyValue().toInt() - cVoucher.currencyValue().toInt();

        cCurrency   cCard( inSum );

        ledCardGiven->setText( cCard.currencyStringSeparator() );
    }
    checkGivenValues();
}

void cDlgCassaAction::on_ledCardGiven_textChanged(const QString&)
{
    if( pbPayCash->isChecked() )
    {
        cCurrency   cToPay( ledAmountToPay->text() );
        cCurrency   cCard( ledCardGiven->text() );
        cCurrency   cVoucher( ledVoucherGiven->text() );

        int inSum = cToPay.currencyValue().toInt() - cCard.currencyValue().toInt() - cVoucher.currencyValue().toInt();

        cCurrency   cCash( inSum );

        ledCashGiven->setText( cCash.currencyStringSeparator() );
    }
    checkGivenValues();
}

void cDlgCassaAction::on_ledVoucherGiven_textChanged(const QString&)
{
    QLineEdit   *poGiven;
    QLineEdit   *poCount;

    if( pbPayCash->isChecked() )
    {
        poGiven = ledCashGiven;
        poCount = ledCardGiven;
    }
    else
    {
        poGiven = ledCardGiven;
        poCount = ledCashGiven;
    }

    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cCount( poCount->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    int inSum = cToPay.currencyValue().toInt() - cCount.currencyValue().toInt() - cVoucher.currencyValue().toInt();

    cCurrency   cCash( inSum );

    poGiven->setText( cCash.currencyStringSeparator() );

    checkGivenValues();
}

void cDlgCassaAction::checkGivenValues()
{
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cCash( ledCashGiven->text() );
    cCurrency   cCard( ledCardGiven->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    int inTotal = cToPay.currencyValue().toInt();
    int inSum   = cCash.currencyValue().toInt() + cCard.currencyValue().toInt() + cVoucher.currencyValue().toInt();

    if( inTotal != inSum )
    {
        ledCashGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(255, 0, 0);}" );
        ledCardGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(255, 0, 0);}" );
        ledVoucherGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(255, 0, 0);}" );
        _updateButtons( false );
    }
    else
    {
        ledCashGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(0, 85, 0);}" );
        ledCardGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(0, 85, 0);}" );
        ledVoucherGiven->setStyleSheet( "QLineEdit {border: 0; background: transparent; color: rgb(0, 85, 0);}" );
        _updateButtons();
    }
}

void cDlgCassaAction::on_cmbCoupon_currentIndexChanged(int index)
{
    QLineEdit   *poGiven;

    if( pbPayCash->isChecked() )
    {
        poGiven = ledCashGiven;
    }
    else
    {
        poGiven = ledCardGiven;
    }

    if( index > 0 )
    {
        cDBDiscount obDBDiscount;

        obDBDiscount.load( cmbCoupon->itemData( cmbCoupon->currentIndex() ).toUInt() );

        cCurrency   cPrice( obDBDiscount.discountedValue( m_poShoppingCart->itemSumPrice() ) );

        ledAmountToPay->setText( cPrice.currencyStringSeparator() );
    }
    else
    {
        cCurrency   cPrice( m_poShoppingCart->itemSumPrice() );

        ledAmountToPay->setText( cPrice.currencyStringSeparator() );
    }
    poGiven->setText( ledAmountToPay->text() );
    on_ledVoucherGiven_textChanged("");

    checkGivenValues();
}

void cDlgCassaAction::updateShoppingCartItem()
{
    unsigned int uiCouponId = cmbCoupon->itemData( cmbCoupon->currentIndex() ).toUInt();

    cCurrency   cCash( ledCashGiven->text() );
    cCurrency   cCard( ledCardGiven->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    m_poShoppingCart->setCash( cCash.currencyValue().toInt() );
    m_poShoppingCart->setCard( cCard.currencyValue().toInt() );
    m_poShoppingCart->setVoucher( cVoucher.currencyValue().toInt() );

    if( uiCouponId > 0 )
    {
        cDBDiscount     obDBDiscount;

        obDBDiscount.load( uiCouponId );
        m_poShoppingCart->setItemDiscount( m_poShoppingCart->itemDiscount()+obDBDiscount.discount(m_poShoppingCart->itemSumPrice()) );

        if( teComment->toPlainText().length() > 0 )
            teComment->append( "\n" );
        teComment->append( tr("Coupon used: %1").arg( cmbCoupon->currentText() ) );
    }

    QString qsComment = m_poShoppingCart->comment();

    if( qsComment.length() > 0 && teComment->toPlainText().length() > 0 )
        qsComment.append( "\n" );
    qsComment.append( teComment->toPlainText() );

    m_poShoppingCart->setComment( qsComment );
}

void cDlgCassaAction::on_pbOk_clicked()
{
    updateShoppingCartItem();
    m_bShoppingCart = false;

    QDialog::accept();
}

void cDlgCassaAction::on_pbShoppingCart_clicked()
{
    updateShoppingCartItem();
    m_poShoppingCart->save();
    m_bShoppingCart = true;

    QDialog::accept();
}

void cDlgCassaAction::on_pbCancel_clicked()
{
    m_bShoppingCart = false;

    QDialog::reject();
}

void cDlgCassaAction::_updateButtons( bool p_bEnabled )
{
    bool    bEnablePayment = p_bEnabled;

    if( pbPayOther->isChecked() && cmbPaymentType->currentIndex() == 0 )
    {
        bEnablePayment = false;
    }

    pbOk->setEnabled( bEnablePayment );
    if( !m_bActionPayment )
    {
        pbShoppingCart->setEnabled( bEnablePayment );
    }
}

void cDlgCassaAction::on_cmbPaymentType_currentIndexChanged(int)
{
    _updateButtons();
}

void cDlgCassaAction::on_pbCalculateCash_clicked()
{
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cCard( ledCardGiven->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    int inSum = cToPay.currencyValue().toInt() - cCard.currencyValue().toInt() - cVoucher.currencyValue().toInt();

    cCurrency   cCash( inSum );

    ledCashGiven->setText( cCash.currencyStringSeparator() );

    checkGivenValues();
}

void cDlgCassaAction::on_pbCalculateCard_clicked()
{
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cCash( ledCashGiven->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    int inSum = cToPay.currencyValue().toInt() - cCash.currencyValue().toInt() - cVoucher.currencyValue().toInt();

    cCurrency   cCard( inSum );

    ledCardGiven->setText( cCard.currencyStringSeparator() );

    checkGivenValues();}

void cDlgCassaAction::on_pbCalculateVoucher_clicked()
{
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cCash( ledCashGiven->text() );
    cCurrency   cCard( ledCardGiven->text() );

    int inSum = cToPay.currencyValue().toInt() - cCash.currencyValue().toInt() - cCard.currencyValue().toInt();

    cCurrency   cVoucher( inSum );

    ledVoucherGiven->setText( cVoucher.currencyStringSeparator() );

    checkGivenValues();
}
