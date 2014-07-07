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
#include <QCryptographicHash>
#include <QProcess>

#include "wndmain.h"
#include "licenceManager.h"
#include "../framework/logger/DatabaseWriter.h"
#include "cdlgtest.h"
#include "belenus.h"

//====================================================================================

#include "db/dbpatientcard.h"
#include "db/dbledger.h"
#include "db/dbshoppingcart.h"
#include "db/dbpatientcardunits.h"
#include "db/dbdiscount.h"

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
#include "crud/dlgproductsell.h"
#include "crud/dlgstorno.h"
#include "crud/dlgpaymentmethod.h"
#include "crud/dlgpanelgroups.h"
#include "crud/dlgpanels.h"
#include "crud/dlgskintypes.h"

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgguestedit.h"
#include "edit/dlgpatientcardedit.h"
#include "edit/dlgpatientcardsell.h"
#include "edit/dlgpatientcardrefill.h"
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
#include "dlg/dlgpaneluse.h"
#include "dlg/dlgpatientcardassign.h"
#include "dlg/dlgmanagedatabase.h"
#include "dlg/dlgexportimport.h"

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
cWndMain::cWndMain( QWidget *parent ) : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    m_qsStatusText                  = "";
    m_bCtrlPressed                  = false;
    m_bSerialRegistration           = false;
    m_inRegistrationTimeout         = 0;
    m_bGlobalDataRequested          = false;
    m_inGlobalDataRequestTimeout    = 0;
    m_uiPatientId                   = 0;
    m_nEnterAction                  = 0;
    m_inPanelStartMinute            = 0;
    m_qsPanelStartBarcode           = "";
    m_inCommunicationCounter        = 0;
    m_bActionProcessing             = false;

    pbLogin->setIcon( QIcon("./resources/40x40_ok.png") );

    frmLogin->setVisible( false );
    frmLogin->setEnabled( false );

    m_dlgProgress = new cDlgProgress( this );

    mdiPanels = new cMdiPanels( centralwidget );
    verticalLayout->addWidget(mdiPanels);
    mdiPanels->setEnabled( false );

    mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );

    connect( mdiPanels, SIGNAL( activePanelChanged() ),                  this, SLOT( updateToolbar() ) );
    connect( mdiPanels, SIGNAL( signalOpenShoppingCart( uint ) ),        this, SLOT( slotOpenShoppingCart( uint )) );
    connect( mdiPanels, SIGNAL( signalPaymentActivated() ),              this, SLOT( on_action_PayCash_triggered() ) );
    connect( mdiPanels, SIGNAL( signalOpenScheduleTable(uint) ),         this, SLOT( slotOpenScheduleTable(uint)) );
    connect( mdiPanels, SIGNAL( signalStatusChanged(uint,uint,QString)), this, SLOT( slotStatusChanged(uint,uint,QString)) );
    connect( mdiPanels, SIGNAL( signalSetCounterText(uint,QString) ),    this, SLOT( slotSetCounterText(uint,QString) ) );
    connect( mdiPanels, SIGNAL( signalSetWaitTime(uint,uint) ),          this, SLOT( slotSetWaitTime(uint,uint) ) );
    connect( mdiPanels, SIGNAL( signalSetInfoText(uint,QString) ),       this, SLOT( slotSetInfoText(uint,QString) ) );

    updateTitle();
    setWindowIcon( QIcon("./resources/belenus.ico") );

    //--------------------------------------------------------------------------------
    // Toolbar buttons
    //--------------------------------------------------------------------------------
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );
    action_LogOut->setIcon( QIcon("./resources/40x40_lock.png") );

    menu_ExportImport->setIcon( QIcon("./resources/40x40_database_sync.png") );
    action_Export->setIcon( QIcon("./resources/35x35_export.png") );
    action_Import->setIcon( QIcon("./resources/35x35_import.png") );

    action_PatientSelect->setIcon( QIcon("./resources/40x40_patient_select.png") );
    action_PatientEmpty->setIcon( QIcon("./resources/40x40_patient_deselect.png") );
    action_EditActualPatient->setIcon( QIcon("./resources/40x40_patient_edit.png") );
    action_PatientNew->setIcon( QIcon("./resources/40x40_patient_new.png") );

    action_UseDevice->setIcon( QIcon( "./resources/40x40_device.png" ) );
    action_UseDeviceLater->setIcon( QIcon( "./resources/40x40_device_later.png" ) );

    action_DeviceClear->setIcon( QIcon( "./resources/40x40_device_clear.png" ) );

    action_DeviceStart->setIcon( QIcon( "./resources/40x40_device_start.png" ) );
    action_DeviceSkipStatus->setIcon( QIcon( "./resources/40x40_device_next.png" ) );
    action_DeviceReset->setIcon( QIcon( "./resources/40x40_stop.png" ) );

    action_DeviceSettings->setIcon( QIcon( "./resources/40x40_device_settings.png" ) );

    action_PatientCardSell->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    action_PatientCardAssign->setIcon( QIcon("./resources/40x40_patientcard_assign.png") );

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
    action_About->setIcon( QIcon("./resources/40x40_information.png") );

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
        action_Panelgroups->setIcon( QIcon("./resources/40x40_panel.png") );
        action_Paneltypes->setIcon( QIcon("./resources/40x40_panel.png") );
        action_PanelStatuses->setIcon( QIcon( "./resources/40x40_device_settings.png" ) );
        action_ValidateSerialKey->setIcon( QIcon( "./resources/40x40_key.png" ) );
        action_ManageDatabase->setIcon( QIcon( "./resources/40x40_connect_db.png" ) );
    action_Preferences->setIcon( QIcon("./resources/40x40_settings.png") );

    menuProduct->setIcon( QIcon("./resources/40x40_product.png") );
    menuCassa->setIcon( QIcon( "./resources/40x40_cassa.png" ) );

//    menuPatient->setIcon( QIcon("./resources/40x40_patient.png") );
    menuPatientCard->setIcon( QIcon("./resources/40x40_patientcard.png") );
        action_PCSaveToDatabase->setIcon( QIcon( "./resources/40x40_patientcardadd.png" ) );
        action_PCActivate->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    menuDevice->setIcon( QIcon( "./resources/40x40_device.png" ) );

    action_ReportViewer->setIcon( QIcon( "./resources/40x40_book_ledger.png" ) );
    action_Accounting->setIcon( QIcon( "./resources/40x40_book.png" ) );
    action_ReportPatients->setIcon( QIcon("./resources/40x40_patient.png") );
    action_ReportPatientcards->setIcon( QIcon( "./resources/40x40_patientcards.png" ) );
    action_PatientcardsObsolete->setIcon( QIcon( "./resources/40x40_patientcards.png" ) );
    action_ReportPatientcardUses->setIcon( QIcon( "./resources/40x40_device_withcard.png" ) );
    action_CassaHistory->setIcon( QIcon( "./resources/40x40_cassa.png" ) );

    action_EditLicenceInformation->setIcon( QIcon("./resources/40x40_key.png") );
    action_EmptyDemoDB->setIcon( QIcon("./resources/40x40_database_sync.png") );
    action_PaymentMethods->setIcon( QIcon("./resources/40x40_paymentmethod.png") );

    action_ManageDevicePanels->setIcon( QIcon( "./resources/40x40_device_settings.png" ) );

    action_ManageSkinTypes->setIcon( QIcon( "./resources/40x40_patientorigin.png" ) );

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    action_LogOut->setEnabled( false );
    action_Exit->setEnabled( false );

    menu_ExportImport->setEnabled( false );
    action_Export->setEnabled( false );
    action_Import->setEnabled( false );

    action_PatientSelect->setEnabled( false );
    action_PatientEmpty->setEnabled( false );
    action_PatientEmpty->setVisible( false );
    action_PatientNew->setEnabled( false );
    action_EditActualPatient->setEnabled( false );
    action_UseDevice->setEnabled( false );
    action_UseDeviceLater->setEnabled( false );
    action_DeviceClear->setEnabled( false );
    action_DeviceStart->setEnabled( false );
    action_DeviceSkipStatus->setEnabled( false );
    action_DeviceReset->setEnabled( false );
    action_DeviceSettings->setEnabled( false );

    action_PatientCardSell->setEnabled( false );
    action_PatientCardAssign->setEnabled( false );

    action_PayCash->setEnabled( false );
    action_Cassa->setEnabled( false );

    action_Accounting->setEnabled( false );
    action_Accounting->setVisible( false );
    action_ReportPatients->setEnabled( false );
    action_ReportPatients->setVisible( false );
    action_ReportPatientcards->setEnabled( false );
    action_ReportPatientcards->setVisible( false );
    action_PatientcardsObsolete->setEnabled( false );
    action_PatientcardsObsolete->setVisible( false );
    action_ReportPatientcardUses->setEnabled( false );
    action_ReportPatientcardUses->setVisible( false );
    action_CassaHistory->setEnabled( false );
    action_CassaHistory->setVisible( false );

    action_EmptyDemoDB->setEnabled( (g_poPrefs->getLicenceId()>1?true:false) );

    action_ManageDatabase->setVisible( false );
    action_ManageDatabase->setEnabled( false );

    action_ManageDevicePanels->setVisible( false );
    action_ManageDevicePanels->setEnabled( false );

    showElementsForComponents();

    m_dlgSecondaryWindow = new cDlgSecondaryWindow( this );

    m_dlgSecondaryWindow->initPanels();

    if( g_poPrefs->isSecondaryWindowVisible() )
    {
        m_dlgSecondaryWindow->move( g_poPrefs->secondaryWindowPosition() );
        m_dlgSecondaryWindow->resize( g_poPrefs->secondaryWindowSize() );
        m_dlgSecondaryWindow->show();
        this->setFocus();
    }

    m_lblStatusRight.setAlignment( Qt::AlignRight );
    m_lblStatusRight.setStyleSheet( "QLabel {font: bold; font-size:14px;}" );

    statusbar->addPermanentWidget( &m_lblStatusLeft, 3 );
    statusbar->addPermanentWidget( &m_lblStatusRight, 1 );
