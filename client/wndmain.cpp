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
#include <QMenu>

#include "wndmain.h"
#include "licenceManager.h"
#include "../framework/logger/DatabaseWriter.h"
#include "cdlgtest.h"
#include "belenus.h"
#include "licenceManager.h"

//====================================================================================

#include "db/dbpatientcard.h"
#include "db/dbledger.h"
#include "db/dbshoppingcart.h"
#include "db/dbpatientcardunits.h"
#include "db/dbdiscount.h"
#include "db/dbwaitlist.h"

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
#include "crud/dlgadvertisements.h"
#include "crud/dlgpatientcardselect.h"

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
#include "dlg/dlgcomment.h"

//====================================================================================

#include "report/repledgermain.h"
#include "report/repcassalist.h"
#include "report/repcarduses.h"
#include "report/reppatientcards.h"
#include "report/reppatientcardsobs.h"

//====================================================================================

extern DatabaseWriter   g_obLogDBWriter;
extern cLicenceManager  g_obLicenceManager;

//====================================================================================
cWndMain::cWndMain( QWidget *parent ) : QMainWindow( parent )
{
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
    m_bProgressErrorVisible         = false;
    m_nProgressCounter              = 0;
    m_bBlnsHttpConnected            = false;
    m_nCommunicationErrorCounter    = 0;
    m_nCommResetStep                = 0;
    m_bBlnsHttpErrorVisible         = false;
    m_uiBlnsErrorAppeared           = 0;
    m_bClosingShift                 = false;
    m_bShoppingCartHasItem          = g_obGen.isShoppingCartHasItems();
    m_nHttpCommCounter              = 0;
    m_bMainWindowActive             = false;

    pbLogin->setIcon( QIcon("./resources/40x40_ok.png") );

    frmLogin->setVisible( false );
    frmLogin->setEnabled( false );

    showAdWindows();

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
    connect( mdiPanels, SIGNAL( signalMainWindowActivated()),            this, SLOT( slotMainWindowActivated() ) );

    updateTitle();
    setWindowIcon( QIcon("./resources/belenus.ico") );

    //--------------------------------------------------------------------------------
    // Toolbar buttons
    //--------------------------------------------------------------------------------
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );
    action_LogOut->setIcon( QIcon("./resources/40x40_lock.png") );
    action_CloseShift->setIcon( QIcon("./resources/40x40_exit.png") );

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

    action_PatientcardInformation->setIcon( QIcon("./resources/40x40_patientcard_info.png") );
    action_PatientCardAssign->setIcon( QIcon("./resources/40x40_patientcard_assign.png") );
    action_ReplaceLostCard->setIcon( QIcon("./resources/40x40_patientcard_replace.png") );

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
        action_Advertisements->setIcon( QIcon( "./resources/40x40_advertisement.png" ) );
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
    action_CloseShift->setEnabled( false );

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

    action_PatientcardInformation->setEnabled( false );
    action_PatientCardAssign->setEnabled( false );
    action_ReplaceLostCard->setEnabled( false );

    action_PatientCardSell->setEnabled( false );
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

    action_Advertisements->setEnabled( false );

    showElementsForComponents();

    m_dlgSecondaryWindow = new cDlgSecondaryWindow( this );

    m_dlgSecondaryWindow->initPanels();
    m_dlgSecondaryWindow->setCaption( tr("Belenus Software System - %1 - (c) Pagony Multimedia Studio Bt.").arg( g_poPrefs->getVersion() ) );

    if( g_poPrefs->isSecondaryWindowVisible() )
    {
        m_dlgSecondaryWindow->move( g_poPrefs->secondaryWindowPosition() );
        m_dlgSecondaryWindow->resize( g_poPrefs->secondaryWindowSize() );
        m_dlgSecondaryWindow->show();
        this->setFocus();
    }

    m_lblStatusRight.setAlignment( Qt::AlignRight );
    m_lblStatusRight.setStyleSheet( "QLabel {font:bold; font-size:14px;}" );

    m_pbStatusCommunication.setIcon( QIcon( "./resources/77x40_off.png" ) );
    m_pbStatusCommunication.setFlat( true );
    m_pbStatusCommunication.setText( "" );
    m_pbStatusCommunication.setIconSize( QSize(40,20) );
    m_pbStatusCommunication.setFixedSize( 42, 22 );

    connect( &m_pbStatusCommunication, SIGNAL(clicked()), this, SLOT(on_CommunicationButtonClicked()) );

    m_pbStatusCommunication.setEnabled( false );

    m_pbStatusKeyboard.setIcon( QIcon( "./resources/40x40_keyboard.png" ) );
    m_pbStatusKeyboard.setFlat( true );
    m_pbStatusKeyboard.setText( "" );
    m_pbStatusKeyboard.setIconSize( QSize(20,20) );
    m_pbStatusKeyboard.setFixedSize( 22, 22 );

    connect( &m_pbStatusKeyboard, SIGNAL(clicked()), this, SLOT(on_KeyboardEnabled()) );

    m_pbStatusKeyboard.setEnabled( false );

    if( !g_poPrefs->isFapados() )
    {
        m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_disabled.png" ) );
        m_pbStatusHttp.setFlat( true );
        m_pbStatusHttp.setText( "" );
        m_pbStatusHttp.setIconSize( QSize(20,20) );
        m_pbStatusHttp.setFixedSize( 22, 22 );

        m_lblHttpCount.setStyleSheet( "QLabel {font-size:8px;}" );

        connect( &m_pbStatusHttp, SIGNAL(clicked()), this, SLOT(on_BlnsHttpIconClicked()) );

        m_pbStatusCommunicationSuspended.setIcon( QIcon( "./resources/40x40_minus.png" ) );
        m_pbStatusCommunicationSuspended.setFlat( true );
        m_pbStatusCommunicationSuspended.setText( "" );
        m_pbStatusCommunicationSuspended.setIconSize( QSize(20,20) );
        m_pbStatusCommunicationSuspended.setFixedSize( 22, 22 );
    }

    statusbar->addPermanentWidget( &m_lblStatusLeft, 3 );
    statusbar->addPermanentWidget( &m_pbStatusCommunication, 0 );
    statusbar->addPermanentWidget( &m_pbStatusKeyboard, 0 );
    if( !g_poPrefs->isFapados() )
    {
        statusbar->addPermanentWidget( &m_pbStatusHttp, 0 );
        statusbar->addPermanentWidget( &m_pbStatusCommunicationSuspended, 0 );
        statusbar->addPermanentWidget( &m_lblHttpCount );
    }
    statusbar->addPermanentWidget( &m_lblStatusRight, 1 );

    g_poBlnsHttp = new cBlnsHttp();

    if( !g_poPrefs->isFapados() )
    {
        connect( g_poBlnsHttp, SIGNAL(signalErrorOccured()),            this, SLOT(on_BlnsHttpErrorOccured()) );
        connect( g_poBlnsHttp, SIGNAL(signalActionProcessed(QString)),  this, SLOT(on_BlnsHttpActionFinished(QString)) );
        connect( g_poBlnsHttp, SIGNAL(signalStepProgress()),            this, SLOT(on_BlnsHttpStepProgress()) );
        connect( g_poBlnsHttp, SIGNAL(signalHideProgress()),            this, SLOT(on_BlnsHttpHideProgress()) );
        connect( g_poBlnsHttp, SIGNAL(signalHttpProcessDisabled()),     this, SLOT(on_BlnsHttpProcessStopped()) );
        connect( g_poBlnsHttp, SIGNAL(signalHttpProcessSuspended()),    this, SLOT(on_BlnsHttpProcessStopped()) );

        g_poBlnsHttp->setTimeout( g_poPrefs->getBlnsHttpMessageWaitTime()*1000 );

        m_lblHttpCount.setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
        m_pbStatusHttp.setToolTip( tr("Number of records to process: %1")
                                   .arg( m_lblHttpCount.text() ) );
    }

    this->setFocus();
}
//====================================================================================
cWndMain::~cWndMain()
{
    m_bMainWindowActive = false;

    QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( QString( "SELECT advertisementId FROM advertisements" ) );

    while( poQuery->next() )
    {
        obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Command" ).arg( poQuery->value(0).toInt() ), "EXIT" );
    }

    delete m_dlgProgress;
    delete m_dlgSecondaryWindow;

    killTimer( m_nTimer );
}
//====================================================================================
void cWndMain::startMainTimer()
{
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

    _checkVersions();
    _checkIsActivationNeeded();

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
        if( g_poPrefs->isBlnsHttpEnabled() )
        {
            g_poBlnsHttp->checkHttpServerAvailability();
        }
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

    m_bMainWindowActive = true;

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

    m_pbStatusKeyboard.setIcon( QIcon( "./resources/40x40_keyboard_enabled.png" ) );
}
//====================================================================================
void cWndMain::logoutUser()
{
    cTracer obTrace( "cWndMain::logoutUser" );

    if( g_obCassa.isCassaEnabled() )
    {
        if( !m_bClosingShift && !g_poPrefs->getCassaAutoClose() )
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
    m_bMainWindowActive = false;
}
//====================================================================================
void cWndMain::keyPressEvent( QKeyEvent *p_poEvent )
{
    m_nHttpCommCounter = 0;
    g_poPrefs->setBlnsHttpSuspended( true );
    m_pbStatusCommunicationSuspended.setIcon( QIcon( "./resources/40x40_minus.png" ) );
    setCursor( Qt::ArrowCursor);
    m_bMainWindowActive = true;

    if( !g_obUser.isLoggedIn() )
    {
        m_bMainWindowActive = false;
        return;
    }
    else if( m_bActionProcessing && ( p_poEvent->key() == Qt::Key_Enter || p_poEvent->key() == Qt::Key_Return ) )
    {
        g_obLogger(cSeverity::DEBUG) << "Keypress (ENTER) rejected "
                                     << "Action processing ["
                                     << m_bActionProcessing
                                     << "]"
                                     << EOM;
        return;
    }

    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = true;
        _setStatusText( tr("Q -> Exit application | F -> pay device usage | S -> start device | N -> skip status | T -> device cleared | K -> open shopping kart") );
    }

    if( m_bCtrlPressed )
    {
        if( p_poEvent->key() == Qt::Key_Q )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + Q" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            close();
        }
        else if( p_poEvent->key() == Qt::Key_S && action_DeviceStart->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + S" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            on_KeyboardDisabled();
            on_action_DeviceStart_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_T && action_DeviceClear->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + T" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            on_KeyboardDisabled();
            on_action_DeviceClear_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_F && action_PayCash->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + F" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            on_KeyboardDisabled();
            on_action_PayCash_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_K && action_ShoppingCart->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + K" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            on_KeyboardDisabled();
            on_action_ShoppingCart_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_N && action_DeviceSkipStatus->isEnabled() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + N" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
            on_KeyboardDisabled();
            on_action_DeviceSkipStatus_triggered();
        }
        else if( p_poEvent->key() == Qt::Key_F12 )
        {
            g_obLogger(cSeverity::INFO) << "User pressed CTRL + F12" << EOM;
            m_bCtrlPressed = false;
            _setStatusText( m_qsStatusText );
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
                    _setStatusText( m_qsStatusText );
                    on_KeyboardDisabled();
                    on_action_PayCash_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_START:
                    _setStatusText( m_qsStatusText );
                    on_KeyboardDisabled();
                    on_action_DeviceStart_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_SKIP:
                    _setStatusText( m_qsStatusText );
                    on_KeyboardDisabled();
                    on_action_DeviceSkipStatus_triggered();
                    break;

                case cDBApplicationAction::APPACT_DEVICE_CLEAN:
                    _setStatusText( m_qsStatusText );
                    on_KeyboardDisabled();
                    on_action_DeviceClear_triggered();
                    break;
            }
        }
        else if( ((p_poEvent->key() >= Qt::Key_0 && p_poEvent->key() <= Qt::Key_9) ||
                  (p_poEvent->key() >= Qt::Key_A && p_poEvent->key() <= Qt::Key_Z)) )
        {
            _setStatusText( m_qsStatusText );

            cDlgInputStart  obDlgInputStart( this );

            obDlgInputStart.setInitialText( p_poEvent->text() );
            if( obDlgInputStart.exec() == QDialog::Accepted )
            {
                if( obDlgInputStart.m_bCard )
                {
                    on_KeyboardDisabled();
                    processInputPatientCard( obDlgInputStart.getEditText() );
                }
                else if( obDlgInputStart.m_bProd )
                {
                    on_KeyboardDisabled();
                    processInputProduct( obDlgInputStart.getEditText() );
                }
                else if( obDlgInputStart.m_bTime )
                {
                    on_KeyboardDisabled();
                    processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
                }
            }

            p_poEvent->ignore();
        }
        else if( p_poEvent->key() == Qt::Key_Escape && mdiPanels->isStatusCanBeReseted() )
        {
            g_obLogger(cSeverity::INFO) << "User pressed ESC" << EOM;
            _setStatusText( m_qsStatusText );
            mdiPanels->clear();
            m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();
        }
        else if( p_poEvent->key() == Qt::Key_Space )
        {
            g_obLogger(cSeverity::INFO) << "User pressed SPACE" << EOM;
            on_KeyboardDisabled();
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

    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = false;
        _setStatusText( m_qsStatusText );
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

    if( g_poPrefs->isFapados() )
    {
        action_Export->setEnabled( false );
        action_Import->setEnabled( false );
        menu_ExportImport->setEnabled( false );

        action_Company->setEnabled( false );
        action_HealthInsurance->setEnabled( false );
        action_RegionZipCity->setEnabled( false );
        action_ManageSkinTypes->setEnabled( false );
        action_Advertisements->setEnabled( false );

        action_UseDeviceLater->setEnabled( false );
    }
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
void cWndMain::updateStatusText( QString /*p_qsStatusText*/ )
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

    _setStatusText( m_qsStatusText );
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
    action_CloseShift->setEnabled( !mdiPanels->isPanelWorking() && bIsUserLoggedIn );

    menu_Edit->setEnabled( bIsUserLoggedIn );
        action_Guests->setEnabled( bIsUserLoggedIn );
        action_CardTypes->setEnabled( bIsUserLoggedIn );
        action_Cards->setEnabled( bIsUserLoggedIn );
        menuAdministrator->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
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
            action_ManageDevicePanels->setEnabled( !mdiPanels->isPanelWorking() );
            action_Advertisements->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) );
        action_Preferences->setEnabled( bIsUserLoggedIn );

    menu_Action->setEnabled( bIsUserLoggedIn );
        menuDevice->setEnabled( bIsUserLoggedIn );
            action_UseDevice->setEnabled( bIsUserLoggedIn && (mdiPanels->isCanBeStartedByCard() || mdiPanels->isCanBeStartedByTime()) );
            action_UseDeviceLater->setEnabled( bIsUserLoggedIn );
            action_DeviceClear->setEnabled( bIsUserLoggedIn && mdiPanels->isNeedToBeCleaned() );
            action_DeviceStart->setEnabled( bIsUserLoggedIn && ((!mdiPanels->isPanelWorking(mdiPanels->activePanel()) && mdiPanels->mainProcessTime() > 0) || mdiPanels->isDeviceStopped() ) );
            action_DeviceSkipStatus->setEnabled( bIsUserLoggedIn && mdiPanels->isStatusCanBeSkipped(mdiPanels->activePanel()) );
