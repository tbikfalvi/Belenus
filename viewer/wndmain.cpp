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

//====================================================================================

#include "wndmain.h"
#include "dlgdemo.h"
#include "creportdaily.h"

//====================================================================================
cWndMain::cWndMain( QWidget *parent ) : QMainWindow( parent )
//====================================================================================
{
    setupUi( this );

    m_qsRPSW = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";

    connect( this, SIGNAL(setCheckedReportDaily(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( this, SIGNAL(setCheckedReportLedger(bool)), this, SLOT(slotCheckReportLedger(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardActive(bool)), this, SLOT(slotCheckReportPatientcardActive(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardInactive(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardUsage(bool)), this, SLOT(slotCheckReportPatientcardUsage(bool)) );

    connect( ledUserName, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );
    connect( ledPassword, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );

    _initActions();
    _initToolbar();
    _initFilterbar();
    _initTabInformation();

    on_tabReports_currentChanged( 0 );

    ledUserName->setFocus();
}
//====================================================================================
cWndMain::~cWndMain()
//====================================================================================
{
}
//====================================================================================
void cWndMain::_initActions()
//====================================================================================
{
    // SIGNALS
    connect( action_Bookkeeping_Daily, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( action_Bookkeeping_Ledger, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportLedger(bool)) );

    connect( action_Patientcards_Active, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardActive(bool)) );
    connect( action_Patientcards_Inactive, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( action_Patientcards_Usage, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardUsage(bool)) );

    // ICONS
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    action_Bookkeeping_Daily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    action_Bookkeeping_Ledger->setIcon( QIcon("./resources/40x40_book_ledger.png") );

    action_Patientcards_Active->setIcon( QIcon("./resources/40x40_patientcard_active.png") );
    action_Patientcards_Inactive->setIcon( QIcon("./resources/40x40_patientcard_inactive.png") );
    action_Patientcards_Usage->setIcon( QIcon("./resources/40x40_patientcard_usage.png") );

    // BEHAVIOUR
    action_FilterBar->setEnabled( false );

    action_Bookkeeping_Daily->setEnabled( false );
    action_Bookkeeping_Ledger->setEnabled( false );

    action_Patientcards_Active->setEnabled( false );
    action_Patientcards_Inactive->setEnabled( false );
    action_Patientcards_Usage->setEnabled( false );
}
//====================================================================================
void cWndMain::_initToolbar()
//====================================================================================
{
    // SIGNALS
    connect( pbExit, SIGNAL(clicked()), this, SLOT(close()) );

    connect( pbBookkeepingDaily, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( pbBookkeepingLedger, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportLedger(bool)) );

    connect( pbPatientcardsActive, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardActive(bool)) );
    connect( pbPatientcardsInactive, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( pbPatientcardsUsage, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardUsage(bool)) );

    // ICONS
    pbExit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    pbBookkeepingDaily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    pbBookkeepingLedger->setIcon( QIcon("./resources/40x40_book_ledger.png") );

    pbPatientcardsActive->setIcon( QIcon("./resources/40x40_patientcard_active.png") );
    pbPatientcardsInactive->setIcon( QIcon("./resources/40x40_patientcard_inactive.png") );
    pbPatientcardsUsage->setIcon( QIcon("./resources/40x40_patientcard_usage.png") );

    pbPrint->setIcon( QIcon("./resources/40x40_print.png") );

    // BEHAVIOUR
    pbBookkeepingDaily->setEnabled( false );
    pbBookkeepingLedger->setEnabled( false );

    pbPatientcardsActive->setEnabled( false );
    pbPatientcardsInactive->setEnabled( false );
    pbPatientcardsUsage->setEnabled( false );

    pbPrint->setEnabled( false );
}
//====================================================================================
void cWndMain::_initFilterbar()
//====================================================================================
{
    // ICONS
    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );

    // BEHAVIOUR
    frameFilterbar->setVisible( false );
}
//====================================================================================
void cWndMain::_initTabInformation()
//====================================================================================
{
    // ICONS
    tabReports->setTabIcon( 0, QIcon("./resources/40x40_information.png") );
    pbAuthenticate->setIcon( QIcon("./resources/40x40_key.png") );

    // BEHAVIOUR
    _setAuthInfoType( AUTH_NEEDED );
}
//====================================================================================
void cWndMain::_setAuthInfoType(authType p_tAuthType)
//====================================================================================
{
    ledUserName->setEnabled( true );
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

        ledUserName->setEnabled( false );
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
//====================================================================================
void cWndMain::on_action_Demo_triggered()
//====================================================================================
{
    cDlgDemo  obDlgDemo( this );

    obDlgDemo.exec();
}
//====================================================================================
void cWndMain::on_tabReports_tabCloseRequested(int index)
//====================================================================================
{
    if( index > 0 )
    {
        if( m_repDaily && m_repDaily->index() == index )
            emit setCheckedReportDaily( false );
        else if( m_repLedger && m_repLedger->index() == index )
            emit setCheckedReportLedger( false );
        else if( m_repCardActive && m_repCardActive->index() == index )
            emit setCheckedReportPatientcardActive( false );
        else if( m_repCardInactive && m_repCardInactive->index() == index )
            emit setCheckedReportPatientcardInactive( false );
        else if( m_repCardUsage && m_repCardUsage->index() == index )
            emit setCheckedReportPatientcardUsage( false );
    }
}
//====================================================================================
void cWndMain::on_action_Toolbar_triggered(bool checked)
//====================================================================================
{
    frameToolbar->setVisible( checked );
}
//====================================================================================
void cWndMain::on_action_FilterBar_triggered(bool checked)
//====================================================================================
{
    frameFilterbar->setVisible( checked );
}
//====================================================================================
void cWndMain::on_pbAuthenticate_clicked()
//====================================================================================
{
    authType    atRet = AUTH_NEEDED;

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
//====================================================================================
cWndMain::authType cWndMain::_authenticateUser()
//====================================================================================
{
    authType    atRet = AUTH_ERROR;
    QByteArray  obPwdHash = QCryptographicHash::hash( ledPassword->text().toAscii(), QCryptographicHash::Sha1 );

    if( ledUserName->text().compare( "root" ) == 0 )
    {
        if( m_qsRPSW.compare( QString( obPwdHash.toHex() ) ) == 0 )
        {
            atRet = AUTH_OK;
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
            QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + ledUserName->text() + "\"" );

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
//====================================================================================
void cWndMain::_setReportsEnabled(bool p_bEnable)
//====================================================================================
{
    action_FilterBar->setEnabled( p_bEnable );
    action_FilterBar->setChecked( p_bEnable );
    on_action_FilterBar_triggered( p_bEnable );

    action_Bookkeeping_Daily->setEnabled( p_bEnable );
    action_Bookkeeping_Ledger->setEnabled( p_bEnable );

    action_Patientcards_Active->setEnabled( p_bEnable );
    action_Patientcards_Inactive->setEnabled( p_bEnable );
    action_Patientcards_Usage->setEnabled( p_bEnable );

    pbBookkeepingDaily->setEnabled( p_bEnable );
    pbBookkeepingLedger->setEnabled( p_bEnable );

    pbPatientcardsActive->setEnabled( p_bEnable );
    pbPatientcardsInactive->setEnabled( p_bEnable );
    pbPatientcardsUsage->setEnabled( p_bEnable );

    pbPrint->setEnabled( p_bEnable );
}
//====================================================================================
void cWndMain::slotCheckReportDaily(bool p_bChecked)
//====================================================================================
{
    action_Bookkeeping_Daily->setChecked( p_bChecked );
    pbBookkeepingDaily->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repDaily = new cReportDaily();

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
        _updateReportIndexes();
    }
}
//====================================================================================
void cWndMain::slotCheckReportLedger( bool p_bChecked )
//====================================================================================
{
    action_Bookkeeping_Ledger->setChecked( p_bChecked );
    pbBookkeepingLedger->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repLedger = new cReportLedger();

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
        _updateReportIndexes();
    }
}
//====================================================================================
void cWndMain::slotCheckReportPatientcardActive( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Active->setChecked( p_bChecked );
    pbPatientcardsActive->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardActive = new cReportCardActive();

        m_qvReports.append( m_repCardActive );
        m_repCardActive->setIndex( tabReports->addTab( m_repCardActive, QIcon("./resources/40x40_book_daily.png"), m_repCardActive->name() ) );
        tabReports->setCurrentIndex( m_repCardActive->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardActive->index()-1 );
        tabReports->removeTab( m_repCardActive->index() );
        delete m_repCardActive;
        m_repCardActive = NULL;
        _updateReportIndexes();
    }
}
//====================================================================================
void cWndMain::slotCheckReportPatientcardInactive( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Inactive->setChecked( p_bChecked );
    pbPatientcardsInactive->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardInactive = new cReportCardInactive();

        m_qvReports.append( m_repCardInactive );
        m_repCardInactive->setIndex( tabReports->addTab( m_repCardInactive, QIcon("./resources/40x40_book_daily.png"), m_repCardInactive->name() ) );
        tabReports->setCurrentIndex( m_repCardInactive->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardInactive->index()-1 );
        tabReports->removeTab( m_repCardInactive->index() );
        delete m_repCardInactive;
        m_repCardInactive = NULL;
        _updateReportIndexes();
    }
}
//====================================================================================
void cWndMain::slotCheckReportPatientcardUsage( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Usage->setChecked( p_bChecked );
    pbPatientcardsUsage->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardUsage = new cReportCardUsage();

        m_qvReports.append( m_repCardUsage );
        m_repCardUsage->setIndex( tabReports->addTab( m_repCardUsage, QIcon("./resources/40x40_book_daily.png"), m_repCardUsage->name() ) );
        tabReports->setCurrentIndex( m_repCardUsage->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardUsage->index()-1 );
        tabReports->removeTab( m_repCardUsage->index() );
        delete m_repCardUsage;
        m_repCardUsage = NULL;
        _updateReportIndexes();
    }
}
//====================================================================================
void cWndMain::on_tabReports_currentChanged(int index)
//====================================================================================
{
    if( index == 0 )
    {
        lblReportDescription->setText( tr("<i>Please select a report to show the related filters ...</i>") );
        _setFiltersEnabled( false );
    }
    else
    {
        QString qsDescription = QString( "<b>%1</b>" ).arg( m_qvReports.at(index-1)->name() );

        if( m_qvReports.at(index-1)->description().length() > 0 )
        {
            qsDescription.append( QString( " - %1" ).arg( m_qvReports.at(index-1)->description() ) );
        }
        lblReportDescription->setText( qsDescription );
        _setFiltersEnabled( true );
    }
}
//====================================================================================
void cWndMain::_setFiltersEnabled(bool p_bEnable)
//====================================================================================
{
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

    pbRefresh->setEnabled( p_bEnable );
    pbRefresh->setVisible( p_bEnable );
}
//====================================================================================
void cWndMain::_updateReportIndexes()
//====================================================================================
{
    for( int i=0; i<m_qvReports.count(); i++ )
    {
        m_qvReports.at(i)->setIndex( i+1 );
    }
}
//====================================================================================

