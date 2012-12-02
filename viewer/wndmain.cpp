

#include "wndmain.h"
#include "dlgdemo.h"

cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    setupUi( this );

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

cWndMain::~cWndMain()
{
}

void cWndMain::_initActions()
{
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

void cWndMain::_initToolbar()
{
    // SIGNALS
    connect( pbExit, SIGNAL(clicked()), this, SLOT(close()) );

    connect( pbBookkeepingDaily, SIGNAL(toggled(bool)), this, SLOT(on_action_Bookkeeping_Daily_triggered(bool)) );
    connect( pbBookkeepingLedger, SIGNAL(toggled(bool)), this, SLOT(on_action_Bookkeeping_Ledger_triggered(bool)) );

    connect( pbPatientcardsActive, SIGNAL(toggled(bool)), this, SLOT(on_action_Patientcards_Active_triggered(bool)) );
    connect( pbPatientcardsInactive, SIGNAL(toggled(bool)), this, SLOT(on_action_Patientcards_Inactive_triggered(bool)) );
    connect( pbPatientcardsUsage, SIGNAL(toggled(bool)), this, SLOT(on_action_Patientcards_Usage_triggered(bool)) );

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

void cWndMain::_initFilterbar()
{
    // ICONS
    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );

    // BEHAVIOUR
    frameFilterbar->setVisible( false );
}

void cWndMain::_initTabInformation()
{
    // ICONS
    pbAuthenticate->setIcon( QIcon("./resources/40x40_key.png") );

    // BEHAVIOUR
    _setAuthInfoType( AUTH_NEEDED );
}

void cWndMain::_setAuthInfoType(authType p_tAuthType)
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

void cWndMain::on_action_Demo_triggered()
{
    cDlgDemo  obDlgDemo( this );

    obDlgDemo.exec();
}

void cWndMain::on_tabReports_tabCloseRequested(int index)
{
    if( index > 0 )
    {
        tabReports->removeTab(index);
    }
}

void cWndMain::on_action_Toolbar_triggered(bool checked)
{
    frameToolbar->setVisible( checked );
}

void cWndMain::on_action_FilterBar_triggered(bool checked)
{
    frameFilterbar->setVisible( checked );
}

void cWndMain::on_action_Bookkeeping_Daily_triggered(bool checked)
{
    action_Bookkeeping_Daily->setChecked( checked );
    pbBookkeepingDaily->setChecked( checked );

    _showReportDaily( checked );
}

void cWndMain::on_action_Bookkeeping_Ledger_triggered(bool checked)
{
    action_Bookkeeping_Ledger->setChecked( checked );
    pbBookkeepingLedger->setChecked( checked );
}

void cWndMain::on_action_Patientcards_Active_triggered(bool checked)
{
    action_Patientcards_Active->setChecked( checked );
    pbPatientcardsActive->setChecked( checked );
}

void cWndMain::on_action_Patientcards_Inactive_triggered(bool checked)
{
    action_Patientcards_Inactive->setChecked( checked );
    pbPatientcardsInactive->setChecked( checked );
}

void cWndMain::on_action_Patientcards_Usage_triggered(bool checked)
{
    action_Patientcards_Usage->setChecked( checked );
    pbPatientcardsUsage->setChecked( checked );
}

void cWndMain::_showReportDaily( bool p_bShow )
{
    if( p_bShow )
    {
        m_repDaily = new cReport();

        m_nReportDailyIndex = tabReports->addTab( m_repDaily, QIcon("./resources/40x40_book_daily.png"), tr(" Daily ledger") );
    }
    else
    {
        on_tabReports_tabCloseRequested( m_nReportDailyIndex );
        delete m_repDaily;
        m_repDaily = NULL;
    }
}

void cWndMain::_showReportLedger( bool p_bShow )
{

}

void cWndMain::_showReportPatientcardsActive( bool p_bShow )
{

}

void cWndMain::_showReportPatientcardsInactive( bool p_bShow )
{

}

void cWndMain::_showReportPatientcardsUsage( bool p_bShow )
{

}

void cWndMain::on_pbAuthenticate_clicked()
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
    }
    else
    {
        _setReportsEnabled( false );
    }
}

cWndMain::authType cWndMain::_authenticateUser()
{
    return AUTH_OK;
}

void cWndMain::_setReportsEnabled(bool p_bEnable)
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