//            action_DeviceReset->setEnabled( bIsUserLoggedIn && mdiPanels->isMainProcess() );
            action_DeviceReset->setEnabled( bIsUserLoggedIn && mdiPanels->isStatusCanBeStopped(mdiPanels->activePanel()) );
            action_ManageDatabase->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::ADMIN) && !mdiPanels->isPanelWorking() );
            action_ManageDevicePanels->setEnabled( bIsUserLoggedIn && g_obUser.isInGroup(cAccessGroup::SYSTEM) );
        menuPatientCard->setEnabled( bIsUserLoggedIn );
            action_PatientcardInformation->setEnabled( bIsUserLoggedIn );
            action_PatientCardSell->setEnabled( bIsUserLoggedIn );
            action_PatientCardAssign->setEnabled( bIsUserLoggedIn );
            action_ReplaceLostCard->setEnabled( bIsUserLoggedIn );
            action_PCSaveToDatabase->setEnabled( bIsUserLoggedIn );
        menuProduct->setEnabled( bIsUserLoggedIn );
            action_SellProduct->setEnabled( bIsUserLoggedIn );
        menuCassa->setEnabled( bIsUserLoggedIn );
            action_CassaActionStorno->setEnabled( bIsUserLoggedIn );

    menu_Reports->setEnabled( bIsUserLoggedIn );

    menu_View->setEnabled( bIsUserLoggedIn );
        action_Cassa->setEnabled( bIsUserLoggedIn && g_obCassa.isCassaEnabled() );
