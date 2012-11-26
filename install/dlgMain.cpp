//=======================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//=======================================================================================
//
// Filename    : dlgMain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//=======================================================================================
//
//=======================================================================================

#include <windows.h>
#include <winuser.h>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QDir>

//=======================================================================================

#include "dlgMain.h"
#include "ui_dlgMain.h"
#include "../client/communication_serial.h"

//=======================================================================================
dlgMain::dlgMain(QWidget *parent, bool bUninstall) : QDialog(parent)
//=======================================================================================
{
    setupUi(this);

    // Set the pager control to the first - Welcome - page
    pagerControl->setCurrentIndex( CONST_PAGE_WELCOME );

    // Load the images, icons etc.
    setWindowIcon( QIcon( QString(":/icons/belenus.ico") ) );

    // Initialize variables
    // Get common settings from registry
    // Set flags for install/update process
    _initializeInstall();

    // Set setup type based on previously set flag
    if( m_bBelenusAlreadyInstalled )
        m_pInstallType = rbUpdate;
    else
        m_pInstallType = rbInstall;

    // Initialize pages has to show on start
    m_vPages.clear();
    m_vPages.append( CONST_PAGE_WELCOME );
    m_vPages.append( CONST_PAGE_INSTALL_SELECTION );
    m_vPages.append( CONST_PAGE_FINISH );

    // Set current page index to welcome page
    m_nCurrentPage = CONST_PAGE_WELCOME;

    // Initialize GUI components
    cmbLanguage->addItem( "Magyar (hu)" );
    cmbLanguage->addItem( "English (en)" );

    // If application called with uninstall flag, start uninstall process
    if( bUninstall )
        _uninstallBelenus();
}
//=======================================================================================
dlgMain::~dlgMain()
//=======================================================================================
{
    if( m_obLog != NULL )       delete m_obLog;
    if( m_poDB != NULL )        delete m_poDB;
    if( m_poHardware != NULL )  delete m_poHardware;
}
//=======================================================================================
//
//  _initializeInstall
//
//=======================================================================================
//  Get and set default values from registry, variables etc
//  Check system and application components
//=======================================================================================
void dlgMain::_initializeInstall()
//=======================================================================================
{
    //-----------------------------------------------------------------------------------
    //  Get Windows default values from registry
    //-----------------------------------------------------------------------------------

    m_qsPathWindows     = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "SystemRoot", "c:\\windows" );
    m_qsPathPrograms    = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Common Programs", "" );
    m_qsPathDesktop     = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Common Desktop", "" );

    if( m_qsPathWindows.length() == 0 || m_qsPathPrograms.length() == 0 || m_qsPathDesktop.length() == 0 )
    {
        int nErrCode = 0;
        if( m_qsPathWindows.length() == 0 ) nErrCode += 1;
        if( m_qsPathPrograms.length() == 0 ) nErrCode += 2;
        if( m_qsPathDesktop.length() == 0 ) nErrCode += 4;
        pbNext->setEnabled( false );
        QMessageBox::critical( this, tr("Error"),
                               tr("Error occured during initialization.\n"
                                  "Please contact system administrator.\n"
                                  "Error code: ErrSysRegKeysFail%1").arg(nErrCode) );
        return;
    }

    //-----------------------------------------------------------------------------------
    //
    //  Create temporary install directory and log file if not exists
    //
    //-----------------------------------------------------------------------------------
    QDir    qdTempInstallDir( QString("%1\\Temp\\BelenusInstall").arg(m_qsPathWindows) );

    if( !qdTempInstallDir.exists() )
    {
        if( !_createTargetDirectory( QString("%1\\Temp\\BelenusInstall").arg(m_qsPathWindows) ) )
        {
            pbNext->setEnabled( false );
            QMessageBox::critical( this, tr("Error"),
                                   tr("Error occured during initialization.\n"
                                      "Please contact system administrator.\n"
                                      "Error code: ErrInstDirCreateFail\n"
                                      "[%1]").arg(QString("%1\\Temp\\BelenusInstall").arg(m_qsPathWindows)) );
            return;
        }
    }

    m_obLog = new QFile( QString("%1\\Temp\\BelenusInstall\\BelenusSetup.log").arg(m_qsPathWindows) );
    if( m_obLog == NULL )
    {
        pbNext->setEnabled( false );
        QMessageBox::critical( this, tr("Error"),
                               tr("Error occured during initialization.\n"
                                  "Please contact system administrator.\n"
                                  "Error code: ErrLogCreateFail\n"
                                  "[%1]").arg(QString("%1\\Temp\\BelenusInstall\\BelenusSetup.log").arg(m_qsPathWindows)) );
        return;
    }

    _logProcess( QString("") );
    _logProcess( QString("=====================================================================") );
    _logProcess( QString("") );
    _logProcess( QString("Belenus Setup V%1").arg(CONST_INSTALL_APP_VERSION) );
    _logProcess( QString("") );
    _logProcess( QString("System directories:") );
    _logProcess( QString("Windows: %1").arg(m_qsPathWindows) );
    _logProcess( QString("Desktop: %1").arg(m_qsPathDesktop) );
    _logProcess( QString("Programs: %1").arg(m_qsPathPrograms) );
    _logProcess( QString("") );

    //-----------------------------------------------------------------------------------
    //  Initialize public and private variables
    //-----------------------------------------------------------------------------------

    // Set this flag when component selection page passed to block welcome page
    m_bInstallStarted           = false;

    // Set when application called with -uninstall parameter
    m_bUninstallCalled          = false;

    // Flag for demo mode
    m_bDemoMode                 = false;

    // Identifies which component has to be processed
    m_bProcessWamp              = true;
    m_bProcessDatabase          = true;
    m_bProcessHWConnection      = true;
    m_bProcessBelenusClient     = true;
    m_bProcessViewer            = true;

    // Flags for timer
