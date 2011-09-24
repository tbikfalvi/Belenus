#include <QPushButton>
#include <QMessageBox>

#include "dlgserialreg.h"
#include "../licenceManager.h"

//extern LicenceManager g_obLicenceManager;

cDlgSerialReg::cDlgSerialReg( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Validate Serial key") );
    setWindowIcon( QIcon("./resources/40x40_key.png") );

    pbValidate->setIcon( QIcon("./resources/40x40_edit.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
// 'TO BE SOLVED'
//    ledSerial->setText( g_obLicenceManager.getClientSerial() );
    ledSerial->setFocus();
    ledSerial->selectAll();
}

cDlgSerialReg::~cDlgSerialReg()
{
}

void cDlgSerialReg::on_pbValidate_clicked()
{
// 'TO BE SOLVED'
//    g_obLicenceManager.validateLicence(ledSerial->text());

    QDialog::accept();
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}