action_Logs->setVisible( false );
        action_Logs->setEnabled( false /*bIsUserLoggedIn*/ );
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

    m_pbStatusKeyboard.setEnabled( bIsUserLoggedIn );
    if( !bIsUserLoggedIn )
    {
        m_pbStatusKeyboard.setIcon( QIcon( "./resources/40x40_keyboard.png" ) );
    }
    m_pbStatusHttp.setEnabled( bIsUserLoggedIn );
    m_pbStatusCommunicationSuspended.setEnabled( bIsUserLoggedIn );

    showElementsForComponents();
}
//====================================================================================
void cWndMain::timerEvent(QTimerEvent *)
{
    switch( m_nCommResetStep )
    {
        case 1:
            m_pbStatusCommunication.setIcon( QIcon( "./resources/77x40_off.png" ) );
            g_poHardware->closeCommunication();
            g_poHardware->ModuleTurnOff();
            m_nCommResetStep++;
            break;
        case 2:
            m_nCommResetStep++;
            break;
        case 3:
            g_poHardware->ModuleTurnOn();
            g_poHardware->init( g_poPrefs->getCommunicationPort() );
            m_pbStatusCommunication.setIcon( QIcon( "./resources/77x40_on.png" ) );
            m_nCommResetStep++;
            break;
        default:
            m_nCommResetStep = 0;
            break;
    }

    // Suspend patientcard synchronization if panels are in use
    bool bIsPanelWorking    = mdiPanels->isPanelWorking();
    int  nCountHttpRecord   = m_lblHttpCount.text().toInt();
    int  nHttpSyncAutoSecs  = g_poPrefs->getStartHttpSyncAutoSeconds()*4+2;

    if( bIsPanelWorking || !m_bMainWindowActive )
    {
        m_nHttpCommCounter = 0;
        g_poPrefs->setBlnsHttpSuspended( true );
        m_pbStatusCommunicationSuspended.setIcon( QIcon( "./resources/40x40_minus.png" ) );
        setCursor( Qt::ArrowCursor);
    }
    else
    {
        m_nHttpCommCounter++;
    }

    // 15 masodpercenkent 250 tizedmasodperces timer -> 4 * 15 + 2 = 60
    // 15 -> ini file-bol jon
    if( g_poPrefs->isStartHttpSyncAuto() &&
        m_nHttpCommCounter == nHttpSyncAutoSecs )
    {
        g_poPrefs->setBlnsHttpSuspended( false );
        m_pbStatusCommunicationSuspended.setIcon( QIcon( "./resources/40x40_ok.png" ) );
        if( nCountHttpRecord > 0 )
        {
            _processHttpActions();
        }
    }


    QFile   fileCheck( "belenus.chk" );

    if( fileCheck.size() > 0 )
    {
        fileCheck.open( QIODevice::WriteOnly );
        fileCheck.write( "" );
        fileCheck.close();
    }
    updateStatusText();
    updateToolbar();

    if( m_bProgressErrorVisible )
    {
        m_nProgressCounter--;
    }
    if( m_nProgressCounter < 1 )
    {
        m_bProgressErrorVisible = false;
        m_dlgProgress->hide();
    }

    m_inCommunicationCounter++;

    if( m_inCommunicationCounter > 4 )
    {
        m_inCommunicationCounter = 0;

        if( g_poHardware->isCommunicationStopped() )
        {
            m_nCommunicationErrorCounter++;

            if( m_nCommunicationErrorCounter > 2 )
            {
                _resetCommunication();
            }
            else if( m_nCommunicationErrorCounter > 4 )
            {
                m_pbStatusCommunication.setIcon( QIcon( "./resources/77x40_off.png" ) );
                g_obLogger(cSeverity::WARNING) << "Communication stopped with hardware controller" << EOM;
    //            m_dlgProgress->showError( tr("Communication stopped with hardware controller") );
                _setStatusText( tr("Communication stopped with hardware controller"), true );
            }
        }
        else
        {
            m_pbStatusCommunication.setIcon( QIcon( "./resources/77x40_on.png" ) );
        }

        m_lblStatusRight.setText( QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss  " ) );
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

    if( m_bShoppingCartHasItem )
    {
        mdiPanels->itemAddedToShoppingCart();
    }
    else
    {
        mdiPanels->itemRemovedFromShoppingCart();
    }

    updateTitle();
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
        if( m_bClosingShift ||
            QMessageBox::question( this, tr("Question"),
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

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;

    if( g_poPrefs->isBlnsHttpEnabled() )
    {
        m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_enabled.png" ) );
    }
    else
    {
        m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_disabled.png" ) );
    }
}
//====================================================================================
void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgUsers  obDlgUsers( this );

    m_dlgProgress->hideProgress();

    obDlgUsers.exec();

    m_bMainWindowActive = true;

    updateTitle();  //needed in case the login or real name of current user changed
}
//====================================================================================
void cWndMain::on_action_Logs_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Logs_triggered" );

    m_bMainWindowActive = false;

    cDlgLogs  obDlgLogs( this );

    m_bMainWindowActive = true;

    obDlgLogs.exec();
}
//====================================================================================
void cWndMain::on_action_Hardwaretest_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Hardwaretest_triggered" );

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
}

//====================================================================================
void cWndMain::on_action_LogOut_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Log_Out_triggered" );

    m_bMainWindowActive = false;

    logoutUser();

    g_obLogger(cSeverity::INFO) << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged out" << EOM;

    g_obUser.logOut();
    g_obLogDBWriter.setAppUser( 0 );
    updateTitle();

    if( !showLogIn() ) close();
}
//====================================================================================
void cWndMain::on_action_CloseShift_triggered()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to close the current shift?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        m_bMainWindowActive = false;
        m_bClosingShift = true;

        on_action_LogOut_triggered();
        on_action_ReportViewer_triggered();
        close();

        m_bClosingShift = false;
    }
}
//====================================================================================
void cWndMain::on_action_Panelgroups_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Panelgroups_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPanelGroups  obDlgPanelGroups( this );

    m_dlgProgress->hideProgress();

    obDlgPanelGroups.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Paneltypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Paneltypes_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPanelTypes  obDlgPanelTypes( this );

    m_dlgProgress->hideProgress();

    obDlgPanelTypes.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Guests_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Guests_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgGuest  obDlgGuest( this );

    m_dlgProgress->hideProgress();

    obDlgGuest.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_PatientNew_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientNew_triggered" );

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_DeviceClear_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceClear_triggered" );

    if( mdiPanels->isPanelWorking( mdiPanels->activePanel() ) )
    {
        on_KeyboardEnabled();
        return;
    }

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
    on_KeyboardEnabled();
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
    }
    on_KeyboardEnabled();
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

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPanelStatuses   obDlgPanelStatuses( this );

    m_dlgProgress->hideProgress();

    obDlgPanelStatuses.exec();

    if( obDlgPanelStatuses.isStatusChanged() )
    {
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );
    }

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_UseDevice_triggered()
{
    cTracer obTrace( "cWndMain::on_action_UseDevice_triggered" );

    m_bMainWindowActive = false;

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
                    on_KeyboardEnabled();                    
                    m_bMainWindowActive = true;
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

            g_obLogger(cSeverity::DEBUG) << "Device prepare with card Id:"
                                         << obDlgPanelUse.panelUsePatientCardId()
                                         << " Units:"
                                         << obDlgPanelUse.panelUnitIds().join("|")
                                         << EOM;

            mdiPanels->setMainProcessTime( obDlgPanelUse.panelUsePatientCardId(), obDlgPanelUse.panelUnitIds(), obDlgPanelUse.panelUseSecondsCard() );

            int nCount = obDlgPanelUse.countPatientCardUnitsLeft();
            mdiPanels->setTextInformation( tr( "%1 units left on the selected card" ).arg(nCount) );
        }
        if( obDlgPanelUse.panelUseSecondsCash() > 0 )
        {
            mdiPanels->setMainProcessTime( obDlgPanelUse.panelUseSecondsCash(), obDlgPanelUse.panelUsePrice() );
        }
    }
    on_KeyboardEnabled();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_UseDeviceLater_triggered()
{
    cTracer obTrace( "cWndMain::on_action_UseDeviceLater_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();
    cDlgPanelUse obDlgPanelUse( this, mdiPanels->activePanelId() );
    m_dlgProgress->hideProgress();

    if( obDlgPanelUse.exec() == QDialog::Accepted )
    {
        int             inLengthCash    = 0;
        int             inPrice         = 0;
        unsigned int    uiPatientCardId = 0;
        QString         qsBarcode       = "";
        QString         qsUnitIds       = "";
        int             inLengthCard    = 0;
        unsigned int    uiLedgerId      = 0;
        int             inPayType       = 0;
        unsigned int    uiPanelTypeId   = obDlgPanelUse.panelTypeId();

        if( obDlgPanelUse.panelUsePatientCardId() > 0 && obDlgPanelUse.panelUseSecondsCard() > 0 )
        {
            uiPatientCardId = obDlgPanelUse.panelUsePatientCardId();
            qsBarcode = obDlgPanelUse.panelUsePatientCardBarcode();
            qsUnitIds = obDlgPanelUse.panelUnitIds().join( "|" );
            inLengthCard = obDlgPanelUse.panelUseSecondsCard();
        }
        if( obDlgPanelUse.panelUseSecondsCash() > 0 )
        {
            inLengthCash = obDlgPanelUse.panelUseSecondsCash();
            inPrice = obDlgPanelUse.panelUsePrice();
        }

        unsigned int uiShoppingCartItemId = 0;

        if( inPrice > 0 )
        {
            if( !g_obCassa.isCassaEnabled() )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("Cassa is disabled!\n\n"
                                         "Please relogin to enable cassa.") );

                m_bMainWindowActive = true;
                return;
            }

            int                 inPriceTotal;
            int                 inPriceNet = inPrice;

            cCurrency cPrice( inPriceNet, cCurrency::CURR_GROSS, g_poPrefs->getDeviceUseVAT() );

            inPriceTotal            = cPrice.currencyValue().toInt();
            int nTimezoneDiscount   = 0;

            try
            {
                cDBDiscount obDiscount;

                obDiscount.loadTimeZone();
                nTimezoneDiscount = obDiscount.discount( inPriceTotal );
            }
            catch( cSevException &e )
            {
        //        g_obLogger(e.severity()) << e.what() << EOM;
            }

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
            obDBShoppingCart.setItemDiscount( nTimezoneDiscount );
            obDBShoppingCart.setItemSumPrice( inPriceTotal-nTimezoneDiscount );

            cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

            obDlgCassaAction.setPayWithCash();

            int             inCassaAction   = obDlgCassaAction.exec();
            QString         qsComment       = tr("Using device later");
            bool            bShoppingCart   = false;
            unsigned int    uiCouponId      = 0;
//            cDBDiscount     obDBDiscount;

            obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

            if( inCassaAction == QDialog::Accepted && !bShoppingCart )
            {
                /*if( uiCouponId > 0 )
                {
                    obDBDiscount.load( uiCouponId );

                    obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                }*/
                uiLedgerId = g_obCassa.cassaProcessDeviceUse( obDBShoppingCart, qsComment, inPayType, mdiPanels->getPanelCaption(obDBShoppingCart.panelId()) );
            }
            else if( inCassaAction == QDialog::Accepted && bShoppingCart )
            {
                //mdiPanels->itemAddedToShoppingCart();
                uiShoppingCartItemId = obDBShoppingCart.id();
            }
        }

        dlgComment  obDlgComment( this, tr("Enter comment") );
        QString     qsComment = "";

        if( obDlgComment.exec() == QDialog::Accepted )
        {
            qsComment = obDlgComment.resultComment();
        }

        cDBWaitlist obDBWaitlist;

        obDBWaitlist.setLicenceId( g_poPrefs->getLicenceId() );
        obDBWaitlist.setPatientCardId( uiPatientCardId );
        obDBWaitlist.setLedgerId( uiLedgerId );
        obDBWaitlist.setShoppingCartItemId( uiShoppingCartItemId );
        obDBWaitlist.setPanelTypeId( uiPanelTypeId );
        obDBWaitlist.setBarcode( qsBarcode );
        obDBWaitlist.setUnitIds( qsUnitIds );
        obDBWaitlist.setLengthCash( inLengthCash );
        obDBWaitlist.setLengthCard( inLengthCard );
        obDBWaitlist.setUseTime( inLengthCash );
        obDBWaitlist.setUsePrice( inPrice );
        obDBWaitlist.setComment( qsComment );
        obDBWaitlist.save();

        QStringList qslUnitIds = qsUnitIds.split( '|' );

        for( int i=0; i<qslUnitIds.count(); i++ )
        {
            cDBPatientcardUnit obDBPatientcardUnit;

            obDBPatientcardUnit.load( qslUnitIds.at(i).toInt() );
            obDBPatientcardUnit.setPrepared( true );
            obDBPatientcardUnit.save();
        }

        mdiPanels->addPatientToWaitingQueue( true );
//        mdiPanels->addPatientToWaitingQueue( inLengthCash, inPrice, uiPatientCardId, qsUnitIds, inLengthCard, uiLedgerId, inPayType );

        m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();
    }

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Cards_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Cards_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPatientCard obDlgPatientCard( this );

    connect( &obDlgPatientCard, SIGNAL(signalReplacePatientCard(QString)), this, SLOT(slotReplacePatientCard(QString)) );
    connect( &obDlgPatientCard, SIGNAL(signalAssignPartnerCard(QString)), this, SLOT(slotAssignPartnerCard(QString)) );

    m_dlgProgress->hideProgress();

    obDlgPatientCard.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_CardTypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_CardTypes_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPatientCardType obDlgPatientCardType( this );

    m_dlgProgress->hideProgress();

    obDlgPatientCardType.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_ProductTypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ProductTypes_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgProductType obDlgProductType( this );

    m_dlgProgress->hideProgress();

    obDlgProductType.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_ProductActionType_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ProductActionType_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgProductActionType obDlgProductActionType( this );

    m_dlgProgress->hideProgress();

    obDlgProductActionType.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Products_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Products_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgProduct obDlgProduct( this );

    m_dlgProgress->hideProgress();

    obDlgProduct.exec();

    m_bMainWindowActive = true;
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

    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgProductSell obDlgProductSell( this, p_qsBarcode );

    connect( &obDlgProductSell, SIGNAL(signalPaymentProcessed(cDBShoppingCart)), this, SLOT(processProductSellPayment(cDBShoppingCart)) );

    m_dlgProgress->hideProgress();

    obDlgProductSell.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_ShoppingCart_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ShoppingCart_triggered" );

    slotOpenShoppingCart( 0 );

    on_KeyboardEnabled();
}
//====================================================================================
void cWndMain::slotOpenShoppingCart( unsigned int /*p_uiPanelId*/ )
{
    cTracer obTrace( "cWndMain::slotOpenShoppingCart" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgShoppingCart    obDlgShoppingCart( this );

    connect( &obDlgShoppingCart, SIGNAL(signalPaymentProcessed(unsigned int,unsigned int,int)), this, SLOT(processDeviceUsePayment(unsigned int,unsigned int,int)) );
    connect( &obDlgShoppingCart, SIGNAL(signalSellPatientCard()), this, SLOT(on_action_PatientCardSell_triggered()) );
    connect( &obDlgShoppingCart, SIGNAL(signalSellProduct()), this, SLOT(on_action_SellProduct_triggered()) );

//    if( p_uiPanelId > 0 )
//        obDlgShoppingCart.setPanelFilter( p_uiPanelId );

    m_dlgProgress->hideProgress();

    obDlgShoppingCart.exec();

    m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();

    on_KeyboardEnabled();

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgStorno  obDlgStorno( this );

    m_dlgProgress->hideProgress();

    obDlgStorno.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_PCSaveToDatabase_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PCSaveToDatabase_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPatientCardAdd  obDlgPatientCardAdd( this );

    m_dlgProgress->hideProgress();

    obDlgPatientCardAdd.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Cassa_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Cassa_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgCassaEdit   obDlgCassaEdit( this );

    m_dlgProgress->hideProgress();

    obDlgCassaEdit.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_Accounting_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Accounting_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgRepLedgerMain  obDlgLedgerMain( this );

    m_dlgProgress->hideProgress();

    obDlgLedgerMain.exec();

    m_bMainWindowActive = true;
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
    on_KeyboardEnabled();
}
//====================================================================================
void cWndMain::on_action_ValidateSerialKey_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ValidateSerialKey_triggered" );

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_PatientCardSell_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientCardSell_triggered" );

    m_bMainWindowActive = false;

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

                m_bMainWindowActive = true;
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
                    m_bMainWindowActive = true;
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
        m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();
    }

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_PatientCardAssign_triggered()
{
    slotAssignPartnerCard( "" );
}
//====================================================================================
void cWndMain::on_action_ReplaceLostCard_triggered()
{
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    m_bMainWindowActive = false;

    QMessageBox::warning( this, tr("Request"), tr("Please select the guest from the list") );

    cDlgPatientSelect  obDlgPatientSelect( this );

    if( obDlgPatientSelect.exec() == QDialog::Accepted )
    {
        if( obDlgPatientSelect.selectedPatientId() > 0 )
        {
            QMessageBox::warning( this, tr("Request"), tr("Please select the patientcard from the list") );

            m_dlgProgress->showProgress();

            cDlgPatientCardSelect   obDlgSelect( this, obDlgPatientSelect.selectedPatientId() );

            unsigned int uiPCardId = 0;

            m_dlgProgress->hideProgress();

            if( obDlgSelect.exec() == QDialog::Accepted )
            {
                uiPCardId = obDlgSelect.selected();

                slotReplacePatientCard( "", uiPCardId );
            }
        }
    }

    m_bMainWindowActive = true;
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
                    _setStatusText( m_qsStatusText );
                    mdiPanels->clear();
                }
                else
                {
                    on_KeyboardEnabled();
                    return;
                }
                break;
            case 2:
                m_qsPanelStartBarcode = p_stBarcode;
                on_action_UseDeviceLater_triggered();
                on_KeyboardEnabled();
                return;
                break;
            case 3:
            default:
                on_KeyboardEnabled();
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
    on_KeyboardEnabled();
}
//====================================================================================
void cWndMain::processInputProduct( QString p_stBarcode )
{
    cTracer obTrace( "cWndMain::processInputProduct" );

    on_action_SellProduct_triggered( p_stBarcode );

    on_KeyboardEnabled();
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
                    _setStatusText( m_qsStatusText );
                    mdiPanels->clear();
                }
                else
                {
                    on_KeyboardEnabled();
                    return;
                }
                break;
            case 2:
                m_inPanelStartMinute = p_inMinute;
                on_action_UseDeviceLater_triggered();
                on_KeyboardEnabled();
                return;
                break;
            case 3:
            default:
                on_KeyboardEnabled();
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
        if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            mdiPanels->setMainProcessTime( p_inMinute*60, 0 );
        }
        else
        {
            on_action_UseDevice_triggered();
        }
    }
    on_KeyboardEnabled();
}
//====================================================================================
void cWndMain::on_action_EditActualPatient_triggered()
{
    cTracer obTrace( "cWndMain::on_action_EditActualPatient_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgGuestEdit  obDlgEdit( this, &g_obGuest );
    obDlgEdit.setWindowTitle( g_obGuest.name() );

    m_dlgProgress->hideProgress();

    obDlgEdit.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_DeviceSettings_triggered()
{
    cTracer obTrace( "cWndMain::on_action_DeviceSettings_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPanelSettings   obDlgEdit( this, mdiPanels->activePanelId() );

    m_dlgProgress->hideProgress();

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        mdiPanels->reload();
        g_obLogger(cSeverity::DEBUG) << QString::number( mdiPanels->activePanel() ) << EOM;
        m_dlgSecondaryWindow->refreshTitle( mdiPanels->activePanel() );
    }

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

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
    unsigned int    uiCouponId      = 0;

    obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

    if( inCassaAction == QDialog::Accepted && !bShoppingCart )
    {
        unsigned int uiLedgerId = g_obCassa.cassaProcessDeviceUse( obDBShoppingCart, qsComment, inPayType, mdiPanels->getPanelCaption(obDBShoppingCart.panelId()) );
        processDeviceUsePayment( obDBShoppingCart.panelId(), uiLedgerId, inPayType );
    }
    else if( inCassaAction == QDialog::Accepted && bShoppingCart )
    {
        mdiPanels->itemAddedToShoppingCart(obDBShoppingCart.id());
        mdiPanels->cashPayed( 0 );
    }
    m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();
    on_KeyboardEnabled();

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

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
    m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

    dlgLicenceEdit  obDlgLicenceEdit( this );

    obDlgLicenceEdit.exec();

    m_bMainWindowActive = true;
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

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgDiscount  obDlgDiscount( this );

    m_dlgProgress->hideProgress();

    obDlgDiscount.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_PatientcardsObsolete_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_PatientcardsObsolete_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgReportPatientCardObs  obDlgReportPatientCardObs( this );

    m_dlgProgress->hideProgress();

    obDlgReportPatientCardObs.exec();

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::on_action_EmptyDemoDB_triggered()
//====================================================================================
{
    cTracer obTrace( "cWndMain::on_action_EmptyDemoDB_triggered" );

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
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
void cWndMain::slotReplacePatientCard(const QString &p_qsBarcode,
                                      const unsigned int p_uiPCId )
{
    cTracer obTrace( "cWndMain::slotReplacePatientCard" );

    m_bMainWindowActive = false;

    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    m_dlgProgress->showProgress();

    cDBPatientCard  obDBPatientCardOld;

    if( p_uiPCId > 0 )
    {
        obDBPatientCardOld.load( p_uiPCId );
    }
    else
    {
        obDBPatientCardOld.load( p_qsBarcode );
    }

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

                m_bMainWindowActive = true;
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

            m_bMainWindowActive = true;
            return;
        }
        else
        {
            obDBPatientCardNew.setActive( true );
            obDBPatientCardNew.setPatientCardTypeId( obDBPatientCardOld.patientCardTypeId() );
            obDBPatientCardNew.setParentId( obDBPatientCardOld.parentId() );
            obDBPatientCardNew.setPatientId( obDBPatientCardOld.patientId() );
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
                m_bMainWindowActive = true;
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
            obDBPatientCardNew.synchronizeTime();
            obDBPatientCardNew.save();
        }
        m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();
    }

    m_bMainWindowActive = true;
}
//====================================================================================
void cWndMain::slotAssignPartnerCard( const QString &p_qsBarcode )
{
    cTracer obTracer( "cWndMain::slotAssignPartnerCard" );

    m_bMainWindowActive = false;

    cDlgPatientCardAssign obDlgPatientCardAssign( this, p_qsBarcode );

    obDlgPatientCardAssign.exec();

    m_bShoppingCartHasItem = g_obGen.isShoppingCartHasItems();

    m_bMainWindowActive = true;
}

void cWndMain::on_action_PaymentMethods_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PaymentMethods_triggered" );

    m_bMainWindowActive = false;

    m_dlgProgress->showProgress();

    cDlgPaymentMethod   obDlgPaymentMethod( this );

    m_dlgProgress->hideProgress();

    obDlgPaymentMethod.exec();

    m_bMainWindowActive = true;
}

void cWndMain::on_action_TestDlgStarted()
{
    cTracer obTrace( "cWndMain::on_action_TestDlgStarted" );

    m_bMainWindowActive = false;

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        cDlgTest    obDlgTest( this );

        obDlgTest.exec();
    }

    m_bMainWindowActive = true;
}


