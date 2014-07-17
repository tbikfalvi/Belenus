
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

    pbActivateKey->setIcon( QIcon("./resources/40x40_key.png") );
    pbValidateCode->setIcon( QIcon("./resources/40x40_validate.png") );
    pbValidateApplication->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_exit.png") );

    g_obLogger(cSeverity::INFO) << "Licence id: " << g_poPrefs->getLicenceId() << EOM;

    ledSerialKey->setText( g_obLicenceManager.licenceKey() );
    lblValidDays->setText( "   " );

    ledSerialKey->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    pbActivateKey->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    ledCodeActivation->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    deLastValidated->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    pbValidateApplication->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    pbOk->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    deLastValidated->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    if( /*g_poPrefs->getLicenceId() > 1 &&*/ g_obLicenceManager.activateLicence( ledSerialKey->text() ) == cLicenceManager::ERR_NO_ERROR )
    {
        ledCodeActivation->setText( g_obLicenceManager.validationKey() );
        deLastValidated->setDate( QDate::fromString( g_obLicenceManager.lastValidated(), "yyyy-MM-dd" ) );
        ledCodeValidation->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        ledCodeValidation->setText( g_obLicenceManager.activationKey() );
        pbValidateCode->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        lblValidDays->setText( QString::number( g_obLicenceManager.daysRemain() ) );
        ledCodeValidation->setFocus();
    }

    if( g_obLicenceManager.ltLicenceType() == cLicenceManager::LTYPE_REGISTERED )
    {
        deLastValidated->setEnabled( false );
        pbValidateApplication->setEnabled( false );
    }
}

cDlgSerialReg::~cDlgSerialReg()
{
}

void cDlgSerialReg::on_pbActivateKey_clicked()
{
    int nRet = g_obLicenceManager.activateLicence( ledSerialKey->text() );

    switch( nRet )
    {
        case cLicenceManager::ERR_NO_ERROR:
            g_obLicenceManager.initialize();
            QMessageBox::information( this, tr("Information"),
                                      tr("Your licence key has been verified successfully.\n"
                                         "Please send the 'Activation code' to the\n"
                                         "provider of the Belenus application and\n"
                                         "enter the given code into the 'Validation code' field.\n"
                                         "Finally click on the 'Validate' button.") );
            ledCodeActivation->setText( g_obLicenceManager.validationKey() );
            ledCodeActivation->setFocus();
            ledCodeActivation->selectAll();
            ledCodeValidation->setEnabled( true );
            pbValidateCode->setEnabled( true );
            lblValidDays->setText( QString::number( g_obLicenceManager.daysRemain() ) );
            deLastValidated->setDate( QDate::fromString( g_obLicenceManager.lastValidated(), "yyyy-MM-dd" ) );
            break;

        case cLicenceManager::ERR_KEY_FORMAT_MISMATCH:
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The format of the licence key you entered is not valid.\n"
                                     "Please check your licence key and retype it if necessary.") );
            ledCodeActivation->setText( "" );
            pbValidateCode->setEnabled( false );
            break;

        case cLicenceManager::ERR_KEY_NUMBER_INCORRECT:
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The order number of the licence key you entered is not correct.\n"
                                     "Please check your licence key and retype it if necessary.") );
            ledCodeActivation->setText( "" );
            pbValidateCode->setEnabled( false );
            break;

        case cLicenceManager::ERR_KEY_NOT_EXISTS:
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The licence key you entered is not valid.\n"
                                     "Please check your licence key and retype it if necessary..") );
            ledCodeActivation->setText( "" );
            pbValidateCode->setEnabled( false );
            break;
    }
}

void cDlgSerialReg::on_pbValidateCode_clicked()
{
    int nRet = g_obLicenceManager.validateLicence( ledCodeValidation->text() );

    switch( nRet )
    {
        case cLicenceManager::ERR_NO_ERROR:
            QMessageBox::information( this, tr("Information"),
                                      tr("Your licence key has been activated successfully.\n\n"
                                         "Please note that you have to validate your application\n"
                                         "regulary by your franchise partner.\n"
                                         "Without validation the application can run in DEMO mode.\n\n"
                                         "You have %1 days until the next validation.").arg( g_obLicenceManager.daysRemain() ) );
            deLastValidated->setEnabled( true );
            pbValidateApplication->setEnabled( true );
            break;

        case cLicenceManager::ERR_ACT_KEY_INCORRECT:
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The validation code you entered does not match with the requested.\n"
                                     "Please check your code and retype it if necessary.") );
            break;
    }
}

void cDlgSerialReg::on_pbValidateApplication_clicked()
{
    if( !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("You are not authorized to validate the application.") );
    }
    else
    {
//        if( g_poPrefs->getLicenceId() > 1 )
//        {
            g_obLicenceManager.validateApplication( deLastValidated->date().toString("yyyy-MM-dd") );
            lblValidDays->setText( QString::number( g_obLicenceManager.daysRemain() ) );
//        }
    }
}

void cDlgSerialReg::on_pbOk_clicked()
{
    QDialog::accept();
}

void cDlgSerialReg::on_pbCancel_clicked()
{
    QDialog::reject();
}








/*
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

            case cLicenceManager::ERR_ACT_KEY_INCORRECT:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The validation code you entered does not match with the requested.\n"
                                         "Please check your code and retype it if necessary.") );
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
                ledSerial->setText( "" );
                break;

            case cLicenceManager::ERR_KEY_FORMAT_MISMATCH:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The format of the licence key you entered is not valid.\n"
                                         "Please check your licence key and retype it if necessary.") );
                break;

            case cLicenceManager::ERR_KEY_NUMBER_INCORRECT:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The order number of the licence key you entered is not correct.\n"
                                         "Please check your licence key and retype it if necessary.") );
                break;

            case cLicenceManager::ERR_KEY_NOT_EXISTS:
                QMessageBox::warning( this, tr("Warning"),
                                      tr("The licence key you entered is not valid.\n"
                                         "Please check your licence key and retype it if necessary..") );
                break;
        }
    }
}
*/
