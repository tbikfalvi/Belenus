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

    if( _isRegPathExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" ) )
        m_pInstallType = rbUpdate;
    else
        m_pInstallType = rbInstall;

    m_vPages.clear();
    m_vPages.append( CONST_PAGE_WELCOME );
    m_vPages.append( CONST_PAGE_INSTALL_SELECTION );
    m_vPages.append( CONST_PAGE_FINISH );

    m_nCurrentPage          = CONST_PAGE_WELCOME;

    m_bInstallStarted       = false;

    m_bProcessWamp          = true;
    m_bProcessDatabase      = true;
    m_bProcessHWConnection  = true;
    m_bProcessInternet      = true;
    m_bProcessBelenusClient = true;

    m_bStartWampInstall     = false;
    m_bInitializeWamp       = false;
    m_bCreateDatabase       = false;

    m_bDatabaseInstalled    = false;

    m_bRestartRequired      = false;

    m_poDB                  = NULL;
    m_poDB                  = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    m_poHardware            = NULL;
    m_nComPort              = 0;
    m_nCountDevices         = 0;

    m_bDemoMode             = false;

    m_qsClientInstallDir    = QString( "C:\\Program Files\\Belenus" );
    m_qsIPAddress           = QString( "127.0.0.1" );
}
//=======================================================================================
dlgMain::~dlgMain()
//=======================================================================================
{
    if( m_poDB != NULL )        delete m_poDB;
    if( m_poHardware != NULL )  delete m_poHardware;
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
void dlgMain::_initializePage( int p_nPage )
//=======================================================================================
{
    switch( p_nPage )
    {
        case CONST_PAGE_INSTALL_SELECTION:
            _initializeInstallSelection();
            break;

        case CONST_PAGE_COMPONENT_SELECTION:
            _initializeComponentSelection();
            break;

        case CONST_PAGE_WAMP_INSTALL:
            _initializeWampInstall();
            break;

        case CONST_PAGE_DATABASE_INSTALL:
            _initializeDatabaseInstall();
            break;

        case CONST_PAGE_HARDWARE_INSTALL:
            _initializeHardwareInstall();
            break;

        case CONST_PAGE_INTERNET_INSTALL:
            _initializeInternetInstall();
            break;

        case CONST_PAGE_CLIENT_INSTALL:
            _initializeClientInstall();
            break;

        case CONST_PAGE_PROCESS:
            _initializeInstallProcess();
            break;

        case CONST_PAGE_FINISH:
            _initializeFinishPage();
            break;
    }
}
//=======================================================================================
void dlgMain::_initializeInstallSelection()
//=======================================================================================
{
    if( _isRegPathExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" ) )
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
void dlgMain::_initializeComponentSelection()
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
void dlgMain::_initializeWampInstall()
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
void dlgMain::_initializeDatabaseInstall()
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
void dlgMain::_initializeHardwareInstall()
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
void dlgMain::_initializeInternetInstall()
//=======================================================================================
{
    ledDBIPAddress->setText( m_qsIPAddress );
    ledDBIPPort->setText( QString::number(m_nPort ) );
}
//=======================================================================================
void dlgMain::_initializeClientInstall()
//=======================================================================================
{
    ledClientInstallDir->setText( m_qsClientInstallDir );
}
//=======================================================================================
void dlgMain::_initializeInstallProcess()
//=======================================================================================
{
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
}
//=======================================================================================
bool dlgMain::_processPage( int p_nPage )
//=======================================================================================
{
    bool bRet = true;

    switch( p_nPage )
    {
        case 1:
            bRet = _processInstallSelection();
            break;

        case 2:
            bRet = _processComponentSelection();
            break;

        case 3:
            bRet = _processWampInstall();
            break;

        case 4:
            bRet = _processDatabaseInstall();
            break;

        case 5:
            bRet = _processHardwareInstall();
            break;

        case 6:
            bRet = _processInternetInstall();
            break;

        case 7:
            bRet = _processClientInstall();
            break;

        case 99: // Installation
        {
            m_obFile = new QFile( QString("C:\\Program Files\\Belenus\\Kliens\\belenus.exe") );
            m_obFile->link( QString("C:\\Development\\Qt\\belenus.lnk") );
            delete m_obFile;
            break;
        }
    }

    return bRet;
}
//=======================================================================================
bool dlgMain::_processInstallSelection()
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
bool dlgMain::_processComponentSelection()
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
bool dlgMain::_processWampInstall()
//=======================================================================================
{
    bool    bRet = true;

    return bRet;
}
//=======================================================================================
bool dlgMain::_processDatabaseInstall()
//=======================================================================================
{
    bool    bRet = true;

    return bRet;
}
//=======================================================================================
bool dlgMain::_processHardwareInstall()
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
bool dlgMain::_processInternetInstall()
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
bool dlgMain::_processClientInstall()
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
        strPath = obReg.get_REG_SZ( "Inno Setup: App Path" );
        obReg.CloseKey();
    }
    if( strPath.length() )
    {
        QString strMySQLConfig = QString( "%1\\bin\\mysql\\mysql5.1.36\\bin\\MySQLInstanceConfig.exe" ).arg(strPath);

        STARTUPINFO         si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si,sizeof(si));
        si.cb=sizeof(si);
        ZeroMemory(&pi,sizeof(pi));

        WCHAR   wsMySQLConfig[1000];

        memset( wsMySQLConfig, 0, 1000 );
        strMySQLConfig.toWCharArray( wsMySQLConfig );
//QMessageBox::information(this,"",QString::fromStdWString(wsMySQLConfig));

        if(!CreateProcess(wsMySQLConfig,NULL,0,0,0,0,0,0,&si,&pi))
//        if(!CreateProcess(strMySQLConfig.toStdString().c_str(),NULL,0,0,0,0,0,0,&si,&pi))
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