void cWndMain::on_ledPassword_returnPressed()
{
    on_pbLogin_clicked();
}

void cWndMain::on_action_ReportViewer_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ReportViewer_triggered" );

    QProcess *qpReportViewer = new QProcess(this);

//    if( !qpReportViewer->startDetached( QString("ReportViewer.exe %1 %2").arg(g_obUser.name()).arg(ledPassword->text()) ) )
    if( !qpReportViewer->startDetached( QString("ReportViewer.exe") ) )
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

    m_bMainWindowActive = false;

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

    m_bMainWindowActive = true;
}
//====================================================================================
int cWndMain::customMsgBox(QWidget */*parent*/, msgBoxType msgtype, QString buttonstext, QString msg, QString details)
//====================================================================================
{
    QMessageBox msgBox;

    //msgBox.setParent( parent );

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
    m_bMainWindowActive = false;

    cDlgManageDatabase  obDlgManageDatabase(this);

    obDlgManageDatabase.exec();

    m_bMainWindowActive = true;
}

void cWndMain::on_action_ManageDevicePanels_triggered()
{    
    m_bMainWindowActive = false;

    cDlgPanels  obDlgPanels( this );

    obDlgPanels.exec();

    m_bMainWindowActive = true;
}


void cWndMain::on_action_ManageSkinTypes_triggered()
{    
    m_bMainWindowActive = false;

    cDlgSkinTypes obDlgSkinTypes( this );

    obDlgSkinTypes.exec();

    m_bMainWindowActive = true;
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

void cWndMain::on_BlnsHttpErrorOccured()
{
    m_bBlnsHttpErrorVisible = true;
    m_bBlnsHttpConnected = false;
    m_uiBlnsErrorAppeared = QDateTime::currentDateTime().toTime_t();
    g_poPrefs->setBlnsHttpEnabled( false, true );
    m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_disabled.png" ) );
    _setStatusText( g_poBlnsHttp->errorMessage(), true );
}

void cWndMain::on_BlnsHttpActionFinished(QString p_qsInfo)
{
    if( p_qsInfo.left(10).compare( "HTTPMSG_01" ) == 0 )
    {
        m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_enabled.png" ) );
        m_bBlnsHttpConnected = true;
    }
    g_obLogger(cSeverity::INFO) << p_qsInfo << EOM;
}

void cWndMain::on_BlnsHttpStepProgress()
{
    m_lblHttpCount.setStyleSheet( "QLabel {font:bold; font-size:12px;}" );
    m_dlgProgress->stepProgressBar();
    m_lblHttpCount.setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    m_pbStatusHttp.setToolTip( tr("Number of records to process: %1")
                               .arg( m_lblHttpCount.text() ) );
}

void cWndMain::on_BlnsHttpHideProgress()
{
    m_dlgProgress->hideProgress();
}

void cWndMain::on_BlnsHttpIconClicked()
{
    if( !g_obUser.isLoggedIn() )
    {
        // If user logged out, menu won't be visible
        return;
    }

    QMenu   qmMenu;

    if( g_poPrefs->isBlnsHttpEnabled() )
    {
        if( m_bBlnsHttpConnected )
        {
            qmMenu.addAction( QIcon( "./resources/40x40_refresh.png" ), tr("Process remaining actions") );
            if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                qmMenu.addSeparator();
                qmMenu.addAction( QIcon( "./resources/40x40_patientcard_info.png" ), tr("Update all patientcards") );
                qmMenu.addSeparator();
                qmMenu.addAction( QIcon( "./resources/40x40_report_patientcard_inactive.png" ), tr("Remove inactive patientcards") );
            }
            qmMenu.addAction( QIcon( "./resources/40x40_report_patientcard_inactive.png" ), tr("Remove patientcard") );
        }
        else
        {
            qmMenu.addAction( QIcon( "./resources/40x40_check_connection.png" ), tr("Test connection") );
        }
        qmMenu.addSeparator();
        qmMenu.addAction( QIcon( "./resources/40x40_cancel.png" ), tr("Disable communication") );
        qmMenu.addSeparator();
        qmMenu.addAction( QIcon( "./resources/40x40_pause.png" ), tr("Suspend communication") );
    }
    else
    {
        qmMenu.addAction( QIcon( "./resources/40x40_ok.png" ), tr("Enable communication") );
    }
    if( g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        qmMenu.addSeparator();
        qmMenu.addAction( QIcon( "./resources/40x40_delete.png" ), tr("Clear waiting queue") );
    }

    QAction *qaRet = qmMenu.exec( QCursor::pos() );

    if( qaRet )
    {
        if( qaRet->text().compare( tr("Process remaining actions") ) == 0 )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Are you sure about to process all remaining actions?\n"
                                          "This process may take long time and slow down the application." ),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                _processHttpActions();
            }
        }
        else if( qaRet->text().compare( tr("Update all patientcard") ) == 0 )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Are you sure about to update all patientcards on web?\n"
                                          "This process may take long time and slow down the application." ),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                _updateAllPatientcardToWeb();
                _processHttpActions();
            }
        }
        else if( qaRet->text().compare( tr("Remove inactive patientcards") ) == 0 )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Are you sure about to remove all inactive patientcards from web?\n"
                                          "This process may take long time and slow down the application." ),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                _removeAllPatientcardFromWeb();
                _processHttpActions();
            }
        }
        else if( qaRet->text().compare( tr("Remove patientcard") ) == 0 )
        {
            _removePatientcardFromWeb();
        }
        else if( qaRet->text().compare( tr("Test connection") ) == 0 )
        {
            g_poBlnsHttp->checkHttpServerAvailability();
        }
        else if( qaRet->text().compare( tr("Disable communication") ) == 0 )
        {
            m_pbStatusHttp.setIcon( QIcon( "./resources/40x40_http_disabled.png" ) );
            g_poPrefs->setBlnsHttpEnabled( false, true );
        }
        else if( qaRet->text().compare( tr("Enable communication") ) == 0 )
        {
            g_poPrefs->setBlnsHttpEnabled( true, true );
            g_poBlnsHttp->checkHttpServerAvailability();
        }
        else if( qaRet->text().compare( tr("Suspend communication") ) == 0 )
        {
            m_nHttpCommCounter = 0;
            g_poPrefs->setBlnsHttpSuspended( true );
            m_pbStatusCommunicationSuspended.setIcon( QIcon( "./resources/40x40_minus.png" ) );
            setCursor( Qt::ArrowCursor);
        }
        else if( qaRet->text().compare( tr("Clear waiting queue") ) == 0 )
        {
            setCursor( Qt::ArrowCursor);
            g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo" );
            m_lblHttpCount.setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
            m_pbStatusHttp.setToolTip( tr("Number of records to process: %1")
                                       .arg( m_lblHttpCount.text() ) );
        }
    }
}

