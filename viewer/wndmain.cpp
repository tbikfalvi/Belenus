//====================================================================================
//
// Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2012
//
//====================================================================================
//
// Filename    : wndmain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo allomanya.
//====================================================================================

#include <QCryptographicHash>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPrintDialog>

//====================================================================================

#include "../framework/qtmysqlquerymodel.h"
#include "wndmain.h"

//====================================================================================
//
// Uj report hozzaadasakor a gui-n kell csinalni egy action-t es a toolbar-ban egy
// kulon gombot. Az action-t a menu reszbe kell berakni
// A wndmain.h-ban illetve itt a cpp-ben a <_NEW_REPORT_> szovegre rakeresve lehet
// megtalalni azokat a reszeket, ahova be kell illeszteni az uj report-ot
// Minden report-nak sajat cReport osztalya van ezeket a sajat creport...cpp es .h
// file-jaikban kell megcsinalni.
//
//------------------------------------------------------------------------------------
cWndMain::cWndMain(QWidget *parent , QString p_qsAppVersion) : QMainWindow( parent )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    m_qsRPSW                = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";
    m_bReportTabSwitching   = false;
    m_enGroup               = GROUP_MIN;

    setWindowTitle( tr("Belenus Report Viewer - v.%1").arg( p_qsAppVersion ) );

    // <_NEW_REPORT_> signal es slot osszekapcsolas
    connect( this, SIGNAL(setCheckedReportDaily(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( this, SIGNAL(setCheckedReportLedger(bool)), this, SLOT(slotCheckReportLedger(bool)) );
    connect( this, SIGNAL(setCheckedReportCassaHistory(bool)), this, SLOT(slotCheckReportCassaHistory(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardType(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardInactive(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardDetails(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardSells(bool)), this, SLOT(slotCheckReportPatientcardSells(bool)) );
    connect( this, SIGNAL(setCheckedReportProducts(bool)), this, SLOT(slotCheckReportProducts(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardDebts(bool)), this, SLOT(slotCheckReportPatientcardDebts(bool)) );
    connect( this, SIGNAL(setCheckedReportProductStatus(bool)), this, SLOT(slotCheckReportProductStatus(bool)) );
    connect( this, SIGNAL(setCheckedReportProductHistory(bool)), this, SLOT(slotCheckReportProductHistory(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardUsages(bool)), this, SLOT(slotCheckReportPatientcardUsages(bool)) );
    connect( this, SIGNAL(setCheckedReportMonthClose(bool)), this, SLOT(slotCheckReportMonthClose(bool)) );
    connect( this, SIGNAL(setCheckedReportGuests(bool)), this, SLOT(slotCheckReportGuests(bool)) );
    connect( this, SIGNAL(setCheckedReportDeviceUsages(bool)), this, SLOT(slotCheckReportDeviceUsages(bool)) );
    connect( this, SIGNAL(setCheckedReportDeviceMinuteUsages(bool)), this, SLOT(slotCheckReportDeviceMinuteUsages(bool)) );

    connect( cmbName, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );
    connect( ledPassword, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );

    _initActions();
    _initToolbar();
    _initFilterbar();
    _initTabInformation();

    on_tabReports_currentChanged( 0 );

    lblInformation1->setVisible( true );
    lblInformation2->setVisible( false );

    cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
    m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
    cmbName->setModel( m_poModel );

    ledPassword->setFocus();
}
//------------------------------------------------------------------------------------
cWndMain::~cWndMain()
//------------------------------------------------------------------------------------
{
}
//------------------------------------------------------------------------------------
void cWndMain::setLoginData(QString p_qsName, QString p_qsPassword)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::setLoginData" );

    if( p_qsName.length() > 0 )
    {
        for( int i=0; i<cmbName->count(); i++ )
        {
            if( cmbName->itemText(i).contains(p_qsName) )
            {
                cmbName->setCurrentIndex( i );
                break;
            }
        }
    }

    if( p_qsPassword.length() > 0 )
    {
        ledPassword->setText( p_qsPassword );
    }

    if( ledPassword->text().length() > 0 )
    {
        on_pbAuthenticate_clicked();
    }
}
//------------------------------------------------------------------------------------
void cWndMain::_initActions()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initActions" );

    // SIGNALS
    // <_NEW_REPORT_> az action es a slot osszekapcsolasa
    connect( action_Bookkeeping_Daily, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( action_Bookkeeping_Ledger, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportLedger(bool)) );
    connect( action_MonthlyClosure, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportMonthClose(bool)) );
    connect( action_Bookkeeping_CassaHistory, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportCassaHistory(bool)) );

    connect( action_PatientcardTypes, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( action_Patientcards_Inactive, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( action_Patientcards_Details, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );
    connect( action_Patientcard_Usages, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardUsages(bool)) );
    connect( action_Patientcard_Sells, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardSells(bool)) );
    connect( action_Patientcard_Debts, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardDebts(bool)) );

    connect( action_Products, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportProducts(bool)) );
    connect( action_Product_Status, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportProductStatus(bool)) );
    connect( action_Product_History, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportProductHistory(bool)) );

    connect( action_Guests, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportGuests(bool)) );

    connect( action_DeviceUsages, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportDeviceUsages(bool)) );
    connect( action_DeviceMinuteUsages, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportDeviceMinuteUsages(bool)) );

    // ICONS
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    // <_NEW_REPORT_> az action es az ikonjanak osszekapcsolasa
    action_Bookkeeping_Daily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    action_Bookkeeping_Ledger->setIcon( QIcon("./resources/40x40_book_ledger.png") );
    action_MonthlyClosure->setIcon( QIcon("./resources/40x40_book.png") );
    action_Bookkeeping_CassaHistory->setIcon( QIcon("./resources/40x40_cassa.png") );

    action_PatientcardTypes->setIcon( QIcon("./resources/40x40_report_patientcardtypes.png") );
    action_Patientcards_Inactive->setIcon( QIcon("./resources/40x40_report_patientcard_inactive.png") );
    action_Patientcards_Details->setIcon( QIcon("./resources/40x40_report_patientcard_details.png") );
    action_Patientcard_Usages->setIcon( QIcon("./resources/40x40_report_patientcard_usages.png") );
    action_Patientcard_Sells->setIcon( QIcon("./resources/40x40_report_patientcard_sell.png") );
    action_Patientcard_Debts->setIcon( QIcon("./resources/40x40_report_patientcard_debt.png") );

    action_Products->setIcon( QIcon("./resources/40x40_report_products.png") );
    action_Product_Status->setIcon( QIcon("./resources/40x40_report_product_status.png") );
    action_Product_History->setIcon( QIcon("./resources/40x40_report_product_history.png") );

    action_Guests->setIcon( QIcon("./resources/40x40_patient.png") );

    action_DeviceUsages->setIcon( QIcon("./resources/40x40_device.png") );
    action_DeviceMinuteUsages->setIcon( QIcon("./resources/40x40_device_later.png") );

    // BEHAVIOUR
    action_FilterBar->setEnabled( false );

    // <_NEW_REPORT_> az action alapertelmezett letiltasa
    action_Bookkeeping_Daily->setEnabled( false );
    action_Bookkeeping_Ledger->setEnabled( false );
    action_MonthlyClosure->setEnabled( false );
    action_Bookkeeping_CassaHistory->setEnabled( false );

    action_PatientcardTypes->setEnabled( false );
    action_Patientcards_Inactive->setEnabled( false );
    action_Patientcards_Details->setEnabled( false );
    action_Patientcard_Usages->setEnabled( false );
    action_Patientcard_Sells->setEnabled( false );
    action_Patientcard_Debts->setEnabled( false );

    action_Products->setEnabled( false );
    action_Product_Status->setEnabled( false );
    action_Product_History->setEnabled( false );

    action_Guests->setEnabled( false );

    action_DeviceUsages->setEnabled( false );
    action_DeviceMinuteUsages->setEnabled( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initToolbar()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initToolbar" );

    // SIGNALS
    connect( pbExit, SIGNAL(clicked()), this, SLOT(close()) );

    // <_NEW_REPORT_> a toolbar gomb es a slot osszekapcsolasa
    connect( pbBookkeepingDaily, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( pbBookkeepingLedger, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportLedger(bool)) );
    connect( pbBookkeepingMonth, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportMonthClose(bool)) );
    connect( pbBookkeepingCassaHistory, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportCassaHistory(bool)) );

    connect( pbPatientcardType, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( pbPatientcardsInactive, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( pbPatientcardsDetails, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );
    connect( pbPatientcardUsages, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardUsages(bool)) );
    connect( pbPatientcardSells, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardSells(bool)) );
    connect( pbPatientcardDebts, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardDebts(bool)) );

    connect( pbProducts, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportProducts(bool)) );
    connect( pbProductStatus, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportProductStatus(bool)) );
    connect( pbProductHistory, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportProductHistory(bool)) );

    connect( pbGuests, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportGuests(bool)) );

    connect( pbDeviceUsages, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportDeviceUsages(bool)) );
    connect( pbDeviceMinuteUsages, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportDeviceMinuteUsages(bool)) );

    // ICONS
    pbExit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    // <_NEW_REPORT_> a toolbar gomb es az ikon osszekapcsolasa
    pbBookkeepingDaily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    pbBookkeepingLedger->setIcon( QIcon("./resources/40x40_book_ledger.png") );
    pbBookkeepingMonth->setIcon( QIcon("./resources/40x40_book.png") );
    pbBookkeepingCassaHistory->setIcon( QIcon("./resources/40x40_cassa.png") );

    pbPatientcardType->setIcon( QIcon("./resources/40x40_report_patientcardtypes.png") );
    pbPatientcardsInactive->setIcon( QIcon("./resources/40x40_report_patientcard_inactive.png") );
    pbPatientcardsDetails->setIcon( QIcon("./resources/40x40_report_patientcard_details.png") );
    pbPatientcardUsages->setIcon( QIcon("./resources/40x40_report_patientcard_usages.png") );
    pbPatientcardSells->setIcon( QIcon("./resources/40x40_report_patientcard_sell.png") );
    pbPatientcardDebts->setIcon( QIcon("./resources/40x40_report_patientcard_debt.png") );

    pbProducts->setIcon( QIcon("./resources/40x40_report_products.png") );
    pbProductStatus->setIcon( QIcon("./resources/40x40_report_product_status.png") );
    pbProductHistory->setIcon( QIcon("./resources/40x40_report_product_history.png") );

    pbGuests->setIcon( QIcon("./resources/40x40_patient.png") );

    pbDeviceUsages->setIcon( QIcon("./resources/40x40_device.png") );
    pbDeviceMinuteUsages->setIcon( QIcon("./resources/40x40_device_later.png") );

    pbSave->setIcon( QIcon("./resources/40x40_save.png") );
    pbPrint->setIcon( QIcon("./resources/40x40_print.png") );

    // BEHAVIOUR
    // <_NEW_REPORT_> a toolbar gomb alapertelmezett letiltasa
    pbBookkeepingDaily->setEnabled( false );
    pbBookkeepingLedger->setEnabled( false );
    pbBookkeepingMonth->setEnabled( false );
    pbBookkeepingCassaHistory->setEnabled( false );

    pbPatientcardType->setEnabled( false );
    pbPatientcardsInactive->setEnabled( false );
    pbPatientcardsDetails->setEnabled( false );
    pbPatientcardUsages->setEnabled( false );
    pbPatientcardSells->setEnabled( false );
    pbPatientcardDebts->setEnabled( false );

    pbProducts->setEnabled( false );
    pbProductStatus->setEnabled( false );
    pbProductHistory->setEnabled( false );

    pbGuests->setEnabled( false );

    pbDeviceUsages->setEnabled( false );
    pbDeviceMinuteUsages->setEnabled( false );

    pbSave->setEnabled( false );
    pbPrint->setEnabled( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initFilterbar()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initFilterbar" );

    // ICONS
    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );

    // BEHAVIOUR
    frameFilterbar->setVisible( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initTabInformation()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initTabInformation" );

    // ICONS
    tabReports->setTabIcon( 0, QIcon("./resources/40x40_information.png") );
    pbAuthenticate->setIcon( QIcon("./resources/40x40_key.png") );

    // BEHAVIOUR
    _setAuthInfoType( AUTH_NEEDED );
}
//------------------------------------------------------------------------------------
void cWndMain::_setAuthInfoType(authType p_tAuthType)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setAuthInfoType" );

    cmbName->setEnabled( true );
    ledPassword->setEnabled( true );

    pbAuthenticate->setText( tr("Login") );

    if( p_tAuthType == AUTH_NEEDED )
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        lblAuthenticationInformation->setText( tr("Need authentication ...") );
    }
    else if( p_tAuthType == AUTH_OK )
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: green;}" );
        lblAuthenticationInformation->setText( tr("User successfully authenticated.") );

        cmbName->setEnabled( false );
        ledPassword->setEnabled( false );

        pbAuthenticate->setText( tr("Logout") );
    }
    else
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: red;}" );
        if( p_tAuthType == AUTH_USER_NOTFOUND )
            lblAuthenticationInformation->setText( tr("The username entered not found in the Belenus database") );
        else if( p_tAuthType == AUTH_PASSWORD_INCORRECT )
            lblAuthenticationInformation->setText( tr("The password entered is incorrect") );
        else if( p_tAuthType == AUTH_CONNECTION_FAILED )
            lblAuthenticationInformation->setText( tr("Connection to Belenus database failed") );
        else
            lblAuthenticationInformation->setText( tr("Authentication failed. Please retry later ...") );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_tabReports_tabCloseRequested(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_tabReports_tabCloseRequested" );

// addig amig nem jovok ra, hogy miert szall el a program, addig nem engedelyezem az X-et
return;

    if( index > 0 )
    {
        // <_NEW_REPORT_> a megfelelo report tab X gombjanak lekezelese
        if( m_repDaily && m_repDaily->index() == index )
            emit setCheckedReportDaily( false );
        else if( m_repLedger && m_repLedger->index() == index )
            emit setCheckedReportLedger( false );
        else if( m_repMonthClose && m_repMonthClose->index() == index )
            emit setCheckedReportMonthClose( false );
        else if( m_repCassaHistory && m_repCassaHistory->index() == index )
            emit setCheckedReportCassaHistory( false );
        else if( m_repCardType && m_repCardType->index() == index )
            emit setCheckedReportPatientcardType( false );
        else if( m_repCardInactive && m_repCardInactive->index() == index )
            emit setCheckedReportPatientcardInactive( false );
        else if( m_repCardDetails && m_repCardDetails->index() == index )
            emit setCheckedReportPatientcardDetails( false );
        else if( m_repCardUsages && m_repCardUsages->index() == index )
            emit setCheckedReportPatientcardUsages( false );
        else if( m_repCardSells && m_repCardSells->index() == index )
            emit setCheckedReportPatientcardSells( false );
        else if( m_repProducts && m_repProducts->index() == index )
            emit setCheckedReportProducts( false );
        else if( m_repCardDebts && m_repCardDebts->index() == index )
            emit setCheckedReportPatientcardDebts( false );
        else if( m_repProdStatus && m_repProdStatus->index() == index )
            emit setCheckedReportProductStatus( false );
        else if( m_repProdHistory && m_repProdHistory->index() == index )
            emit setCheckedReportProductHistory( false );
        else if( m_repGuests && m_repGuests->index() == index )
            emit setCheckedReportGuests( false );
        else if( m_repDeviceUsages && m_repDeviceUsages->index() == index )
            emit setCheckedReportDeviceUsages( false );
        else if( m_repDeviceMinuteUsages && m_repDeviceMinuteUsages->index() == index )
            emit setCheckedReportDeviceMinuteUsages( false );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_action_Toolbar_triggered(bool checked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_action_Toolbar_triggered" );

    frameToolbar->setVisible( checked );
}
//------------------------------------------------------------------------------------
void cWndMain::on_action_FilterBar_triggered(bool checked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_action_FilterBar_triggered" );

    frameFilterbar->setVisible( checked );
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbAuthenticate_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_pbAuthenticate_clicked" );

    authType    atRet = AUTH_NEEDED;

    m_enGroup = GROUP_MIN;

    if( pbAuthenticate->text().compare( tr("Login") ) == 0 )
    {
        atRet = _authenticateUser();
    }

    _setAuthInfoType( atRet );

    if( atRet == AUTH_OK )
    {
        _setReportsEnabled();
        ledPassword->setText( "" );
    }
    else
    {
        _setReportsEnabled( false );
    }
}
//------------------------------------------------------------------------------------
cWndMain::authType cWndMain::_authenticateUser()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_authenticateUser" );

    string  stName = cmbName->currentText().toStdString();
    stName = stName.substr( 0, stName.find( '(' ) - 1 );
    authType    atRet = AUTH_ERROR;
    QByteArray  obPwdHash = QCryptographicHash::hash( ledPassword->text().toAscii(), QCryptographicHash::Sha1 );

    m_enGroup = GROUP_MIN;

    if( QString::fromStdString(stName).compare( "root" ) == 0 )
    {
        if( m_qsRPSW.compare( QString( obPwdHash.toHex() ) ) == 0 )
        {
            atRet = AUTH_OK;
            m_enGroup = GROUP_ROOT;
        }
        else
        {
            atRet = AUTH_PASSWORD_INCORRECT;
        }
    }
    else
    {
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + QString::fromStdString(stName) + "\"" );

            if( poQuery->size() != 1 )
            {
                atRet = AUTH_USER_NOTFOUND;
            }
            else
            {
                poQuery->first();
                if( poQuery->value(4).toString().compare( QString( obPwdHash.toHex() ) ) == 0 )
                {
                    atRet = AUTH_OK;
                    m_enGroup = poQuery->value(5).toInt();
                    m_uiUserId = poQuery->value(0).toUInt();
                }
                else
                {
                    atRet = AUTH_PASSWORD_INCORRECT;
                }
            }
        }
        catch( cSevException &e )
        {
            atRet = AUTH_CONNECTION_FAILED;
        }
    }

    return atRet;
}
//------------------------------------------------------------------------------------
void cWndMain::_setReportsEnabled(bool p_bEnable)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setReportsEnabled" );

    lblInformation1->setVisible( !p_bEnable );
    lblInformation2->setVisible( p_bEnable );
    action_FilterBar->setEnabled( p_bEnable );
    action_FilterBar->setChecked( p_bEnable );
    on_action_FilterBar_triggered( p_bEnable );

    // <_NEW_REPORT_> az action engedelyezese/tiltasa
    action_Bookkeeping_Daily->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Bookkeeping_Ledger->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    action_MonthlyClosure->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Bookkeeping_CassaHistory->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );

    action_PatientcardTypes->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    action_Patientcards_Inactive->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Patientcards_Details->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Patientcard_Usages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Patientcard_Sells->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Patientcard_Debts->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    action_Products->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_Product_Status->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    action_Product_History->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    action_Guests->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    action_DeviceUsages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    action_DeviceMinuteUsages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );

    // <_NEW_REPORT_> a toolbar gomb engedelyezese/tiltasa
    pbBookkeepingDaily->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbBookkeepingLedger->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    pbBookkeepingMonth->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbBookkeepingCassaHistory->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );

    pbPatientcardType->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    pbPatientcardsInactive->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbPatientcardsDetails->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbPatientcardUsages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbPatientcardSells->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbPatientcardDebts->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    pbProducts->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbProductStatus->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );
    pbProductHistory->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    pbGuests->setEnabled( p_bEnable && _isInGroup( GROUP_ADMIN ) );

    pbDeviceUsages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );
    pbDeviceMinuteUsages->setEnabled( p_bEnable && _isInGroup( GROUP_USER ) );

    _updateReportButtons( p_bEnable );
}

