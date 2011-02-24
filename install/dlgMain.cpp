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

#include "vregistry.h"

//=======================================================================================

#include "dlgMain.h"
#include "ui_dlgMain.h"
#include "../client/communication_serial.h"

//=======================================================================================

using namespace voidrealms::win32;

//=======================================================================================
dlgMain::dlgMain(QWidget *parent) : QDialog(parent)
//=======================================================================================
{
    setupUi(this);

    pagerControl->setCurrentIndex( 0 );

    Logo->setPixmap( QPixmap( QString(":/images/Logo.png") ) );
    setWindowIcon( QIcon( QString(":/icons/belenus.ico") ) );

    // Set when passed component selection page to block welcome page
    m_bInstallStarted       = false;

    // Flag for demo mode
    m_bDemoMode             = false;

    // Identifies which component has to be processed
    m_bProcessWamp          = true;
    m_bProcessDatabase      = true;
    m_bProcessHWConnection  = true;
    m_bProcessInternet      = true;
    m_bProcessBelenusClient = true;

    // Flags for timer
    m_bStartWampInstall     = false;
    m_bInitializeWamp       = false;
    m_bCreateDatabase       = false;
    m_bInstallClient        = false;
    m_bInstallFinished      = false;

    // If database created during install, set this flag
    m_bDatabaseInstalled    = false;

    // Default settings for hardware
    m_poHardware            = NULL;
    m_nComPort              = 0;
    m_nCountDevices         = 0;

    // Default settings for internet connection
    m_qsIPAddress           = QString( "127.0.0.1" );

    // Default settings for client
    m_qsClientInstallDir    = QString( "C:\\Program Files\\Belenus" );

    // If computer restart required, set this flag
    m_bRestartRequired      = false;

    // Get common settings from registry, set flags for install/update process
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
    m_nCurrentPage          = CONST_PAGE_WELCOME;
}
//=======================================================================================
dlgMain::~dlgMain()
//=======================================================================================
{
    if( m_poDB != NULL )        delete m_poDB;
    if( m_poHardware != NULL )  delete m_poHardware;
}
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
void dlgMain::on_pbPrev_clicked()
//=======================================================================================
{
    //_processPage( m_vPages.at( m_nCurrentPage ) );

    if( m_nCurrentPage > (m_bInstallStarted?CONST_PAGE_COMPONENT_SELECTION:CONST_PAGE_WELCOME) )
        m_nCurrentPage--;

    pagerControl->setCurrentIndex( m_vPages.at( m_nCurrentPage ) );

    if( m_nCurrentPage == (m_bInstallStarted?CONST_PAGE_COMPONENT_SELECTION:CONST_PAGE_WELCOME) )
        pbPrev->setEnabled( false );
    if( m_nCurrentPage < m_vPages.size()-1 )
        pbNext->setEnabled( true );

    pbCancel->setEnabled( true );
    pbStartExit->setEnabled( false );
    pbStartExit->setText( tr("Start") );

    //_initializePage( m_vPages.at( m_nCurrentPage ) );
}
//=======================================================================================
void dlgMain::on_pbNext_clicked()
//=======================================================================================
{
    if ( !_processPage( m_vPages.at( m_nCurrentPage ) ) )
        return;

    if( m_nCurrentPage < m_vPages.size()-1 )
        m_nCurrentPage++;

    if( m_nCurrentPage > CONST_PAGE_COMPONENT_SELECTION )
        m_bInstallStarted = true;

    pagerControl->setCurrentIndex( m_vPages.at( m_nCurrentPage ) );

    if( m_nCurrentPage > 0 )
        pbPrev->setEnabled( true );
    if( m_nCurrentPage == m_vPages.size()-1 )
    {
        pbCancel->setEnabled( false );
        pbPrev->setEnabled( false );
        pbNext->setEnabled( false );
        pbStartExit->setEnabled( true );
        pbStartExit->setText( tr("Exit") );
    }

    _initializePage( m_vPages.at( m_nCurrentPage ) );
}
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
void dlgMain::_initializeInstall()
//=======================================================================================
{
    VRegistry   obReg;

    m_qsPathWindows     = "";
    m_qsPathPrograms    = "";
    m_qsPathDesktop     = "";
    m_qsPathWampServer  = "";

    m_bBelenusAlreadyInstalled = _isRegPathExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" );

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion") ) )
    {
        m_qsPathWindows = obReg.get_REG_SZ( "SystemRoot" );
        obReg.CloseKey();
    }

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders") ) )
    {
        m_qsPathPrograms    = obReg.get_REG_SZ( QString("Common Programs") );
        m_qsPathDesktop     = obReg.get_REG_SZ( QString("Common Desktop") );
        obReg.CloseKey();
    }

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1") ) )
    {
        m_qsPathWampServer = obReg.get_REG_SZ( "Inno Setup: App Path" );
        obReg.CloseKey();
    }

    m_poDB = NULL;
    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    if( m_qsPathWindows.length() == 0 || m_qsPathPrograms.length() == 0 || m_qsPathDesktop.length() == 0 )
    {
        pbNext->setEnabled( false );
        QMessageBox::critical( this, tr("Error"),
                               tr("Error occured during initialization.\n"
                                  "Please contact system administrator.\n"
                                  "Error code: ErrSysRegKeysFail") );
    }
}
//=======================================================================================
void dlgMain::_initializePage( int p_nPage )
//=======================================================================================
{
    switch( p_nPage )
    {
        case CONST_PAGE_INSTALL_SELECTION:
            _initializeSelectionPage();
            break;

        case CONST_PAGE_COMPONENT_SELECTION:
            _initializeComponentSelectionPage();
            break;

        case CONST_PAGE_WAMP_INSTALL:
            _initializeWampInstallPage();
            break;

        case CONST_PAGE_DATABASE_INSTALL:
            _initializeDatabaseInstallPage();
            break;

        case CONST_PAGE_HARDWARE_INSTALL:
            _initializeHardwareInstallPage();
            break;

        case CONST_PAGE_INTERNET_INSTALL:
            _initializeInternetInstallPage();
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
void dlgMain::_initializeSelectionPage()
//=======================================================================================
{
    if( m_bBelenusAlreadyInstalled )
    {
        rbInstall->setEnabled( false );
        lblTextInstall->setEnabled( false );
        rbUpdate->setEnabled( true );
        lblTextUpdate->setEnabled( true );
        rbRemove->setEnabled( true );
        lblTextRemove->setEnabled( true );
    }
    else
    {
        rbInstall->setEnabled( true );
        lblTextInstall->setEnabled( true );
        rbUpdate->setEnabled( false );
        lblTextUpdate->setEnabled( false );
        rbRemove->setEnabled( false );
        lblTextRemove->setEnabled( false );

        rbInstall->setChecked( true );
    }
    m_pInstallType->setChecked( true );
}
//=======================================================================================
void dlgMain::_initializeComponentSelectionPage()
//=======================================================================================
{
    if( m_pInstallType == rbInstall )
    {
        chkWamp->setEnabled( true );
        chkDatabase->setEnabled( true );
        chkHardware->setEnabled( true );
        chkInternet->setEnabled( true );
        chkBelenus->setEnabled( true );
        if( m_bDatabaseInstalled )
        {
            m_bProcessDatabase = false;
            chkDatabase->setEnabled( false );
        }
    }
    else if( m_pInstallType == rbUpdate )
    {
        chkWamp->setEnabled( false );
        lblTextWamp->setEnabled( false );
    }

    chkWamp->setChecked( m_bProcessWamp );
    chkDatabase->setChecked( m_bProcessDatabase );
    chkHardware->setChecked( m_bProcessHWConnection );
    chkInternet->setChecked( m_bProcessInternet );
    chkBelenus->setChecked( m_bProcessBelenusClient );

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::_initializeWampInstallPage()
//=======================================================================================
{
    pbNext->setEnabled( false );
    lblText3_1->setVisible( true );
    lblText3_2->setVisible( false );
    lblText3_3->setVisible( false );
    ledDBRootPassword->setVisible( false );
    ledDBRootPassword->setEnabled( false );
    ledDBRootPassword->setText( "" );
    pbCheckRootPsw->setVisible( false );
    pbCheckRootPsw->setEnabled( false );
    lblText3_4->setVisible( false );
    imgOk3->setVisible( false );
    imgFail3->setVisible( false );

    m_bStartWampInstall = true;
    m_nTimer = startTimer( 200 );
}
//=======================================================================================
void dlgMain::_initializeDatabaseInstallPage()
//=======================================================================================
{
    if( m_bDatabaseInstalled )
    {
        on_pbNext_clicked();
    }
    else
    {
        imgOk4_1->setVisible( false );
        imgOk4_2->setVisible( false );
        imgOk4_3->setVisible( false );
        imgOk4_4->setVisible( false );
        imgOk4_5->setVisible( false );
        imgFail4_1->setVisible( false );
        imgFail4_2->setVisible( false );
        imgFail4_3->setVisible( false );
        imgFail4_4->setVisible( false );
        imgFail4_5->setVisible( false );

        m_bCreateDatabase = true;
        m_nTimer = startTimer( 200 );
    }
}
//=======================================================================================
void dlgMain::_initializeHardwareInstallPage()
//=======================================================================================
{
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
void dlgMain::_initializeInternetInstallPage()
//=======================================================================================
{
    ledDBIPAddress->setText( m_qsIPAddress );
    ledDBIPPort->setText( QString::number(m_nPort ) );
}
//=======================================================================================
void dlgMain::_initializeClientInstallPage()
//=======================================================================================
{
    ledClientInstallDir->setText( m_qsClientInstallDir );
    prbDBInstallClient->setValue( 0 );
    prbDBInstallClient->update();
}
//=======================================================================================
void dlgMain::_initializeInstallProcessPage()
//=======================================================================================
{
    pbNext->setEnabled( false );
    m_bInstallClient = true;
    m_nTimer = startTimer( 500 );
}
//=======================================================================================
void dlgMain::_initializeFinishPage()
//=======================================================================================
{
    QString qsProcess;

    if( m_pInstallType == rbInstall )
    {
        lblTitleInstallFinished->setText( tr( "Installation finished" ) );
        qsProcess = "Installing";
    }
    else if( m_pInstallType == rbUpdate )
    {
        lblTitleInstallFinished->setText( tr( "Update finished" ) );
        qsProcess = "Updating";
    }
    else if( m_pInstallType == rbRemove )
    {
        lblTitleInstallFinished->setText( tr( "Uninstall finished" ) );
        qsProcess = "Removing";
    }

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
void dlgMain::timerEvent(QTimerEvent *)
//=======================================================================================
{
    if( m_bStartWampInstall )
    {
        pbNext->setEnabled( false );
        m_bStartWampInstall = false;
        killTimer( m_nTimer );
        if( _processWampServerInstall() )
        {
            m_bInitializeWamp = true;
            m_nTimer = startTimer( 200 );
        }
        else
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("Wamp server installation failed.\n"
                                     "Please try to reinstall it with going back one page "
                                     "then return to this page.\n\n"
                                     "If Wamp install continuously fails please contact Belenus software support.") );
        }
    }
    else if( m_bInitializeWamp )
    {
        pbNext->setEnabled( false );
        m_bInitializeWamp = false;
        killTimer( m_nTimer );
        if( _initializeWampServer() )
        {
            lblText3_1->setVisible( false );
            lblText3_2->setVisible( true );
            lblText3_3->setVisible( true );
            ledDBRootPassword->setVisible( true );
            ledDBRootPassword->setEnabled( true );
            ledDBRootPassword->setFocus();
            pbCheckRootPsw->setVisible( true );
            pbCheckRootPsw->setEnabled( true );
        }
        else
        {
            pbNext->setEnabled( false );
        }
    }
    else if( m_bCreateDatabase )
    {
        m_bCreateDatabase = false;
        killTimer( m_nTimer );

        if( _processDatabaseCreate() )
        {
            imgOk4_1->setVisible( true );
        }
        else
        {
            imgFail4_1->setVisible( true );
            return;
        }

        if( _processBelenusUserCreate() )
        {
            imgOk4_2->setVisible( true );
        }
        else
        {
            imgFail4_2->setVisible( true );
            return;
        }

        if( _processBelenusUserRights() )
        {
            imgOk4_3->setVisible( true );
        }
        else
        {
            imgFail4_3->setVisible( true );
            return;
        }

        if( _processBelenusTablesCreate() )
        {
            imgOk4_4->setVisible( true );
        }
        else
        {
            imgFail4_4->setVisible( true );
            return;
        }

        if( _processBelenusTablesFill() )
        {
            imgOk4_5->setVisible( true );
        }
        else
        {
            imgFail4_5->setVisible( true );
            return;
        }
        m_bDatabaseInstalled = true;
    }
    else if( m_bInstallClient )
    {
        m_bInstallClient = false;
        killTimer( m_nTimer );
        _processClientInstall();
    }
    else if( m_bInstallFinished )
    {
        m_bInstallFinished = false;
        killTimer( m_nTimer );
        on_pbNext_clicked();
    }
}
//=======================================================================================
bool dlgMain::_processPage( int p_nPage )
//=======================================================================================
{
    bool bRet = true;

    switch( p_nPage )
    {
        case 1:
            bRet = _processSelectionPage();
            break;

        case 2:
            bRet = _processComponentSelectionPage();
            break;

        case 3:
            bRet = _processWampInstallPage();
            break;

        case 4:
            bRet = _processDatabaseInstallPage();
            break;

        case 5:
            bRet = _processHardwareInstallPage();
            break;

        case 6:
            bRet = _processInternetInstallPage();
            break;

        case 7:
            bRet = _processClientInstallPage();
            break;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processSelectionPage()
//=======================================================================================
{
    if( m_pInstallType == rbInstall )
    {
        m_bProcessWamp          = true;
        m_bProcessDatabase      = true;
        m_bProcessHWConnection  = true;
        m_bProcessInternet      = true;
        m_bProcessBelenusClient = true;
    }
    else if( m_pInstallType == rbUpdate )
    {
        m_bProcessWamp          = false;
        m_bProcessDatabase      = false;
        m_bProcessHWConnection  = false;
        m_bProcessInternet      = false;
        m_bProcessBelenusClient = false;
    }
    else if( m_pInstallType == rbRemove )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Are you sure you want to remove Belenus Application system and all of it's components?"),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        {
            return false;
        }
        m_bProcessWamp          = true;
        m_bProcessDatabase      = true;
        m_bProcessHWConnection  = true;
        m_bProcessInternet      = true;
        m_bProcessBelenusClient = true;
    }

    _refreshPages();

    return true;
}
//=======================================================================================
bool dlgMain::_processComponentSelectionPage()
//=======================================================================================
{
    m_bProcessWamp = chkWamp->isChecked();
    m_bProcessDatabase = chkDatabase->isChecked();
    m_bProcessHWConnection = chkHardware->isChecked();
    m_bProcessInternet = chkInternet->isChecked();
    m_bProcessBelenusClient = chkBelenus->isChecked();

    _refreshPages();

    return true;
}
//=======================================================================================
bool dlgMain::_processWampInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    return bRet;
}
//=======================================================================================
bool dlgMain::_processDatabaseInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    return bRet;
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
bool dlgMain::_processInternetInstallPage()
//=======================================================================================
{
    bool    bRet = true;

    if( m_bDemoMode )
    {
        ledDBIPAddress->setText( "127.0.0.1" );
        ledDBIPPort->setText( "1000" );
    }
    else
    {
        bool        ok           = false;
        int         nValue       = 0;
        QStringList qslIPAddress = ledDBIPAddress->text().split( '.' );

        if( qslIPAddress.size() != 4 )
        {
            bRet = false;
        }
        else
        {
            for( int i=0; i<qslIPAddress.size(); i++ )
            {
                nValue = qslIPAddress.at(i).toInt(&ok);
                if( !ok || qslIPAddress.at(i).length() < 1 || nValue < 0 || nValue > 999 )
                {
                    bRet = false;
                    break;
                }
            }
        }
        if( !bRet )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("IP Address of Database Server is invalid." ) );
        }
        else
        {
            m_qsIPAddress = ledDBIPAddress->text();

            nValue = ledDBIPPort->text().toInt(&ok);
            if( !ok || ledDBIPPort->text().length() < 1 || nValue < 1 )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("Connection port of Database Server is invalid." ) );
                bRet = false;
            }
            else
            {
                m_nPort = nValue;
            }
        }
    }

    if( bRet )
    {
        ledDBIPAddress->setText( m_qsIPAddress );
        ledDBIPPort->setText( QString::number(m_nPort) );
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processClientInstallPage()
//=======================================================================================
{
    bool    bRet = true;


    return bRet;
}
//=======================================================================================
bool dlgMain::_processWampServerInstall()
//=======================================================================================
{
    bool    bRet        = true;
    bool    bVersion    = false;

    if( _isRegPathExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1" ) )
    {
        bVersion = _isRegStringMatch( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1",
                                      "Inno Setup: Setup Version",
                                      "5.2.3");
    }
    else
    {
        STARTUPINFO         si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si,sizeof(si));
        si.cb=sizeof(si);
        ZeroMemory(&pi,sizeof(pi));

        if(!CreateProcess(L"Wamp\\WampServer2.0i.exe",NULL,0,0,0,0,0,0,&si,&pi))
            bRet = false;

        WaitForSingleObject(pi.hProcess,INFINITE);

        bVersion = _isRegStringMatch( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1",
                                      "Inno Setup: Setup Version",
                                      "5.2.3");
    }

    if( !bVersion )
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
            prbDBInstall->setValue( prbDBInstall->value()+1 );
            prbDBInstall->update();
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
            prbDBInstall->setValue( prbDBInstall->value()+1 );
            prbDBInstall->update();
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
            prbDBInstall->setValue( prbDBInstall->value()+1 );
            prbDBInstall->update();
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
        QFile file("sql/db_create.sql");

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
            return false;

        QString qsSQLCommand = "";
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            qsSQLCommand.append( line );
            if( line.contains( QChar(';') ))
            {
                m_poDB->exec( qsSQLCommand );
                prbDBInstall->setValue( prbDBInstall->value()+1 );
                prbDBInstall->update();
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
        QFile file("sql/db_fill.sql");

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
            return false;

        QString qsSQLCommand = "";
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            qsSQLCommand.append( line );
            if( line.contains( QChar(';') ))
            {
                m_poDB->exec( qsSQLCommand );
                prbDBInstall->setValue( prbDBInstall->value()+1 );
                prbDBInstall->update();
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
        for( int i=0; i<m_nCountDevices; i++ )
        {
            QString qsSQL = QString("INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES"
                                    "( 1, 1, \"%1 %2\", 0, 0, 1, \"ARC\" )").arg(tr("Device")).arg(i);
            m_poDB->exec( qsSQL );
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
    bool    bRet = true;

    pbCancel->setEnabled( false );

    prbDBInstallClient->setValue( 0 );
    prbDBInstallClient->update();

    QDir    qdInstallDir( m_qsClientInstallDir );

    if( m_pInstallType == rbInstall )
    {
        if( qdInstallDir.exists() )
        {
            if( QMessageBox::warning( this, tr("Attention"),
                                      tr("The specified directory already exits.\n"
                                         "All the files will be deleted or overwritten.\n\n"
                                         "Are you sure you want to continue?"),
                                      QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
            {
                pbCancel->setEnabled( true );
                return true;
            }
            if( !_emptyTargetDirectory( m_qsClientInstallDir ) )
            {
                QMessageBox::information( this, tr("Attention"),
                                          tr("Unable to empty the specified directory.\n"
                                             "%1\n"
                                             "Please manually delete the directory if copying new files fails.") );
            }
        }

        if( !_createTargetDirectory( m_qsClientInstallDir ) ||
            !_createTargetDirectory( QString("%1\\lang").arg(m_qsClientInstallDir) ) ||
            !_createTargetDirectory( QString("%1\\resources").arg(m_qsClientInstallDir) ) )
        {
            return false;
        }

        bRet = _copyInstallFiles( "install.li" );

        QString     qsFrom  = QString( "%1\\Setup.exe" ).arg(QDir::currentPath());
        QString     qsTo    = QString( "%1\\Temp\\Setup.exe" ).arg(m_qsPathWindows);

        if( !QFile::copy( qsFrom, qsTo ) )
        {
            bRet = false;
        }

        if( bRet )
        {
            bRet = _createFolderShortcut();
        }
    }
    else if( m_pInstallType == rbUpdate )
    {
        bRet = _copyInstallFiles( "update.li" );
    }
    else if( m_pInstallType == rbRemove )
    {
        if( !_emptyTargetDirectory( m_qsClientInstallDir ) )
        {
            QMessageBox::information( this, tr("Attention"),
                                      tr("Unable to empty the specified directory.\n"
                                         "Some of the files or subdirectories can not be removed." ) );
        }
    }

    if( bRet )
    {
        lblText8_2->setText( "" );
        pbNext->setEnabled( true );
        m_bInstallFinished = true;
        m_nTimer = startTimer( 2000 );
    }
    else
    {
        pbCancel->setEnabled( true );
    }

    return bRet;
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
void dlgMain::on_pbCheckRootPsw_clicked()
//=======================================================================================
{
    lblText3_4->setVisible( true );

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "mysql" );
    m_poDB->setUserName( "root" );
    m_poDB->setPassword( ledDBRootPassword->text() );

    if( m_poDB->open() )
    {
        imgOk3->setVisible( true );
        imgFail3->setVisible( false );
        m_poDB->close();
        pbNext->setEnabled( true );
        ledDBRootPassword->setEnabled( false );
        pbCheckRootPsw->setEnabled( false );
        m_qsRootPassword = ledDBRootPassword->text();
    }
    else
    {
        imgOk3->setVisible( false );
        imgFail3->setVisible( true );
        imgFail3->update();
        QString strErr = tr("Error occured when trying to connect to Wamp server.\n"
                            "Wamp server error message:\n\n%1").arg(m_poDB->lastError().text() );
        QMessageBox::warning(this, "Attention", strErr );
        pbNext->setEnabled( false );
        m_qsRootPassword = "";
    }
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
        chkDatabase->setChecked( true & m_bProcessDatabase );
        chkBelenus->setChecked( true );
    }

    _setEnableNextButton();
}
//=======================================================================================
void dlgMain::on_chkInternet_clicked()
//=======================================================================================
{
    m_bProcessInternet = chkInternet->isChecked();

    if( m_pInstallType == rbInstall && chkInternet->isChecked() )
    {
        if( chkWamp->isEnabled() )
            chkWamp->setChecked( true );
        chkDatabase->setChecked( true & m_bProcessDatabase );
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
        chkDatabase->setChecked( true & m_bProcessDatabase );
    }

    _setEnableNextButton();
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
bool dlgMain::_initializeWampServer()
//=======================================================================================
{
    bool        bRet = true;
    VRegistry   obReg;
    QString     strPath = "";

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1") ) )
    {
        strPath = obReg.get_REG_SZ( "InstallLocation" );
        obReg.CloseKey();
    }
    if( strPath.length() )
    {
//        QString qsTemplate      = QString( "%1\\bin\\mysql\\mysql5.1.36\\bin\\my-template.cnf" ).arg(strPath);
//        QString qsConfig        = QString( "%1\\bin\\mysql\\mysql5.1.36\\my-template.ini" ).arg(strPath);
//        QString strMySQLConfig  = QString( "\"%1\\bin\\mysql\\mysql5.1.36\\bin\\MySQLInstanceConfig.exe\" -t%2 -c%3" ).arg(strPath).arg(qsTemplate).arg(qsConfig);
        QString strMySQLConfig  = QString( "\"%1\\bin\\mysql\\mysql5.1.36\\bin\\MySQLInstanceConfig.exe\"" ).arg(strPath);

        STARTUPINFO         si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si,sizeof(si));
        si.cb=sizeof(si);
        ZeroMemory(&pi,sizeof(pi));

        WCHAR   wsMySQLConfig[1000];

        memset( wsMySQLConfig, 0, 1000 );
        strMySQLConfig.toWCharArray( wsMySQLConfig );

        if(!CreateProcess(wsMySQLConfig,NULL,0,0,0,0,0,0,&si,&pi))
            bRet = false;

        WaitForSingleObject(pi.hProcess,INFINITE);
    }

    return bRet;
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
        if( m_bProcessDatabase)
            m_vPages.append( CONST_PAGE_DATABASE_INSTALL );
        if( m_bProcessHWConnection )
            m_vPages.append( CONST_PAGE_HARDWARE_INSTALL );
        if( m_bProcessInternet )
            m_vPages.append( CONST_PAGE_INTERNET_INSTALL );
        if( m_bProcessBelenusClient )
            m_vPages.append( CONST_PAGE_CLIENT_INSTALL );
    }
    else if( m_pInstallType == rbUpdate )
    {
        m_vPages.append( CONST_PAGE_COMPONENT_SELECTION );
        if( m_bProcessDatabase)
            m_vPages.append( CONST_PAGE_DATABASE_INSTALL );
        if( m_bProcessHWConnection )
            m_vPages.append( CONST_PAGE_HARDWARE_INSTALL );
        if( m_bProcessInternet )
            m_vPages.append( CONST_PAGE_INTERNET_INSTALL );
        if( m_bProcessBelenusClient )
            m_vPages.append( CONST_PAGE_CLIENT_INSTALL );
    }
    /*else if( m_pInstallType == rbRemove )
    {
        if( m_bProcessBelenusClient )
            m_vPages.append( CONST_PAGE_CLIENT_INSTALL );
        if( m_bProcessInternet )
            m_vPages.append( CONST_PAGE_INTERNET_INSTALL );
        if( m_bProcessHWConnection )
            m_vPages.append( CONST_PAGE_HARDWARE_INSTALL );
        if( m_bProcessDatabase)
            m_vPages.append( CONST_PAGE_DATABASE_INSTALL );
        if( m_bProcessWamp)
            m_vPages.append( CONST_PAGE_WAMP_INSTALL );
    }*/

    m_vPages.append( CONST_PAGE_PROCESS );
    m_vPages.append( CONST_PAGE_FINISH );
}
//=======================================================================================
bool dlgMain::_isRegPathExists( QString p_qsPath )
//=======================================================================================
{
    bool        bRet = false;
    VRegistry   obReg;

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, p_qsPath ) )
    {
        bRet = true;
        obReg.CloseKey();
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_isRegStringMatch( QString p_qsPath, QString p_qsKey, QString p_qsValue )
//=======================================================================================
{
    bool        bRet = false;
    VRegistry   obReg;

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, p_qsPath ) )
    {
        if( p_qsValue.compare( obReg.get_REG_SZ( p_qsKey ) ) == 0 )
            bRet = true;

        obReg.CloseKey();
    }

    return bRet;
}
//=======================================================================================
void dlgMain::_setEnableNextButton()
//=======================================================================================
{
    if( !chkWamp->isChecked() && !chkDatabase->isChecked() && !chkHardware->isChecked() && !chkInternet->isChecked() && !chkBelenus->isChecked() )
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
    bool    bRet = true;
    QDir    qdInstallDir( p_qsPath );

    if( !qdInstallDir.mkpath( p_qsPath ) )
    {
        QMessageBox::critical( this, tr("System error"),
                               tr("Unable to create directory:\n\n%1").arg(p_qsPath));
        pbCancel->setEnabled( true );
        return false;
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_copyClientFile( QString p_qsFileName, bool p_bInstall )
//=======================================================================================
{
    QString     qsFrom  = QString( "%1\\%2" ).arg(QDir::currentPath()).arg(p_qsFileName);
    QString     qsTo    = QString( "%1%2" ).arg(m_qsClientInstallDir.left(m_qsClientInstallDir.length()-7)).arg(p_qsFileName);

    qsFrom.replace( '/', '\\' );

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

    if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        pbCancel->setEnabled( true );
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

    prbDBInstallClient->setMaximum( qslFiles.size()+1 );

    for( int i=0; i<qslFiles.size(); i++ )
    {
        prbDBInstallClient->setValue( i+1 );
        prbDBInstallClient->update();
        if( p_bInstall )
        {
            lblText8_2->setText( tr("Copying file: ..\\%1").arg(qslFiles.at(i)) );
        }
        else
        {
            lblText8_2->setText( tr("Updating file: ..\\%1").arg(qslFiles.at(i)) );
        }
        if( !_copyClientFile( qslFiles.at(i), p_bInstall ) )
        {
            if( p_bInstall )
                QMessageBox::critical( this, tr("System error"),
                                       tr("Unable to copy file:\n\n%1").arg(qslFiles.at(i)));
            else
                QMessageBox::critical( this, tr("System error"),
                                       tr("Unable to update file:\n\n%1").arg(qslFiles.at(i)));
            bRet = false;
            break;
        }
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_createFolderShortcut()
//=======================================================================================
{
    bool        bRet = true;
    VRegistry   obReg;
    QString     qsDirPrograms;
    QString     qsDirDesktop;

    lblText8_2->setText( tr("Creating folder and shortcut for client application.") );

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders") ) )
    {
        qsDirPrograms   = obReg.get_REG_SZ( QString("Common Programs") );
        qsDirDesktop    = obReg.get_REG_SZ( QString("Common Desktop") );
        obReg.CloseKey();

        if( qsDirPrograms.contains( ":\\" ) )
        {
            if( !_createTargetDirectory( QString("%1\\Belenus").arg(qsDirPrograms) ) )
                return false;

            m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
            m_obFile->link( QString("%1\\Belenus\\belenus.lnk").arg(qsDirPrograms) );
            delete m_obFile;
        }

        if( qsDirDesktop.contains( ":\\" ) )
        {
            m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
            m_obFile->link( QString("%1\\belenus.lnk").arg(qsDirDesktop) );
            delete m_obFile;
        }

        if( obReg.CreateKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus") ) )
        {
            if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus") ) )
            {
                obReg.set_REG_SZ( QString("DisplayIcon"), QString("%1\\resources\\belenus.ico").arg(m_qsClientInstallDir) );
                obReg.set_REG_SZ( QString("DisplayName"), tr("Belenus Application System") );
                obReg.set_REG_SZ( QString("DisplayVersion"), QString("1.0.0.0") );
                obReg.set_REG_SZ( QString("InstallLocation"), m_qsClientInstallDir );
                obReg.set_REG_SZ( QString("Publisher"), QString("Pagony Multimédia Stúdió Bt.") );
                obReg.set_REG_SZ( QString("UninstallString"), QString("%1\\setup.exe -uninstall").arg(m_qsClientInstallDir) );
                obReg.set_REG_SZ( QString("URLInfoAbout"), QString("http://belenus.pagonymedia.hu") );
                obReg.CloseKey();
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
        prbDBInstallClient->setValue( prbDBInstallClient->value()+1 );
        prbDBInstallClient->update();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=======================================================================================
