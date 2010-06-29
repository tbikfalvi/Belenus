#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa action") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );

    lblCurrencyShort->setText( g_poPrefs->getCurrencyShort() );
}

cDlgCassaAction::~cDlgCassaAction()
{
}

void cDlgCassaAction::setInitialMoney( int p_nMoney )
{
    ledMoney->setText( QString::number(p_nMoney) );
    on_ledMoney_textEdited("");
}

void cDlgCassaAction::setPayWithCash()
{
    rbPayCash->setChecked( true );
}

void cDlgCassaAction::setPayWithVoucher()
{
    rbVoucher->setChecked( true );
}

void cDlgCassaAction::setPayWithCreditcard()
{
    rbCreditcard->setChecked( true );
}

void cDlgCassaAction::setCassaAction()
{
    cbInvoice->setEnabled( false );
    setPayWithCash();
    rbPayCash->setEnabled( false );
    rbVoucher->setEnabled( false );
    rbCreditcard->setEnabled( false );
}

QString cDlgCassaAction::cassaResult( int *p_nPayType, QString *p_qsComment )
{
    if( rbPayCash->isChecked() ) *p_nPayType = 1;
    else if( rbVoucher->isChecked() ) *p_nPayType = 2;
    else if( rbCreditcard->isChecked() ) *p_nPayType = 3;

    *p_qsComment = teComment->toPlainText();
    return ledMoney->text().remove( QChar(',') );
}

void cDlgCassaAction::on_pbOk_clicked()
{
    QDialog::accept();
}

void cDlgCassaAction::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgCassaAction::on_ledMoney_textEdited(QString)
{
    QString qsValue = ledMoney->text();
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
    ledMoney->setText( qsRet );
}
