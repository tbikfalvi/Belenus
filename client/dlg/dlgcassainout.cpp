
#include <QPushButton>
#include <QMessageBox>

#include "dlgcassainout.h"

cDlgCassaInOut::cDlgCassaInOut( QWidget *p_poParent, QString p_qsTitle ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( p_qsTitle );
    setWindowIcon( QIcon("./resources/40x40_key.png") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
}

cDlgCassaInOut::~cDlgCassaInOut()
{
}

int cDlgCassaInOut::resultAmount()
{
    cCurrency   cAmount( ledAmount->text() );

    return cAmount.currencyValue().toInt();
}

QString cDlgCassaInOut::resultComment()
{
    return teComment->toPlainText();
}

void cDlgCassaInOut::on_pbOk_clicked()
{
    QDialog::accept();
}

void cDlgCassaInOut::on_pbCancel_clicked()
{
    QDialog::reject();
}