void cWndMain::showAdWindows()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT advertisementId FROM advertisements WHERE active=1" ) );

    while( poQuery->next() )
    {
        QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

        QString qsStatus = obPrefFile.value( QString::fromAscii( "Advertisement%1/Status" ).arg( poQuery->value(0).toUInt() ), "" ).toString();

        if( qsStatus.compare( "RUNNING" ) == 0 )
        {
            continue;
        }

        QProcess *qpAdv = new QProcess(this);

        obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Command" ).arg( poQuery->value(0).toInt() ), "START" );

        if( !qpAdv->startDetached( QString("Advertisement.exe %1").arg( poQuery->value(0).toUInt() ) ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("Error occured when starting process:Advertisement.exe\n\nError code: %1\n"
                                     "0 > The process failed to start.\n"
                                     "1 > The process crashed some time after starting successfully.\n"
                                     "2 > The last waitFor...() function timed out.\n"
                                     "4 > An error occurred when attempting to write to the process.\n"
                                     "3 > An error occurred when attempting to read from the process.\n"
                                     "5 > An unknown error occurred.").arg(qpAdv->error()) );
        }
        delete qpAdv;
/*
        cDlgAdvertisementWindow *poAdWnd = new cDlgAdvertisementWindow( this, poQuery->value(0).toUInt() );

        m_obAdWnd.push_back( poAdWnd );
        poAdWnd->show();
*/
    }
    this->setFocus();
}

