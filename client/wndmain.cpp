//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : wndmain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo ablakat kezelo allomany.
//====================================================================================

#include <QMessageBox>

#include "wndmain.h"
#include "licenceManager.h"
#include "../framework/logger/DatabaseWriter.h"

//====================================================================================

#include "db/dbpatientcard.h"
#include "db/dbledger.h"
#include "db/dbshoppingcart.h"

//====================================================================================

#include "crud/dlgpaneltypes.h"
#include "crud/dlgpatientorigin.h"
#include "crud/dlgreasontovisit.h"
#include "crud/dlgusers.h"
#include "crud/dlgguest.h"
#include "crud/dlgpatientselect.h"
#include "crud/dlgpanelstatuses.h"
#include "crud/dlgpatientcardtype.h"
#include "crud/dlgpatientcard.h"
#include "crud/dlgpanelsettings.h"
#include "crud/dlgpaneluseselect.h"
#include "crud/dlgcompany.h"
#include "crud/dlgzipregioncity.h"
#include "crud/dlgdiscount.h"
#include "crud/dlgproducttype.h"
#include "crud/dlgproductactiontype.h"
#include "crud/dlgproduct.h"
#include "crud/dlgshoppingcart.h"

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgguestedit.h"
#include "edit/dlgpatientcardedit.h"
#include "edit/dlgcassaedit.h"
#include "edit/dlgpatientcarduse.h"
#include "edit/dlglicenceedit.h"

//====================================================================================

#include "dlg/dlglogin.h"
#include "dlg/dlgpreferences.h"
#include "dlg/dlghardwaretest.h"
#include "dlg/dlglogs.h"
#include "dlg/dlginputstart.h"
#include "dlg/dlgpatientcardadd.h"
#include "dlg/dlgserialreg.h"
#include "dlg/dlgcassaaction.h"

//====================================================================================

#include "report/repledgermain.h"
#include "report/repcassalist.h"
#include "report/repcarduses.h"
#include "report/reppatientcards.h"
#include "report/reppatientcardsobs.h"

//====================================================================================

extern DatabaseWriter g_obLogDBWriter;
//extern LicenceManager g_obLicenceManager;

//====================================================================================
cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    m_bCtrlPressed                  = false;
    m_bSerialRegistration           = false;
    m_inRegistrationTimeout         = 0;

    m_bGlobalDataRequested          = false;
    m_inGlobalDataRequestTimeout    = 0;

    m_uiPatientId                   = 0;

    m_dlgProgress = new cDlgProgress( this );

    mdiPanels = new cMdiPanels( centralwidget );
    verticalLayout->addWidget(mdiPanels);

    mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );

    connect( mdiPanels, SIGNAL( activePanelChanged() ), this, SLOT( updateToolbar() ) );

    updateTitle();
    setWindowIcon( QIcon("./resources/belenus.ico") );

    //--------------------------------------------------------------------------------
    // Toolbar buttons
    //--------------------------------------------------------------------------------
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );
    action_LogOut->setIcon( QIcon("./resources/40x40_lock.png") );

    action_PatientSelect->setIcon( QIcon("./resources/40x40_patient_select.png") );
    action_PatientEmpty->setIcon( QIcon("./resources/40x40_patient_deselect.png") );
    action_EditActualPatient->setIcon( QIcon("./resources/40x40_patient_edit.png") );
    action_PatientNew->setIcon( QIcon("./resources/40x40_patient_new.png") );

    action_UseWithCard->setIcon( QIcon( "./resources/40x40_device_withcard.png" ) );
    action_UseByTime->setIcon( QIcon( "./resources/40x40_device_withtime.png" ) );

    action_DeviceStart->setIcon( QIcon( "./resources/40x40_device_start.png" ) );
    action_DeviceSkipStatus->setIcon( QIcon( "./resources/40x40_device_next.png" ) );
    action_DeviceReset->setIcon( QIcon( "./resources/40x40_stop.png" ) );

    action_DeviceSettings->setIcon( QIcon( "./resources/40x40_device_settings.png" ) );

    action_PatientCardSell->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );

    action_ProductTypes->setIcon( QIcon("./resources/40x40_producttype.png") );
    action_ProductActionType->setIcon( QIcon("./resources/40x40_productactiontype.png") );
    action_Products->setIcon( QIcon("./resources/40x40_product.png") );

    action_PayCash->setIcon( QIcon( "./resources/40x40_paycash.png" ) );
    action_SellProduct->setIcon( QIcon( "./resources/40x40_productsell.png" ) );
    action_ShoppingCart->setIcon( QIcon( "./resources/40x40_shoppingcart.png" ) );
    action_Cassa->setIcon( QIcon( "./resources/40x40_cassa.png" ) );
    action_CassaActionStorno->setIcon( QIcon( "./resources/40x40_cassa_storno.png" ) );

    //--------------------------------------------------------------------------------
    // Menu items
    //--------------------------------------------------------------------------------
    action_Patients->setIcon( QIcon("./resources/40x40_patient.png") );
    action_Guests->setIcon( QIcon("./resources/40x40_patient.png") );
    action_CardTypes->setIcon( QIcon( "./resources/40x40_patientcardtype.png" ) );
    action_Cards->setIcon( QIcon( "./resources/40x40_patientcards.png" ) );
    menuAdministrator->setIcon( QIcon("./resources/40x40_key.png") );
        action_Users->setIcon( QIcon("./resources/40x40_user.png") );
        action_Company->setIcon( QIcon("./resources/40x40_company.png") );
        action_HealthInsurance->setIcon( QIcon("./resources/40x40_health_insurance.png") );
        action_Discounts->setIcon( QIcon("./resources/40x40_discount.png") );
        action_RegionZipCity->setIcon( QIcon("./resources/40x40_address.png") );
        action_Paneltypes->setIcon( QIcon("./resources/40x40_panel.png") );
        action_PanelStatuses->setIcon( QIcon( "./resources/40x40_device_settings.png" ) );
        action_ValidateSerialKey->setIcon( QIcon( "./resources/40x40_key.png" ) );
    action_Preferences->setIcon( QIcon("./resources/40x40_settings.png") );

    menuProduct->setIcon( QIcon("./resources/40x40_product.png") );
    menuCassa->setIcon( QIcon( "./resources/40x40_cassa.png" ) );

