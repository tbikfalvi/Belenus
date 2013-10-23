#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"
#include "db/dbdiscount.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent, cDBShoppingCart *p_poShoppingCart ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa action") );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

    pbPayCash->setIcon( QIcon("./resources/40x40_paywithcash.png") );
    pbPayCard->setIcon( QIcon("./resources/40x40_paywithcard.png") );
    pbPayOther->setIcon( QIcon("./resources/40x40_question.png") );

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

        connect( ledAmountToPay, SIGNAL(textChanged(QString)), this, SLOT(updateMoneyBack()) );
        connect( ledAmountGiven, SIGNAL(textChanged(QString)), this, SLOT(updateMoneyBack()) );
    }
    else
    {
        ledAmountGiven->setEnabled( false );
    }
g_obLogger(cSeverity::DEBUG) << "1" << EOM;
    cmbPaymentType->addItem( tr("<Not selected>"), 0 );
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT paymentMethodId, name FROM paymentmethods WHERE active=1 AND archive<>\"DEL\" AND paymentMethodId>2" ) );
    while( poQuery->next() )
    {
        cmbPaymentType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }
g_obLogger(cSeverity::DEBUG) << "2" << EOM;
    if( cmbCoupon )
    {
g_obLogger(cSeverity::DEBUG) << "3" << EOM;
        cmbCoupon->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT discountId, name FROM discounts WHERE patientId=0 AND companyId=0 AND paymentMethodId=0 AND productId=0 AND regularCustomer=0 AND employee=0 AND service=0 AND active=1 AND archive<>\"DEL\" " ) );
g_obLogger(cSeverity::DEBUG) << "4" << EOM;
        while( poQuery->next() )
        {
            cmbCoupon->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        }
    }

    connect( ledAmountToPay, SIGNAL(textEdited(QString)), this, SLOT(ledAmountToPay_textEdited(QString)) );
    connect( ledAmountGiven, SIGNAL(textEdited(QString)), this, SLOT(ledAmountGiven_textEdited(QString)) );

    m_nHeightSmall  = 315;
    m_nHeightBig    = 430;

    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );

    if( ledAmountGiven->isEnabled() )
    {
        ledAmountGiven->selectAll();
    }
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
    pbShoppingCart->setEnabled( false );
    ledVoucherGiven->setEnabled( false );
    cmbCoupon->setEnabled( false );
    frmMoneyVoucher->setVisible( false );
    frmCoupon->setVisible( false );
    frmMoneyGiven->setVisible( false );
    frmPayment->setVisible( false );
    m_nHeightSmall  = 217;
    m_nHeightBig    = 332;
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

void cDlgCassaAction::updateMoneyBack()
{
    cCurrency   cGiven( ledAmountGiven->text() );
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cVoucher( ledVoucherGiven->text() );
    cCurrency   cToBack( cGiven.currencyValue().toInt() + cVoucher.currencyValue().toInt() - cToPay.currencyValue().toInt() );

    if( cGiven.currencyValue().toInt() > 0 &&
        cGiven.currencyValue().toInt() + cVoucher.currencyValue().toInt() > cToPay.currencyValue().toInt() )
    {
        lblAmountToBack->setText( cToBack.currencyStringSeparator() );
    }
    else
    {
        lblAmountToBack->setText("");
    }
}

void cDlgCassaAction::ledAmountToPay_textEdited(QString text)
{
//    cCurrency   cPrice( text );

//    ledAmountToPay->setText( cPrice.currencyStringSeparator() );
}

void cDlgCassaAction::ledAmountGiven_textEdited(QString text)
{
//    cCurrency   cPrice( text );

//    ledAmountGiven->setText( cPrice.currencyString() );
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

void cDlgCassaAction::on_cmbPaymentType_currentIndexChanged(int index)
{

}

void cDlgCassaAction::on_pbPayCash_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: bold; color: blue;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: normal;}" );

    cmbPaymentType->setEnabled( false );

    if( pbPayCash->isChecked() )
    {
        pbPayCard->setChecked( false );
        pbPayOther->setChecked( false );
    }
    else
    {
        pbPayCash->setChecked( true );
    }

    ledVoucherGiven->setFocus();
    ledVoucherGiven->selectAll();
}

void cDlgCassaAction::on_pbPayCard_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: bold; color: blue;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: normal;}" );

    cmbPaymentType->setEnabled( false );

    if( pbPayCard->isChecked() )
    {
        pbPayCash->setChecked( false );
        pbPayOther->setChecked( false );
    }
    else
    {
        pbPayCard->setChecked( true );
    }

    ledVoucherGiven->setFocus();
    ledVoucherGiven->selectAll();
}

void cDlgCassaAction::on_pbPayOther_clicked()
{
    pbPayCash->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayCard->setStyleSheet( "QPushButton {font: normal;}" );
    pbPayOther->setStyleSheet( "QPushButton {font: bold; color: blue;}" );

    cmbPaymentType->setEnabled( true );

    if( pbPayOther->isChecked() )
    {
        pbPayCash->setChecked( false );
        pbPayCard->setChecked( false );
    }
    else
    {
        pbPayOther->setChecked( true );
    }

    cmbPaymentType->setFocus();
}

void cDlgCassaAction::on_ledVoucherGiven_textChanged(const QString &arg1)
{
    cCurrency   cPrice( m_poShoppingCart->itemSumPrice() );
    cCurrency   cVoucher( ledVoucherGiven->text() );

    int nSum = cPrice.currencyValue().toInt() - cVoucher.currencyValue().toInt();

    if( nSum < 0 )
        nSum = 0;

    cCurrency   cToPay( nSum );

    ledAmountToPay->setText( cToPay.currencyStringSeparator() );
}


void cDlgCassaAction::on_cmbCoupon_currentIndexChanged(int index)
{
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
}

void cDlgCassaAction::updateShoppingCartItem()
{
    unsigned int uiCouponId = cmbCoupon->itemData( cmbCoupon->currentIndex() ).toUInt();

    m_poShoppingCart->setVoucher( ledVoucherGiven->text().remove( QChar(',') ).toInt() * 100 );

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