//    m_bStartWampInstall         = false;
    m_bInitializeWamp           = false;
    m_bInstallClient            = false;
    m_bInstallFinished          = false;

    // If Wamp server installed
    m_bWampServerAlreadyInstalled = false;

    // If database created during install, set this flag
    m_bDatabaseAlreadyInstalled = false;

    // If root user created, set this flag
    m_bRootUserExists           = false;

    // If belenus user created, set this flag
    m_bBelenusUserExists        = false;

    // Determines which wamp needed to be installed
    m_bIsWindows32Bit           = true;

    // Default settings for Wamp server
    m_qsPathWampServer          = "";
    m_qsUninstallWampExec       = "";

    // Default settings for database
    m_qsRootPassword            = "adminpass";
    m_qsRootPasswordNew         = "";

    // Default settings for hardware
    m_poHardware                = NULL;
    m_nComPort                  = 0;
    m_nCountDevices             = 0;

    // Default settings for internet connection
    m_qsIPAddress               = QString( "127.0.0.1" );
    m_nPort                     = 1000;

    m_qsIniFileName             = "";

    // Default settings for client
    m_qsClientInstallDir        = QString( "C:\\Program Files\\Belenus" );
    // If computer restart required, set this flag
    m_bRestartRequired          = false;
    // Language of the client application
    m_qsLanguage                = "hu";

    //-----------------------------------------------------------------------------------
    //  Check install components
    //-----------------------------------------------------------------------------------

    // Check Wamp server
    if( _checkWampServer() == 0 )
        m_bWampServerAlreadyInstalled = true;

    _logProcess( QString("Wamp server %1").arg(m_bWampServerAlreadyInstalled?"installed":"not installed") );

    // Initialize SQL connection
    m_poDB = NULL;
    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    // If Wamp server installed get settings and check Belenus database and user
    if( m_bWampServerAlreadyInstalled )
    {
        m_qsPathWampServer = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1", "Inno Setup: App Path", "" );
        m_qsUninstallWampExec = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1", "UninstallString", "" );

        _logProcess( QString("Wamp Server: %1").arg(m_qsPathWampServer) );
        _logProcess( QString("") );

        // Check root user
        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "mysql" );
        m_poDB->setUserName( "root" );
        m_poDB->setPassword( m_qsRootPassword );

        if( m_poDB->open() )
        {
            _logProcess( QString("Root user already set") );
            m_bRootUserExists = true;
            m_poDB->close();
        }

        // Check database server and database
        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "belenus" );
        m_poDB->setUserName( "belenus" );
        m_poDB->setPassword( "belenus" );

        if( m_poDB->open() )
        {
            _logProcess( QString("Belenus user already set") );
            m_bBelenusUserExists = true;
            m_poDB->close();
        }
    }

    // Check Belenus client
    m_bBelenusAlreadyInstalled = g_obReg.isRegPathExists( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" );

    _logProcess( QString("Belenus application %1").arg(m_bBelenusAlreadyInstalled?"installed":"not installed") );

    if( m_bBelenusAlreadyInstalled )
    {
        _logProcess( QString("Belenus application installed") );
        m_qsClientInstallDir = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", "InstallLocation", "" );
        m_qsLanguage = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("Language"), "hu" );
        QString qsTemp = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", "Components", "" );
        m_qslComponents = qsTemp.split( "#" );
        _logProcess( QString("Application location: %1").arg(m_qsClientInstallDir) );
        _logProcess( QString("Setup application language: %1").arg(m_qsLanguage) );
        _logProcess( QString("Installed components:") );
        for( int i=0; i<m_qslComponents.count(); i++ )
        {
            _logProcess( QString("%1").arg(m_qslComponents.at(i)) );
        }

        m_qsIniFileName = QString( "%1\\belenus.ini" ).arg(m_qsClientInstallDir);
    }
}
//=======================================================================================
//
//  _uninstallBelenus
//
//=======================================================================================
//  If application called with -uninstall parameter start uninstall process
//  using timer
//=======================================================================================
void dlgMain::_uninstallBelenus()
//=======================================================================================
{
    m_bUninstallCalled = true;
    m_nTimer = startTimer( 500 );
}
//=======================================================================================
//
//  closeEvent
//
//=======================================================================================
//  Called when user click on window close button or press ALT+F4
//=======================================================================================
void dlgMain::closeEvent( QCloseEvent *p_poEvent )
//=======================================================================================
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to abort installation?"),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
    {
        p_poEvent->ignore();
    }
}
//=======================================================================================
//
//  on_pbCancel_clicked
//
//=======================================================================================
//  Called when user clicks on Cancel button
//=======================================================================================
void dlgMain::on_pbCancel_clicked()
//=======================================================================================
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to abort installation?"),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        QDialog::reject();
    }
}
//=======================================================================================
//
//  on_pbPrev_clicked
//
//=======================================================================================
//  Called when Previous button clicked. Set pager control to previous page.
//  Welcome page can not be accessed after Component selection page has left
//=======================================================================================
void dlgMain::on_pbPrev_clicked()
//=======================================================================================
{
    // Check if step back can be done
    if( m_nCurrentPage > (m_bInstallStarted?CONST_PAGE_COMPONENT_SELECTION:CONST_PAGE_WELCOME) )
        m_nCurrentPage--;

    // Step back to previous page
    pagerControl->setCurrentIndex( m_vPages.at( m_nCurrentPage ) );

    // If current page is the first page (depends on install started), disable Prev button
    if( m_nCurrentPage == (m_bInstallStarted?CONST_PAGE_COMPONENT_SELECTION:CONST_PAGE_WELCOME) )
        pbPrev->setEnabled( false );
    // If current page is not the last page, enable Next button
    if( m_nCurrentPage < m_vPages.size()-1 )
        pbNext->setEnabled( true );

    // Enable Cancel button
    pbCancel->setEnabled( true );
    // Disable Start/Exit button
    pbStartExit->setEnabled( false );
    // Because left the last page set Start/Exit button text to Start
    pbStartExit->setText( tr("Start") );
}
//=======================================================================================
//
//  on_pbNext_clicked
//
//=======================================================================================
//  Called when Next button clicked. Set pager control to next page.
//=======================================================================================
void dlgMain::on_pbNext_clicked()
//=======================================================================================
{
    // Process the actual page. If any error occures, halt
    if ( !_processPage( m_vPages.at( m_nCurrentPage ) ) )
        return;

    // Check if step forward can be done
    if( m_nCurrentPage < m_vPages.size()-1 )
        m_nCurrentPage++;

    // If just left Component selection page, set InstallStarted flag
    // (no return to Welcome page)
    if( m_nCurrentPage > CONST_PAGE_COMPONENT_SELECTION )
        m_bInstallStarted = true;

    // Step forward to next page
    pagerControl->setCurrentIndex( m_vPages.at( m_nCurrentPage ) );

    // Enable Prev button if not on the first page
    if( m_nCurrentPage > CONST_PAGE_WELCOME )
        pbPrev->setEnabled( true );
    // If last page reached disable all button except Start/Exit button
    // Set Start/Exit button text to Exit
    if( m_nCurrentPage == m_vPages.size()-1 )
    {
        pbCancel->setEnabled( false );
        pbPrev->setEnabled( false );
        pbNext->setEnabled( false );
        pbStartExit->setEnabled( true );
        pbStartExit->setText( tr("Exit") );
    }

    // Process the actual page initialization
    _initializePage( m_vPages.at( m_nCurrentPage ) );
}
//=======================================================================================
//
//  on_pbStartExit_clicked
//
//=======================================================================================
// Called when Start/Exit button clicked
// If text is "Start" then start the installation process
// If text is "Exit" then exit from install application
//=======================================================================================
void dlgMain::on_pbStartExit_clicked()
//=======================================================================================
{
    if( pbStartExit->text().compare( tr("Exit") ) == 0 )
    {
        QDialog::accept();
    }
}
//=======================================================================================
//
//  _initializePage
//
//=======================================================================================
// Initialize actual page
//=======================================================================================
void dlgMain::_initializePage( int p_nPage )
//=======================================================================================
{
    switch( p_nPage )
    {
        case CONST_PAGE_WELCOME:
            _initializeWelcomePage();
            break;

        case CONST_PAGE_INSTALL_SELECTION:
            _initializeSelectionPage();
            break;

        case CONST_PAGE_COMPONENT_SELECTION:
            _initializeComponentSelectionPage();
            break;

        case CONST_PAGE_WAMP_INSTALL:
            _initializeWampInstallPage();
            break;

        case CONST_PAGE_INIT_SQL:
            _initializeSQLPage();
            break;

        case CONST_PAGE_HARDWARE_INSTALL:
            _initializeHardwareInstallPage();
            break;

        case CONST_PAGE_CLIENT_INSTALL:
            _initializeClientInstallPage();
            break;

        case CONST_PAGE_PROCESS:
            _initializeInstallProcessPage();
            break;

        case CONST_PAGE_FINISH:
            _initializeFinishPage();
            break;
    }
}
//=======================================================================================
//
//  _processPage
//
//=======================================================================================
// Process the actual page
//=======================================================================================
bool dlgMain::_processPage( int p_nPage )
//=======================================================================================
{
    bool bRet = true;

    switch( p_nPage )
    {
        case CONST_PAGE_INSTALL_SELECTION:
            bRet = _processSelectionPage();
            break;

        case CONST_PAGE_COMPONENT_SELECTION:
            bRet = _processComponentSelectionPage();
            break;

        case CONST_PAGE_WAMP_INSTALL:
            bRet = _processWampInstallPage();
            break;

        case CONST_PAGE_INIT_SQL:
            bRet = _processSQLPage();
            break;

        case CONST_PAGE_HARDWARE_INSTALL:
            bRet = _processHardwareInstallPage();
            break;

        case CONST_PAGE_CLIENT_INSTALL:
            bRet = _processClientInstallPage();
            break;
    }

    return bRet;
}
//=======================================================================================
//
//  timerEvent
//
//=======================================================================================
// Called when timer has been initialized by one of the pages
//=======================================================================================
void dlgMain::timerEvent(QTimerEvent *)
//=======================================================================================
{
    if( m_bInitializeWamp  )
    {
        _installSQLServer();
    }
    // If install process needs to be started
    else if( m_bInstallClient )
    {
        _processInstall();
    }
    // If install process finished
    else if( m_bInstallFinished )
    {
        m_bInstallFinished = false;
        killTimer( m_nTimer );
        on_pbNext_clicked();
    }
    // In any other case (practically when uninstall called and welcome page displayed)
    else // if( m_bUninstallCalled )
    {
        killTimer( m_nTimer );
        _initializePage( m_vPages.at( m_nCurrentPage ) );
    }
}
//=======================================================================================
//
//  W E L C O M E  P A G E
//
//=======================================================================================
void dlgMain::_initializeWelcomePage()
//=======================================================================================
{
    if( m_bUninstallCalled )
    {
        //on_cmbLanguage_currentIndexChanged( cmbLanguage->findText( QString("(%1)").arg(m_qsLanguage) ) );
        pbCancel->setEnabled( false );
        pbNext->setEnabled( false );
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Are you sure you want to uninstall Belenus Application System and all of it's components?\n"
                                      "All of the data will be deleted from the computer."),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        {
            m_vPages.clear();
            m_vPages.append( CONST_PAGE_WELCOME );
            m_vPages.append( CONST_PAGE_PROCESS );
            m_vPages.append( CONST_PAGE_FINISH );
            m_pInstallType = rbRemove;
            on_pbNext_clicked();
        }
        else
        {
            QDialog::accept();
        }
    }
}
//=======================================================================================
void dlgMain::on_cmbLanguage_currentIndexChanged(int index)
//=======================================================================================
{
    if( !m_bUninstallCalled )
        m_qsLanguage = cmbLanguage->itemText( cmbLanguage->currentIndex() ).right(3).left(2);

    _logProcess( QString("Language selected: %1").arg(m_qsLanguage) );

    apMainApp->removeTranslator( poTransSetup );
    apMainApp->removeTranslator( poTransQT );

    if( m_qsLanguage.compare("en") )
    {
        QString qsLangSetup = QString("%1\\setup_%2.qm").arg(g_qsCurrentPath).arg( m_qsLanguage );
        QString qsLangQT = QString("%1\\qt_%2.qm").arg(g_qsCurrentPath).arg( m_qsLanguage );

        poTransSetup->load( qsLangSetup );
        poTransQT->load( qsLangQT );

        apMainApp->installTranslator( poTransSetup );
        apMainApp->installTranslator( poTransQT );
    }

    retranslateUi( this );
}
//=======================================================================================
//
//  P A G E  S E L E C T I O N
//
//=======================================================================================
void dlgMain::_initializeSelectionPage()
//=======================================================================================
{
    // If Belenus already installed enable only update and remove
    if( m_bBelenusAlreadyInstalled )
    {
        rbInstall->setEnabled( false );
        imgInstall1->setEnabled( false );
        lblTextInstall->setEnabled( false );
        rbUpdate->setEnabled( true );
        imgUpdate->setEnabled( true );
        lblTextUpdate->setEnabled( true );
        rbRemove->setEnabled( true );
        imgRemove->setEnabled( true );
        lblTextRemove->setEnabled( true );
    }
    // If Belenus not or not fully installed enable only install
    else
    {
        rbInstall->setEnabled( true );
        imgInstall1->setEnabled( true );
        lblTextInstall->setEnabled( true );
        rbUpdate->setEnabled( false );
        imgUpdate->setEnabled( false );
        lblTextUpdate->setEnabled( false );
        rbRemove->setEnabled( false );
        imgRemove->setEnabled( false );
        lblTextRemove->setEnabled( false );

        rbInstall->setChecked( true );
    }
    // Check the radio button selected by initialization
    m_pInstallType->setChecked( true );
}
//=======================================================================================
void dlgMain::on_rbInstall_clicked()
//=======================================================================================
{
    m_pInstallType = rbInstall;
}
//=======================================================================================
void dlgMain::on_rbUpdate_clicked()
//=======================================================================================
{
    m_pInstallType = rbUpdate;
}
//=======================================================================================
void dlgMain::on_rbRemove_clicked()
//=======================================================================================
{
    m_pInstallType = rbRemove;
}
//=======================================================================================
bool dlgMain::_processSelectionPage()
//=======================================================================================
{
    // If install selected, set all components to be installed
    if( m_pInstallType == rbInstall )
    {
        m_bProcessWamp          = true;
        m_bProcessDatabase      = true;
        m_bProcessHWConnection  = true;
        m_bProcessBelenusClient = true;
        m_bProcessViewer        = true;
    }
    // If update selected, set all component except Wamp server
    else if( m_pInstallType == rbUpdate )
    {
        m_bProcessWamp          = false;
        m_bProcessDatabase      = true;
        m_bProcessHWConnection  = true;
        m_bProcessBelenusClient = true;
        m_bProcessViewer        = true;
    }
    // If remove selected, start remove process if verified
    else if( m_pInstallType == rbRemove )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Are you sure you want to uninstall Belenus Application System and all of it's components?\n"
                                      "All of the data will be deleted from the computer."),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        {
            m_vPages.clear();
            m_vPages.append( CONST_PAGE_WELCOME );
            m_vPages.append( CONST_PAGE_PROCESS );
            m_vPages.append( CONST_PAGE_FINISH );
            m_nCurrentPage = CONST_PAGE_WELCOME;
            return true;
        }
        else
        {
            return false;
        }
    }

    // In case of install or update, enable the needed pages in pager control
    _refreshPages();

    return true;
}
//=======================================================================================
//
//  C O M P O N E N T  S E L E C T I O N
//
//=======================================================================================
void dlgMain::_initializeComponentSelectionPage()
//=======================================================================================
{
    if( m_pInstallType == rbInstall )
    {
        chkWamp->setEnabled( true );
        imgWamp->setEnabled( true );
        lblTextWamp->setEnabled( true );

        chkDatabase->setEnabled( true );
        imgDatabase->setEnabled( true );
        lblTextDatabase->setEnabled( true );

        chkHardware->setEnabled( true );
        imgConnection->setEnabled( true );
        lblTextHardware->setEnabled( true );

        chkBelenus->setEnabled( true );
        imgBelenus->setEnabled( true );
        lblTextBelenus->setEnabled( true );

// Currently no standalone viewer exists
//        chkViewer->setEnabled( true );
//        imgViewer->setEnabled( true );
//        lblTextViewer->setEnabled( true );
    }
    else if( m_pInstallType == rbUpdate )
    {
        m_bProcessWamp = false;

        if( m_qslComponents.count() > 0 )
        {
            if( m_qslComponents.indexOf( "Database" ) == -1 )
                m_bProcessDatabase = false;
            if( m_qslComponents.indexOf( "Hardware" ) == -1 )
                m_bProcessHWConnection = false;
            if( m_qslComponents.indexOf( "Client" ) == -1 )
                m_bProcessBelenusClient = false;
// Currently no standalone viewer exists
//            if( m_qslComponents.indexOf( "Viewer" ) == -1 )
//                m_bProcessViewer = false;
        }
    }

    if( m_bWampServerAlreadyInstalled )
    {
        chkWamp->setEnabled( false );
        imgWamp->setEnabled( false );
        lblTextWamp->setEnabled( false );
        m_bProcessWamp = false;
    }

    chkWamp->setChecked( m_bProcessWamp );
    chkDatabase->setChecked( m_bProcessDatabase );
    chkHardware->setChecked( m_bProcessHWConnection );
    chkBelenus->setChecked( m_bProcessBelenusClient );
// Currently no standalone viewer exists
//    chkViewer->setChecked( m_bProcessViewer );

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkWamp_clicked()
//=======================================================================================
{
    m_bProcessWamp = chkWamp->isChecked();
    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkDatabase_clicked()
//=======================================================================================
{
    m_bProcessDatabase = chkDatabase->isChecked();

    if( m_pInstallType == rbInstall && chkDatabase->isChecked() )
    {
        if( chkWamp->isEnabled() )
            chkWamp->setChecked( true );
        chkBelenus->setChecked( true );
    }

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkHardware_clicked()
//=======================================================================================
{
    m_bProcessHWConnection = chkHardware->isChecked();

    if( m_pInstallType == rbInstall && chkHardware->isChecked() )
    {
        if( chkWamp->isEnabled() )
            chkWamp->setChecked( true );
        chkDatabase->setChecked( true );
        chkBelenus->setChecked( true );
    }

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkBelenus_clicked()
//=======================================================================================
{
    m_bProcessBelenusClient = chkBelenus->isChecked();

    if( m_pInstallType == rbInstall && chkBelenus->isChecked() )
    {
        if( chkWamp->isEnabled() )
            chkWamp->setChecked( true );
        chkDatabase->setChecked( true );
    }

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkViewer_clicked()
//=======================================================================================
{
    m_bProcessViewer = chkViewer->isChecked();

    if( m_pInstallType == rbInstall && chkViewer->isChecked() )
    {
        chkWamp->setChecked( true );
        chkDatabase->setChecked( true );
    }

    _setEnableNextButton();
}
//=======================================================================================
bool dlgMain::_processComponentSelectionPage()
//=======================================================================================
{
    if( m_pInstallType == rbInstall )
    {
        if( chkViewer->isChecked() )
        {
            chkWamp->setChecked( true );
            chkDatabase->setChecked( true );
        }

        if( chkBelenus->isChecked() )
        {
            chkWamp->setChecked( true );
            chkDatabase->setChecked( true );
            chkViewer->setChecked( true );
        }

        if( chkDatabase->isChecked() )
        {
            chkWamp->setChecked( true );
        }

        if( m_bWampServerAlreadyInstalled )
            chkWamp->setChecked( false );
    }

    m_bProcessWamp          = chkWamp->isChecked();
    m_bProcessDatabase      = chkDatabase->isChecked();
    m_bProcessHWConnection  = chkHardware->isChecked();
    m_bProcessBelenusClient = chkBelenus->isChecked();
// Currently no standalone viewer exists
//    m_bProcessViewer        = chkViewer->isChecked();

    _refreshPages();

    m_qslComponents.clear();

    return true;
}
//=======================================================================================
//
//  W A M P / S Q L  I N S T A L L
//
//=======================================================================================
void dlgMain::_initializeWampInstallPage()
//=======================================================================================
{
    if( m_bWampServerAlreadyInstalled )
    {
        on_pbNext_clicked();
    }
    else
    {
        pbNext->setEnabled( false );

        rbWin32->setChecked( m_bIsWindows32Bit );
        pbStartWampInstall->setEnabled( true );
    }
}
//=======================================================================================
void dlgMain::on_pbStartWampInstall_clicked()
//=======================================================================================
{
    _installWampServer();
}
//=======================================================================================
void dlgMain::_installWampServer()
//=======================================================================================
{
    _logProcess( QString("Wamp installation ..."), false );
    pbNext->setEnabled( false );
    QString qsMessage;
//    m_bStartWampInstall = false;
    if( _processWampServerInstall(&qsMessage) )
    {
        _logProcess( QString(" SUCCEEDED") );
        m_bInitializeWamp = true;
        m_nTimer = startTimer( 1000 );
    }
    else
    {
        pbCancel->setEnabled( true );
        pbPrev->setEnabled( true );
        _logProcess( QString(" FAILED") );
        QMessageBox::warning( this, tr("Attention"),
                              tr("Error occured during installation.\n\n%1\n\n"
                                 "If installation continuously fails please\ncontact Belenus software support.").arg(qsMessage) );
    }
}
//=======================================================================================
bool dlgMain::_processWampServerInstall( QString *p_qsMessage )
//=======================================================================================
{
    bool    bRet            = false;
    QString qsProcessPath   = "";
    QString qsRedistPack    = "";
    QString qsWampServer    = "";
    int     nRet            = 0;

    qsProcessPath = g_qsCurrentPath;

    if( rbWin32->isChecked() )
    {
        qsProcessPath.append( "\\win32" );
        qsRedistPack = "vcredist_x86.exe";
        qsWampServer = "wampserver2.2e_win32.exe";
    }
    else if( rbWin64->isChecked() )
    {
        qsProcessPath.append( "\\win64" );
        qsRedistPack = "vcredist_x64.exe";
        qsWampServer = "wampserver2.2e_win64.exe";
    }

    _logProcess( QString("Check Wamp install in registry\n") );

    nRet = _checkWampServer();

    if( nRet == 1 )
    {
        _logProcess( QString("Execute %1\\%2").arg(qsProcessPath).arg(qsRedistPack) );

        hide();

        QProcess *qpRedist = new QProcess();
        if( qpRedist->execute( QString("%1\\%2").arg(qsProcessPath).arg(qsRedistPack) ) )
            nRet = 3;
        delete qpRedist;

        _logProcess( QString("Execute %1\\%2").arg(qsProcessPath).arg(qsWampServer) );

        QProcess *qpWamp = new QProcess();
        if( qpWamp->execute( QString("%1\\%2").arg(qsProcessPath).arg(qsWampServer) ) )
            nRet = 4;
        delete qpWamp;

        show();
        QApplication::processEvents();

        nRet = _checkWampServer();
    }

    switch( nRet )
    {
        case 0:
        {
            _logProcess( "Wamp Server installed and registered into registry\n" );
            bRet = true;
            break;
        }
        case 1:
        {
            _logProcess( "Wamp Server application is not registered in Windows registry\n" );
            *p_qsMessage = QString( "Wamp Server application is not registered in Windows registry" );
            break;
        }
        case 2:
        {
            _logProcess( "Wamp version not match.\n" );
            QString qsVersion = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1",
                                                  "Inno Setup: Setup Version",
                                                  "" );
            *p_qsMessage = QString( "Currently installed Wamp Server version (%1)\n"
                                   "is not match with the required 5.4.0 (a)\n"
                                   "Please uninstall the current Wamp Server and\n"
                                   "restart the Wamp Server installation process." ).arg(qsVersion);
            break;
        }
        case 3:
        {
            _logProcess( "Installing Microsoft Redistributeable Package failed.\n" );
            *p_qsMessage = QString( "Installing Microsoft Redistributeable Package failed." );
            break;
        }
        case 4:
        {
            _logProcess( "Installing Wamp Server application failed.\n" );
            *p_qsMessage = QString( "Installing Wamp Server application failed." );
            break;
        }
        default:
        {
            _logProcess( "Unexpected error occured\n" );
            *p_qsMessage = QString( "Unexpected error occured." );
            break;
        }
    }

    return bRet;
}
//=======================================================================================
int dlgMain::_checkWampServer()
//=======================================================================================
{
    int nRet = 0;

    if( g_obReg.isRegPathExists( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1" ) )
    {
        QString qsVersion = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1",
                                              "Inno Setup: Setup Version",
                                      "" );
        if( qsVersion.compare( "5.4.0 (a)" ) != 0 )
        {
            nRet = 2;
        }
    }
    else
    {
        nRet = 1;
    }

    return nRet;
}
//=======================================================================================
void dlgMain::_installSQLServer()
//=======================================================================================
{
    killTimer( m_nTimer );
    pbNext->setEnabled( false );
//    m_bStartWampInstall = false;
    m_bInitializeWamp = false;

    if( !_initializeMySQL() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Wamp server initialization failed.\n"
                                 "Please try to reinstall it with going back one page "
                                 "then return to this page.\n\n"
                                 "If Wamp install continuously fails please contact Belenus software support.") );
        pbCancel->setEnabled( true );
        pbNext->setEnabled( true );
        return;
    }
    on_pbNext_clicked();
}
//=======================================================================================
bool dlgMain::_initializeMySQL()
//=======================================================================================
{
    bool bRet = false;

    if( g_obReg.isRegPathExists( QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wampmysqld") ) )
    {
        g_obReg.setKeyValueN( QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wampmysqld"), QString("Start"), 2 );
        bRet = true;
    }
    _logProcess( QString("Setting reg. value wampmysqld/Start ... %1").arg((bRet?"SUCCEEDED":"FAILED")) );

    return bRet;
}
//=======================================================================================
bool dlgMain::_processWampInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    return bRet;
}
//=======================================================================================
//
//  S Q L  S E T T I N G S  I N I T I A L I Z A T I O N
//
//=======================================================================================
void dlgMain::_initializeSQLPage()
//=======================================================================================
{
    pbCancel->setEnabled( true );
    pbNext->setEnabled( true );

    if( m_pInstallType == rbInstall )
    {
        lblTitleInitSQL->setText( tr("Initialize SQL connection") );
        lblText4_1->setText( tr("Enter the root password :") );
        lblText4_3->setVisible( false );
        lblText4_4->setVisible( false );
        lblText4_5->setVisible( false );
        ledSQLRootPsw1->setVisible( false );
        ledSQLRootPsw1->setEnabled( false );
        ledSQLRootPsw2->setVisible( false );
        ledSQLRootPsw2->setEnabled( false );

        ledSQLRootPsw->setText( m_qsRootPassword );
    }
    else if( m_pInstallType == rbUpdate )
    {
        lblTitleInitSQL->setText( tr("Update SQL connection") );
        lblText4_1->setText( tr("Enter original root password :") );
        lblText4_3->setVisible( true );
        lblText4_4->setVisible( true );
        lblText4_5->setVisible( true );
        ledSQLRootPsw1->setVisible( true );
        ledSQLRootPsw1->setEnabled( true );
        ledSQLRootPsw2->setVisible( true );
        ledSQLRootPsw2->setEnabled( true );

        ledSQLRootPsw1->setText( m_qsRootPasswordNew );
        ledSQLRootPsw2->setText( m_qsRootPasswordNew );
    }
}
//=======================================================================================
void dlgMain::on_chkShowSQLPassword_clicked()
//=======================================================================================
{
    if( chkShowSQLPassword->isChecked() )
    {
        ledSQLRootPsw->setEchoMode( QLineEdit::Normal );
        ledSQLRootPsw1->setEchoMode( QLineEdit::Normal );
        ledSQLRootPsw2->setEchoMode( QLineEdit::Normal );
    }
    else
    {
        ledSQLRootPsw->setEchoMode( QLineEdit::Password );
        ledSQLRootPsw1->setEchoMode( QLineEdit::Password );
        ledSQLRootPsw2->setEchoMode( QLineEdit::Password );
    }
}
//=======================================================================================
bool dlgMain::_processSQLPage()
//=======================================================================================
{
    bool    bRet = true;

    m_qsRootPassword = ledSQLRootPsw->text();

    if( m_pInstallType == rbUpdate )
    {
        if( ledSQLRootPsw1->text().compare( ledSQLRootPsw2->text() ) )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("The new password is not the same in the two field.") );
            bRet = false;
        }
        else
            m_qsRootPasswordNew = ledSQLRootPsw1->text();
    }

    return bRet;
}
//=======================================================================================
//
//  H A R D W A R E  I N S T A L L
//
//=======================================================================================
void dlgMain::_initializeHardwareInstallPage()
//=======================================================================================
{
    pbCancel->setEnabled( true );
    pbNext->setEnabled( true );

    _logProcess( QString("Serial communication init") );
    m_poHardware = new CS_Communication_Serial();
    if( m_poHardware != NULL )
    {
        _fillAvailableComPorts();
    }
    else
    {
        QMessageBox::critical( this, tr("Error"), tr("System error occured during COM ports initialization.\n"
                                                     "Please restart application and/or the operating system.\n"
                                                     "If the error continuously occures again, please contact system administrator.") );
        _logProcess( QString("COM init failed") );
        pbNext->setEnabled( false );
    }
    lblText5_3->setVisible( false );
    imgFail5_1->setVisible( false );
    imgOk5_1->setVisible( false );
    lblText5_4->setVisible( false );
    ledPanelsAvailable->setVisible( false );
    lblText5_5->setVisible( false );
    lblText5_6->setVisible( false );
    ledPanelsInstalled->setVisible( false );
    ledPanelsInstalled->setEnabled( false );
    ledPanelsInstalled->setText( QString::number(m_nCountDevices) );
}
//=======================================================================================
bool dlgMain::_processHardwareInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    m_bDemoMode = false;
    m_nCountDevices = ledPanelsInstalled->text().toInt();

    if( cmbCOMPorts->currentIndex() == 0 )
    {
        QMessageBox::information( this, tr("Information"),
                                  tr("There is no COM port selected for hardware unit communication.\n"
                                     "The Belenus client will be installed in DEMO mode.\n") );
        m_nCountDevices = 3;
        m_bDemoMode = true;
    }
    else if( m_nCountDevices < 1 || m_nCountDevices > ledPanelsAvailable->text().toInt() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Invalid value in number of panels field.\n"
                                 "Please enter a valid number between 1 and %1").arg(ledPanelsAvailable->text()) );
        bRet = false;
    }

    if( m_poHardware != NULL )
    {
        delete m_poHardware;
        m_poHardware = NULL;
    }

    return bRet;
}
//=======================================================================================
//
// C L I E N T  I N S T A L L
//
//=======================================================================================
void dlgMain::_initializeClientInstallPage()
//=======================================================================================
{
    pbCancel->setEnabled( true );
    pbNext->setEnabled( true );

    if( m_pInstallType == rbInstall )
    {
        ledClientInstallDir->setText( m_qsClientInstallDir );
    }
    else
    {
        on_pbNext_clicked();
    }
}
//=======================================================================================
bool dlgMain::_processClientInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    if( m_pInstallType != rbRemove )
        m_qsLanguage = cmbLanguage->itemText( cmbLanguage->currentIndex() ).right(3).left(2);

    return bRet;
}
//=======================================================================================
//
//  M A I N  I N S T A L L  P R O C E S S
//
//=======================================================================================
void dlgMain::_initializeInstallProcessPage()
//=======================================================================================
{
    _logProcess( "Start main install process" );

    pbCancel->setEnabled( false );
    pbPrev->setEnabled( false );
    pbNext->setEnabled( false );

    if( m_pInstallType == rbRemove )
    {
        lblTitleProcessInstall->setText( tr("Uninstall process") );
        lblText8_1->setText( tr("Please wait while the uninstall process finish.") );
    }

    m_bInstallClient = true;
    m_nTimer = startTimer( 500 );
}
//=======================================================================================
void dlgMain::_processInstall()
//=======================================================================================
{
    bool    bProcessSucceeded = true;

    m_bInstallClient = false;
    killTimer( m_nTimer );

    prbDBInstallClient->setValue( 0 );
    prbDBInstallClient->setMaximum( _getProcessActionCount() );
    prbDBInstallClient->update();

    _logProcess( QString("Number of actions to be processed: %1").arg(prbDBInstallClient->maximum()) );
    m_qsProcessErrorMsg = "";

    m_qsIniFileName = QString( "%1\\belenus.ini" ).arg(m_qsClientInstallDir);

    if( m_pInstallType == rbInstall )
    {
        m_qslFiles.clear();

        if( bProcessSucceeded && m_bProcessDatabase )
        {
            bProcessSucceeded = _processDatabaseInstall();
            _logProcess( "Process database install ... ", false );
            _logProcess( bProcessSucceeded?"OK":"FAILED" );
            if( bProcessSucceeded )
                m_qslComponents.append( "Database" );
        }
        if( bProcessSucceeded && m_bProcessHWConnection )
        {
            bProcessSucceeded = _processHWSettings();
            _logProcess( "Process HW settings ... ", false );
            _logProcess( bProcessSucceeded?"OK":"FAILED" );
            if( bProcessSucceeded )
            {
                m_qslComponents.append( "Hardware" );
            }
            else
            {
                m_qsProcessErrorMsg = QString( "ProcessHWSettingsFailed" );
            }
        }
        if( bProcessSucceeded && m_bProcessBelenusClient )
        {
            bProcessSucceeded = _processClientInstall();
            _logProcess( "Process client install ... ", false );
            _logProcess( bProcessSucceeded?"OK":"FAILED" );
            if( bProcessSucceeded )
            {
                m_qslComponents.append( "Client" );
                m_qslComponents.append( "Viewer" );
            }
        }
        if( bProcessSucceeded )
        {
            bProcessSucceeded = _copyUninstallFiles();
        }
    }
    else if( m_pInstallType == rbUpdate )
    {
        if( bProcessSucceeded && m_bProcessDatabase && m_qsRootPasswordNew.length() )
        {
            bProcessSucceeded = _processRootModify();
        }
        if( bProcessSucceeded && m_bProcessDatabase )
        {
            bProcessSucceeded = _processDatabaseUpdate();
        }
        if( bProcessSucceeded && m_bProcessBelenusClient )
        {
            bProcessSucceeded = _copyInstallFiles( QString("%1/update.li").arg(g_qsCurrentPath), false );
        }
    }
    else if( m_pInstallType == rbRemove )
    {
        bProcessSucceeded = _removeInstalledFilesFolders();

        if( !_emptyTargetDirectory( m_qsClientInstallDir ) )
        {
            QMessageBox::information( this, tr("Attention"),
                                      tr("Unable to empty the specified directory.\n"
                                         "%1\n"
                                         "Some of the files or subdirectories can not be removed." ).arg(m_qsClientInstallDir) );
        }
    }

    if( bProcessSucceeded )
    {
        lblTextProcessInfo->setText( "" );
        m_bInstallFinished = true;
        m_nTimer = startTimer( 2000 );
    }
    else
    {
        QMessageBox::warning( this, "Attention",
                              tr("Error occured during installing Belenus Application System.\n"
                                 "Please contact Belenus software support.\n\n"
                                 "Error code: %1").arg(m_qsProcessErrorMsg) );
        pbCancel->setEnabled( true );
    }
}
//=======================================================================================
int dlgMain::_getProcessActionCount()
//=======================================================================================
{
    int nCount = 0;

    if( m_bProcessDatabase )
    {
        if( m_pInstallType == rbInstall )
        {
            // Increase number with
            // DB create, User create, User grant privs
            nCount += 3;

            // If not created increase number with root user creation
            if( !m_bRootUserExists )
            {
                nCount++;
            }

            // Increase number with table creates
            QFile fileCreate("sql/db_create.sql");

            if( fileCreate.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileCreate);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    if( line.contains( QChar(';') ))
                        nCount++;
                }
                fileCreate.close();
            }

            // Increase number with default data fill
            QFile fileFill("sql/db_fill.sql");

            if( fileFill.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileFill);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    if( line.contains( QChar(';') ))
                        nCount++;
                }
                fileFill.close();
            }
        }
        else if( m_pInstallType == rbUpdate )
        {
            // Increase number with table creates
            QFile fileCreate("sql/db_update.sql");

            if( fileCreate.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileCreate);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    if( line.contains( QChar(';') ))
                        nCount++;
                }
                fileCreate.close();
            }
        }
        else if( m_pInstallType == rbRemove )
        {
            nCount += 1;
        }
    }
    if( m_bProcessHWConnection )
    {
        nCount += 1;
    }
    if( m_bProcessBelenusClient )
    {
        if( m_pInstallType == rbInstall )
        {
            nCount += 3;
            QFile fileCreate( QString("%1/install.li").arg(g_qsCurrentPath) );

            if( fileCreate.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileCreate);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    nCount += line.count( QChar('#') );
                }
                fileCreate.close();
            }
        }
        else if( m_pInstallType == rbUpdate )
        {
            QFile fileUpdate( QString("%1/update.li").arg(g_qsCurrentPath) );

            if( fileUpdate.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileUpdate);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    nCount += line.count( QChar('#') );
                }
                fileUpdate.close();
            }
        }
        else if( m_pInstallType == rbRemove )
        {
            nCount += 3;
            QFile fileCreate( QString("%1/Temp/BelenusInstall/uninstall.li").arg(m_qsPathWindows) );

            if( fileCreate.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QTextStream in(&fileCreate);
                while( !in.atEnd() )
                {
                    QString line = in.readLine();

                    if( line.contains( QChar('#') ))
                        nCount++;
                }
                fileCreate.close();
            }
        }
    }

    if( m_pInstallType == rbInstall )
    {
        nCount++;
    }

    return nCount;
}
//=======================================================================================
bool dlgMain::_processDatabaseInstall()
//=======================================================================================
{
    if( !m_bRootUserExists )
    {
        _logProcess( QString("Creating root user ..."), false );
        lblTextProcessInfo->setText( tr("Creating root user ...") );
        if( _processRootCreate() )
        {
            _logProcess( QString(" OK") );
        }
        else
        {
            m_qsProcessErrorMsg = "CreateRootFailed";
            _logProcess( QString(" FAIL") );
            return false;
        }
    }

    _logProcess( QString("Creating database ..."), false );
    lblTextProcessInfo->setText( tr("Creating database ...") );
    if( _processDatabaseCreate() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "CreateDBFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    _logProcess( QString("Creating Belenus user ..."), false );
    lblTextProcessInfo->setText( tr("Creating Belenus user ...") );
    if( _processBelenusUserCreate() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "CreateUserFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    _logProcess( QString("Granting privileges for Belenus user ..."), false );
    lblTextProcessInfo->setText( tr("Granting privileges for Belenus user ...") );
    if( _processBelenusUserRights() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "GrantUserFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    m_bBelenusUserExists = true;

    _logProcess( QString("Creating tables in database ..."), false );
    lblTextProcessInfo->setText( tr("Creating tables in database ...") );
    if( _processBelenusTablesCreate() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "CreateTableFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    _logProcess( QString("Adding default data to tables ..."), false );
    lblTextProcessInfo->setText( tr("Adding default data to tables ...") );
    if( _processBelenusTablesFill() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "FillTableFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    _logProcess( QString("Add %1 panel to database ...").arg(m_nCountDevices), false );
    if( _processBelenusDeviceFill() )
    {
        _logProcess( QString(" OK") );
    }
    else
    {
        m_qsProcessErrorMsg = "FillPanelFailed";
        _logProcess( QString(" FAIL") );
        return false;
    }

    _logProcess( QString("Database creation SUCCEEDED") );
    m_bDatabaseAlreadyInstalled = true;

    return true;
}
//=======================================================================================
bool dlgMain::_processDatabaseUpdate()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        QFile file("sql/db_update.sql");

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
            return false;

        QString qsSQLCommand = "";
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            if( line.left(2).compare("--") )
                qsSQLCommand.append( line );

            if( line.contains( QChar(';') ))
            {
                QSqlQuery sqlQuery;

                if( !sqlQuery.exec( qsSQLCommand ) )
                {
                    _logProcess( sqlQuery.lastError().text() );
                    bRet = false;
                }

                prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
                prbDBInstallClient->update();
                Sleep(50);
                qsSQLCommand = "";
            }
        }
        file.close();

        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processRootCreate()
//=======================================================================================
{
    bool bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( "" );

    if( m_poDB->open() )
    {
        m_poDB->exec( QString("SET PASSWORD FOR 'root'@'127.0.0.1' = PASSWORD( '%1' );").arg(m_qsRootPassword) );
        m_poDB->exec( QString("SET PASSWORD FOR 'root'@'localhost' = PASSWORD( '%1' );").arg(m_qsRootPassword) );
        m_poDB->close();
    }
    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( m_qsRootPassword );

    if( m_poDB->open() )
    {
        m_bRootUserExists = true;
        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processRootModify()
//=======================================================================================
{
    bool bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( m_qsRootPassword );

    if( m_poDB->open() )
    {
        m_poDB->exec( QString("SET PASSWORD FOR 'root'@'127.0.0.1' = PASSWORD( '%1' );").arg(m_qsRootPasswordNew) );
        m_poDB->exec( QString("SET PASSWORD FOR 'root'@'localhost' = PASSWORD( '%1' );").arg(m_qsRootPasswordNew) );
        m_poDB->close();

        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "mysql" );
        m_poDB->setUserName( "root" );
        m_poDB->setPassword( m_qsRootPasswordNew );

        if( m_poDB->open() )
        {
            m_bRootUserExists = true;
            m_poDB->close();
        }
        else
        {
            bRet = false;
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processDatabaseCreate()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( m_qsRootPassword );

    if( m_poDB->open() )
    {
        m_poDB->exec( "DROP DATABASE IF EXISTS `belenus`; CREATE DATABASE `belenus` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;" );
        m_poDB->close();

        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "belenus" );
        m_poDB->setUserName( "root" );
        m_poDB->setPassword( m_qsRootPassword );

        if( m_poDB->open() )
        {
            m_poDB->close();
            prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
            prbDBInstallClient->update();
            Sleep(50);
        }
        else
            bRet = false;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processBelenusUserCreate()
//=======================================================================================
{
    bool        bRet = true;
    QSqlQuery   poQuery;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( m_qsRootPassword );

    if( m_poDB->open() )
    {
        poQuery = m_poDB->exec( "SELECT Host FROM user WHERE User='belenus';" );
        if( !poQuery.first() )
            m_poDB->exec( "DROP USER 'belenus'@'localhost';" );

        m_poDB->exec( "CREATE USER 'belenus'@'localhost' IDENTIFIED BY 'belenus';" );

        poQuery = m_poDB->exec( "SELECT Host FROM user WHERE User='belenus';" );
        if( !poQuery.first() )
        {
            bRet = false;
        }
        else
        {
            prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
            prbDBInstallClient->update();
            Sleep(50);
        }
        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processBelenusUserRights()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( m_qsRootPassword );

    if( m_poDB->open() )
    {
        m_poDB->exec( "GRANT ALL PRIVILEGES ON `belenus` . * TO 'belenus'@'localhost' WITH GRANT OPTION;" );
        m_poDB->close();

        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "belenus" );
        m_poDB->setUserName( "belenus" );
        m_poDB->setPassword( "belenus" );

        if( m_poDB->open() )
        {
            prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
            prbDBInstallClient->update();
            Sleep(50);
            m_poDB->close();
        }
        else
        {
            bRet = false;
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processBelenusTablesCreate()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        QFile file( QString("%1/sql/db_create.sql").arg(g_qsCurrentPath) );

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            _logProcess( QString("OpenCreateSql %1").arg(QString("%1/sql/db_create.sql").arg(g_qsCurrentPath)) );
            return false;
        }

        QString qsSQLCommand = "";
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            if( line.left(2).compare("--") )
                qsSQLCommand.append( line );

            if( line.contains( QChar(';') ))
            {
                QSqlQuery sqlQuery;

                if( !sqlQuery.exec( qsSQLCommand ) )
                {
                    _logProcess( sqlQuery.lastError().text() );
                    bRet = false;
                }
                prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
                prbDBInstallClient->update();
                //Sleep(50);
                qsSQLCommand = "";
            }
        }
        file.close();

        m_poDB->close();
    }
    else
    {
        _logProcess( QString("LogInWithBelenusUser") );
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processBelenusTablesFill()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        QFile file( QString("%1/sql/db_fill.sql").arg(g_qsCurrentPath) );

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
            return false;

        QString qsSQLCommand = "";
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            if( line.left(2).compare("--") )
                qsSQLCommand.append( line );

            if( line.contains( QChar(';') ))
            {
                QSqlQuery sqlQuery;

                if( !sqlQuery.exec( qsSQLCommand ) )
                {
                    _logProcess( sqlQuery.lastError().text() );
                    bRet = false;
                }
                prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
                prbDBInstallClient->update();
                //Sleep(50);
                qsSQLCommand = "";
            }
        }
        file.close();

        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processBelenusDeviceFill()
//=======================================================================================
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        m_poDB->exec( QString("DELETE FROM `panels` WHERE `panelTypeId`>0") );

        for( int i=0; i<m_nCountDevices; i++ )
        {
            QString qsSQL = QString("INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES"
                                    "( 1, 1, \"%1 %2\", 0, 0, 1, \"ARC\" )").arg(tr("KiwiSun device")).arg(i);
            m_poDB->exec( qsSQL );
            prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
            prbDBInstallClient->update();
            Sleep(50);
        }

        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processClientInstall()
//=======================================================================================
{
    bool    bRet = false;
    QDir    qdInstallDir( m_qsClientInstallDir );

    _logProcess( QString("Start Client install") );
    if( qdInstallDir.exists() )
    {
        if( QMessageBox::warning( this, tr("Attention"),
                                  tr("The Belenus Application systems target directory already exits.\n"
                                     "%1\n"
                                     "All the files will be deleted or overwritten.\n\n"
                                     "Are you sure you want to continue?").arg(m_qsClientInstallDir),
                                  QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        {
            _logProcess( QString("Belenus target directory exists. User cancelled client install.") );
            pbCancel->setEnabled( true );
            return true;
        }
        if( !_emptyTargetDirectory( m_qsClientInstallDir ) )
        {
            _logProcess( QString("Belenus target directory exists. Empty directory failed.") );
            QMessageBox::information( this, tr("Attention"),
                                      tr("Unable to empty the specified directory.\n"
                                         "%1\n"
                                         "Please manually delete the directory if copying new files fails.") );
        }
    }

    _logProcess( QString("Creating directories (target, lang, resource) ..."), false );
    if( !_createTargetDirectory( m_qsClientInstallDir ) ||
        !_createTargetDirectory( QString("%1\\lang").arg(m_qsClientInstallDir) ) ||
        !_createTargetDirectory( QString("%1\\resources").arg(m_qsClientInstallDir) ) ||
        !_createTargetDirectory( QString("%1\\tools").arg(m_qsClientInstallDir) ))
    {
        _logProcess( QString(" FAIL") );
        m_qsProcessErrorMsg = QString( "CreateClientDirFailed" );
        return false;
    }
    _logProcess( QString("OK") );

    _logProcess( QString("Copying files from install.li ..."), false );
    if( (bRet = _copyInstallFiles( QString("%1/install.li").arg(g_qsCurrentPath) )) )
        _logProcess( QString("OK") );

    if( bRet )
    {
        _logProcess( QString("Creating folders, shortcuts ..."), false );
        if( (bRet = _createFolderShortcut()) )
            _logProcess( QString("OK") );
        else
            _logProcess( QString("FAIL") );
    }

    QSettings  obPrefFile( m_qsIniFileName, QSettings::IniFormat );
    obPrefFile.setValue( QString::fromAscii( "Lang" ), m_qsLanguage );

    _logProcess( QString("Client install successfully finished") );

    return bRet;
}
//=======================================================================================
bool dlgMain::_processHWSettings()
//=======================================================================================
{
    _logProcess( QString("Process HW settings ..."), false );
    lblTextProcessInfo->setText( tr("Processing hardware settings ...") );
    QSettings  obPrefFile( m_qsIniFileName, QSettings::IniFormat );
    obPrefFile.setValue( QString::fromAscii( "Hardware/ComPort" ), m_nComPort );
    prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
    prbDBInstallClient->update();
    Sleep(50);

    return ( obPrefFile.value(QString::fromAscii("Hardware/ComPort" ),"-1").toInt()==m_nComPort ? true : false );
}
//=======================================================================================
bool dlgMain::_copyUninstallFiles()
//=======================================================================================
{
    QSettings   obReg( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus"), QSettings::NativeFormat );

    if( obReg.contains( QString("Components") ) )
    {
        obReg.setValue( QString("Components"), m_qslComponents.join("#") );
    }

    QString     qsFrom  = QString( "%1/Setup.exe" ).arg(g_qsCurrentPath);
    QString     qsTo    = QString( "%1/Temp/BelenusInstall/Setup.exe" ).arg(m_qsPathWindows);

    if( QFile::exists( qsTo ) )
    {
        QFile::remove( qsTo );
    }
    if( !QFile::copy( qsFrom, qsTo ) )
    {
        m_qsProcessErrorMsg = QString( "CopySetupExeFailed" );
        return false;
    }

    qsFrom  = QString( "%1/setup_hu.qm" ).arg(g_qsCurrentPath);
    qsTo    = QString( "%1/Temp/BelenusInstall/setup_hu.qm" ).arg(m_qsPathWindows);

    if( QFile::exists( qsTo ) )
    {
        QFile::remove( qsTo );
    }
    if( !QFile::copy( qsFrom, qsTo ) )
    {
        m_qsProcessErrorMsg = QString( "CopySetupQmFailed" );
        return false;
    }

    qsFrom  = QString( "%1/qt_hu.qm" ).arg(g_qsCurrentPath);
    qsTo    = QString( "%1/Temp/BelenusInstall/qt_hu.qm" ).arg(m_qsPathWindows);

    if( QFile::exists( qsTo ) )
    {
        QFile::remove( qsTo );
    }
    if( !QFile::copy( qsFrom, qsTo ) )
    {
        m_qsProcessErrorMsg = QString( "CopyQtQmFailed" );
        return false;
    }

    QFile   *obUnistall = new QFile( QString("%1/Temp/BelenusInstall/uninstall.li").arg(m_qsPathWindows) );
    if( obUnistall->open( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text ) )
    {
        QTextStream out( obUnistall );
        out << m_qslFiles.join("#");
        obUnistall->close();
    }

    return true;
}
//=======================================================================================
//
//  F I N I S H  I N S T A L L
//
//=======================================================================================
void dlgMain::_initializeFinishPage()
//=======================================================================================
{
    QString qsProcess;

    if( m_pInstallType == rbInstall )
    {
        lblTitleInstallFinished->setText( tr( "Installation finished" ) );
        qsProcess = tr("Installing");
    }
    else if( m_pInstallType == rbUpdate )
    {
        lblTitleInstallFinished->setText( tr( "Update finished" ) );
        qsProcess = tr("Updating");
    }
    else if( m_pInstallType == rbRemove )
    {
        lblTitleInstallFinished->setText( tr( "Uninstall finished" ) );
        qsProcess = tr("Removing");
    }

    _logProcess( lblTitleInstallFinished->text() );

    if( m_bRestartRequired )
    {
        lblTextInstallFinished->setText( tr( "%1 Belenus Application System has been finished."
                                             "To use the system correcty you need to restart your "
                                             "computer after exiting the installer.\n\n"
                                             "Press Exit to close the installer." ).arg(qsProcess) );
        pbExitRestart->setEnabled( true );
        pbExitRestart->setVisible( true );
    }
    else
    {
        lblTextInstallFinished->setText( tr( "%1 Belenus Application System has been finished.\n\n"
                                             "Press Exit to close the installer." ).arg(qsProcess) );
        pbExitRestart->setEnabled( false );
        pbExitRestart->setVisible( false );
    }
}
//=======================================================================================
void dlgMain::on_cmbCOMPorts_currentIndexChanged(int index)
//=======================================================================================
{
    if( cmbCOMPorts->currentIndex() )
    {
        m_nComPort = cmbCOMPorts->currentText().right(cmbCOMPorts->currentText().length()-3).toInt();
        pbTestHWConnection->setEnabled( true );
        lblText5_3->setVisible( true );
        lblText5_4->setVisible( true );
        ledPanelsAvailable->setVisible( true );
    }
    else
    {
        m_nComPort = 0;
        pbTestHWConnection->setEnabled( false );
        lblText5_3->setVisible( false );
        lblText5_4->setVisible( false );
        ledPanelsAvailable->setVisible( false );
    }
    ledPanelsAvailable->setText( "" );
    imgOk5_1->setVisible( false );
    imgFail5_1->setVisible( false );
    lblText5_5->setVisible( false );
    lblText5_6->setVisible( false );
    ledPanelsInstalled->setVisible( false );
    ledPanelsInstalled->setEnabled( false );
    ledPanelsInstalled->setText( "" );
}
//=======================================================================================
void dlgMain::on_pbTestHWConnection_clicked()
//=======================================================================================
{
    m_poHardware->init( m_nComPort );

    if( m_poHardware->isHardwareConnected() )
    {
        imgOk5_1->setVisible( true );
        imgFail5_1->setVisible( false );
        ledPanelsAvailable->setText( QString::number(m_poHardware->getHardwareModuleCount()) );
        lblText5_5->setVisible( true );
        lblText5_6->setVisible( true );
        ledPanelsInstalled->setVisible( true );
        ledPanelsInstalled->setEnabled( true );
    }
    else
    {
        imgOk5_1->setVisible( false );
        imgFail5_1->setVisible( true );
        ledPanelsAvailable->setText( "N/A" );
        lblText5_5->setVisible( false );
        lblText5_6->setVisible( false );
        ledPanelsInstalled->setVisible( false );
        ledPanelsInstalled->setEnabled( false );
    }
    ledPanelsInstalled->setText( "" );
    m_poHardware->closeCommunication();
}
//=======================================================================================
void dlgMain::on_pbSelectDir_clicked()
//=======================================================================================
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       m_qsClientInstallDir,
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( qsDir.length() > 0 )
    {
        qsDir.replace( '/', '\\' );
        if( qsDir.right(7).compare("Belenus") )
        {
            m_qsClientInstallDir = QString("%1\\%2").arg(qsDir).arg(QString("Belenus"));
        }
        else
        {
            m_qsClientInstallDir = qsDir;
        }
    }
    ledClientInstallDir->setText( m_qsClientInstallDir );
}
//=======================================================================================
void dlgMain::on_pbExitRestart_clicked()
//=======================================================================================
{
    _exitInstaller( true );
}
//=======================================================================================
void dlgMain::_exitInstaller( bool m_bRestartPC )
//=======================================================================================
{
    QDialog::accept();

    if( m_bRestartPC )
        ExitWindowsEx( 0x00000040/*EWX_RESTARTAPPS*/, 0x00000002/*SHTDN_REASON_MINOR_INSTALLATION*/ );
}
//=======================================================================================
void dlgMain::_refreshPages()
//=======================================================================================
{
    m_vPages.clear();

    m_vPages.append( CONST_PAGE_WELCOME );
    m_vPages.append( CONST_PAGE_INSTALL_SELECTION );

    if( m_pInstallType == rbInstall )
    {
        m_vPages.append( CONST_PAGE_COMPONENT_SELECTION );
        if( m_bProcessWamp)
            m_vPages.append( CONST_PAGE_WAMP_INSTALL );
        if( m_bProcessDatabase )
            m_vPages.append( CONST_PAGE_INIT_SQL );
        if( m_bProcessHWConnection )
            m_vPages.append( CONST_PAGE_HARDWARE_INSTALL );
        if( m_bProcessBelenusClient )
            m_vPages.append( CONST_PAGE_CLIENT_INSTALL );
    }
    else if( m_pInstallType == rbUpdate )
    {
        m_vPages.append( CONST_PAGE_COMPONENT_SELECTION );
        if( m_bProcessDatabase )
            m_vPages.append( CONST_PAGE_INIT_SQL );
        if( m_bProcessHWConnection )
            m_vPages.append( CONST_PAGE_HARDWARE_INSTALL );
        if( m_bProcessBelenusClient )
            m_vPages.append( CONST_PAGE_CLIENT_INSTALL );
    }

    m_vPages.append( CONST_PAGE_PROCESS );
    m_vPages.append( CONST_PAGE_FINISH );
}
//=======================================================================================
bool dlgMain::_isRegStringMatch( QString p_qsPath, QString p_qsKey, QString p_qsValue )
//=======================================================================================
{
    bool        bRet = false;
/*    VRegistry   obReg;

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, p_qsPath ) )
    {
        if( p_qsValue.compare( obReg.get_REG_SZ( p_qsKey ) ) == 0 )
            bRet = true;

        obReg.CloseKey();
    }
*/
    return bRet;
}
//=======================================================================================
void dlgMain::_setEnableNextButton()
//=======================================================================================
{
    if( !chkWamp->isChecked() && !chkDatabase->isChecked() && !chkHardware->isChecked() && !chkBelenus->isChecked() )
        pbNext->setEnabled( false );
    else
        pbNext->setEnabled( true );
}
//=======================================================================================
void dlgMain::_fillAvailableComPorts()
//=======================================================================================
{
    cmbCOMPorts->clear();

    cmbCOMPorts->addItem( tr("<Not selected>") );
    for( int i=0; i<m_poHardware->getCountAvailablePorts(); i++ )
    {
        QString qsCOM = QString( "COM%1" ).arg( m_poHardware->getComPort(i) );

        cmbCOMPorts->addItem( qsCOM );
    }
    _logProcess( QString("Number of available COM ports: %1").arg(m_poHardware->getCountAvailablePorts()) );
}
//=======================================================================================
bool dlgMain::_emptyTargetDirectory( QString p_qsPath )
//=======================================================================================
{
    bool            bRet = true;
    QDir            qdTarget( p_qsPath );

    if( qdTarget.exists() )
    {
        QStringList qstFiles = qdTarget.entryList();

        for( int nCount=0; nCount<qstFiles.size(); nCount++ )
        {
            if( qstFiles.at(nCount).compare(".") == 0 || qstFiles.at(nCount).compare("..") == 0 )
                continue;

            if( qstFiles.at(nCount).indexOf('.') == -1 )
            {
                if( !_emptyTargetDirectory( QString("%1\\%2").arg(p_qsPath).arg(qstFiles.at(nCount)) ) )
                {
                    bRet = false;
                }
                else
                {
                    qdTarget.rmpath( QString("%1\\%2").arg(p_qsPath).arg(qstFiles.at(nCount)) );
                }
            }
            else
            {
                qdTarget.remove( QString("%1\\%2").arg(p_qsPath).arg(qstFiles.at(nCount)) );
            }
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_createTargetDirectory( QString p_qsPath )
//=======================================================================================
{
    QDir    qdInstallDir( p_qsPath );

    if( !qdInstallDir.mkpath( p_qsPath ) )
    {
        QMessageBox::critical( this, tr("System error"),
                               tr("Unable to create directory:\n\n%1").arg(p_qsPath));
        pbCancel->setEnabled( true );
        return false;
    }

    return true;
}
//=======================================================================================
bool dlgMain::_copyClientFile( QString p_qsFileName, bool p_bInstall )
//=======================================================================================
{
    QString     qsFrom  = QString( "%1/%2" ).arg(g_qsCurrentPath).arg(p_qsFileName);
    QString     qsTo    = QString( "%1/%2" ).arg(m_qsClientInstallDir).arg(p_qsFileName);

    qsFrom.replace( '\\', '/' );
    qsTo.replace( '\\', '/' );

    if( !p_bInstall )
    {
        QFile::remove( qsTo );
    }

    return QFile::copy( qsFrom, qsTo );
}
//=======================================================================================
bool dlgMain::_copyInstallFiles( QString p_qsFileName, bool p_bInstall )
//=======================================================================================
{
    bool    bRet = true;
    QFile   file( p_qsFileName );

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        pbCancel->setEnabled( true );
        m_qsProcessErrorMsg = QString( "OpenListFileFailed" );
        return false;
    }

    QString qsTemp = "";
    QTextStream in(&file);

    while( !in.atEnd() )
    {
        qsTemp.append( in.readLine() );
    }
    file.close();

    QStringList qslFiles = qsTemp.split( '#' );

    for( int i=0; i<qslFiles.size(); i++ )
    {
        prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
        prbDBInstallClient->update();
        if( p_bInstall )
        {
            lblTextProcessInfo->setText( tr("Copying file: ../%1").arg(qslFiles.at(i)) );
        }
        else
        {
            lblTextProcessInfo->setText( tr("Updating file: ../%1").arg(qslFiles.at(i)) );
        }
        if( !_copyClientFile( qslFiles.at(i), p_bInstall ) )
        {
            if( p_bInstall )
                QMessageBox::critical( this, tr("System error"),
                                       tr("Unable to copy file:\n\n%1").arg(qslFiles.at(i)));
            else
                QMessageBox::critical( this, tr("System error"),
                                       tr("Unable to update file:\n\n%1").arg(qslFiles.at(i)));
            m_qsProcessErrorMsg = QString( "CopyFileFailed" );
            _logProcess( " FAIL" );
            _logProcess( QString("%1").arg(qslFiles.at(i)) );
            bRet = false;
            break;
        }
        if( p_qsFileName.right(10).compare("install.li") == 0 )
        {
            m_qslFiles.append( QString( "%1%2" ).arg(m_qsClientInstallDir).arg(qslFiles.at(i)) );
        }
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_createFolderShortcut()
//=======================================================================================
{
    bool        bRet = true;

    if( m_qsPathPrograms.contains( ":\\" ) )
    {
        if( !_createTargetDirectory( QString("%1\\Belenus").arg(m_qsPathPrograms) ) )
        {
            m_qsProcessErrorMsg = QString( "CreateClientFolderFailed" );
            return false;
        }

        m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Belenus\\belenus.lnk").arg(m_qsPathPrograms) );
        m_obFile->link( QString("%1\\Belenus\\belenus.lnk").arg(m_qsPathPrograms) );
        delete m_obFile;
        m_qslFiles.append( QString("%1\\Belenus\\belenus.lnk").arg(m_qsPathPrograms) );
        m_qslFiles.append( QString("%1\\Belenus\\").arg(m_qsPathPrograms) );
    }

    if( m_qsPathDesktop.contains( ":\\" ) )
    {
        m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\belenus.lnk").arg(m_qsPathDesktop) );
        m_obFile->link( QString("%1\\belenus.lnk").arg(m_qsPathDesktop) );
        m_qslFiles.append( QString("%1\\belenus.lnk").arg(m_qsPathDesktop) );
        delete m_obFile;
    }

    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("Language"), m_qsLanguage );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("Components"), m_qslComponents.join("#") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("DisplayIcon"), QString("%1\\resources\\belenus.ico").arg(m_qsClientInstallDir) );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("DisplayName"), tr("Belenus Application System") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("DisplayVersion"), QString("1.0.0.0") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("InstallLocation"), m_qsClientInstallDir );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("Publisher"), QString("Pagony Multimédia Stúdió Bt.") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("UninstallString"), QString("%1\\Temp\\BelenusInstall\\setup.exe -uninstall").arg(m_qsPathWindows) );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("URLInfoAbout"), QString("http://belenus.pagonymedia.hu") );

    prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
    prbDBInstallClient->update();

    return bRet;
}
//=======================================================================================
bool dlgMain::_removeInstalledFilesFolders()
//=======================================================================================
{
    bool    bRet = true;
    QFile   file( QString("%1/Temp/BelenusInstall/uninstall.li").arg(m_qsPathWindows) );

    if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        pbCancel->setEnabled( true );
        m_qsProcessErrorMsg = QString( "OpenListFileFailed" );
        return false;
    }

    QString qsTemp = "";
    QTextStream in(&file);

    while( !in.atEnd() )
    {
        qsTemp.append( in.readLine() );
    }
    file.close();

    QStringList qslFiles = qsTemp.split( '#' );

    QDir    qdTarget( m_qsClientInstallDir );

    if( qdTarget.exists() )
    {
        for( int i=0; i<qslFiles.size(); i++ )
        {
            if( QString(qslFiles.at(i)).right(1).compare("\\") == 0 )
            {
                // Remove directory
                qdTarget.rmpath( qslFiles.at(i) );
            }
            else
            {
                // Remove file
                qdTarget.remove( qslFiles.at(i) );
            }
        }

        g_obReg.deleteRegPath( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus") );

        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "mysql" );
        m_poDB->setUserName( "root" );
        m_poDB->setPassword( m_qsRootPassword );

        if( m_poDB->open() )
        {
            m_poDB->exec( "DROP USER 'belenus'@'localhost';" );
            m_poDB->exec( "DROP DATABASE IF EXISTS `belenus`;" );

            m_poDB->close();
        }

        if( m_qsUninstallWampExec.length() > 0 )
        {
            m_qsUninstallWampExec.remove( "\"" );

            hide();
            QProcess *qpWamp = new QProcess();
            qpWamp->execute( m_qsUninstallWampExec );
            delete qpWamp;
            show();
            QApplication::processEvents();
        }

        _emptyTargetDirectory( QString("%1\\Temp\\BelenusInstall\\").arg(m_qsPathWindows) );
    }
    else
    {
        pbCancel->setEnabled( true );
        m_qsProcessErrorMsg = QString( "ClientAppDirMissing" );
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
void dlgMain::_logProcess( QString p_qsLog, bool p_bInsertNewLine )
//=======================================================================================
{
    if( m_obLog->open( QIODevice::Append | QIODevice::Text ) )
    {
        QTextStream out( m_obLog );
        out << p_qsLog;
        if( p_bInsertNewLine )
            out << "\n";
        m_obLog->close();
    }
}
//=======================================================================================

