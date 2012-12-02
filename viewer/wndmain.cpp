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

    connect( ledUserName, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );
    connect( ledPassword, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );

    m_nReportDailyIndex                 = 0;
    m_nReportLedgerIndex                = 0;
    m_nReportPatientcardsActiveIndex    = 0;
    m_nReportPatientcardsInactiveIndex  = 0;
    m_nReportPatientcardsUsageIndex     = 0;

    _initActions();
    _initToolbar();
    _initFilterbar();
    _initTabInformation();
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
        if( m_nReportDailyIndex == index )
            emit setCheckedReportDaily( false );
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

        m_nReportDailyIndex = tabReports->addTab( m_repDaily, QIcon("./resources/40x40_book_daily.png"), tr(" Daily ledger") );
        tabReports->setCurrentIndex( m_nReportDailyIndex );
    }
    else
    {
        tabReports->removeTab( m_nReportDailyIndex );
        m_nReportDailyIndex = 0;
        delete m_repDaily;
        m_repDaily = NULL;
    }

}
//====================================================================================
void cWndMain::slotCheckReportLedger( bool p_bChecked )
//====================================================================================
{
    action_Bookkeeping_Ledger->setChecked( p_bChecked );
    pbBookkeepingLedger->setChecked( p_bChecked );

}
//====================================================================================
void cWndMain::slotCheckReportPatientcardActive( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Active->setChecked( p_bChecked );
    pbPatientcardsActive->setChecked( p_bChecked );

}
//====================================================================================
void cWndMain::slotCheckReportPatientcardInactive( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Inactive->setChecked( p_bChecked );
    pbPatientcardsInactive->setChecked( p_bChecked );

}
//====================================================================================
void cWndMain::slotCheckReportPatientcardUsage( bool p_bChecked )
//====================================================================================
{
    action_Patientcards_Usage->setChecked( p_bChecked );
    pbPatientcardsUsage->setChecked( p_bChecked );

}
//====================================================================================