//    menuPatient->setIcon( QIcon("./resources/40x40_patient.png") );
    menuPatientCard->setIcon( QIcon("./resources/40x40_patientcard.png") );
        action_PCSaveToDatabase->setIcon( QIcon( "./resources/40x40_patientcardadd.png" ) );
        action_PCActivate->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    menuDevice->setIcon( QIcon( "./resources/40x40_device.png" ) );

    action_Accounting->setIcon( QIcon( "./resources/40x40_book.png" ) );
    action_ReportPatients->setIcon( QIcon("./resources/40x40_patient.png") );
    action_ReportPatientcards->setIcon( QIcon( "./resources/40x40_patientcards.png" ) );
    action_PatientcardsObsolete->setIcon( QIcon( "./resources/40x40_patientcards.png" ) );
    action_ReportPatientcardUses->setIcon( QIcon( "./resources/40x40_device_withcard.png" ) );
    action_CassaHistory->setIcon( QIcon( "./resources/40x40_cassa.png" ) );

    action_EditLicenceInformation->setIcon( QIcon("./resources/40x40_key.png") );
    action_EmptyDemoDB->setIcon( QIcon("./resources/40x40_database_sync.png") );

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    action_Exit->setEnabled( false );
    action_LogOut->setEnabled( false );
    action_PatientSelect->setEnabled( false );
    action_PatientEmpty->setEnabled( false );
    action_PatientEmpty->setVisible( false );
    action_PatientNew->setEnabled( false );
    action_EditActualPatient->setEnabled( false );
    action_UseWithCard->setEnabled( false );
    action_UseByTime->setEnabled( false );
    action_DeviceStart->setEnabled( false );
    action_DeviceSkipStatus->setEnabled( false );
    action_DeviceReset->setEnabled( false );
    action_DeviceSettings->setEnabled( false );

    action_PatientCardSell->setEnabled( false );

    action_PayCash->setEnabled( false );
    action_Cassa->setEnabled( false );

    action_EmptyDemoDB->setEnabled( (g_poPrefs->getLicenceId()>1?true:false) );

    showElementsForComponents();
}
//====================================================================================
cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );

    delete m_dlgProgress;

    killTimer( m_nTimer );
}
//====================================================================================
void cWndMain::startMainTimer()
{
    m_nTimer = startTimer( 300 );
}
//====================================================================================
void cWndMain::autoSynchronizeGlobalData()
//====================================================================================
{
    m_bGlobalDataRequested = true;
}
//====================================================================================
bool cWndMain::showLogIn()
{
    cTracer obTrace( "cWndMain::showLogIn" );

    cDlgLogIn  obDlgLogIn( this );
    bool       boLogInOK = ( obDlgLogIn.exec() == QDialog::Accepted );

    if( boLogInOK )
    {
        g_obLogDBWriter.setAppUser( g_obUser.id() );
        g_obLogger(cSeverity::INFO) << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged in" << EOM;

        if( g_obUser.password() == "da39a3ee5e6b4b0d3255bfef95601890afd80709" ) //password is an empty string
        {
            QMessageBox::warning( this, tr( "Password" ),
                                  tr( "Your password is empty. Please change it to a valid password." ) );

            cDlgUserEdit  obDlgEdit( this, &g_obUser );
            obDlgEdit.setWindowTitle( g_obUser.realName() );
            obDlgEdit.exec();
        }

        updateTitle();

        if( g_poPrefs->isComponentInternetInstalled() )
        {
            checkDemoLicenceKey();
        }
        loginUser();

    } // if ( boLogInOK )

    obTrace << boLogInOK;
    return boLogInOK;
}
//====================================================================================
void cWndMain::initPanels()
{
    mdiPanels->initPanels();
}
//====================================================================================
void cWndMain::checkDemoLicenceKey()
{
/*
    if( g_obLicenceManager.getType() == LicenceManager::VALID_SERVER_ERROR )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The application has valid serial key registered but was not able to validate it with the server.\n"
                                 "Please note that without validation the application will work only for the next %1 days\n\n"
                                 "Please also note you need live internet connection for the validation process.").arg(g_obLicenceManager.getDaysRemaining()) );
    }
    else if( g_obLicenceManager.getType() == LicenceManager::VALID_CODE_2_ERROR )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The application has valid serial key registered but failed to validate the installation with the server.\n"
                                 "Please call Service to validate your installation.\n\n"
                                 "Note that without validation the application will work only for the next %1 days").arg(g_obLicenceManager.getDaysRemaining()) );
    }
    else if( g_obLicenceManager.getType() == LicenceManager::VALID_EXPIRED ||
             g_obLicenceManager.getType() == LicenceManager::VALID_CODE_2_EXPIRED )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Your licence key has expired.\n"
                                 "The application has a serial key registered but failed to validate it with the server since the last %1 days.\n\n"
                                 "Please note you need live internet connection for the validation process.").arg(LicenceManager::EXPIRE_IN_DAYS) );
    }
    else if( g_obLicenceManager.getType()==LicenceManager::NOT_VALID )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Your licence key validation has failed.\n"
                                 "Please call Service") );
    } else if ( g_obLicenceManager.getType()==LicenceManager::DEMO )
    {
        if( QMessageBox::warning( this,
                                  tr("Attention"),
                                  tr("The application has no valid serial key registered.\n"
                                     "The application will only control the hardware with DEMO serial key for %1 days.\n\n"
                                     "Do you want to enter a valid serial key and register the application?\n"
                                     "Please note you need live internet connection for the registration process.").arg(LicenceManager::EXPIRE_IN_DAYS),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
        {
            cDlgSerialReg   obDlgSerialReg( this );
            obDlgSerialReg.exec();
        }
    }
*/
}
//====================================================================================
void cWndMain::loginUser()
{
    // Felhasznalo ellenorzese
    if( g_obUser.isInGroup( cAccessGroup::ROOT ) || g_obUser.isInGroup( cAccessGroup::SYSTEM) )
    { // root, vagy rendszeradmin felhasznalo lepett be, NINCS penztar akcio
        g_obCassa.setDisabled();
        return;
    }

    // Felhasznalohoz tartozo, legutolso nem lezart kassza keresese
    if( g_obCassa.loadOpenCassa(g_obUser.id()) )
    {// Letezik nyitva hagyott, felhasznalohoz tartozo rekord

        g_obCassa.cassaContinue();
    }
    else
    {// Nincs korabban nyitva hagyott, felhasznalohoz tartozo rekord

        // Legutolso nem lezart kassza keresese
        if( g_obCassa.loadOpenCassa() )
        {// Van nyitva hagyott kassza rekord

            if( QMessageBox::question( this, tr("Question"),
                                       tr( "The latest cassa record still not closed:\n\n"
                                           "Owner: %1\n"
                                           "Balance: %2\n\n"
                                           "Do you want to continue this cassa?\n\n"
                                           "Please note: if you click NO, new cassa record will be opened "
                                           "and this cassa forced to close with reseting it's balance.").arg( g_obCassa.cassaOwnerStr() ).arg( g_obGen.convertCurrency(g_obCassa.cassaBalance(), g_poPrefs->getCurrencyShort()) ),
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
                g_obCassa.cassaContinue( g_obUser.id() );
            }
            else
            {
                g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cassa left in open.") );
                g_obCassa.cassaClose();
                g_obCassa.createNew( g_obUser.id() );
            }
        }// Volt nyitva hagyott kassza rekord
        else
        {// Nem volt nyitva hagyott kassza rekord

            // Legutolso nem kiuritett kassza rekord betoltese
            if( g_obCassa.loadLatestCassaWithCash() )
            {
                // Akarja-e a felhasznalo folytatni a kasszat
                if( QMessageBox::question( this, tr("Question"),
                                           tr( "The latest cassa record closed with balance:\n\n"
                                               "%1\n\n"
                                               "Do you want to continue this cassa?\n\n"
                                               "Please note: if you click NO, new cassa record will be opened "
                                               "and this cassa forced to close with reseting it's balance.").arg( g_obGen.convertCurrency(g_obCassa.cassaBalance(), g_poPrefs->getCurrencyShort()) ),
                                           QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                {// Kassza folytatasa
                    g_obCassa.cassaReOpen();
                }
                else
                {// Uj kassza nyitasa
                    g_obCassa.cassaReOpen();
                    g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                    g_obCassa.cassaClose();
                    g_obCassa.createNew( g_obUser.id() );
                }
            }
            else
            {// Nem volt nem kiuritett kassza

                // Felhasznalo utolso kasszajanak betoltese
                if( g_obCassa.loadLatestCassa( g_obUser.id() ) )
                {
                    // Akarja-e a felhasznalo folytatni a kasszat
                    if( QMessageBox::question( this, tr("Question"),
                                               tr( "The latest cassa record used:\n\n"
                                                   "from %1 to %2\n\n"
                                                   "Do you want to continue this cassa?").arg( g_obCassa.cassaOpenDate() ).arg( g_obCassa.cassaCloseDate() ),
                                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                    {// Kassza folytatasa
                        g_obCassa.cassaReOpen();
                    }
                    else
                    {// Uj kassza nyitasa
                        g_obCassa.createNew( g_obUser.id() );
                    }
                }
                else
                {// Uj kassza nyitasa
                    g_obCassa.createNew( g_obUser.id() );
                }
            }
        }// Nem volt nyitva hagyott kassza rekord
    }// Nincs korabban nyitva hagyott, felhasznalohoz tartozo rekord
}
//====================================================================================
void cWndMain::logoutUser()
{
    if( g_obCassa.isCassaEnabled() )
    {
        if( !g_poPrefs->getCassaAutoClose() )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to close your cassa?"),
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
                if( g_obCassa.cassaBalance() > 0 )
                {
                    if( QMessageBox::question( this, tr("Question"),
                                               tr("There are some cash left in your cassa.\n"
                                                  "Current balance: %1\n\n"
                                                  "Do you want to close the cassa with automatic "
                                                  "cash withdawal?\n\n"
                                                  "Please note: if you click NO, the cassa will "
                                                  "be closed with the actual balance.").arg( g_obGen.convertCurrency(g_obCassa.cassaBalance(), g_poPrefs->getCurrencyShort()) ),
                                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                    {
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
                    }
                }
                g_obCassa.cassaClose();
            }
        }
        else
        {
            if( g_obCassa.cassaBalance() > 0 && !g_poPrefs->getCassaAutoWithdrawal() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("There are some cash left in your cassa.\n"
                                              "Current balance: %1\n\n"
                                              "Do you want to close the cassa with automatic "
                                              "cash withdawal?\n\n"
                                              "Please note: if you click NO, the cassa will "
                                              "be closed with the actual balance.").arg( g_obGen.convertCurrency(g_obCassa.cassaBalance(), g_poPrefs->getCurrencyShort()) ),
                                           QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                {
                    g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
                }
            }
            else if( g_obCassa.cassaBalance() > 0 && g_poPrefs->getCassaAutoWithdrawal() )
            {
                g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
            }
            g_obCassa.cassaClose();
        }
    }
}
//====================================================================================
void cWndMain::keyPressEvent ( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = true;
    }

    if( m_bCtrlPressed && p_poEvent->key() == Qt::Key_Q )
    {
        close();
    }
    else if( m_bCtrlPressed && p_poEvent->key() == Qt::Key_S )
    {
        on_action_DeviceStart_triggered();
    }
    else if( m_bCtrlPressed && p_poEvent->key() == Qt::Key_F )
    {
        on_action_PayCash_triggered();
    }
    else if( (p_poEvent->key() >= Qt::Key_0 && p_poEvent->key() <= Qt::Key_9) ||
             (p_poEvent->key() >= Qt::Key_A && p_poEvent->key() <= Qt::Key_Z) ||
             (p_poEvent->key() == Qt::Key_Space) )
    {
        cDlgInputStart  obDlgInputStart( this );

        obDlgInputStart.setInitialText( p_poEvent->text() );
        if( obDlgInputStart.exec() == QDialog::Accepted )
        {
            if( obDlgInputStart.m_bPat )
            {
                processInputPatient( obDlgInputStart.getEditText().trimmed() );
            }
            else if( obDlgInputStart.m_bCard )
            {
                processInputPatientCard( obDlgInputStart.getEditText() );
            }
            else if( obDlgInputStart.m_bTime )
            {
                processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
            }
        }

        p_poEvent->ignore();
    }
    else if( p_poEvent->key() == Qt::Key_Escape && mdiPanels->isStatusCanBeReseted() )
    {
        mdiPanels->clear();
    }

    QMainWindow::keyPressEvent( p_poEvent );
}
//====================================================================================
void cWndMain::keyReleaseEvent ( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = false;
    }

    QMainWindow::keyPressEvent( p_poEvent );
}
//====================================================================================
void cWndMain::showElementsForComponents()
{
//    cTracer obTrace( "cWndMain::showElementsForComponents" );

    if( g_poPrefs->isComponentKiwiSunInstalled() )
    {
        toolBarSchedule->setEnabled( false );
        toolBarSchedule->setVisible( false );

        action_Patients->setVisible( false );
        action_Company->setVisible( false );
        action_HealthInsurance->setVisible( false );
        action_Discounts->setVisible( false );
        action_ReportPatients->setVisible( false );
    }

}
//====================================================================================
void cWndMain::updateTitle()
{
    //cTracer obTrace( "cWndMain::updateTitle" );

    QString  qsTitle = tr( "Belenus " );
    qsTitle += g_poPrefs->getVersion();

    qsTitle += " - ";
    qsTitle += QString::fromStdString( g_poHardware->getCustomCaption() );

    if( QString::fromStdString( g_poHardware->getCustomCaption() ).compare( "DEMO" ) == 0 )
    {
        action_Hardwaretest->setEnabled( false );
    }

    if( g_obUser.isLoggedIn() )
    {
        qsTitle += " - ";
        qsTitle += g_obUser.realName();
        qsTitle += " [";
        qsTitle += cAccessGroup::toStr( g_obUser.group() );
        qsTitle += "]";
    }

    if( g_obGuest.id() > 0 )
    {
        qsTitle += tr(" <=> Current patient: [");
        qsTitle += g_obGuest.name();
        qsTitle += "]";
    }
    else
    {
        qsTitle += tr(" <=> NO PATIENT SELECTED");
    }

    action_Paneltypes->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    action_PanelStatuses->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    setWindowTitle( qsTitle );
}
//====================================================================================
void cWndMain::updateToolbar()
{
    action_Exit->setEnabled( !mdiPanels->isPanelWorking() );
    action_LogOut->setEnabled( true );

    action_PatientSelect->setEnabled( !(g_obGuest.id()>0) );
    action_PatientSelect->setVisible( !(g_obGuest.id()>0) );
    action_PatientEmpty->setEnabled( g_obGuest.id()>0 );
    action_PatientEmpty->setVisible( g_obGuest.id()>0 );
    action_PatientNew->setEnabled( true );
    action_EditActualPatient->setEnabled( false/*g_obGuest.id()>0*/ );

    action_UseWithCard->setEnabled( mdiPanels->isCanBeStartedByCard() );
    action_UseByTime->setEnabled( mdiPanels->isCanBeStartedByTime() );

    action_DeviceStart->setEnabled( !mdiPanels->isPanelWorking(mdiPanels->activePanel()) &&
                                    mdiPanels->mainProcessTime() > 0 );
    action_DeviceSkipStatus->setEnabled( mdiPanels->isStatusCanBeSkipped( mdiPanels->activePanel()) );
    action_DeviceReset->setEnabled( mdiPanels->isMainProcess() );

    action_DeviceSettings->setEnabled( !mdiPanels->isPanelWorking(mdiPanels->activePanel()) );

    action_PatientCardSell->setEnabled( true );

    action_PayCash->setEnabled( mdiPanels->isHasToPay() );
    action_Cassa->setEnabled( g_obCassa.isCassaEnabled() );

    showElementsForComponents();
}
//====================================================================================
void cWndMain::timerEvent(QTimerEvent *)
{
    updateToolbar();

    if( m_bSerialRegistration )
    {
        if( g_poPrefs->getLicenceId() > 1 )
        {
            m_bSerialRegistration = false;

            g_obCassa.cassaClose();
            g_obCassa.createNew( g_obUser.id() );

            if( QMessageBox::question( this, tr("Question"),
                                       tr("Application licence key successfully registered.\n"
                                          "The application users currently attached to DEMO licence key.\n\n"
                                          "Do you want to update application users and attach them to the newly registered licence key?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                g_poDB->executeQTQuery( QString("UPDATE users SET licenceId=%1 WHERE licenceId=1").arg(g_poPrefs->getLicenceId()) );
            }
            else
            {
                g_poDB->executeQTQuery( QString("UPDATE users SET licenceId=%1 WHERE (userId=1 OR userId=2) AND licenceId=1").arg(g_poPrefs->getLicenceId()) );
            }

            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to set the additional information of the studio now?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                dlgLicenceEdit  obDlgLicenceEdit( this );

                obDlgLicenceEdit.exec();
            }
        }
        else
        {
            if( m_inRegistrationTimeout > 20 )
            {
                m_bSerialRegistration = false;
                m_inRegistrationTimeout = 0;

                QMessageBox::warning( this, tr("Warning"),
                                      tr("Registration of the licence key has been failed.\n\n"
                                         "Please check your internet connection and "
                                         "try to restart the application.\n"
                                         "Please also check whether the defined licence key is valid "
                                         "and not used by somebody else. For this information please "
                                         "contact your franchise distributor.") );
            }
            else
            {
                m_inRegistrationTimeout++;
            }
        }
    }
/*
    if( m_uiPatientId != g_obGuest.id() )
    {
        updateTitle();

        m_uiPatientId = g_obGuest.id();
    }
*/
}
//====================================================================================
void cWndMain::closeEvent( QCloseEvent *p_poEvent )
{
    bool    bIsShutdown = true;

    if( mdiPanels->isPanelWorking() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("At least one Panel is still working.\n"
                                 "Please stop them before closing the application.") );
        p_poEvent->ignore();
        bIsShutdown = false;
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Are you sure you want to close the application?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            logoutUser();
/*
            if( g_obDBMirror.isAvailable() )
            {
                if( g_poPrefs->getDBAutoArchive() ||
                    (g_obDBMirror.checkIsSynchronizationNeeded() &&
                     QMessageBox::question( this, tr("Question"),
                                            tr("Database synchronization needed.\n"
                                               "Do you want to synchronize database with server?"),
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes ) )
                {
                    hide();

                    cDlgSynchronization     obDlgSynchronization( this );

                    obDlgSynchronization.autoSynchronization();
                    obDlgSynchronization.exec();
                }
            }
*/
            p_poEvent->accept();
        }
        else
        {
            p_poEvent->ignore();
            bIsShutdown = false;
        }
    }
    if( !bIsShutdown && g_obUser.id() < 1 )
    {
        on_action_LogOut_triggered();
    }
}
//====================================================================================
void cWndMain::on_action_Preferences_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Preferences_triggered" );

    cDlgPreferences  obDlgPrefs( this );

    if( obDlgPrefs.exec() == QDialog::Accepted )
    {
        mdiPanels->hide();
        mdiPanels->placeSubWindows();
        mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );
        mdiPanels->show();
    }
}
//====================================================================================
void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    cDlgUsers  obDlgUsers( this );

    obDlgUsers.exec();

    updateTitle();  //needed in case the login or real name of current user changed
}
//====================================================================================
void cWndMain::on_action_Logs_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Logs_triggered" );

    cDlgLogs  obDlgLogs( this );

    obDlgLogs.exec();
}
//====================================================================================
void cWndMain::on_action_Hardwaretest_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Hardwaretest_triggered" );

    if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        cDlgHardwareTest  obDlgHardwareTest( this );

        obDlgHardwareTest.exec();
    }
    else
    {
        QMessageBox::warning( this, tr( "Information" ),
                              tr( "This area is restricted for system administrators only!" ) );
    }
}
//====================================================================================
void cWndMain::on_action_LogOut_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Log_Out_triggered" );

    logoutUser();

    g_obLogger(cSeverity::INFO) << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged out" << EOM;

    g_obUser.logOut();
    g_obLogDBWriter.setAppUser( 0 );
    updateTitle();

    if( !showLogIn() ) close();
}
//====================================================================================
void cWndMain::on_action_Paneltypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Paneltypes_triggered" );

    cDlgPanelTypes  obDlgPanelTypes( this );

    obDlgPanelTypes.exec();
}
//====================================================================================
void cWndMain::on_action_Patientorigin_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_Patientorigin_triggered" );

    cDlgPatientOrigin  obDlgPatientOrigin( this );

    obDlgPatientOrigin.exec();
