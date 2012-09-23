
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

#include "dlgserialreg.h"
#include "../licenceManager.h"

extern cLicenceManager g_obLicenceManager;

cDlgSerialReg::cDlgSerialReg( QWidget *p_poParent ) : QDialog( p_poParent )
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

    // be kell irni a liszensz kulcsot - minden exe-hez különbözõ van, SORSZÁMmal
    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Internet Explorer\Registration -> ProductId
    // HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\ComputerName\ComputerName -> ComputerName
    // ebbõl a kettõbõl és a sorszámból csináljon egy ^11 -es karaktersort és azt küldje el
    // válaszképp kap egy másik karaktersort, azt írja be és kész

    QDialog::accept();
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}
