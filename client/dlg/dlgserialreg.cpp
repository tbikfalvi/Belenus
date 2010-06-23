#include <QPushButton>
#include <QMessageBox>

#include "dlgserialreg.h"

cDlgSerialReg::cDlgSerialReg( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Validate Serial key") );

    pbValidate->setIcon( QIcon("./resources/40x40_edit.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );

    ledSerial->setText( g_poPrefs->getClientSerial() );
    ledSerial->setFocus();
    ledSerial->selectAll();
}

cDlgSerialReg::~cDlgSerialReg()
{
}

void cDlgSerialReg::on_pbValidate_clicked()
{
    // GABOR : IDE JÖN A SERIAL KEY VALIDÁLÁSA

    QDialog::accept();
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}