//====================================================================================
// <_NEW_REPORT_> a report sajat slot-ja
//====================================================================================

//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportDaily(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportDaily" );

    m_bReportTabSwitching = true;

    action_Bookkeeping_Daily->setChecked( p_bChecked );
    pbBookkeepingDaily->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repDaily = new cReportDaily( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repDaily );
        m_repDaily->setIndex( tabReports->addTab( m_repDaily, QIcon("./resources/40x40_book_daily.png"), m_repDaily->name() ) );
        tabReports->setCurrentIndex( m_repDaily->index() );
    }
    else
    {
        m_qvReports.remove( m_repDaily->index()-1 );
        tabReports->removeTab( m_repDaily->index() );
        delete m_repDaily;
        m_repDaily = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportLedger( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportLedger" );

    m_bReportTabSwitching = true;

    action_Bookkeeping_Ledger->setChecked( p_bChecked );
    pbBookkeepingLedger->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repLedger = new cReportLedger( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repLedger );
        m_repLedger->setIndex( tabReports->addTab( m_repLedger, QIcon("./resources/40x40_book_ledger.png"), m_repLedger->name() ) );
        tabReports->setCurrentIndex( m_repLedger->index() );
    }
    else
    {
        m_qvReports.remove( m_repLedger->index()-1 );
        tabReports->removeTab( m_repLedger->index() );
        delete m_repLedger;
        m_repLedger = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportMonthClose(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportMonthClose" );

    m_bReportTabSwitching = true;

    action_MonthlyClosure->setChecked( p_bChecked );
    pbBookkeepingMonth->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repMonthClose = new cReportMonthClose( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repMonthClose );
        m_repMonthClose->setIndex( tabReports->addTab( m_repMonthClose, QIcon("./resources/40x40_book.png"), m_repMonthClose->name() ) );
        tabReports->setCurrentIndex( m_repMonthClose->index() );
    }
    else
    {
        m_qvReports.remove( m_repMonthClose->index()-1 );
        tabReports->removeTab( m_repMonthClose->index() );
        delete m_repMonthClose;
        m_repMonthClose = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportCassaHistory(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportCassaHistory" );

    m_bReportTabSwitching = true;

    action_Bookkeeping_CassaHistory->setChecked( p_bChecked );
    pbBookkeepingCassaHistory->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCassaHistory = new cReportCassaHistory( this, "", _isInGroup( GROUP_ADMIN ), m_uiUserId );

        m_qvReports.append( m_repCassaHistory );
        m_repCassaHistory->setIndex( tabReports->addTab( m_repCassaHistory, QIcon("./resources/40x40_cassa.png"), m_repCassaHistory->name() ) );
        tabReports->setCurrentIndex( m_repCassaHistory->index() );
    }
    else
    {
        m_qvReports.remove( m_repCassaHistory->index()-1 );
        tabReports->removeTab( m_repCassaHistory->index() );
        delete m_repCassaHistory;
        m_repCassaHistory = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardType( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardType" );

    m_bReportTabSwitching = true;

    action_PatientcardTypes->setChecked( p_bChecked );
    pbPatientcardType->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardType = new cReportPatientCardType( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardType );
        m_repCardType->setIndex( tabReports->addTab( m_repCardType, QIcon("./resources/40x40_report_patientcardtypes.png"), m_repCardType->name() ) );
        tabReports->setCurrentIndex( m_repCardType->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardType->index()-1 );
        tabReports->removeTab( m_repCardType->index() );
        delete m_repCardType;
        m_repCardType = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardInactive( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardInactive" );

    m_bReportTabSwitching = true;

    action_Patientcards_Inactive->setChecked( p_bChecked );
    pbPatientcardsInactive->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardInactive = new cReportCardInactive( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardInactive );
        m_repCardInactive->setIndex( tabReports->addTab( m_repCardInactive, QIcon("./resources/40x40_report_patientcard_inactive.png"), m_repCardInactive->name() ) );
        tabReports->setCurrentIndex( m_repCardInactive->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardInactive->index()-1 );
        tabReports->removeTab( m_repCardInactive->index() );
        delete m_repCardInactive;
        m_repCardInactive = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardDetails( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardDetails" );

    m_bReportTabSwitching = true;

    action_Patientcards_Details->setChecked( p_bChecked );
    pbPatientcardsDetails->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardDetails = new cReportCardDetails( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardDetails );
        m_repCardDetails->setIndex( tabReports->addTab( m_repCardDetails, QIcon("./resources/40x40_report_patientcard_details.png"), m_repCardDetails->name() ) );
        tabReports->setCurrentIndex( m_repCardDetails->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardDetails->index()-1 );
        tabReports->removeTab( m_repCardDetails->index() );
        delete m_repCardDetails;
        m_repCardDetails = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardUsages( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardUsages" );

    m_bReportTabSwitching = true;

    action_Patientcard_Usages->setChecked( p_bChecked );
    pbPatientcardUsages->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardUsages = new cReportPCUsages( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardUsages );
        m_repCardUsages->setIndex( tabReports->addTab( m_repCardUsages, QIcon("./resources/40x40_report_patientcard_usages.png"), m_repCardUsages->name() ) );
        tabReports->setCurrentIndex( m_repCardUsages->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardUsages->index()-1 );
        tabReports->removeTab( m_repCardUsages->index() );
        delete m_repCardUsages;
        m_repCardUsages = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardSells( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardSells" );

    m_bReportTabSwitching = true;

    action_Patientcard_Sells->setChecked( p_bChecked );
    pbPatientcardSells->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardSells = new cReportPatientcardSell( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardSells );
        m_repCardSells->setIndex( tabReports->addTab( m_repCardSells, QIcon("./resources/40x40_report_patientcard_sell.png"), m_repCardSells->name() ) );
        tabReports->setCurrentIndex( m_repCardSells->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardSells->index()-1 );
        tabReports->removeTab( m_repCardSells->index() );
        delete m_repCardSells;
        m_repCardSells = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardDebts( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardDebts" );

    m_bReportTabSwitching = true;

    action_Patientcard_Debts->setChecked( p_bChecked );
    pbPatientcardDebts->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardDebts = new cReportPatientcardDebts( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repCardDebts );
        m_repCardDebts->setIndex( tabReports->addTab( m_repCardDebts, QIcon("./resources/40x40_report_patientcard_sell.png"), m_repCardDebts->name() ) );
        tabReports->setCurrentIndex( m_repCardDebts->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardDebts->index()-1 );
        tabReports->removeTab( m_repCardDebts->index() );
        delete m_repCardDebts;
        m_repCardDebts = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportProducts(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportProducts" );

    m_bReportTabSwitching = true;

    action_Products->setChecked( p_bChecked );
    pbProducts->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repProducts = new cReportProducts( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repProducts );
        m_repProducts->setIndex( tabReports->addTab( m_repProducts, QIcon("./resources/40x40_report_products.png"), m_repProducts->name() ) );
        tabReports->setCurrentIndex( m_repProducts->index() );
    }
    else
    {
        m_qvReports.remove( m_repProducts->index()-1 );
        tabReports->removeTab( m_repProducts->index() );
        delete m_repProducts;
        m_repProducts = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportProductStatus(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportProducts" );

    m_bReportTabSwitching = true;

    action_Product_Status->setChecked( p_bChecked );
    pbProductStatus->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repProdStatus = new cReportProductStatus( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repProdStatus );
        m_repProdStatus->setIndex( tabReports->addTab( m_repProdStatus, QIcon("./resources/40x40_report_product_status.png"), m_repProdStatus->name() ) );
        tabReports->setCurrentIndex( m_repProdStatus->index() );
    }
    else
    {
        m_qvReports.remove( m_repProdStatus->index()-1 );
        tabReports->removeTab( m_repProdStatus->index() );
        delete m_repProdStatus;
        m_repProdStatus = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportProductHistory(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportProducts" );

    m_bReportTabSwitching = true;

    action_Product_History->setChecked( p_bChecked );
    pbProductHistory->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repProdHistory = new cReportProductHistory( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repProdHistory );
        m_repProdHistory->setIndex( tabReports->addTab( m_repProdHistory, QIcon("./resources/40x40_report_product_history.png"), m_repProdHistory->name() ) );
        tabReports->setCurrentIndex( m_repProdHistory->index() );
    }
    else
    {
        m_qvReports.remove( m_repProdHistory->index()-1 );
        tabReports->removeTab( m_repProdHistory->index() );
        delete m_repProdHistory;
        m_repProdHistory = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportGuests(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportGuests" );

    m_bReportTabSwitching = true;

    action_Guests->setChecked( p_bChecked );
    pbGuests->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repGuests = new cReportGuests( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repGuests );
        m_repGuests->setIndex( tabReports->addTab( m_repGuests, QIcon("./resources/40x40_patient.png"), m_repGuests->name() ) );
        tabReports->setCurrentIndex( m_repGuests->index() );
    }
    else
    {
        m_qvReports.remove( m_repGuests->index()-1 );
        tabReports->removeTab( m_repGuests->index() );
        delete m_repGuests;
        m_repGuests = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportDeviceUsages(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportDeviceUsages" );

    m_bReportTabSwitching = true;

    action_DeviceUsages->setChecked( p_bChecked );
    pbDeviceUsages->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repDeviceUsages = new cReportDeviceUsages( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repDeviceUsages );
        m_repDeviceUsages->setIndex( tabReports->addTab( m_repDeviceUsages, QIcon("./resources/40x40_device.png"), m_repDeviceUsages->name() ) );
        tabReports->setCurrentIndex( m_repDeviceUsages->index() );
    }
    else
    {
        m_qvReports.remove( m_repDeviceUsages->index()-1 );
        tabReports->removeTab( m_repDeviceUsages->index() );
        delete m_repDeviceUsages;
        m_repDeviceUsages = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportDeviceMinuteUsages(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportDeviceMinuteUsages" );

    m_bReportTabSwitching = true;

    action_DeviceMinuteUsages->setChecked( p_bChecked );
    pbDeviceMinuteUsages->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repDeviceMinuteUsages = new cReportDeviceMinuteUsages( this, "", _isInGroup( GROUP_ADMIN ) );

        m_qvReports.append( m_repDeviceMinuteUsages );
        m_repDeviceMinuteUsages->setIndex( tabReports->addTab( m_repDeviceMinuteUsages, QIcon("./resources/40x40_device_later.png"), m_repDeviceMinuteUsages->name() ) );
        tabReports->setCurrentIndex( m_repDeviceMinuteUsages->index() );
    }
    else
    {
        m_qvReports.remove( m_repDeviceMinuteUsages->index()-1 );
        tabReports->removeTab( m_repDeviceMinuteUsages->index() );
        delete m_repDeviceMinuteUsages;
        m_repDeviceMinuteUsages = NULL;
    }

    _updateReportIndexes();
    m_bReportTabSwitching = false;
}
//====================================================================================
// Altalanos fuggvenyek
//====================================================================================

//------------------------------------------------------------------------------------
void cWndMain::on_tabReports_currentChanged(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_tabReports_currentChanged" );

    m_bReportTabSwitching = true;

    if( index == 0 )
    {
        lblReportDescription->setText( tr("<i>Please select a report to show the related filters ...</i>") );
        _setFiltersEnabled( false );
    }
    else
    {
        cReport *obReport = m_qvReports.at(index-1);
        QString qsDescription = QString( "<b>%1</b>" ).arg( m_qvReports.at(index-1)->name() );

        if( m_qvReports.at(index-1)->description().length() > 0 )
        {
            qsDescription.append( QString( " - %1" ).arg( m_qvReports.at(index-1)->description() ) );
        }
        lblReportDescription->setText( qsDescription );
        _setFiltersEnabled( false );
        _setFiltersEnabledReport( obReport );
    }

    m_bReportTabSwitching = false;
}

//====================================================================================
// Privat fuggvenyek
//====================================================================================

//------------------------------------------------------------------------------------
void cWndMain::_setFiltersEnabled(bool p_bEnable)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setFiltersEnabled" );

    lblFilterDateStart->setVisible( p_bEnable );
    dtFilterDateStart->setEnabled( p_bEnable );
    dtFilterDateStart->setVisible( p_bEnable );

    lblFilterDateStop->setVisible( p_bEnable );
    dtFilterDateStop->setEnabled( p_bEnable );
    dtFilterDateStop->setVisible( p_bEnable );

    lblFilterDataName->setVisible( p_bEnable );
    ledFilterDataName->setEnabled( p_bEnable );
    ledFilterDataName->setVisible( p_bEnable );

    lblFilterDataType->setVisible( p_bEnable );
    cmbFilterDataTypes->setEnabled( p_bEnable );
    cmbFilterDataTypes->setVisible( p_bEnable );

    lblFilterDataSubType->setVisible( p_bEnable );
    cmbFilterDataSubTypes->setEnabled( p_bEnable );
    cmbFilterDataSubTypes->setVisible( p_bEnable );

    chkFilterIsVisible->setEnabled( p_bEnable );
    chkFilterIsVisible->setVisible( p_bEnable );

    pbRefresh->setEnabled( p_bEnable );
    pbRefresh->setVisible( p_bEnable );

    lblPrintPaperOrientation->setVisible( p_bEnable );
    rbOrientationPortrait->setEnabled( p_bEnable );
    rbOrientationPortrait->setVisible( p_bEnable );
    rbOrientationLandscape->setEnabled( p_bEnable );
    rbOrientationLandscape->setVisible( p_bEnable );
}
//------------------------------------------------------------------------------------
void cWndMain::_setFiltersEnabledReport(cReport *obReport)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setFiltersEnabledReport" );

    spacerFilter1->changeSize( 0, 20 );
    spacerFilter2->changeSize( 0, 20 );
    spacerFilter3->changeSize( 0, 20 );

    if( obReport->isDateStartEnabled() )
    {
        lblFilterDateStart->setVisible( true );
        dtFilterDateStart->setEnabled( true );
        dtFilterDateStart->setVisible( true );
        lblFilterDateStart->setText( obReport->labelDateStartText() );
        dtFilterDateStart->setDate( obReport->filterDateStart() );
        spacerFilter1->changeSize( 20, 20 );
    }

    if( obReport->isDateStopEnabled() )
    {
        lblFilterDateStop->setVisible( true );
        dtFilterDateStop->setEnabled( true );
        dtFilterDateStop->setVisible( true );
        lblFilterDateStop->setText( obReport->labelDateStopText() );
        dtFilterDateStop->setDate( obReport->filterDateStop() );
        spacerFilter1->changeSize( 20, 20 );
    }

    if( obReport->isDataNameEnabled() )
    {
        lblFilterDataName->setVisible( true );
        ledFilterDataName->setEnabled( true );
        ledFilterDataName->setVisible( true );
        lblFilterDataName->setText( obReport->labelDataNameText() );
        ledFilterDataName->setText( obReport->filterName() );
        spacerFilter2->changeSize( 20, 20 );
    }

    if( obReport->isDataTypeEnabled() )
    {
        lblFilterDataType->setVisible( true );
        cmbFilterDataTypes->setEnabled( true );
        cmbFilterDataTypes->setVisible( true );
        lblFilterDataType->setText( obReport->labelDataTypeText() );

        QStringList qslDataTypes    = obReport->filterTypeList().split('#');
        int         nCurrentIndex   = 0;

        cmbFilterDataTypes->clear();
        for( int i=0; i<qslDataTypes.count(); i++ )
        {
            QStringList qslDataType = qslDataTypes.at(i).split('|');
            cmbFilterDataTypes->addItem( qslDataType.at(1), qslDataType.at(0).toInt() );
            if( obReport->filterType().compare( qslDataTypes.at(i) ) == 0 )
            {
                nCurrentIndex = i;
            }
        }
        cmbFilterDataTypes->setCurrentIndex( nCurrentIndex );
        spacerFilter3->changeSize( 20, 20 );
    }

    if( obReport->isDataSubTypeEnabled() )
    {
        lblFilterDataSubType->setVisible( true );
        cmbFilterDataSubTypes->setEnabled( true );
        cmbFilterDataSubTypes->setVisible( true );
        lblFilterDataSubType->setText( obReport->labelDataSubTypeText() );

        QStringList qslDataSubTypes    = obReport->filterSubTypeList().split('#');
        int         nCurrentIndex   = 0;

        cmbFilterDataSubTypes->clear();
        for( int i=0; i<qslDataSubTypes.count(); i++ )
        {
            QStringList qslDataSubType = qslDataSubTypes.at(i).split('|');
            cmbFilterDataSubTypes->addItem( qslDataSubType.at(1), qslDataSubType.at(0).toInt() );
            if( obReport->filterSubType().compare( qslDataSubTypes.at(i) ) == 0 )
            {
                nCurrentIndex = i;
            }
        }
        cmbFilterDataSubTypes->setCurrentIndex( nCurrentIndex );
        spacerFilter3->changeSize( 20, 20 );
    }

    if( obReport->isDataIsVisibleEnabled() )
    {
        chkFilterIsVisible->setEnabled( true );
        chkFilterIsVisible->setVisible( true );
        chkFilterIsVisible->setText( obReport->labelIsVisibleText() );
        chkFilterIsVisible->setChecked( obReport->filterIsVisible() );
    }

    pbRefresh->setEnabled( true );
    pbRefresh->setVisible( true );

    lblPrintPaperOrientation->setVisible( true );
    rbOrientationPortrait->setEnabled( true );
    rbOrientationPortrait->setVisible( true );
    rbOrientationLandscape->setEnabled( true );
    rbOrientationLandscape->setVisible( true );
    if( obReport->pageOrientation() == QPrinter::Portrait )
        rbOrientationPortrait->setChecked( true );
    else
        rbOrientationLandscape->setChecked( true );
}
//------------------------------------------------------------------------------------
void cWndMain::_updateReportIndexes()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_updateReportIndexes" );

    for( int i=0; i<m_qvReports.count(); i++ )
    {
        m_qvReports.at(i)->setIndex( i+1 );
    }
    _updateReportButtons( true );
}
//------------------------------------------------------------------------------------
void cWndMain::on_dtFilterDateStart_dateChanged(const QDate &date)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_dtFilterDateStart_dateChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDateStart( date );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_dtFilterDateStop_dateChanged(const QDate &date)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_dtFilterDateStop_dateChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDateStop( date );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_ledFilterDataName_textEdited(const QString &arg1)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_ledFilterDataName_textEdited" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDataName( arg1 );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_cmbFilterDataTypes_currentIndexChanged(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_cmbFilterDataTypes_currentIndexChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    QString qsDataType = QString( "%1|%2" ).arg( cmbFilterDataTypes->itemData(index).toInt() ).arg( cmbFilterDataTypes->itemText(index) );

    obReport->setFilterDataType( qsDataType );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_cmbFilterDataSubTypes_currentIndexChanged(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_cmbFilterDataSubTypes_currentIndexChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    QString qsDataSubType = QString( "%1|%2" ).arg( cmbFilterDataSubTypes->itemData(index).toInt() ).arg( cmbFilterDataSubTypes->itemText(index) );

    obReport->setFilterDataSubType( qsDataSubType );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_rbOrientationPortrait_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_rbOrientationPortrait_clicked" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setPageOrientation( QPrinter::Portrait );
}
//------------------------------------------------------------------------------------
void cWndMain::on_rbOrientationLandscape_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_rbOrientationLandscape_clicked" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setPageOrientation( QPrinter::Landscape );
}
//------------------------------------------------------------------------------------
void cWndMain::on_chkFilterIsVisible_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_chkFilterIsVisible_clicked" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterIsVisible( chkFilterIsVisible->isChecked() );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbRefresh_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_pbRefresh_clicked" );

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->refreshReport();
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbPrint_clicked()
//------------------------------------------------------------------------------------
{
    QPrinter        obQPrinter;
    QPrintDialog    obDlgPrinter( &obQPrinter, this );
    cReport        *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obQPrinter.setOrientation( obReport->pageOrientation() );

    if( obDlgPrinter.exec() == QDialog::Accepted )
    {
        obReport->printReport( &obQPrinter );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbSave_clicked()
//------------------------------------------------------------------------------------
{
    QString qsFileName = QFileDialog::getSaveFileName(this,
                                                      tr("Save report to ..."),
                                                      QDir::currentPath(),
                                                      tr("Html files (*.html);;Text files (*.txt)") );

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->saveReport( qsFileName );
}
//------------------------------------------------------------------------------------
bool cWndMain::_isInGroup(groupUser p_enGroup)
//------------------------------------------------------------------------------------
{
    return ( p_enGroup <= m_enGroup );
}
//------------------------------------------------------------------------------------
void cWndMain::_updateReportButtons(bool p_bEnable)
//------------------------------------------------------------------------------------
{
    bool bReportVisible = false;

    if( m_qvReports.count() > 0 )
    {
        bReportVisible = true;
    }

    pbSave->setEnabled( p_bEnable && bReportVisible );
    pbPrint->setEnabled( p_bEnable && bReportVisible );
}
//------------------------------------------------------------------------------------