/*
    g_poGibbig = new cGibbig();

    g_poGibbig->setHost( g_poPrefs->getServerAddress() );
    g_poGibbig->setUserName( g_poPrefs->getGibbigName() );
    g_poGibbig->setPassword( g_poPrefs->getGibbigPassword() );
    g_poGibbig->setTimeout( 10000 );

    g_poGibbig->gibbigAuthenticate();
*/
}
//====================================================================================
cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );

    delete m_dlgProgress;
    delete m_dlgSecondaryWindow;

    killTimer( m_nTimer );
}
//====================================================================================
void cWndMain::startMainTimer()
{
    cTracer obTrace( "cWndMain::startMainTimer" );

    mdiPanels->refreshDisplay();
    m_nTimer = startTimer( 250 );
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

    cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
    m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
    cmbName->setModel( m_poModel );

    int inIdx = cmbName->findText( g_poPrefs->getLastUser() );
    if( inIdx != -1 ) cmbName->setCurrentIndex( inIdx );

    ledPassword->setText( "" );

    enableElementsByLogin( false );

    m_dlgProgress->hideProgress();

    ledPassword->setFocus();

    return true;
}
//====================================================================================
void cWndMain::on_pbLogin_clicked()
{
    cTracer obTrace( "cWndMain::on_pbLogin_clicked" );

    try
    {
        string  stName = cmbName->currentText().toStdString();
        stName = stName.substr( 0, stName.find( '(' ) - 1 );
        g_obUser.load( QString::fromStdString(stName) );
        QByteArray  obPwdHash = QCryptographicHash::hash( ledPassword->text().toAscii(), QCryptographicHash::Sha1 );
        g_obUser.logIn( QString( obPwdHash.toHex() ) );

        g_poPrefs->setLastUser( cmbName->currentText(), true );

        enableElementsByLogin( true );

        g_obLogDBWriter.setAppUser( g_obUser.id() );
        g_obLogger(cSeverity::WARNING) << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged in" << EOM;

        if( g_obUser.password() == "da39a3ee5e6b4b0d3255bfef95601890afd80709" ) //password is an empty string
        {
            QMessageBox::warning( this, tr( "Password" ),
                                  tr( "Your password is empty. Please change it to a valid password." ) );

            cDlgUserEdit  obDlgEdit( this, &g_obUser );
            obDlgEdit.setWindowTitle( g_obUser.realName() );
            obDlgEdit.exec();
        }

        updateTitle();
        loginUser();
    }
    catch( cSevException &e )
    {
        g_obLogger(cSeverity::WARNING) << "User " << cmbName->currentText() << " failed to log in" << EOM;

        g_obUser.logOut();
        QMessageBox::critical( this, tr( "Login failed" ),
                               tr( "Incorrect User Name and/or Password. Please try again." ) );
    }
}
//====================================================================================
void cWndMain::enableElementsByLogin(bool p_bEnable)
{
    frmLogin->setVisible( !p_bEnable );
    frmLogin->setEnabled( !p_bEnable );
    mdiPanels->setEnabled( p_bEnable );
}
//====================================================================================
void cWndMain::showProgress()
{
    m_dlgProgress->showProgress();
}
//====================================================================================
void cWndMain::initPanels()
{
    cTracer obTrace( "cWndMain::initPanels" );

    mdiPanels->initPanels();
}
//====================================================================================
void cWndMain::loginUser()
{
    cTracer obTrace( "cWndMain::loginUser" );

    // Felhasznalo ellenorzese
    if( g_obUser.isInGroup( cAccessGroup::ROOT ) || g_obUser.isInGroup( cAccessGroup::SYSTEM) )
    { // root, vagy rendszeradmin felhasznalo lepett be, NINCS penztar akcio
        g_obLogger(cSeverity::INFO) << "User is system administrator. Cassa disabled." << EOM;
        g_obCassa.setDisabled();
        return;
    }

    // Felhasznalohoz tartozo, legutolso nem lezart kassza keresese
    if( g_obCassa.loadOpenCassa(g_obUser.id()) )
    {// Letezik nyitva hagyott, felhasznalohoz tartozo rekord

        g_obLogger(cSeverity::INFO) << "Opened cassa found for user" << EOM;
        if( g_obCassa.cassaOpenDate().left(10).compare( QDate::currentDate().toString("yyyy-MM-dd") ) )
        {
            g_obLogger(cSeverity::INFO) << "Opened cassa started different date: "
                                        << g_obCassa.cassaOpenDate().left(10)
                                        << " "
                                        << QDate::currentDate().toString( "yyyy-MM-dd" )
                                        << EOM;
            if( g_poPrefs->getCassaAutoCreate() )
            {
                switch( g_poPrefs->getCassaCreateType() )
                {
                    case 2:
                    {
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "Settings forced to close cassa with money withdraw and create new cassa" << EOM;
                        break;
                    }
                    case 1:
                    default:
                    {
                        int inBalance = g_obCassa.cassaBalance();
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obCassa.cassaIncreaseMoney( inBalance, tr("Cassa continue with balance") );
                        g_obLogger(cSeverity::INFO) << "Settings forced to start new cassa with balance" << EOM;
                        break;
                    }
                }
            }
            else
            {
                switch( customMsgBox( this, MSG_QUESTION,
                                      tr(" Use opened cassa | Start new cassa with balance | Start new cassa "),
                                      tr("The latest cassa opened on a different date.\n"
                                         "What would you like to do?"),
                                      tr("The previously created and not closed cassa started to be used on "
                                         "a different date. If you continue to use that cassa, your daily "
                                         "report will include summary of more than one day.\n"
                                         "Click the 'Use opened cassa' button if you want to use the opened cassa. "
                                         "In this case the start and the end date will be different of the cassa and the "
                                         "cassa daily report could be misleading.\n"
                                         "Click the 'Start new cassa with balance' button if you want to use a new cassa "
                                         "but you want to use the balance of the old cassa as a starting balance. In this "
                                         "case the previous cassa balance will be reseted and a new cassa will be opened for you "
                                         "with balance of the previous cassa\n"
                                         "Click the 'Start new cassa' button if you want to use a new cassa. In this case the "
                                         "previous cassa balance will be reseted and a new cassa will be opened for you."
                                         )))
                {
                    case 1:
                        g_obCassa.cassaContinue();
                        g_obLogger(cSeverity::INFO) << "User selected to continue to use cassa" << EOM;
                        break;
                    case 2:
                    {
                        int inBalance = g_obCassa.cassaBalance();
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obCassa.cassaIncreaseMoney( inBalance, tr("Cassa continue with balance") );
                        g_obLogger(cSeverity::INFO) << "User selected to start new cassa with balance" << EOM;
                        break;
                    }
                    case 3:
                    default:
                    {
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "User selected to close cassa with money withdraw and create new cassa" << EOM;
                        break;
                    }
                }
            }
        }
        else
        {
            g_obCassa.cassaContinue();
            g_obLogger(cSeverity::INFO) << "Continue cassa automatically" << EOM;
        }
    }
    else
    {// Nincs korabban nyitva hagyott, felhasznalohoz tartozo rekord

        g_obLogger(cSeverity::INFO) << "Opened cassa for user not found" << EOM;
        // Legutolso nem lezart kassza keresese
        if( g_obCassa.loadOpenCassa() )
        {// Van nyitva hagyott kassza rekord

            g_obLogger(cSeverity::INFO) << "Opened cassa found. Owner: " << g_obCassa.cassaOwnerStr() << EOM;

            if( g_obCassa.cassaOpenDate().left(10).compare( QDate::currentDate().toString("yyyy-MM-dd") ) &&
                g_poPrefs->getCassaAutoCreate() )
            {
                switch( g_poPrefs->getCassaCreateType() )
                {
                    case 2:
                    {
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "Settings forced to close cassa with money withdraw and create new cassa" << EOM;
                        break;
                    }
                    case 1:
                    default:
                    {
                        int inBalance = g_obCassa.cassaBalance();
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obCassa.cassaIncreaseMoney( inBalance, tr("Cassa continue with balance") );
                        g_obLogger(cSeverity::INFO) << "Settings forced to start new cassa with balance" << EOM;
                        break;
                    }
                }
            }
            else
            {
                cCurrency cBalance( g_obCassa.cassaBalance() );

                switch( customMsgBox( this, MSG_QUESTION,
                                      tr(" Use opened cassa | Start new cassa with balance | Start new cassa "),
                                      tr("The latest cassa record still not closed:\n\n"
                                         "Owner: %1\n"
                                         "Balance: %2\n\n"
                                         "What would you like to do?").arg( g_obCassa.cassaOwnerStr() ).arg( cBalance.currencyFullStringShort() ),
                                      tr("Cassa still not closed means the cassa owner did not closed his/her cassa "
                                         "and possibly would like to continue the work with the cassa.\n"
                                         "But it is possible that the cassa owner forgot to close his/her cassa.\n"
                                         "You can decide how to start your work:\n"
                                         "Click the 'Use opened cassa'' if you logged in just for a short time. "
                                         "In this case your actions will be linked to your name, "
                                         "but every cassa action will belong to the cassa owner.\n"
                                         "Click the 'Start new cassa with balance' button if you want to use a new cassa "
                                         "but you want to use the balance of the old cassa as a starting balance. In this "
                                         "case the previous cassa balance will be reseted and a new cassa will be opened for you "
                                         "with balance of the previous cassa\n"
                                         "Click the 'Start new cassa' button if you want to use a new cassa. In this case the "
                                         "previous cassa balance will be reseted and a new cassa will be opened for you."
                                         ) ) )
                {
                    case 1:
                        g_obCassa.cassaContinue( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "User selected to use cassa" << EOM;
                        break;
                    case 2:
                    {
                        int inBalance = g_obCassa.cassaBalance();
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cassa left in open.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obCassa.cassaIncreaseMoney( inBalance, tr("Cassa continue with balance") );
                        g_obLogger(cSeverity::INFO) << "User selected to start new cassa with balance" << EOM;
                        break;
                    }
                    case 3:
                    default:
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaOwnerId(), g_obCassa.cassaBalance(), tr("Cassa left in open.") );
                        g_obCassa.cassaClose();
                        g_obCassa.createNew( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "User selected to close previous and create new cassa" << EOM;
                        break;
                }
            }
        }// Volt nyitva hagyott kassza rekord
        else
        {// Nem volt nyitva hagyott kassza rekord

            g_obLogger(cSeverity::INFO) << "Opened cassa not found" << EOM;
            // Legutolso nem kiuritett kassza rekord betoltese
            if( g_obCassa.loadLatestCassaWithCash() )
            {
                g_obLogger(cSeverity::INFO) << "Closed latest cassa with non zero cash found" << EOM;

                if( g_obCassa.cassaOpenDate().left(10).compare( QDate::currentDate().toString("yyyy-MM-dd") ) &&
                    g_poPrefs->getCassaAutoCreate() )
                {
                    switch( g_poPrefs->getCassaCreateType() )
                    {
                        case 2:
                        {
                            g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                            g_obCassa.createNew( g_obUser.id() );
                            g_obLogger(cSeverity::INFO) << "Settings forced to close cassa with money withdraw and create new cassa" << EOM;
                            break;
                        }
                        case 1:
                        default:
                        {
                            int inBalance = g_obCassa.cassaBalance();
                            g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                            g_obCassa.createNew( g_obUser.id() );
                            g_obCassa.cassaIncreaseMoney( inBalance, tr("Cassa continue with balance") );
                            g_obLogger(cSeverity::INFO) << "Settings forced to start new cassa with balance" << EOM;
                            break;
                        }
                    }
                }
                else
                {
                    // Akarja-e a felhasznalo folytatni a kasszat
                    cCurrency cBalance( g_obCassa.cassaBalance() );
                    switch( customMsgBox( this, MSG_QUESTION,
                                          tr(" Reopen cassa | Start new cassa with balance | Start new cassa "),
                                          tr( "The latest cassa record closed with balance:\n\n"
                                              "Owner: %1\n"
                                              "Balance: %2\n\n"
                                              "Do you want to continue this cassa?\n" ).arg( g_obCassa.cassaOwnerStr() ).arg( cBalance.currencyFullStringShort() ),
                                          tr( "Cassa closed with balance means that previously the owner closed the cassa "
                                              "but there are cash left in cassa. The default action when closing the cassa "
                                              "is to withdraw money from the cassa therefore the cassa balance will be zero.\n"
                                              "Click the 'Reopen cassa' if previously the cassa closed with reason without "
                                              "money withdraw and now you want to use that cassa again.\n"
                                              "Click the 'Start new cassa with balance' button if you want to use a new cassa "
                                              "but you want to use the balance of the old cassa as a starting balance. In this "
                                              "case the previous cassa balance will be reseted and a new cassa will be opened for you "
                                              "with balance of the previous cassa\n"
                                              "Click the 'Start new cassa' button if you want to use a new cassa. In this case the "
                                              "previous cassa balance will be reseted and a new cassa will be opened for you."
                                              ) ) )
                    {
                        case 1:
                            // Kassza folytatasa
                            g_obCassa.cassaReOpen();
                            g_obLogger(cSeverity::INFO) << "User selected to reopen cassa" << EOM;
                            break;
                        case 2:
                        {
                            int inBalance = g_obCassa.cassaBalance();
                            g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                            g_obCassa.createNew( g_obUser.id() );
                            g_obCassa.cassaIncreaseMoney( inBalance, tr("Start cassa with balance") );
                            g_obLogger(cSeverity::INFO) << "User selected to start new cassa with balance" << EOM;
                            break;
                        }
                        case 3:
                        default:
                            // Uj kassza nyitasa
                            g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Cash left in cassa.") );
                            g_obCassa.createNew( g_obUser.id() );
                            g_obLogger(cSeverity::INFO) << "User selected to close cassa with money withdraw and create new cassa" << EOM;
                            break;
                    }
                }
            }
            else
            {// Nem volt nem kiuritett kassza

                g_obLogger(cSeverity::INFO) << "Load latest cassa" << EOM;
                // Felhasznalo utolso kasszajanak betoltese
                if( g_obCassa.loadLatestCassa( g_obUser.id() ) )
                {
                    g_obLogger(cSeverity::INFO) << "Latest cassa related to user" << EOM;

                    if( g_obCassa.cassaOpenDate().left(10).compare( QDate::currentDate().toString("yyyy-MM-dd") ) &&
                        g_poPrefs->getCassaAutoCreate() )
                    {
                        g_obCassa.createNew( g_obUser.id() );
                        g_obLogger(cSeverity::INFO) << "Settings forced to create new cassa" << EOM;
                    }
                    else
                    {
                        // Akarja-e a felhasznalo folytatni a kasszat
                        switch( customMsgBox( this, MSG_QUESTION,
                                              tr(" Reopen cassa | Start new cassa "),
                                              tr( "The latest cassa record used:\n\n"
                                                  "from %1 to %2\n\n"
                                                  "Do you want to continue this cassa?").arg( g_obCassa.cassaOpenDate() ).arg( g_obCassa.cassaCloseDate() ),
                                              tr( "It is possible to use your latest cassa with reopening it for example "
                                                  "if you closed it previously for security reasons.\n"
                                                  "Click the 'Reopen cassa' if you want to use your previously closed cassa.\n"
                                                  "Click the 'Start new cassa' if you don't want to use the previously closed cassa. "
                                                  "In this case a new cassa will be opened for you.") ) )
                        {
                            case 1:
                                // Kassza folytatasa
                                g_obCassa.cassaReOpen();
                                g_obLogger(cSeverity::INFO) << "User selected to reopen last cassa" << EOM;
                                break;
                            case 2:
                            default:
                                // Uj kassza nyitasa
                                g_obCassa.createNew( g_obUser.id() );
                                g_obLogger(cSeverity::INFO) << "User selected to create new cassa" << EOM;
                                break;
                        }
                    }
                }
                else
                {// Uj kassza nyitasa
                    g_obLogger(cSeverity::INFO) << "Latest cassa related to different user" << EOM;
                    g_obCassa.createNew( g_obUser.id() );
                    g_obLogger(cSeverity::INFO) << "Create new cassa automatically" << EOM;
                }
            }
        }// Nem volt nyitva hagyott kassza rekord
    }// Nincs korabban nyitva hagyott, felhasznalohoz tartozo rekord

    g_obCassa.setEnabled();
    g_obLogger(cSeverity::INFO) << "Cassa enabled" << EOM;
}
//====================================================================================
void cWndMain::logoutUser()
{
    cTracer obTrace( "cWndMain::logoutUser" );

    if( g_obCassa.isCassaEnabled() )
    {
        if( !g_poPrefs->getCassaAutoClose() )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to close your cassa?"),
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
                g_obLogger(cSeverity::INFO) << "User selected to close cassa" << EOM;
                if( g_obCassa.cassaBalance() > 0 )
                {
                    cCurrency   cBalance( g_obCassa.cassaBalance() );

                    switch( customMsgBox( this, MSG_QUESTION,
                                          tr("Withdraw cash|Leave cash in cassa"),
                                          tr("There are some cash left in your cassa.\n"
                                             "Current balance: %1\n\n"
                                             "Do you want to close the cassa with automatic cash withdawal?\n" ).arg( cBalance.currencyFullStringShort() ),
                                          tr( "You can decide if you want to leave the cash in the cassa "
                                              "or withdraw cash from the cassa.\n"
                                              "Click on the 'Withdraw cash' button if you want to withdraw cash "
                                              "from cassa before closing it\n"
                                              "Click on the 'Leave cash in cassa' if you want to leave the cash "
                                              "in the cassa before closing it") ) )
                    {
                        case 1:
                            g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
                            g_obLogger(cSeverity::INFO) << "User selected to withdraw cash from cassa" << EOM;
                            break;

                        case 2:
                        default:
                            g_obLogger(cSeverity::INFO) << "User selected to leave cash in cassa" << EOM;
                            break;
                    }
                }
                g_obCassa.cassaClose();
                g_obLogger(cSeverity::INFO) << "Cassa closed" << EOM;
            }
            else
            {
                g_obLogger(cSeverity::INFO) << "User selected to leave cassa opened" << EOM;
            }
        }
        else
        {
            if( g_obCassa.cassaBalance() > 0 && !g_poPrefs->getCassaAutoWithdrawal() )
            {
                cCurrency   cBalance( g_obCassa.cassaBalance() );

                switch( customMsgBox( this, MSG_QUESTION,
                                      tr("Withdraw cash|Leave cash in cassa"),
                                      tr("There are some cash left in your cassa.\n"
                                         "Current balance: %1\n\n"
                                         "Do you want to close the cassa with automatic cash withdawal?\n" ).arg( cBalance.currencyFullStringShort() ),
                                      tr( "You can decide if you want to leave the cash in the cassa "
                                          "or withdraw cash from the cassa.\n"
                                          "Click on the 'Withdraw cash' button if you want to withdraw cash "
                                          "from cassa before closing it\n"
                                          "Click on the 'Leave cash in cassa' if you want to leave the cash "
                                          "in the cassa before closing it") ) )
                {
                    case 1:
                        g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
                        g_obLogger(cSeverity::INFO) << "User selected to withdraw cash from cassa" << EOM;
                        break;

                    case 2:
                    default:
                        g_obLogger(cSeverity::INFO) << "User selected to leave cash in cassa" << EOM;
                        break;
                }
            }
            else if( g_obCassa.cassaBalance() > 0 && g_poPrefs->getCassaAutoWithdrawal() )
            {
                g_obCassa.cassaDecreaseMoney( g_obCassa.cassaBalance(), tr("Automatic cassa close.") );
                g_obLogger(cSeverity::INFO) << "Automatic cassa withdraw" << EOM;
            }
            g_obCassa.cassaClose();
            g_obLogger(cSeverity::INFO) << "Cassa closed" << EOM;
        }
    }
}
//====================================================================================
void cWndMain::keyPressEvent( QKeyEvent *p_poEvent )
{
    if( !g_obUser.isLoggedIn() || m_bActionProcessing )
        return;

    cTracer obTrace( "cWndMain::keyPressEvent" );

    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = true;
        m_lblStatusLeft.setText( tr("Q -> Exit application | F -> pay device usage | S -> start device | N -> skip status | T -> device cleared | K -> open shopping kart") );
    }

    if( m_bCtrlPressed )
    {
        if( p_poEvent->key() == Qt::Key_Q )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + Q" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            close();
        }
        else if( p_poEvent->key() == Qt::Key_S && action_DeviceStart->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + S" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            m_bActionProcessing = true;
            on_action_DeviceStart_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_T && action_DeviceClear->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + T" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            m_bActionProcessing = true;
            on_action_DeviceClear_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_F && action_PayCash->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + F" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            m_bActionProcessing = true;
            on_action_PayCash_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_K && action_ShoppingCart->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + K" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            m_bActionProcessing = true;
            on_action_ShoppingCart_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_N && action_DeviceSkipStatus->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + N" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            m_bActionProcessing = true;
            on_action_DeviceSkipStatus_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_F12 )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + F12" << EOM;
            m_bCtrlPressed = false;
            m_lblStatusLeft.setText( m_qsStatusText );
            on_action_TestDlgStarted();
        }
    }
    else
    {
        if( p_poEvent->key() == Qt::Key_Enter || p_poEvent->key() == Qt::Key_Return )
        {
            g_obLogger(cSeverity::INFO) << "User pressed ENTER" << EOM;
            switch( m_nEnterAction )
            {
                case cDBApplicationAction::APPACT_DEVICE_PAYCASH:
                    m_lblStatusLeft.setText( m_qsStatusText );
                    m_bActionProcessing = true;
                    on_action_PayCash_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_START:
                    m_lblStatusLeft.setText( m_qsStatusText );
                    m_bActionProcessing = true;
                    on_action_DeviceStart_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_SKIP:
                    m_lblStatusLeft.setText( m_qsStatusText );
                    m_bActionProcessing = true;
                    on_action_DeviceSkipStatus_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_CLEAN:
                    m_lblStatusLeft.setText( m_qsStatusText );
                    m_bActionProcessing = true;
                    on_action_DeviceClear_triggered();
                    break;
            }
        }
        else if( ((p_poEvent->key() >= Qt::Key_0 && p_poEvent->key() <= Qt::Key_9) ||
                  (p_poEvent->key() >= Qt::Key_A && p_poEvent->key() <= Qt::Key_Z)) )
        {
            m_lblStatusLeft.setText( m_qsStatusText );

            cDlgInputStart  obDlgInputStart( this );

            obDlgInputStart.setInitialText( p_poEvent->text() );
            if( obDlgInputStart.exec() == QDialog::Accepted )
            {
                if( obDlgInputStart.m_bCard )
                {
                    m_bActionProcessing = true;
                    processInputPatientCard( obDlgInputStart.getEditText() );
                }
                else if( obDlgInputStart.m_bProd )
                {
                    m_bActionProcessing = true;
                    processInputProduct( obDlgInputStart.getEditText() );
                }
                else if( obDlgInputStart.m_bTime )
                {
                    m_bActionProcessing = true;
                    processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
                }
            }

            p_poEvent->ignore();
        }
        else if( p_poEvent->key() == Qt::Key_Escape && mdiPanels->isStatusCanBeReseted() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed ESC" << EOM;
            m_lblStatusLeft.setText( m_qsStatusText );
            mdiPanels->clear();
        }
        else if( p_poEvent->key() == Qt::Key_Space )
        {
            g_obLogger(cSeverity::INFO) << "User pressed SPACE" << EOM;
            m_bActionProcessing = true;
            on_action_UseDevice_triggered();
        }
    }

    QMainWindow::keyPressEvent( p_poEvent );
}
//====================================================================================
void cWndMain::keyReleaseEvent( QKeyEvent *p_poEvent )
{
    if( !g_obUser.isLoggedIn() )
        return;

    cTracer obTrace( "cWndMain::keyReleaseEvent" );

    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = false;
        m_lblStatusLeft.setText( m_qsStatusText );
    }

    QMainWindow::keyReleaseEvent( p_poEvent );
}
//====================================================================================
void cWndMain::showElementsForComponents()
{
    toolBarSchedule->setEnabled( false );
    toolBarSchedule->setVisible( false );

    action_Patients->setVisible( false );
    action_Company->setVisible( false );
    action_HealthInsurance->setVisible( false );
    action_RegionZipCity->setVisible( false );
    action_ReportPatients->setVisible( false );
}
//====================================================================================
void cWndMain::updateTitle()
{
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

    action_Panelgroups->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    action_Paneltypes->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    action_PanelStatuses->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    setWindowTitle( qsTitle );
}
//====================================================================================
void cWndMain::updateStatusText( QString p_qsStatusText )
//====================================================================================
{
    if( m_bCtrlPressed )
        return;

    bool    bIsUserLoggedIn = g_obUser.isLoggedIn();

    m_qsStatusText = tr("SPACE -> Enter time/barcode ...");
    m_nEnterAction = 0;

    if( bIsUserLoggedIn && mdiPanels->isHasToPay() )
    {
        m_qsStatusText.append( tr(" | ENTER -> %1").arg( action_PayCash->toolTip() ) );
        m_nEnterAction = cDBApplicationAction::APPACT_DEVICE_PAYCASH;
    }
    else if( bIsUserLoggedIn && !mdiPanels->isPanelWorking(mdiPanels->activePanel()) && mdiPanels->mainProcessTime() > 0 )
    {
        m_qsStatusText.append( tr(" | ENTER -> %1").arg( action_DeviceStart->toolTip() ) );
        m_nEnterAction = cDBApplicationAction::APPACT_DEVICE_START;
    }
    else if( bIsUserLoggedIn && mdiPanels->isNeedToBeCleaned() )
    {
        m_qsStatusText.append( tr(" | ENTER -> %1").arg( action_DeviceClear->toolTip() ) );
        m_nEnterAction = cDBApplicationAction::APPACT_DEVICE_CLEAN;
    }
    else if( bIsUserLoggedIn && mdiPanels->isStatusCanBeSkipped( mdiPanels->activePanel()) )
    {
        m_qsStatusText.append( tr(" | ENTER -> %1").arg( action_DeviceSkipStatus->toolTip() ) );
        m_nEnterAction = cDBApplicationAction::APPACT_DEVICE_SKIP;
    }

    m_lblStatusLeft.setText( m_qsStatusText );
}
//====================================================================================
void cWndMain::updateToolbar()
{
    bool    bIsUserLoggedIn = g_obUser.isLoggedIn();

    action_Export->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
    action_Import->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
    menu_ExportImport->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );

    action_LogOut->setEnabled( bIsUserLoggedIn );
    action_Exit->setEnabled( !mdiPanels->isPanelWorking() );

    menu_Edit->setEnabled( bIsUserLoggedIn );
        action_Guests->setEnabled( bIsUserLoggedIn );
        action_CardTypes->setEnabled( bIsUserLoggedIn );
        action_Cards->setEnabled( bIsUserLoggedIn );
        menuAdministrator->setEnabled( bIsUserLoggedIn );
            action_Users->setEnabled( bIsUserLoggedIn );
            action_Company->setEnabled( bIsUserLoggedIn && g_poPrefs->isComponentKiwiSunInstalled() );
            action_HealthInsurance->setEnabled( bIsUserLoggedIn && g_poPrefs->isComponentKiwiSunInstalled() );
            action_Discounts->setEnabled( bIsUserLoggedIn );
            action_RegionZipCity->setEnabled( bIsUserLoggedIn && g_poPrefs->isComponentKiwiSunInstalled() );
            action_Panelgroups->setEnabled( bIsUserLoggedIn );
            action_Paneltypes->setEnabled( bIsUserLoggedIn );
            action_PanelStatuses->setEnabled( bIsUserLoggedIn );
            action_ProductTypes->setEnabled( bIsUserLoggedIn );
            action_ProductActionType->setEnabled( bIsUserLoggedIn );
            action_Products->setEnabled( bIsUserLoggedIn );
            action_ManageSkinTypes->setEnabled( bIsUserLoggedIn );
            action_PaymentMethods->setEnabled( bIsUserLoggedIn );
            action_ValidateSerialKey->setEnabled( bIsUserLoggedIn );
            action_EditLicenceInformation->setEnabled( bIsUserLoggedIn );
            action_EmptyDemoDB->setEnabled( bIsUserLoggedIn );
        action_Preferences->setEnabled( bIsUserLoggedIn );

    menu_Action->setEnabled( bIsUserLoggedIn );
        menuDevice->setEnabled( bIsUserLoggedIn );
            action_UseDevice->setEnabled( bIsUserLoggedIn && (mdiPanels->isCanBeStartedByCard() || mdiPanels->isCanBeStartedByTime()) );
            action_UseDeviceLater->setEnabled( bIsUserLoggedIn );
            action_DeviceClear->setEnabled( bIsUserLoggedIn && mdiPanels->isNeedToBeCleaned() );
            action_DeviceStart->setEnabled( bIsUserLoggedIn && ((!mdiPanels->isPanelWorking(mdiPanels->activePanel()) && mdiPanels->mainProcessTime() > 0) || mdiPanels->isDeviceStopped() ) );
            action_DeviceSkipStatus->setEnabled( bIsUserLoggedIn && mdiPanels->isStatusCanBeSkipped(mdiPanels->activePanel()) );
            action_DeviceReset->setEnabled( bIsUserLoggedIn /*&& mdiPanels->isMainProcess()*/ );
            action_ManageDatabase->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
            action_ManageDevicePanels->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::SYSTEM) );
        menuPatientCard->setEnabled( bIsUserLoggedIn );
            action_PatientCardSell->setEnabled( bIsUserLoggedIn );
            action_PatientCardAssign->setEnabled( bIsUserLoggedIn );
            action_PCSaveToDatabase->setEnabled( bIsUserLoggedIn );
        menuProduct->setEnabled( bIsUserLoggedIn );
            action_SellProduct->setEnabled( bIsUserLoggedIn );
        menuCassa->setEnabled( bIsUserLoggedIn );
            action_CassaActionStorno->setEnabled( bIsUserLoggedIn );

    menu_Reports->setEnabled( bIsUserLoggedIn );

    menu_View->setEnabled( bIsUserLoggedIn );
        action_Cassa->setEnabled( bIsUserLoggedIn && g_obCassa.isCassaEnabled() );
        action_Logs->setEnabled( bIsUserLoggedIn );
        action_Hardwaretest->setEnabled( bIsUserLoggedIn );

    toolBarPatient->setEnabled( bIsUserLoggedIn );
        action_PatientNew->setEnabled( bIsUserLoggedIn );
        action_PatientSelect->setEnabled( bIsUserLoggedIn && !(g_obGuest.id()>0) );
        action_PatientEmpty->setEnabled( bIsUserLoggedIn && g_obGuest.id()>0 );
        action_EditActualPatient->setEnabled( bIsUserLoggedIn && g_obGuest.id()>0 );

    toolBarDeviceUse->setEnabled( bIsUserLoggedIn );
        action_DeviceSettings->setEnabled( bIsUserLoggedIn && !mdiPanels->isPanelWorking(mdiPanels->activePanel()) );

    toolBarCassa->setEnabled( bIsUserLoggedIn );
        action_PayCash->setEnabled( bIsUserLoggedIn && mdiPanels->isHasToPay() );

    action_PatientSelect->setVisible( !(g_obGuest.id()>0) );
    action_PatientEmpty->setVisible( g_obGuest.id()>0 );
    action_ManageDatabase->setVisible( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
    action_ManageDevicePanels->setVisible( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::SYSTEM) );

    showElementsForComponents();
}
//====================================================================================
void cWndMain::timerEvent(QTimerEvent *)
{
    QFile   fileCheck( "belenus.chk" );

    if( fileCheck.size() > 0 )
    {
        fileCheck.open( QIODevice::WriteOnly );
        fileCheck.write( "" );
        fileCheck.close();
    }
    updateStatusText();
    updateToolbar();

    m_inCommunicationCounter++;

    if( m_inCommunicationCounter > 4 )
    {
        m_inCommunicationCounter = 0;

        if( g_poHardware->isCommunicationStopped() )
        {
            g_obLogger(cSeverity::ERROR) << "Communication stopped with hardware controller" << EOM;
            m_dlgProgress->showError( tr("Communication stopped with hardware controller") );
        }
    }

    if( m_bSerialRegistration )
    {
        if( g_poPrefs->getLicenceId() > 1 )
        {
            m_bSerialRegistration = false;

//            g_obCassa.cassaClose();
//            g_obCassa.createNew( g_obUser.id() );

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

    if( m_uiPatientId != g_obGuest.id() )
    {
        updateTitle();

        m_uiPatientId = g_obGuest.id();
    }

    m_lblStatusRight.setText( QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss  " ) );
}
//====================================================================================
void cWndMain::closeEvent( QCloseEvent *p_poEvent )
{
    cTracer obTrace( "cWndMain::closeEvent" );

    bool    bIsShutdown = true;

    g_obLogger(cSeverity::INFO) << "Application closure started ..." << EOM;

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
            g_obLogger(cSeverity::INFO) << "User selected to close the application" << EOM;
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

            g_obGen.backupDatabase( this );
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

    m_dlgProgress->showProgress();

    cDlgPreferences  obDlgPrefs( this );

    m_dlgProgress->hideProgress();

    if( obDlgPrefs.exec() == QDialog::Accepted )
    {
        g_obLogger(cSeverity::INFO) << "Preferences dialog closed with OK button pressed" << EOM;
        retranslateUi( this );
        mdiPanels->hide();
        mdiPanels->placeSubWindows();
        mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );
        mdiPanels->show();
        mdiPanels->refreshDisplay();

        m_dlgSecondaryWindow->refreshBackground();

        if( g_poPrefs->isSecondaryWindowVisible() && !m_dlgSecondaryWindow->isVisible() )
        {
            m_dlgSecondaryWindow->move( g_poPrefs->secondaryWindowPosition() );
            m_dlgSecondaryWindow->resize( g_poPrefs->secondaryWindowSize() );
            m_dlgSecondaryWindow->show();
            this->setFocus();
        }
        else if( !g_poPrefs->isSecondaryWindowVisible() )
        {
            g_poPrefs->setSecondaryWindowPosition( QPoint( m_dlgSecondaryWindow->x(), m_dlgSecondaryWindow->y() ), true );
            g_poPrefs->setSecondaryWindowSize( QSize( m_dlgSecondaryWindow->width(), m_dlgSecondaryWindow->height() ), true );
            m_dlgSecondaryWindow->hide();
        }
    }
}
//====================================================================================
void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    m_dlgProgress->showProgress();

    cDlgUsers  obDlgUsers( this );

    m_dlgProgress->hideProgress();

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
void cWndMain::on_action_Panelgroups_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Panelgroups_triggered" );

    m_dlgProgress->showProgress();

    cDlgPanelGroups  obDlgPanelGroups( this );

    m_dlgProgress->hideProgress();

    obDlgPanelGroups.exec();
}
//====================================================================================
void cWndMain::on_action_Paneltypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Paneltypes_triggered" );

    m_dlgProgress->showProgress();

    cDlgPanelTypes  obDlgPanelTypes( this );

    m_dlgProgress->hideProgress();

    obDlgPanelTypes.exec();
}
//====================================================================================
void cWndMain::on_action_Guests_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Guests_triggered" );

    m_dlgProgress->showProgress();

    cDlgGuest  obDlgGuest( this );

    m_dlgProgress->hideProgress();

    obDlgGuest.exec();
}
//====================================================================================
void cWndMain::on_action_PatientNew_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientNew_triggered" );

    m_dlgProgress->showProgress();

    cDBGuest *poGuest = new cDBGuest;
    poGuest->createNew();

    cDlgGuestEdit  obDlgEdit( this, poGuest );
    obDlgEdit.setWindowTitle( tr( "New Patient" ) );

    m_dlgProgress->hideProgress();

    obDlgEdit.exec();

    /*if( poGuest->id() > 0 )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to select the created patient as actual?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_obGuest.load( poGuest->id() );
        }
    }*/

    delete poGuest;
}
//====================================================================================
void cWndMain::on_action_DeviceClear_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceClear_triggered" );

    mdiPanels->clean();

    if( mdiPanels->isPatientWaiting() )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to prepare the device from the waiting queue?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
        {
            mdiPanels->setUsageFromWaitingQueue();
        }
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::on_action_DeviceStart_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceStart_triggered" );

    if( !action_DeviceStart->isEnabled() )
        return;

    if( mdiPanels->isDeviceStopped() )
    {
        mdiPanels->continueStoppedDevice();
    }
    else
    {
        if( mdiPanels->isHasToPay() )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The device usage has to be payed.\n"
                                     "Please process the payment first.") );
            return;
        }

        mdiPanels->start();

        on_action_PatientEmpty_triggered();
        m_bActionProcessing = false;
    }
}
//====================================================================================
void cWndMain::on_action_DeviceReset_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceReset_triggered" );

    mdiPanels->reset();
}
//====================================================================================
void cWndMain::on_action_PatientSelect_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientSelect_triggered" );

    m_dlgProgress->showProgress();

    cDlgPatientSelect  obDlgPatientSelect( this );

    m_dlgProgress->hideProgress();

    if( obDlgPatientSelect.exec() == QDialog::Accepted )
    {
        try
        {
            g_obGuest.load( obDlgPatientSelect.selectedPatientId() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        updateTitle();
    }
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
    cTracer obTrace( "cWndMain::on_action_PanelStatuses_triggered" );

    m_dlgProgress->showProgress();

    cDlgPanelStatuses   obDlgPanelStatuses( this );

    m_dlgProgress->hideProgress();

    obDlgPanelStatuses.exec();

    if( obDlgPanelStatuses.isStatusChanged() )
    {
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );
    }

}
//====================================================================================
void cWndMain::on_action_UseDevice_triggered()
{
    cTracer obTrace( "cWndMain::on_action_UseDevice_triggered" );

    m_dlgProgress->showProgress();

    cDlgPanelUse obDlgPanelUse( this, mdiPanels->activePanelId() );

    obDlgPanelUse.enableCardUsage( mdiPanels->isCanBeStartedByCard() );
    obDlgPanelUse.enableCashUsage( mdiPanels->isCanBeStartedByTime() );

    obDlgPanelUse.setPanelUseTime( mdiPanels->mainProcessTime() );

    if( m_inPanelStartMinute > 0 )
    {
        obDlgPanelUse.setPanelUseTimeCash( m_inPanelStartMinute*60 );
        m_inPanelStartMinute = 0;
    }
    if( m_qsPanelStartBarcode.length() > 0 )
    {
        obDlgPanelUse.setPanelUsePatientCard( m_qsPanelStartBarcode );
        m_qsPanelStartBarcode = "";
    }

    m_dlgProgress->hideProgress();

    if( obDlgPanelUse.exec() == QDialog::Accepted )
    {
        if( obDlgPanelUse.panelUsePatientCardId() > 0 && obDlgPanelUse.panelUseSecondsCard() > 0 )
        {
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( obDlgPanelUse.panelUsePatientCardId() );

            if( g_obGuest.id() == 0 && obDBPatientCard.patientId() > 0 )
            {
                /*cDBGuest  obDBGuest;

                obDBGuest.load( obDBPatientCard.patientId() );
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has been assigned to the following patient.\n\n"
                                              "%1\n\n"
                                              "Do you want to select this patient as actual?").arg( obDBGuest.name() ),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    g_obGuest.load( obDBPatientCard.patientId() );
                }*/
            }
            else if( obDBPatientCard.patientId() != g_obGuest.id() && g_obGuest.id() > 0 && obDBPatientCard.patientId() > 0 )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has been assigned to a different patient.\n"
                                              "Are you sure you want to use this patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    m_bActionProcessing = false;
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

            mdiPanels->setMainProcessTime( obDlgPanelUse.panelUsePatientCardId(), obDlgPanelUse.panelUnitIds(), obDlgPanelUse.panelUseSecondsCard() );

            int nCount = obDlgPanelUse.countPatientCardUnitsLeft();
            mdiPanels->setTextInformation( tr( "%1 units left on the selected card" ).arg(nCount) );
        }
        if( obDlgPanelUse.panelUseSecondsCash() > 0 )
        {
            mdiPanels->setMainProcessTime( obDlgPanelUse.panelUseSecondsCash(), obDlgPanelUse.panelUsePrice() );
        }
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::on_action_UseDeviceLater_triggered()
{
    cTracer obTrace( "cWndMain::on_action_UseDeviceLater_triggered" );

    m_dlgProgress->showProgress();
    cDlgPanelUse obDlgPanelUse( this, mdiPanels->activePanelId() );
    m_dlgProgress->hideProgress();

    if( obDlgPanelUse.exec() == QDialog::Accepted )
    {
        int             inLengthCash    = 0;
        int             inPrice         = 0;
        unsigned int    uiPatientCardId = 0;
        QString         qsUnitIds       = "";
        int             inLengthCard    = 0;
        unsigned int    uiLedgerId      = 0;
        int             inPayType       = 0;

        if( obDlgPanelUse.panelUsePatientCardId() > 0 && obDlgPanelUse.panelUseSecondsCard() > 0 )
        {
            uiPatientCardId = obDlgPanelUse.panelUsePatientCardId();
            qsUnitIds = obDlgPanelUse.panelUnitIds().join( "|" );
            inLengthCard = obDlgPanelUse.panelUseSecondsCard();
        }
        if( obDlgPanelUse.panelUseSecondsCash() > 0 )
        {
            inLengthCash = obDlgPanelUse.panelUseSecondsCash();
            inPrice = obDlgPanelUse.panelUsePrice();
        }

        if( inPrice > 0 )
        {
            if( !g_obCassa.isCassaEnabled() )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("Cassa is disabled!\n\n"
                                         "Please relogin to enable cassa.") );
                return;
            }

            int                 inPriceTotal;
            int                 inPriceNet = inPrice;

            cCurrency cPrice( inPriceNet, cCurrency::CURR_GROSS, g_poPrefs->getDeviceUseVAT() );

            inPriceTotal = cPrice.currencyValue().toInt();

            cDBShoppingCart obDBShoppingCart;

            obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
            obDBShoppingCart.setGuestId( g_obGuest.id() );
            obDBShoppingCart.setProductId( 0 );
            obDBShoppingCart.setPatientCardId( 0 );
            obDBShoppingCart.setPatientCardTypeId( 0 );
            obDBShoppingCart.setPanelId( mdiPanels->activePanelId() );
            obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_DEVICE_USAGE );
            obDBShoppingCart.setItemName( tr("Using panel") );
            obDBShoppingCart.setItemCount( 1 );
            obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
            obDBShoppingCart.setItemVAT( g_poPrefs->getDeviceUseVAT() );
            obDBShoppingCart.setItemDiscount( 0 );
            obDBShoppingCart.setItemSumPrice( inPriceTotal );

            cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

            obDlgCassaAction.setPayWithCash();

            int             inCassaAction   = obDlgCassaAction.exec();
            QString         qsComment       = tr("Using device: %1").arg( mdiPanels->getActivePanelCaption() );
            bool            bShoppingCart   = false;
            unsigned int    uiCouponId = 0;
            cDBDiscount     obDBDiscount;

            obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

            if( inCassaAction == QDialog::Accepted && !bShoppingCart )
            {
                /*if( uiCouponId > 0 )
                {
                    obDBDiscount.load( uiCouponId );

                    obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                }*/
                uiLedgerId = g_obCassa.cassaProcessDeviceUse( obDBShoppingCart, qsComment, inPayType, mdiPanels->getPanelCaption(obDBShoppingCart.panelId()) );
                inPrice = 0;
            }
            else if( inCassaAction == QDialog::Accepted && bShoppingCart )
            {
                mdiPanels->itemAddedToShoppingCart();
                inPrice = 0;
            }
        }

        mdiPanels->addPatientToWaitingQueue( inLengthCash, inPrice, uiPatientCardId, qsUnitIds, inLengthCard, uiLedgerId, inPayType );
    }
}
//====================================================================================
void cWndMain::on_action_Cards_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Cards_triggered" );

    m_dlgProgress->showProgress();

    cDlgPatientCard obDlgPatientCard( this );

    connect( &obDlgPatientCard, SIGNAL(signalReplacePatientCard(QString)), this, SLOT(slotReplacePatientCard(QString)) );
    connect( &obDlgPatientCard, SIGNAL(signalAssignPartnerCard(QString)), this, SLOT(slotAssignPartnerCard(QString)) );

    m_dlgProgress->hideProgress();

    obDlgPatientCard.exec();
}
//====================================================================================
void cWndMain::on_action_CardTypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_CardTypes_triggered" );

    m_dlgProgress->showProgress();

    cDlgPatientCardType obDlgPatientCardType( this );

    m_dlgProgress->hideProgress();

    obDlgPatientCardType.exec();
}
//====================================================================================
void cWndMain::on_action_ProductTypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ProductTypes_triggered" );

    m_dlgProgress->showProgress();

    cDlgProductType obDlgProductType( this );

    m_dlgProgress->hideProgress();

    obDlgProductType.exec();
}
//====================================================================================
void cWndMain::on_action_ProductActionType_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ProductActionType_triggered" );

    m_dlgProgress->showProgress();

    cDlgProductActionType obDlgProductActionType( this );

    m_dlgProgress->hideProgress();

    obDlgProductActionType.exec();
}
//====================================================================================
void cWndMain::on_action_Products_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Products_triggered" );

    m_dlgProgress->showProgress();

    cDlgProduct obDlgProduct( this );

    m_dlgProgress->hideProgress();

    obDlgProduct.exec();
}
//====================================================================================
void cWndMain::on_action_SellProduct_triggered()
{
    cTracer obTrace( "cWndMain::on_action_SellProduct_triggered" );

    on_action_SellProduct_triggered( "" );
}
//====================================================================================
void cWndMain::on_action_SellProduct_triggered( QString p_qsBarcode )
{
    cTracer obTrace( "cWndMain::on_action_SellProduct_triggered( QString )" );

    m_dlgProgress->showProgress();

    cDlgProductSell obDlgProductSell( this, p_qsBarcode );

    connect( &obDlgProductSell, SIGNAL(signalPaymentProcessed(cDBShoppingCart)), this, SLOT(processProductSellPayment(cDBShoppingCart)) );

    m_dlgProgress->hideProgress();

    obDlgProductSell.exec();
}
//====================================================================================
void cWndMain::on_action_ShoppingCart_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ShoppingCart_triggered" );

    slotOpenShoppingCart( 0 );
}
//====================================================================================
void cWndMain::slotOpenShoppingCart( unsigned int p_uiPanelId )
{
    cTracer obTrace( "cWndMain::slotOpenShoppingCart" );

    m_dlgProgress->showProgress();

    cDlgShoppingCart    obDlgShoppingCart( this );

    connect( &obDlgShoppingCart, SIGNAL(signalPaymentProcessed(unsigned int,unsigned int,int)), this, SLOT(processDeviceUsePayment(unsigned int,unsigned int,int)) );
    connect( &obDlgShoppingCart, SIGNAL(signalSellPatientCard()), this, SLOT(on_action_PatientCardSell_triggered()) );
    connect( &obDlgShoppingCart, SIGNAL(signalSellProduct()), this, SLOT(on_action_SellProduct_triggered()) );

    if( p_uiPanelId > 0 )
        obDlgShoppingCart.setPanelFilter( p_uiPanelId );

    m_dlgProgress->hideProgress();

    obDlgShoppingCart.exec();
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::slotOpenScheduleTable( unsigned int p_uiPanelId )
{
    cTracer obTrace( "cWndMain::slotOpenScheduleTable" );

    mdiPanels->activatePanelId( p_uiPanelId );
    if( !mdiPanels->isPanelWorking( mdiPanels->activePanel() ) )
    {
        on_action_DeviceClear_triggered();
    }
}
//====================================================================================
void cWndMain::on_action_CassaActionStorno_triggered()
{
    cTracer obTrace( "cWndMain::on_action_CassaActionStorno_triggered" );

    m_dlgProgress->showProgress();

    cDlgStorno  obDlgStorno( this );

    m_dlgProgress->hideProgress();

    obDlgStorno.exec();
}
//====================================================================================
void cWndMain::on_action_PCSaveToDatabase_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PCSaveToDatabase_triggered" );

    m_dlgProgress->showProgress();

    cDlgPatientCardAdd  obDlgPatientCardAdd( this );

    m_dlgProgress->hideProgress();

    obDlgPatientCardAdd.exec();
}
//====================================================================================
void cWndMain::on_action_Cassa_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Cassa_triggered" );

    m_dlgProgress->showProgress();

    cDlgCassaEdit   obDlgCassaEdit( this );

    m_dlgProgress->hideProgress();

    obDlgCassaEdit.exec();
}
//====================================================================================
void cWndMain::on_action_Accounting_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Accounting_triggered" );

    m_dlgProgress->showProgress();

    cDlgRepLedgerMain  obDlgLedgerMain( this );

    m_dlgProgress->hideProgress();

    obDlgLedgerMain.exec();
}
//====================================================================================
void cWndMain::on_action_DeviceSkipStatus_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceSkipStatus_triggered" );

    if( QMessageBox::question( this, tr("Question"),
                               tr("Do you want to jump to the next status of the device?"),
                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        mdiPanels->next();
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::on_action_ValidateSerialKey_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ValidateSerialKey_triggered" );

    if( !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("You are not authorized to activate or modify\n"
                                 "the licence key or the application's validity.") );
    }
    else
    {
        cDlgSerialReg   obDlgSerialReg( this );

        if( obDlgSerialReg.exec() == QDialog::Accepted )
        {
            m_inRegistrationTimeout = 0;
            m_bSerialRegistration = true;
        }
    }
}
//====================================================================================
void cWndMain::on_action_PatientCardSell_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientCardSell_triggered" );

    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();
    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        cDBPatientCard  obDBPatientCard;
        try
        {
            obDBPatientCard.load( obDlgInputStart.getEditText() );

            if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("This patientcard has been lost and replaced\nand can not be used or sold again.") );
                return;
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
                    obDBPatientCard.setBarcode( obDlgInputStart.getEditText() );
                    obDBPatientCard.save();
                }
            }
        }
        if( obDBPatientCard.active() )
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
                    cDlgPatientCardRefill obDlgPatientCardRefill( this, &obDBPatientCard );
                    obDlgPatientCardRefill.exec();
                }
            }
            else
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard still can be used.\n"
                                              "Do you want to refill anyway?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCard.setPatientCardTypeId( 0 );
                    cDlgPatientCardRefill obDlgPatientCardRefill( this, &obDBPatientCard );
                    obDlgPatientCardRefill.exec();
                }
            }
        }
        else
        {
            cDlgPatientCardSell obDlgPatientCardSell( this, &obDBPatientCard );
            obDlgPatientCardSell.setPatientCardOwner( g_obGuest.id() );
            obDlgPatientCardSell.exec();
        }
    }
}
//====================================================================================
void cWndMain::on_action_PatientCardAssign_triggered()
{
    slotAssignPartnerCard( "" );
}
//====================================================================================
void cWndMain::processInputPatientCard( QString p_stBarcode )
{
    cTracer obTrace( "cWndMain::processInputPatientCard" );

    if( !mdiPanels->isCanBeStartedByCard() )
    {
        switch( customMsgBox( this, MSG_ATTENTION,
                              tr("Reset device|Add to wait list ...|Cancel"),
                              tr("This device already prepared with a patientcard."),
                              tr("To start the device with other conditions, please\n"
                                 "reset the device first with pushing the ESC button.") ) )
        {
            case 1:
                if( mdiPanels->isStatusCanBeReseted() )
                {
                    m_lblStatusLeft.setText( m_qsStatusText );
                    mdiPanels->clear();
                }
                else
                {
                    return;
                }
                break;
            case 2:
                m_qsPanelStartBarcode = p_stBarcode;
                on_action_UseDeviceLater_triggered();
                return;
                break;
            case 3:
            default:
                return;
        }
    }

    m_qsPanelStartBarcode = p_stBarcode;
    if( mdiPanels->isPanelWorking( mdiPanels->activePanel() ) )
    {
        on_action_UseDeviceLater_triggered();
    }
    else
    {
        on_action_UseDevice_triggered();
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::processInputProduct( QString p_stBarcode )
{
    cTracer obTrace( "cWndMain::processInputProduct" );

    on_action_SellProduct_triggered( p_stBarcode );

    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::processInputTimePeriod( int p_inMinute )
{
    cTracer obTrace( "cWndMain::processInputTimePeriod" );

    if( !mdiPanels->isCanBeStartedByTime() )
    {
        switch( customMsgBox( this, MSG_ATTENTION,
                              tr("Reset device|Add to wait list ...|Cancel"),
                              tr("This device already prepared with a time period."),
                              tr("To start the device with other conditions, please\n"
                                 "reset the device first with pushing the ESC button.") ) )
        {
            case 1:
                if( mdiPanels->isStatusCanBeReseted() )
                {
                    m_lblStatusLeft.setText( m_qsStatusText );
                    mdiPanels->clear();
                }
                else
                {
                    return;
                }
                break;
            case 2:
                m_inPanelStartMinute = p_inMinute;
                on_action_UseDeviceLater_triggered();
                return;
                break;
            case 3:
            default:
                return;
        }
    }

    m_inPanelStartMinute = p_inMinute;
    if( mdiPanels->isPanelWorking( mdiPanels->activePanel() ) )
    {
        on_action_UseDeviceLater_triggered();
    }
    else
    {
        on_action_UseDevice_triggered();
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::on_action_EditActualPatient_triggered()
{
    cTracer obTrace( "cWndMain::on_action_EditActualPatient_triggered" );

    m_dlgProgress->showProgress();

    cDlgGuestEdit  obDlgEdit( this, &g_obGuest );
    obDlgEdit.setWindowTitle( g_obGuest.name() );

    m_dlgProgress->hideProgress();

    obDlgEdit.exec();
}
//====================================================================================
void cWndMain::on_action_DeviceSettings_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceSettings_triggered" );

    m_dlgProgress->showProgress();

    cDlgPanelSettings   obDlgEdit( this, mdiPanels->activePanelId() );

    m_dlgProgress->hideProgress();

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        mdiPanels->reload();
        g_obLogger(cSeverity::DEBUG) << QString::number( mdiPanels->activePanel() ) << EOM;
        m_dlgSecondaryWindow->refreshTitle( mdiPanels->activePanel() );
    }
}
//====================================================================================
void cWndMain::on_action_PayCash_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PayCash_triggered" );

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

    cCurrency cPrice( inPriceNet, cCurrency::CURR_GROSS, g_poPrefs->getDeviceUseVAT() );

    inPriceTotal = cPrice.currencyValue().toInt() - inPriceDiscount;

    cDBShoppingCart obDBShoppingCart;

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( 0 );
    obDBShoppingCart.setPatientCardId( 0 );
    obDBShoppingCart.setPatientCardTypeId( 0 );
    obDBShoppingCart.setPanelId( mdiPanels->activePanelId() );
    obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_DEVICE_USAGE );
    obDBShoppingCart.setItemName( tr("Using panel") );
    obDBShoppingCart.setItemCount( 1 );
    obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
    obDBShoppingCart.setItemVAT( g_poPrefs->getDeviceUseVAT() );
    obDBShoppingCart.setItemDiscount( inPriceDiscount );
    obDBShoppingCart.setItemSumPrice( inPriceTotal );

    cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

    obDlgCassaAction.setPayWithCash();

    int             inCassaAction   = obDlgCassaAction.exec();
    int             inPayType       = 0;
    QString         qsComment       = tr("Using device: %1").arg( mdiPanels->getActivePanelCaption() );
    bool            bShoppingCart   = false;
    unsigned int    uiCouponId = 0;
    cDBDiscount     obDBDiscount;

    obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

    if( inCassaAction == QDialog::Accepted && !bShoppingCart )
    {
        /*if( uiCouponId > 0 )
        {
            obDBDiscount.load( uiCouponId );
            obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
        }*/
        unsigned int uiLedgerId = g_obCassa.cassaProcessDeviceUse( obDBShoppingCart, qsComment, inPayType, mdiPanels->getPanelCaption(obDBShoppingCart.panelId()) );
        processDeviceUsePayment( obDBShoppingCart.panelId(), uiLedgerId, inPayType );
    }
    else if( inCassaAction == QDialog::Accepted && bShoppingCart )
    {
        mdiPanels->itemAddedToShoppingCart();
        mdiPanels->cashPayed( 0 );
    }
    m_bActionProcessing = false;
}
//====================================================================================
void cWndMain::processDeviceUsePayment( unsigned int p_uiPanelId, unsigned int p_uiLedgerId, int p_nPaymentType )
{
    cTracer obTrace( "cWndMain::processDeviceUsePayment" );

    mdiPanels->setPaymentMethod( p_uiPanelId, p_nPaymentType );
    mdiPanels->cashPayed( p_uiPanelId, p_uiLedgerId );
    mdiPanels->itemRemovedFromShoppingCart( p_uiPanelId );
}
//====================================================================================
void cWndMain::processProductSellPayment( const cDBShoppingCart &p_obDBShoppingCart )
{
    cTracer obTrace( "cWndMain::processProductSellPayment" );

    cDBShoppingCart obDBShoppingCart = p_obDBShoppingCart;

    cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

    obDlgCassaAction.setPayWithCash();

    int             inCassaAction   = obDlgCassaAction.exec();
    int             inPayType = 0;
    QString         qsComment = tr("Selling product: %1").arg( obDBShoppingCart.itemName() );
    bool            bShoppingCart = false;
    unsigned int    uiCouponId = 0;
    cDBDiscount     obDBDiscount;

    obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

    if( inCassaAction == QDialog::Accepted && !bShoppingCart )
    {
        /*if( uiCouponId > 0 )
        {
            obDBDiscount.load( uiCouponId );

            obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
        }*/

        g_obLogger(cSeverity::DEBUG) << "processProductSellPayment >> Name: " << obDBShoppingCart.itemName() <<
                                        " | Net: " << obDBShoppingCart.itemNetPrice() <<
                                        " | Count: " << obDBShoppingCart.itemCount() <<
                                        " | Disc.: " << obDBShoppingCart.itemDiscount() <<
                                        " | Cash: " << obDBShoppingCart.cash() <<
                                        " | Card: " << obDBShoppingCart.card() <<
                                        " | Vouc.: " << obDBShoppingCart.voucher() <<
                                        " | Sum: " << obDBShoppingCart.itemSumPrice() << EOM;

        g_obCassa.cassaProcessProductSell( obDBShoppingCart, qsComment, inPayType );
    }
}
//====================================================================================
void cWndMain::on_action_CassaHistory_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_CassaHistory_triggered" );

    m_dlgProgress->showProgress();

    cDlgReportCassaList  obDlgReportCassaList( this );

    m_dlgProgress->hideProgress();

    obDlgReportCassaList.exec();
}
//====================================================================================
void cWndMain::on_action_ReportPatientcardUses_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_ReportPatientcardUses_triggered" );

    m_dlgProgress->showProgress();

    cDlgReportCardUses  obDlgReportCardUses( this );

    m_dlgProgress->hideProgress();

    obDlgReportCardUses.exec();
}
//====================================================================================
void cWndMain::on_action_EditLicenceInformation_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_EditLicenceInformation_triggered" );

    dlgLicenceEdit  obDlgLicenceEdit( this );

    obDlgLicenceEdit.exec();
}
//====================================================================================
void cWndMain::on_action_ReportPatientcards_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_ReportPatientcards_triggered" );

    m_dlgProgress->showProgress();

    cDlgReportPatientCard  obDlgReportPatientCard( this );

    m_dlgProgress->hideProgress();

    obDlgReportPatientCard.exec();
}
//====================================================================================
void cWndMain::on_action_Discounts_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_Discounts_triggered" );

    m_dlgProgress->showProgress();

    cDlgDiscount  obDlgDiscount( this );

    m_dlgProgress->hideProgress();

    obDlgDiscount.exec();
}
//====================================================================================
void cWndMain::on_action_PatientcardsObsolete_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_PatientcardsObsolete_triggered" );

    m_dlgProgress->showProgress();

    cDlgReportPatientCardObs  obDlgReportPatientCardObs( this );

    m_dlgProgress->hideProgress();

    obDlgReportPatientCardObs.exec();
}
//====================================================================================
void cWndMain::on_action_EmptyDemoDB_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_EmptyDemoDB_triggered" );

    m_dlgProgress->showProgress();

    try
    {
        g_poDB->executeQTQuery( QString( "DELETE FROM cassaHistory WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM productHistory WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM ledger WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM ledgerDevice WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM shoppingCartItems WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM discounts WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM paymentMethods WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM cassaDenominations WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM cassa WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM address WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM publicPlaces WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM zipRegionCity WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientHistory WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientHistoryType WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM productActionType WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM connectProductWithType WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM products WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM productTypes WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM ledgerTypes WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM denominations WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientCardUnits WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientCardHistories WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM connectPatientWithCard WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientCards WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientCardTypeEnabled WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patientCardTypes WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM patients WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM companies WHERE licenceId=1" ) );
        g_poDB->executeQTQuery( QString( "DELETE FROM users WHERE licenceId=1 and userId>2" ) );

        m_dlgProgress->hideProgress();

        QMessageBox::information( this, tr("Information"),
                                  tr("Deleting data attached to DEMO licence key has been finished."));
    }
    catch( cSevException &e )
    {
        m_dlgProgress->hideProgress();

        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//====================================================================================
void cWndMain::slotStatusChanged( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId, const QString p_qsStatus )
{
    m_dlgSecondaryWindow->setPanelStatus( p_uiPanelId, p_uiPanelStatusId );
    m_dlgSecondaryWindow->setPanelStatusText( p_uiPanelId, p_qsStatus );
}
//====================================================================================
void cWndMain::slotSetCounterText( unsigned int p_uiPanelId, const QString &p_qsCounter )
{
    m_dlgSecondaryWindow->setCounterText( p_uiPanelId, p_qsCounter );
}
//====================================================================================
void cWndMain::slotSetWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime )
{
    m_dlgSecondaryWindow->setPanelWaitTime( p_uiPanelId, p_uiWaitTime );
}
//====================================================================================
void cWndMain::slotSetInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo )
{
    m_dlgSecondaryWindow->setPanelInfoText( p_uiPanelId, p_qsInfo );
}
//====================================================================================
void cWndMain::slotReplacePatientCard(const QString &p_qsBarcode)
{
    cTracer obTrace( "cWndMain::slotReplacePatientCard" );

    m_dlgProgress->showProgress();

    cDBPatientCard  obDBPatientCardOld;

    obDBPatientCardOld.load( p_qsBarcode );

    QMessageBox::warning( this, tr("Request"), tr("Please enter the new patientcard's barcode.") );

    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();

    m_dlgProgress->hideProgress();

    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        cDBPatientCard  obDBPatientCardNew;
        try
        {
            obDBPatientCardNew.load( obDlgInputStart.getEditText() );

            if( obDBPatientCardNew.pincode().compare("LOST") == 0 )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("This patientcard has been lost and replaced\nand can not be used or sold again.") );
                return;
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
                    obDBPatientCardNew.createNew();
                    obDBPatientCardNew.setLicenceId( g_poPrefs->getLicenceId() );
                    obDBPatientCardNew.setBarcode( obDlgInputStart.getEditText() );
                    obDBPatientCardNew.save();
                }
            }
        }

        if( obDBPatientCardNew.active() )
        {
            QMessageBox::warning( this, tr("Warning"), tr("This patientcard already in use.\n"
                                                          "Please select a non-active patientcard.") );
            return;
        }
        else
        {
            obDBPatientCardNew.setActive( true );
            obDBPatientCardNew.setPatientCardTypeId( obDBPatientCardOld.patientCardTypeId() );
            obDBPatientCardNew.setParentId( obDBPatientCardOld.parentId() );
            obDBPatientCardNew.setPatientId( obDBPatientCardOld.patientId() );
            obDBPatientCardNew.setUnits( obDBPatientCardOld.units() );
            obDBPatientCardNew.setTimeLeftStr( obDBPatientCardOld.timeLeftStr() );
            obDBPatientCardNew.setValidDateFrom( obDBPatientCardOld.validDateFrom() );
            obDBPatientCardNew.setValidDateTo( obDBPatientCardOld.validDateTo() );
            obDBPatientCardNew.setComment( obDBPatientCardOld.comment() );

            cCurrency cPrice( QString::number(g_poPrefs->getPatientCardLostPrice()/100) );

            cDBShoppingCart obDBShoppingCart;

            obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
            obDBShoppingCart.setGuestId( obDBPatientCardNew.patientId() );
            obDBShoppingCart.setProductId( 0 );
            obDBShoppingCart.setPatientCardId( obDBPatientCardNew.id() );
            obDBShoppingCart.setPatientCardTypeId( obDBPatientCardNew.patientCardTypeId() );
            obDBShoppingCart.setPanelId( 0 );
            obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PC_LOST_REPLACE );
            obDBShoppingCart.setItemName( QString("%1 -> %2").arg(obDBPatientCardOld.barcode()).arg(obDBPatientCardNew.barcode()) );
            obDBShoppingCart.setItemCount( 1 );
            obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
            obDBShoppingCart.setItemVAT( g_poPrefs->getPatientCardLostPriceVat() );
            obDBShoppingCart.setItemDiscount( 0 );
            obDBShoppingCart.setItemSumPrice( cPrice.currencyValue().toInt() );

            cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

            obDlgCassaAction.setPayWithCash();

            int             inCassaAction   = obDlgCassaAction.exec();
            int             inPayType       = 0;
            QString         qsComment       = tr("Replace patientcard [%1]->[%2]").arg(obDBPatientCardOld.barcode()).arg(obDBPatientCardNew.barcode());
            bool            bShoppingCart   = false;
            unsigned int    uiCouponId = 0;
            cDBDiscount     obDBDiscount;

            obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

            if( inCassaAction == QDialog::Accepted && !bShoppingCart )
            {
                /*if( uiCouponId > 0 )
                {
                    obDBDiscount.load( uiCouponId );

                    obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                }*/
                g_obCassa.cassaProcessPatientCardSell( obDBPatientCardNew, obDBShoppingCart, qsComment, true, inPayType );
            }
            else if( inCassaAction != QDialog::Accepted )
            {
                // Nem tortent meg az eladas
                return;
            }

            cDBPatientcardUnit  obDBPatientcardUnit;

            obDBPatientCardNew.save();
            obDBPatientcardUnit.setPatientCardId( obDBPatientCardOld.id() );
            obDBPatientcardUnit.replacePatientCard( obDBPatientCardNew.id() );

            // Nem kell törölni a kártyát, csak jelezni, hogy elveszett
            /*
            obDBPatientCardOld.setPatientCardTypeId( 0 );
            obDBPatientCardOld.setParentId( 0 );
            obDBPatientCardOld.setPatientId( 0 );
            obDBPatientCardOld.setUnits( 0 );
            obDBPatientCardOld.setAmount( 0 );
            obDBPatientCardOld.setTimeLeft( 0 );
            obDBPatientCardOld.setValidDateFrom( "2000-01-01" );
            obDBPatientCardOld.setValidDateTo( "2000-01-01" );
            */
            obDBPatientCardOld.setComment( tr("Lost and replaced with card: %1").arg(obDBPatientCardNew.barcode()) );
            obDBPatientCardOld.setPincode( "LOST" );
            obDBPatientCardOld.setActive( false );
            obDBPatientCardOld.save();
            obDBPatientCardNew.synchronizeUnits();
        }
    }
}
//====================================================================================
void cWndMain::slotAssignPartnerCard( const QString &p_qsBarcode )
{
    cTracer obTracer( "cWndMain::slotAssignPartnerCard" );

    cDlgPatientCardAssign obDlgPatientCardAssign( this, p_qsBarcode );

    if( obDlgPatientCardAssign.exec() == QDialog::Accepted )
    {
        QString qsBarcodeMain;
        QString qsBarcodeAssign;

        obDlgPatientCardAssign.getCardsBarcode( &qsBarcodeMain, &qsBarcodeAssign );

        try
        {
            cDBPatientCard  obDBPatientCardMain;
            cDBPatientCard  obDBPatientCardAssign;

            obDBPatientCardMain.load( qsBarcodeMain );
            obDBPatientCardAssign.load( qsBarcodeAssign );

            unsigned int uiTimeLeft = obDBPatientCardMain.timeLeft() + obDBPatientCardAssign.timeLeft();
            unsigned int uiUnits    = obDBPatientCardMain.units() + obDBPatientCardAssign.units();

            if( obDBPatientCardAssign.active() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("Are you sure about to merge these patientcards?\n"
                                              "Main card: %1\n"
                                              "Assigned card: %2").arg( obDBPatientCardMain.barcode() ).arg( obDBPatientCardAssign.barcode() ),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCardMain.setUnits( uiUnits );
                    obDBPatientCardMain.setTimeLeft( uiTimeLeft );
                    obDBPatientCardMain.save();

                    obDBPatientCardAssign.setParentId( obDBPatientCardMain.id() );
                    obDBPatientCardAssign.setUnits( uiUnits );
                    obDBPatientCardAssign.setTimeLeft( uiTimeLeft );
                    obDBPatientCardAssign.setComment( tr("Partner card of \"%1\"").arg(obDBPatientCardMain.barcode()) );
                    obDBPatientCardAssign.save();

                    cDBPatientcardUnit  obDBPatientcardUnit;

                    obDBPatientcardUnit.setPatientCardId( obDBPatientCardAssign.id() );
                    obDBPatientcardUnit.replacePatientCard( obDBPatientCardMain.id() );
                }
            }
            else
            {
                obDBPatientCardAssign.setActive( true );
                obDBPatientCardAssign.setPatientCardTypeId( obDBPatientCardMain.patientCardTypeId() );
                obDBPatientCardAssign.setParentId( obDBPatientCardMain.id() );
                obDBPatientCardAssign.setPatientId( 0 );
                obDBPatientCardAssign.setUnits( uiUnits );
                obDBPatientCardAssign.setTimeLeft( uiTimeLeft );
                obDBPatientCardAssign.setValidDateFrom( obDBPatientCardMain.validDateFrom() );
                obDBPatientCardAssign.setValidDateTo( obDBPatientCardMain.validDateTo() );
                obDBPatientCardAssign.setComment( tr("Partner card of \"%1\"").arg(obDBPatientCardMain.barcode()) );

                cCurrency cPrice( QString::number(g_poPrefs->getPatientCardPartnerPrice()/100) );

                cDBShoppingCart obDBShoppingCart;

                obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
                obDBShoppingCart.setGuestId( obDBPatientCardAssign.patientId() );
                obDBShoppingCart.setProductId( 0 );
                obDBShoppingCart.setPatientCardId( obDBPatientCardAssign.id() );
                obDBShoppingCart.setPatientCardTypeId( obDBPatientCardAssign.patientCardTypeId() );
                obDBShoppingCart.setPanelId( 0 );
                obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PC_ASSIGN_PARTNER );
                obDBShoppingCart.setItemName( QString("%1 -> %2").arg(obDBPatientCardMain.barcode()).arg(obDBPatientCardAssign.barcode()) );
                obDBShoppingCart.setItemCount( 1 );
                obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
                obDBShoppingCart.setItemVAT( g_poPrefs->getPatientCardLostPriceVat() );
                obDBShoppingCart.setItemDiscount( 0 );
                obDBShoppingCart.setItemSumPrice( cPrice.currencyValue().toInt() );

                cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

                obDlgCassaAction.setPayWithCash();

                int             inCassaAction   = obDlgCassaAction.exec();
                int             inPayType       = 0;
                QString         qsComment       = tr("Assign patientcard [%1]<-[%2]").arg(obDBPatientCardMain.barcode()).arg(obDBPatientCardAssign.barcode());
                bool            bShoppingCart   = false;
                unsigned int    uiCouponId = 0;
                cDBDiscount     obDBDiscount;

                obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

                if( inCassaAction == QDialog::Accepted && !bShoppingCart )
                {
                    /*if( uiCouponId > 0 )
                    {
                        obDBDiscount.load( uiCouponId );

                        obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                    }*/
                    g_obCassa.cassaProcessPatientCardSell( obDBPatientCardAssign, obDBShoppingCart, qsComment, true, inPayType );
                }
                else if( inCassaAction != QDialog::Accepted )
                {
                    // Nem tortent meg az eladas
                    return;
                }

                obDBPatientCardAssign.save();
            }
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

void cWndMain::on_action_PaymentMethods_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PaymentMethods_triggered" );

    m_dlgProgress->showProgress();

    cDlgPaymentMethod   obDlgPaymentMethod( this );

    m_dlgProgress->hideProgress();

    obDlgPaymentMethod.exec();
}