*/
}
//====================================================================================
void cWndMain::on_action_ReasonToVisit_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ReasonToVisit_triggered" );

    cDlgReasonToVisit  obDlgReasonToVisit( this );

    obDlgReasonToVisit.exec();
}
//====================================================================================
void cWndMain::on_action_Patients_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_Patiens_triggered" );

    cDlgPatient  obDlgPatient( this );

    obDlgPatient.exec();
*/
}
//====================================================================================
void cWndMain::on_action_Guests_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Guests_triggered" );

    cDlgGuest  obDlgGuest( this );

    obDlgGuest.exec();
}
//====================================================================================
void cWndMain::on_action_PatientNew_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatienNew_triggered" );

    cDBGuest *poGuest = new cDBGuest;
    poGuest->createNew();

    cDlgGuestEdit  obDlgEdit( this, poGuest );
    obDlgEdit.setWindowTitle( tr( "New Patient" ) );
    obDlgEdit.exec();

    if( poGuest->id() > 0 )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to select the created patient as actual?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_obGuest.load( poGuest->id() );
        }
    }

    delete poGuest;
}
//====================================================================================
void cWndMain::on_action_DeviceStart_triggered()
{
    if( !action_DeviceStart->isEnabled() )
        return;

    if( mdiPanels->isHasToPay() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The device usage has to be payed.\n"
                                 "Please process the payment first.") );
        return;
    }

    mdiPanels->start();

    on_action_PatientEmpty_triggered();
}
//====================================================================================
void cWndMain::on_action_DeviceReset_triggered()
{
    mdiPanels->reset();
}
//====================================================================================
void cWndMain::on_action_PatientSelect_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientSelect_triggered" );

    cDlgPatientSelect  obDlgPatientSelect( this );

    obDlgPatientSelect.exec();
    updateTitle();
}
//====================================================================================
void cWndMain::on_action_PatientEmpty_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientEmpty_triggered" );

    g_obGuest.createNew();
    updateTitle();
}
//====================================================================================
void cWndMain::on_action_PanelStatuses_triggered()
{
    cDlgPanelStatuses   obDlgPanelStatuses( this );

    obDlgPanelStatuses.exec();
    if( obDlgPanelStatuses.isStatusChanged() )
    {
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );
    }

}
//====================================================================================
void cWndMain::on_action_UseWithCard_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init( g_poPrefs->getBarcodePrefix() );

    QString      qsBarcode = "";
    QSqlQuery   *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM patientCards WHERE patientId=%1" ).arg(g_obGuest.id()) );
    if( poQuery->first() )
    {
        qsBarcode = poQuery->value(0).toString();

        if( QMessageBox::question( this, tr("Question"),
                                   tr("A patientcard with barcode [%1]\n"
                                      "attached to the actual patient.\n\n"
                                      "Do you want to use this patientcard?").arg(qsBarcode),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            processInputPatientCard( qsBarcode );
        }
        else
        {
            if( obDlgInputStart.exec() == QDialog::Accepted )
                processInputPatientCard( obDlgInputStart.getEditText() );
        }
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("There is no patientcard attached to the actual patient.\n"
                                      "Do you want to sell a patientcard for the actual patient?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            on_action_PatientCardSell_triggered();
        }
        else
        {
            if( obDlgInputStart.exec() == QDialog::Accepted )
                processInputPatientCard( obDlgInputStart.getEditText() );
        }
    }
}
//====================================================================================
void cWndMain::on_action_UseByTime_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bTime = true;
    obDlgInputStart.init();
    if( obDlgInputStart.exec() == QDialog::Accepted )
        processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
}
//====================================================================================
void cWndMain::on_action_Cards_triggered()
{
    cDlgPatientCard obDlgPatientCard( this );

    obDlgPatientCard.exec();
}
//====================================================================================
void cWndMain::on_action_CardTypes_triggered()
{
    cDlgPatientCardType obDlgPatientCardType( this );

    obDlgPatientCardType.exec();
}
//====================================================================================
void cWndMain::on_action_ProductTypes_triggered()
{
    cDlgProductType obDlgProductType( this );

    obDlgProductType.exec();
}
//====================================================================================
void cWndMain::on_action_ProductActionType_triggered()
{
    cDlgProductActionType obDlgProductActionType( this );

    obDlgProductActionType.exec();
}
//====================================================================================
void cWndMain::on_action_Products_triggered()
{
    cDlgProduct obDlgProduct( this );

    obDlgProduct.exec();
}
//====================================================================================
void cWndMain::on_action_SellProduct_triggered()
{

}
//====================================================================================
void cWndMain::on_action_ShoppingCart_triggered()
{
    cDlgShoppingCart    obDlgShoppingCart( this );

    obDlgShoppingCart.exec();
}
//====================================================================================
void cWndMain::on_action_CassaActionStorno_triggered()
{

}
//====================================================================================
void cWndMain::on_action_PCSaveToDatabase_triggered()
{
    cDlgPatientCardAdd  obDlgPatientCardAdd( this );

    obDlgPatientCardAdd.exec();
}
//====================================================================================
void cWndMain::on_action_Cassa_triggered()
{
    cDlgCassaEdit   obDlgCassaEdit( this );

    obDlgCassaEdit.exec();
}
//====================================================================================
void cWndMain::on_action_Accounting_triggered()
{
    setCursor( Qt::WaitCursor);
    cDlgRepLedgerMain  obDlgLedgerMain( this );
    setCursor( Qt::ArrowCursor);

    obDlgLedgerMain.exec();
}
//====================================================================================
void cWndMain::on_action_DeviceSkipStatus_triggered()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Do you want to jump to the next status of the device?"),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
    {
        mdiPanels->next();
    }
}
//====================================================================================
void cWndMain::on_action_ValidateSerialKey_triggered()
{
    cDlgSerialReg   obDlgSerialReg( this );

    if( obDlgSerialReg.exec() == QDialog::Accepted )
    {
        m_inRegistrationTimeout = 0;
        m_bSerialRegistration = true;
    }
}
//====================================================================================
void cWndMain::on_action_PatientCardSell_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();
    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        cDBPatientCard  obDBPatientCard;
        try
        {
            obDBPatientCard.load( obDlgInputStart.getEditText() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
            else
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This barcode has not found in the database.\n"
                                              "Do you want to register it for a new patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCard.createNew();
                    obDBPatientCard.setLicenceId( g_poPrefs->getLicenceId() );
                    obDBPatientCard.setBarcode( obDlgInputStart.getEditText() );
                    obDBPatientCard.save();
                }
            }
        }
        if( obDBPatientCard.active() && obDBPatientCard.patientId() > 0 )
        {
            if( obDBPatientCard.units() < 1 || obDBPatientCard.timeLeft() < 1 )
            {
                QString     qsTemp = "";

                if( obDBPatientCard.timeLeft() < 1 )
                {
                    qsTemp = tr("\nDue to there is no time left, the patientcard will be reseted and deactivated.");
                }
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has the following settings:\n\n"
                                              "Available units: %1\n"
                                              "Available time: %2 (hh:mm:ss)\n\n"
                                              "Do you want to refill the patientcard now?%3").arg(obDBPatientCard.units()).arg(obDBPatientCard.timeLeftStr()).arg(qsTemp),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
                else
                {
                    if( obDBPatientCard.timeLeft() < 1 )
                    {
                        obDBPatientCard.setActive( false );
                        obDBPatientCard.setPatientCardTypeId( 0 );
                    }
                    cDlgPatientCardEdit obDlgPatientCardEdit( this, &obDBPatientCard );

                    if( obDBPatientCard.timeLeft() < 1 )
                    {
                        obDlgPatientCardEdit.activatePatientCard();
                    }
                    else
                    {
                        obDlgPatientCardEdit.refillPatientCard();
                    }
                    if( obDlgPatientCardEdit.exec() != QDialog::Accepted )
                    {
                        return;
                    }
                }
            }
            else
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("This patientcard already sold.\n"
                                         "Please select another inactive patientcard.") );
            }
        }
        else
        {
            cDlgPatientCardEdit obDlgPatientCardEdit( this, &obDBPatientCard );
            obDlgPatientCardEdit.setPatientCardOwner( g_obGuest.id() );
            obDlgPatientCardEdit.activatePatientCard();
            obDlgPatientCardEdit.exec();
        }
    }
}
//====================================================================================
void cWndMain::processInputPatient( QString p_stPatientName )
{
// 'SOLARIUM GUEST'
/*
    cDBPatient      obDBPatient;
    unsigned int    uiPatientCount = obDBPatient.getPatientCount(p_stPatientName.trimmed());

    if( uiPatientCount > 1 )
    {
        cDlgPatientSelect  obDlgPatientSelect( this );
        obDlgPatientSelect.setSearchPatientName( p_stPatientName.trimmed() );
        obDlgPatientSelect.exec();
    }
    else if( uiPatientCount == 1 )
    {
        g_obGuest.load( obDBPatient.id() );
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("There is no patient in the database with name like\n\n"
                                      "\'%1\'\n\n"
                                      "Do you want to create a new patient record with this name?").arg(p_stPatientName.trimmed()),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            cDBPatient  obDBPatient;

            obDBPatient.createNew();
            obDBPatient.setName( p_stPatientName );

            cDlgPatientEdit  obDlgEdit( this, &obDBPatient );
            obDlgEdit.setWindowTitle( obDBPatient.name() );
            obDlgEdit.exec();
        }
    }
*/
}
//====================================================================================
void cWndMain::processInputPatientCard( QString p_stBarcode )
{
    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( p_stBarcode );

        if( !mdiPanels->isCanBeStartedByCard() /*&&
            g_obGuest.id() > 0 */ )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("This device already prepared with a patientcard.\n"
                                     "To start the device with other conditions, please\n"
                                     "reset the device first with pushing the ESC button.") );
            return;
        }

        if( obDBPatientCard.active() )
        {
// 'SOLARIUM GUEST'
/*
            if( g_obGuest.id() == 0 )
            {
                cDBPatient  obDBPatient;

                obDBPatient.load( obDBPatientCard.patientId() );
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has been assigned to the following patient.\n\n"
                                              "%1\n\n"
                                              "Do you want to select this patient as actual?").arg( obDBPatient.name() ),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    g_obGuest.load( obDBPatientCard.patientId() );
                }
            }
            if( obDBPatientCard.patientId() != g_obGuest.id() && g_obGuest.id() > 0 && obDBPatientCard.patientId() > 0 )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has been assigned to a different patient.\n"
                                              "Are you sure you want to use this patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
            }
            else if( g_obGuest.id() > 0 && obDBPatientCard.patientId() == 0 )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("There is no patient assigned to this patientcard.\n"
                                              "Do you want to assign this patientcard to the actual patient?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCard.setPatientId( g_obGuest.id() );
                    obDBPatientCard.save();
                }
            }
*/
//            if( g_obGuest.id() > 0 )
//            {
                if( obDBPatientCard.units() < 1 || obDBPatientCard.timeLeft() < 1 )
                {
                    QString     qsTemp = "";

                    if( obDBPatientCard.timeLeft() < 1 )
                    {
                        qsTemp = tr("\nDue to there is no time left, the patientcard will be reseted and deactivated.");
                    }
                    if( QMessageBox::question( this, tr("Question"),
                                               tr("This patientcard can not be used with these settings:\n\n"
                                                  "Available units: %1\n"
                                                  "Available time: %2 (hh:mm:ss)\n\n"
                                                  "Do you want to refill the patientcard now?%3").arg(obDBPatientCard.units()).arg(obDBPatientCard.timeLeftStr()).arg(qsTemp),
                                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                    {
                        return;
                    }
                    else
                    {
                        if( obDBPatientCard.timeLeft() < 1 )
                        {
                            obDBPatientCard.setActive( false );
                            obDBPatientCard.setPatientCardTypeId( 0 );
                        }
                        cDlgPatientCardEdit obDlgPatientCardEdit( this, &obDBPatientCard );

                        if( obDBPatientCard.timeLeft() < 1 )
                        {
                            obDlgPatientCardEdit.activatePatientCard();
                        }
                        else
                        {
                            obDlgPatientCardEdit.refillPatientCard();
                        }
                        if( obDlgPatientCardEdit.exec() != QDialog::Accepted )
                        {
                            return;
                        }
                    }
                }
                cDlgPatientCardUse  obDlgPatientCardUse( this, &obDBPatientCard, mdiPanels->activePanel()+1 );

                if( obDlgPatientCardUse.exec() == QDialog::Accepted )
                {
                    QTime           tLength;
                    QTime           tNewLength;
                    int             inNewLength;
                    int             inUnits;
                    QString         qsLength;

                    obDlgPatientCardUse.getUseUnitsTime( &inUnits, &qsLength );

                    tLength  = QTime::fromString(qsLength,"mm:ss");
                    inNewLength = tLength.minute()*60 + tLength.second();
                    mdiPanels->setMainProcessTime( obDBPatientCard.id(), inUnits, inNewLength );
                }
//            }
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not been activated yet.\n"
                                          "Do you want to activate it now?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                cDlgPatientCardEdit     obDlgPatientCardEdit( this, &obDBPatientCard );

                obDlgPatientCardEdit.activatePatientCard();
// 'SOLARIUM GUEST'
/*
                if( g_obGuest.id() > 0 )
                {
                    if( QMessageBox::question( this, tr("Question"),
                                               tr("Do you want to assign this patientcard to the actual patient?"),
                                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                    {
                        obDlgPatientCardEdit.setPatientCardOwner( g_obGuest.id() );
                    }
                }
*/
                obDlgPatientCardEdit.exec();
            }
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not found in the database.\n"
                                          "Do you want to register it for a new patientcard?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                obDBPatientCard.createNew();
                obDBPatientCard.setLicenceId( g_poPrefs->getLicenceId() );
                obDBPatientCard.setBarcode( p_stBarcode );
                obDBPatientCard.save();
            }
        }
    }
}
//====================================================================================
void cWndMain::processInputTimePeriod( int p_inSecond )
{
    if( !mdiPanels->isCanBeStartedByTime() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("This device already prepared with a time period.\n"
                                 "To start the device with other conditions, please\n"
                                 "reset the device first with pushing the ESC button.") );
        return;
    }
    if( mdiPanels->isHasToPay() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The device usage has to be payed.\n"
                                 "Please process the payment first.") );
        return;
    }

    int inPrice;
    int inCount;

    mdiPanels->isTimeIntervallValid( p_inSecond, &inPrice, &inCount );
    if( inCount > 0 )
    {
        if( inCount > 1 )
        {
            cDlgPanelUseSelect  obDlgPanelUseSelect( this, mdiPanels->activePanel()+1, p_inSecond );

            if( obDlgPanelUseSelect.exec() == QDialog::Accepted )
            {
                inPrice = obDlgPanelUseSelect.getPanelUsePrice();
            }
            else
            {
                return;
            }
        }

        mdiPanels->setMainProcessTime( p_inSecond*60, inPrice );
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("This time period did not saved in the database\n"
                                 "for the actually selected device.") );
    }
}
//====================================================================================
void cWndMain::on_action_EditActualPatient_triggered()
{
/*
    cDlgPatientEdit  obDlgEdit( this, &g_obGuest );
    obDlgEdit.setWindowTitle( g_obGuest.name() );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        cDBPostponed    obDBPostponed;
        obDBPostponed.removePatient( g_obGuest.id() );
    }
*/
}
//====================================================================================
void cWndMain::on_action_DeviceSettings_triggered()
{
    cDlgPanelSettings   obDlgEdit( this, mdiPanels->activePanel()+1 );

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        mdiPanels->reload();
    }
}
//====================================================================================
void cWndMain::on_action_PayCash_triggered()
{
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    int                 inPriceTotal;
    int                 inPriceNet;
    int                 inPriceDiscount;
    unsigned int        uiPatientId;

    mdiPanels->getPanelCashData( &uiPatientId, &inPriceNet, &inPriceDiscount );

    inPriceTotal = inPriceNet + ((inPriceNet / 100) * g_poPrefs->getDeviceUseVAT()) - inPriceDiscount;

    cDBShoppingCart obDBShoppingCart;

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( 0 );
    obDBShoppingCart.setPatientCardId( 0 );
    obDBShoppingCart.setPanelId( mdiPanels->activePanelId() );
    obDBShoppingCart.setItemName( tr("Using panel") );
    obDBShoppingCart.setItemCount( 1 );
    obDBShoppingCart.setItemNetPrice( inPriceNet );
    obDBShoppingCart.setItemVAT( g_poPrefs->getDeviceUseVAT() );
    obDBShoppingCart.setItemDiscount( inPriceDiscount );
    obDBShoppingCart.setItemSumPrice( inPriceTotal );

    cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

    obDlgCassaAction.setPayWithCash();
    if( obDlgCassaAction.exec() == QDialog::Accepted )
    {
        int     inPayType = 0;
        QString qsComment = tr("Using device: %1 - ").arg( mdiPanels->getActivePanelCaption() );

        obDlgCassaAction.cassaResult( &inPayType, &qsComment );
        if( inPayType == cDlgCassaAction::PAY_CASH )
        {
            g_obCassa.cassaAddMoneyAction( inPriceTotal, qsComment );
        }
        mdiPanels->setPaymentMethod( inPayType );

        cDBLedger   obDBLedger;

        obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
        obDBLedger.setLedgerTypeId( 1 );
        obDBLedger.setLedgerDeviceId( 0 );
        obDBLedger.setPaymentMethod( inPayType );
        obDBLedger.setUserId( g_obUser.id() );
        obDBLedger.setProductId( 0 );
        obDBLedger.setPatientCardTypeId( 0 );
        obDBLedger.setPatientCardId( 0 );
        obDBLedger.setPanelId( mdiPanels->activePanel()+1 );
        obDBLedger.setName( mdiPanels->getActivePanelCaption() );
        obDBLedger.setNetPrice( inPriceNet );
        obDBLedger.setDiscount( inPriceDiscount );
        obDBLedger.setVatpercent( g_poPrefs->getDeviceUseVAT() );
        obDBLedger.setComment( qsComment );
        obDBLedger.setActive( true );
        obDBLedger.save();

        mdiPanels->cashPayed( obDBLedger.id() );
    }
}
//====================================================================================
void cWndMain::on_action_IllnessGroup_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_IllnessGroup_triggered" );

    cDlgIllnessGroup  obDlgIllnessGroup( this );

    obDlgIllnessGroup.exec();
