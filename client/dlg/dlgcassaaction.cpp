#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent, cDBShoppingCart *p_poShoppingCart ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa action") );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

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
        ledAmountGiven->setText( cPrice.currencyStringSeparator() );

        ledAmountToPay->setEnabled( false );
        ledAmountGiven->setFocus();

        connect( ledAmountToPay, SIGNAL(textChanged(QString)), this, SLOT(updateMoneyBack()) );
        connect( ledAmountGiven, SIGNAL(textChanged(QString)), this, SLOT(updateMoneyBack()) );
    }
    else
    {
        ledAmountToPay->setFocus();
        ledAmountGiven->setEnabled( false );
    }

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT paymentMethodId, name FROM paymentmethods WHERE active=1 AND archive<>\"DEL\"" ) );
    while( poQuery->next() )
    {
        cmbPaymentType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    connect( ledAmountToPay, SIGNAL(textEdited(QString)), this, SLOT(ledAmountToPay_textEdited(QString)) );
    connect( ledAmountGiven, SIGNAL(textEdited(QString)), this, SLOT(ledAmountGiven_textEdited(QString)) );

    m_nHeightSmall  = 217;
    m_nHeightBig    = 337;

    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );

    if( ledAmountGiven->isEnabled() )
    {
        ledAmountGiven->selectAll();
    }
}

cDlgCassaAction::~cDlgCassaAction()
{
}

void cDlgCassaAction::setPayWithCash()
{
    cmbPaymentType->setCurrentIndex( 0 );
}

void cDlgCassaAction::setPayWithCreditcard()
{
    cmbPaymentType->setCurrentIndex( 1 );
}

void cDlgCassaAction::actionCassaInOut()
{
    setPayWithCash();
    pbShoppingCart->setEnabled( false );
    cmbPaymentType->setEnabled( false );
    lblMoneyToPay->setText( tr("Amount :") );
    pbOk->setText( tr("Ok") );
    frmPayment->setVisible( false );
    m_nHeightSmall  = 165;
    m_nHeightBig    = 285;
    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );
}

void cDlgCassaAction::actionPayment()
{
    pbShoppingCart->setEnabled( false );
}

QString cDlgCassaAction::cassaResult( int *p_nPayType, QString *p_qsComment, bool *p_bShoppingCart )
{
    if( cmbPaymentType->currentIndex() == 0 ) *p_nPayType = cDlgCassaAction::PAY_CASH;
    else if( cmbPaymentType->currentIndex() == 1 ) *p_nPayType = cDlgCassaAction::PAY_CREDITCARD;
    else *p_nPayType = cDlgCassaAction::PAY_OTHER;

    if( teComment->toPlainText().length() > 0 )
        *p_qsComment += QString( " - %1" ).arg(teComment->toPlainText());

    *p_bShoppingCart = m_bShoppingCart;

    return ledAmountToPay->text().remove( QChar(',') );
}

void cDlgCassaAction::on_pbOk_clicked()
{
    m_bShoppingCart = false;

    QDialog::accept();
}

void cDlgCassaAction::on_pbCancel_clicked()
{
    m_bShoppingCart = false;

    QDialog::reject();
}

void cDlgCassaAction::updateMoneyBack()
{
    cCurrency   cGiven( ledAmountGiven->text() );
    cCurrency   cToPay( ledAmountToPay->text() );
    cCurrency   cToBack( cGiven.currencyValue().toInt() - cToPay.currencyValue().toInt() );

    lblAmountToBack->setText( cToBack.currencyStringSeparator() );
/*
    QString qsGiven = ledAmountGiven->text();
    QString qsToPay = ledAmountToPay->text();

    qsGiven.remove(QChar(','));
    qsToPay.remove(QChar(','));

    lblAmountToBack->setText( convertCurrency(QString::number( qsGiven.toInt() - qsToPay.toInt() )) );
*/
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

QString cDlgCassaAction::convertCurrency(const QString &text) const
{
    QString qsValue = text;
    QString qsRet = "";

    qsValue.remove(QChar(','));

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );

    return qsRet;
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

void cDlgCassaAction::on_pbShoppingCart_clicked()
{
    m_poShoppingCart->save();
    m_bShoppingCart = true;

    QDialog::accept();
}

void cDlgCassaAction::on_cmbPaymentType_currentIndexChanged(int index)
{

}