void cWndMain::on_action_TestDlgStarted()
{
    cTracer obTrace( "cWndMain::on_action_TestDlgStarted" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        cDlgTest    obDlgTest( this );

        obDlgTest.exec();
    }
}


void cWndMain::on_ledPassword_returnPressed()
{
    on_pbLogin_clicked();
}

void cWndMain::on_action_ReportViewer_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ReportViewer_triggered" );

    QProcess *qpReportViewer = new QProcess(this);

    if( !qpReportViewer->startDetached( QString("ReportViewer.exe %1 %2").arg(g_obUser.name()).arg(ledPassword->text()) ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:ReportViewer.exe\n\nError code: %1\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.").arg(qpReportViewer->error()) );
    }
}

void cWndMain::on_action_About_triggered()
{
    cTracer obTrace( "cWndMain::on_action_About_triggered" );

    QSqlQuery   *poQuery            = NULL;
    QString      qsLicenceString    = "";
    QString      qsInfoLink         = "";
    QString      qsAppVersion       = "";
    QString      qsDbVersion        = "";

    poQuery = g_poDB->executeQTQuery( QString( "SELECT serial FROM licences WHERE licenceId=%1" ).arg( g_poPrefs->getLicenceId() ) );
    poQuery->first();
    qsLicenceString = poQuery->value( 0 ).toString();

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM settings " ) );
    while( poQuery->next() )
    {
        QString qsIdentifier = poQuery->value(1).toString();

        if( qsIdentifier.compare("ABOUT_INFO_LINK") == 0 )
            qsInfoLink = poQuery->value( 2 ).toString();
        else if( qsIdentifier.compare("APPLICATION_VERSION") == 0 )
            qsAppVersion = poQuery->value( 2 ).toString();
        else if( qsIdentifier.compare("DATABASE_VERSION") == 0 )
            qsDbVersion = poQuery->value( 2 ).toString();
    }

    qsAppVersion.replace( "_", "." );
    qsDbVersion.replace( "_", "." );

    QMessageBox::information( this, tr("About"),
                              tr("<h2>Belenus Software Application System</h2>"
                                 "<p>"
                                 "Application version : %1<br>"
                                 "Database version : %2"
                                 "<p>"
                                 "Copyright 2013 Tamas Bikfalvi. All rights reserved."
                                 "<p>"
                                 "Application Licence : %3"
                                 "<p>"
                                 "The application and all of its related products<br>"
                                 "is the property of KiwiSun Franchise.<br>"
                                 "For more information visit the <a href=\"%4\">KiwiSun website</a>"
                                 ).arg( qsAppVersion ).arg( qsDbVersion ).arg( qsLicenceString ).arg( qsInfoLink ) );
}
//====================================================================================
int cWndMain::customMsgBox(QWidget *parent, msgBoxType msgtype, QString buttonstext, QString msg, QString details)
//====================================================================================
{
    QMessageBox msgBox;

    switch(msgtype)
    {
        case MSG_INFORMATION:
            msgBox.setWindowTitle( QObject::tr("Information") );
            msgBox.setIcon( QMessageBox::Information );
            break;
        case MSG_WARNING:
            msgBox.setWindowTitle( QObject::tr("Warning") );
            msgBox.setIcon( QMessageBox::Warning );
            break;
        case MSG_ATTENTION:
            msgBox.setWindowTitle( QObject::tr("Attention") );
            msgBox.setIcon( QMessageBox::Warning );
            break;
        case MSG_ERROR:
            msgBox.setWindowTitle( QObject::tr("Error") );
            msgBox.setIcon( QMessageBox::Critical );
            break;
        case MSG_QUESTION:
            msgBox.setWindowTitle( QObject::tr("Question") );
            msgBox.setIcon( QMessageBox::Question );
            break;
    }

    msgBox.setText( msg );
    if( details.length() > 0 ) msgBox.setDetailedText( details );

    QList<QPushButton*> qlButtons;
    QStringList         qslButtons = buttonstext.split('|');

    for( int i=0; i<qslButtons.size(); i++ )
    {
        QPushButton *pButton = msgBox.addButton( qslButtons.at(i), QMessageBox::ActionRole );
        qlButtons.append( pButton );
    }

    msgBox.exec();

    int nRet = 0;

    for( int i=0; i<qslButtons.size(); i++ )
    {
        if( msgBox.clickedButton() == (QAbstractButton*)qlButtons.at(i) )
        {
            nRet = i+1;
            break;
        }
    }

    return nRet;
}

void cWndMain::on_action_ManageDatabase_triggered()
{
    cDlgManageDatabase  obDlgManageDatabase(this);

    obDlgManageDatabase.exec();
}

void cWndMain::on_action_ManageDevicePanels_triggered()
{
    cDlgPanels  obDlgPanels( this );

    obDlgPanels.exec();
}


void cWndMain::on_action_ManageSkinTypes_triggered()
{
    cDlgSkinTypes obDlgSkinTypes( this );

    obDlgSkinTypes.exec();
}

void cWndMain::on_action_Import_triggered()
{
    QMessageBox::warning( this, tr("Attention"), tr("Currently not allowed to use. Please contact your franchise provider.") );

//    cDlgExportImport    obDlgExportImport( this, cDlgExportImport::PT_IMPORT );

//    obDlgExportImport.exec();
}

void cWndMain::on_action_Export_triggered()
{
    QMessageBox::warning( this, tr("Attention"), tr("Use report viewer application to display and export program related data.") );

//    cDlgExportImport    obDlgExportImport( this, cDlgExportImport::PT_EXPORT );

//    obDlgExportImport.exec();
}