*/
}
//====================================================================================
void cWndMain::on_action_Company_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_Company_triggered" );

    cDlgCompany  obDlgCompany( this );

    obDlgCompany.exec();
*/
}
//====================================================================================
void cWndMain::on_action_Doctor_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_Doctor_triggered" );

    cDlgDoctor  obDlgDoctor( this );

    obDlgDoctor.exec();
*/
}
//====================================================================================
void cWndMain::on_action_HealthInsurance_triggered()
{
/*
    cTracer obTrace( "cWndMain::on_action_HealthInsurance_triggered" );

    cDlgHealthInsurance  obDlgHealthInsurance( this );

    obDlgHealthInsurance.exec();
*/
}
//====================================================================================
void cWndMain::on_action_RegionZipCity_triggered()
{
    m_dlgProgress->showProgress();
    m_dlgProgress->showProgress();

    cDlgZipRegionCity   obDlgZipRegionCity( this );

    m_dlgProgress->hideProgress();

    obDlgZipRegionCity.exec();
}
//====================================================================================
void cWndMain::on_action_ReportPatients_triggered()
//====================================================================================
{
/*
    setCursor( Qt::WaitCursor);
    cDlgReportPatients  obDlgReportPatients( this );
    setCursor( Qt::ArrowCursor);

    obDlgReportPatients.exec();
*/
}
//====================================================================================
void cWndMain::on_action_CassaHistory_triggered()
//====================================================================================
{
    setCursor( Qt::WaitCursor);
    cDlgReportCassaList  obDlgReportCassaList( this );
    setCursor( Qt::ArrowCursor);

    obDlgReportCassaList.exec();
}
//====================================================================================
void cWndMain::on_action_ReportPatientcardUses_triggered()
//====================================================================================
{
    setCursor( Qt::WaitCursor);
    cDlgReportCardUses  obDlgReportCardUses( this );
    setCursor( Qt::ArrowCursor);

    obDlgReportCardUses.exec();
}
//====================================================================================
void cWndMain::on_action_EditLicenceInformation_triggered()
//====================================================================================
{
    dlgLicenceEdit  obDlgLicenceEdit( this );

    obDlgLicenceEdit.exec();
}
//====================================================================================
void cWndMain::on_action_ReportPatientcards_triggered()
//====================================================================================
{
    setCursor( Qt::WaitCursor);
    cDlgReportPatientCard  obDlgReportPatientCard( this );
    setCursor( Qt::ArrowCursor);

    obDlgReportPatientCard.exec();
}
//====================================================================================
void cWndMain::on_action_Discounts_triggered()
//====================================================================================
{
/*
    setCursor( Qt::WaitCursor);
    cDlgDiscount  obDlgDiscount( this );
    setCursor( Qt::ArrowCursor);

    obDlgDiscount.exec();
*/
}
//====================================================================================
void cWndMain::on_action_PatientcardsObsolete_triggered()
//====================================================================================
{
    setCursor( Qt::WaitCursor);
    cDlgReportPatientCardObs  obDlgReportPatientCardObs( this );
    setCursor( Qt::ArrowCursor);

    obDlgReportPatientCardObs.exec();
}
//====================================================================================
void cWndMain::on_action_SynchronizeDatabase_triggered()
//====================================================================================
{
/*
    if( g_obDBMirror.isAvailable() )
    {
        cDlgSynchronization obDlgSynchronization( this );

        obDlgSynchronization.exec();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Connection to Belenus server is not available."));
    }
*/
}
//====================================================================================
void cWndMain::on_action_AcquireGlobalData_triggered()
//====================================================================================
{
/*
    if( g_obDBMirror.isAvailable() )
    {
        g_obDBMirror.requestGlobalDataTimestamp();
        m_bGlobalDataRequested = true;

        cDlgDBGlobals   obDlgDBGlobals( this );

        obDlgDBGlobals.exec();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Connection to Belenus server is not available."));
    }
*/
}
//====================================================================================
void cWndMain::on_action_EstablishConnection_triggered()
//====================================================================================
{
}
//====================================================================================
void cWndMain::on_action_EmptyDemoDB_triggered()
//====================================================================================
{
    m_dlgProgress->showProgress();

    g_poDB->executeQTQuery( QString( "DELETE FROM ledger WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM ledgerDevice WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM cassaDenominations WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM cassaHistory WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM cassa WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM address WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM zipRegionCity WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM discounts WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM products WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM productTypes WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM patientCardHistories WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM connectPatientWithCard WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM patientCards WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM patientCardTypes WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM patients WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM doctors WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM companies WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM healthInsurances WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM illnessGroups WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM reasonToVisit WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM patientOrigin WHERE licenceId=1" ) );
    g_poDB->executeQTQuery( QString( "DELETE FROM users WHERE licenceId=1 and userId>2" ) );

    m_dlgProgress->hideProgress();

    QMessageBox::information( this, tr("Information"),
                              tr("Deleting data attached to DEMO licence key has been finished."));
}
//====================================================================================
