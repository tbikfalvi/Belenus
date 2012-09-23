
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

    // be kell irni a liszensz kulcsot - minden exe-hez k�l�nb�z� van, SORSZ�Mmal
    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Internet Explorer\Registration -> ProductId
    // HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\ComputerName\ComputerName -> ComputerName
    // ebb�l a kett�b�l �s a sorsz�mb�l csin�ljon egy ^11 -es karaktersort �s azt k�ldje el
    // v�laszk�pp kap egy m�sik karaktersort, azt �rja be �s k�sz

    QDialog::accept();
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}