void cWndMain::on_action_Advertisements_triggered()
{    
    m_bMainWindowActive = false;

    cDlgAdvertisements  obDlgAdvertisements(this);

    obDlgAdvertisements.exec();

    QProcess *qpAdv = new QProcess(this);

    if( !qpAdv->startDetached( QString("Advertisement.exe") ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:Advertisement.exe\n\nError code: %1\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.").arg(qpAdv->error()) );
    }

    delete qpAdv;

    QMessageBox::warning( this, tr("Attention"),
                          tr("Please note that you should restart the application for the modifications to take effect."));

    m_bMainWindowActive = true;
}

void cWndMain::on_CommunicationButtonClicked()
{
    QMenu   qmMenu;
    QString qsEnableHWDebug;
    QString qsIconHWDebug = "./resources/77x40_on.png";

    if( g_poPrefs->isHWDebugEnabled() )
    {
        qsEnableHWDebug = tr( "Disable HW Debug" );
        qsIconHWDebug = "./resources/77x40_on.png";
    }
    else
    {
        qsEnableHWDebug = tr( "Enable HW Debug" );
        qsIconHWDebug = "./resources/77x40_off.png";
    }

    qmMenu.addAction( QIcon( "./resources/40x40_refresh.png" ), tr("Reset communication") );
    if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        qmMenu.addSeparator();
        qmMenu.addAction( QIcon( qsIconHWDebug ), qsEnableHWDebug );
    }

    QAction *qaRet = qmMenu.exec( QCursor::pos() );

    if( qaRet )
    {
        if( qaRet->text().compare( tr("Reset communication") ) == 0 )
        {
            _resetCommunication();
            m_nCommunicationErrorCounter = 0;
        }
        else if( qaRet->text().compare( tr("Enable HW Debug") ) == 0 )
        {
            g_poPrefs->setHWDebug( true );
        }
        else if( qaRet->text().compare( tr("Disable HW Debug") ) == 0 )
        {
            g_poPrefs->setHWDebug( false );
        }
    }
}

