
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

#include "dlgserialreg.h"
#include "../licenceManager.h"

extern cLicenceManager g_obLicenceManager;

cDlgSerialReg::cDlgSerialReg( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_key.png") );

    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    if( g_poPrefs->getLicenceId() > 1 )
    {
        setWindowTitle( tr("Activate application") );
        pbValidate->setIcon( QIcon("./resources/40x40_ok.png") );
        lblSerial->setText( tr("Enter activation key") );
    }
    else
    {
        setWindowTitle( tr("Validate Serial key") );
        pbValidate->setIcon( QIcon("./resources/40x40_edit.png") );
        lblSerial->setText( tr("Enter serial key") );

        ledSerial->setText( g_obLicenceManager.licenceKey() );
    }

    ledSerial->setFocus();
    ledSerial->selectAll();
}

cDlgSerialReg::~cDlgSerialReg()
{
}

void cDlgSerialReg::on_pbValidate_clicked()
{
    if( g_poPrefs->getLicenceId() > 1 )
    {
        int nRet = g_obLicenceManager.activateLicence( ledSerial->text() );

        switch( nRet )
        {
            case cLicenceManager::ERR_NO_ERROR:
                QMessageBox::information( this, tr("Information"),
                                          tr("Your licence key has been activated successfully.\n\n"
                                             "Please note that you have to validate your application\n"
                                             "regulary by your franchise partner.\n"
                                             "Without validation the application can run in DEMO mode.\n\n"
                                             "You have %1 days until the next validation.").arg( g_obLicenceManager.daysRemain() ) );
                QDialog::accept();
                break;
        }
    }
    else
    {
        int nRet = g_obLicenceManager.validateLicence( ledSerial->text() );

        switch( nRet )
        {
            case cLicenceManager::ERR_NO_ERROR:
                QMessageBox::information( this, tr("Information"),
                                          tr("Your licence key has been verified successfully.\n"
                                             "Please send the following validation key to the\n"
                                             "provider of the Belenus application or validate\n"
                                             "the key through the internet and enter the given\n"
                                             "activation key into the edit box.\n\n"
                                             "Validation key: %1").arg( g_obLicenceManager.validationKey() ) );

                setWindowTitle( tr("Activate application") );
                pbValidate->setIcon( QIcon("./resources/40x40_ok.png") );
                lblSerial->setText( tr("Enter activation key") );
                break;

            case cLicenceManager::ERR_KEY_FORMAT_MISMATCH:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The format of the licence key you entered is not valid.\n"
                                         "Please check your licence key and retype it if necessary..") );
                break;

            case cLicenceManager::ERR_KEY_NUMBER_INCORRECT:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The order number of the licence key you entered is not correct.\n"
                                         "Please check your licence key and retype it if necessary..") );
                break;

            case cLicenceManager::ERR_KEY_NOT_EXISTS:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The licence key you entered is not valid.\n"
                                         "Please check your licence key and retype it if necessary..") );
                break;
        }
    }
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}
