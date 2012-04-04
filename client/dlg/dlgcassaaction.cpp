#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent, cDBShoppingCart *p_poShoppingCart )
    : QDialog( p_poParent )
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

    int nMoney = 0;

    if( p_poShoppingCart )
    {
        m_poShoppingCart = p_poShoppingCart;
        nMoney = m_poShoppingCart->itemSumPrice();
    }

    if( nMoney > 0 )
    {
        ledAmountToPay->setText( QString::number( nMoney ) );
        ledAmountGiven->setText( QString::number( nMoney ) );

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

    connect( ledAmountToPay, SIGNAL(textEdited(QString)), this, SLOT(ledAmountToPay_textEdited(QString)) );
    connect( ledAmountGiven, SIGNAL(textEdited(QString)), this, SLOT(ledAmountGiven_textEdited(QString)) );

    m_nHeightSmall  = 217;
    m_nHeightBig    = 337;

    setMinimumHeight( m_nHeightSmall );
    setMaximumHeight( m_nHeightSmall );
}

cDlgCassaAction::~cDlgCassaAction()
{
}

void cDlgCassaAction::setPayWithCash()
{
    rbPayCash->setChecked( true );
}

void cDlgCassaAction::setPayWithCreditcard()
{
    rbCreditcard->setChecked( true );
}

void cDlgCassaAction::actionCassaInOut()
{
    setPayWithCash();
    pbShoppingCart->setEnabled( false );
    rbPayCash->setEnabled( false );
    rbCreditcard->setEnabled( false );
    lblMoneyToPay->setText( tr("Amount :") );
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

QString cDlgCassaAction::cassaResult( int *p_nPayType, QString *p_qsComment )
{
    if( rbPayCash->isChecked() ) *p_nPayType = cDlgCassaAction::PAY_CASH;
    else if( rbCreditcard->isChecked() ) *p_nPayType = cDlgCassaAction::PAY_CREDITCARD;

    *p_qsComment += teComment->toPlainText();
    return ledAmountToPay->text().remove( QChar(',') );
}

void cDlgCassaAction::on_pbOk_clicked()
{
    QDialog::accept();
}

void cDlgCassaAction::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgCassaAction::updateMoneyBack()
{
    QString qsGiven = ledAmountGiven->text();
    QString qsToPay = ledAmountToPay->text();

    qsGiven.remove(QChar(','));
    qsToPay.remove(QChar(','));

    lblAmountToBack->setText( convertCurrency(QString::number( qsGiven.toInt() - qsToPay.toInt() )) );
}

void cDlgCassaAction::ledAmountToPay_textEdited(QString text)
{
    ledAmountToPay->setText( convertCurrency(text) );
}

void cDlgCassaAction::ledAmountGiven_textEdited(QString text)
{
    ledAmountGiven->setText( convertCurrency(text) );
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

}