void cWndMain::setCommunicationEnabled(bool p_bEnabled)
{
    m_pbStatusCommunication.setEnabled( p_bEnabled );
}

void cWndMain::_resetCommunication()
{
    m_nCommResetStep = 1;
}

void cWndMain::on_action_PatientcardInformation_triggered()
{    
    m_bMainWindowActive = false;

    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();

    m_dlgProgress->hideProgress();

    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        g_obGen.showPatientCardInformation( obDlgInputStart.getEditText() );
    }

    m_bMainWindowActive = true;
}

void cWndMain::on_KeyboardEnabled()
{
    m_bActionProcessing = false;
    m_pbStatusKeyboard.setIcon( QIcon( "./resources/40x40_keyboard_enabled.png" ) );
}

void cWndMain::on_KeyboardDisabled()
{
    m_bActionProcessing = true;
    m_pbStatusKeyboard.setIcon( QIcon( "./resources/40x40_keyboard_locked.png" ) );
}

void cWndMain::_updateAllPatientcardToWeb()
{
    m_dlgProgress->showProgress();

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientCardId "
                                                 "FROM patientcards WHERE "
                                                 "active = 1");

    m_dlgProgress->showProgressBar( poQuery->size() );

    while( poQuery->next() )
    {
        try
        {
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( poQuery->value(0).toUInt() );
            obDBPatientCard.sendDataToWeb( false );
            m_dlgProgress->stepProgressBar();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    m_dlgProgress->hideProgress();
}

void cWndMain::_removeAllPatientcardFromWeb()
{
    m_dlgProgress->showProgress();

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientCardId "
                                                 "FROM patientcards WHERE "
                                                 "active = 0 AND "
                                                 "patientCardId>1 AND "
                                                 "barcode<>\"\" ");

    m_dlgProgress->showProgressBar( poQuery->size() );

    while( poQuery->next() )
    {
        try
        {
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( poQuery->value(0).toUInt() );
            g_poBlnsHttp->sendPatientCardData( obDBPatientCard.barcode(), "", false );
            m_dlgProgress->stepProgressBar();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    m_dlgProgress->hideProgress();
}

void cWndMain::_removePatientcardFromWeb()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();

    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        try
        {
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( obDlgInputStart.getEditText() );
            g_poBlnsHttp->sendPatientCardData( obDBPatientCard.barcode(), "" );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

void cWndMain::_setStatusText(QString p_qsText, bool p_bError)
{
    if( m_bBlnsHttpErrorVisible )
    {
        if( (QDateTime::currentDateTime().toTime_t()-m_uiBlnsErrorAppeared) < (unsigned int)(g_poPrefs->getBlnsHttpMessageWaitTime()*1000) )
        {
            return;
        }
        m_bBlnsHttpErrorVisible = false;
    }

    if( p_bError )
    {
        m_lblStatusLeft.setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else
    {
        m_lblStatusLeft.setStyleSheet( "QLabel {font: normal;}" );
    }

    m_lblStatusLeft .setText( p_qsText );
}

void cWndMain::_processHttpActions()
{
    if( g_poPrefs->isBlnsHttpEnabled() )
    {
        m_dlgProgress->showProgress();
        m_dlgProgress->showProgressBar( g_poBlnsHttp->getNumberOfWaitingRecords() );
        g_poBlnsHttp->processWaitingCardData();
    }
}

void cWndMain::_checkVersions()
{
    QSqlQuery   *poQuery            = NULL;
    QString      qsAppVersion       = "";
    QString      qsDbVersion        = "";

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM settings " ) );
    while( poQuery->next() )
    {
        QString qsIdentifier = poQuery->value(1).toString();

        if( qsIdentifier.compare("APPLICATION_VERSION") == 0 )
            qsAppVersion = poQuery->value( 2 ).toString();
        else if( qsIdentifier.compare("DATABASE_VERSION") == 0 )
            qsDbVersion = poQuery->value( 2 ).toString();
    }

    qsAppVersion.replace( "_", "." );
    qsDbVersion.replace( "_", "." );

    if( qsAppVersion.compare( g_poPrefs->getVersion() ) ||
        qsDbVersion.compare( g_poPrefs->getVersionDb() ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "The version information stored in database is not match.\n\n"
                                  "Correct version numbers:\n"
                                  "Application version number: %1\n"
                                  "Database version number: %2\n\n"
                                  "Version numbers stored in database:\n"
                                  "Application version number: %3\n"
                                  "Database version number: %4\n\n"
                                  "It is recommended to exit application and to contact system administrator.")
                              .arg( g_poPrefs->getVersion() )
                              .arg( g_poPrefs->getVersionDb() )
                              .arg( qsAppVersion )
                              .arg( qsDbVersion ) );
    }
}

void cWndMain::_checkIsActivationNeeded()
{
    if( m_bClosingShift )
        return;

    int     nDaysRemain = g_obLicenceManager.daysRemain();

    if( nDaysRemain < cLicenceManager::EXPIRE_MAX_DAYS )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "The validity of the application's licence\n"
                                  "will be expire in %1 days.\n\n"
                                  "Please contact your franchise provider\n"
                                  "and extend your licence valid time period.")
                              .arg( nDaysRemain ) );
    }
    if( g_obLicenceManager.ltLicenceType() == cLicenceManager::LTYPE_REGISTERED )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "The application's licence is registered\n"
                                  "but not validated by your franchise provider.\n\n"
                                  "Please contact your franchise provider\n"
                                  "and validate your application's licence" ) );
    }
}

void cWndMain::on_BlnsHttpProcessStopped()
{
    m_lblHttpCount.setStyleSheet( "QLabel {font:normal; font-size:8px;}" );
    m_lblHttpCount.setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    m_pbStatusHttp.setToolTip( tr("Number of records to process: %1")
                               .arg( m_lblHttpCount.text() ) );
}

void cWndMain::slotMainWindowActivated()
{
    m_bMainWindowActive = true;
    setCursor( Qt::ArrowCursor);
}
