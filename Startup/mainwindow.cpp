
#include <QProcessEnvironment>
#include <QDateTime>
#include <QDomDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QDir>

#include "windows.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PROCESS_INSTALL 1
#define PROCESS_REMOVE  2
#define PROCESS_UPDATE  3

extern QString         g_qsCurrentPath;

cRegistry        g_obReg;

//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_pbLangEn_clicked();

    ui->rbProcessUpdate->setVisible( false );

    m_qsErrorReportFile             = QString( "c:/Kiwisun/belenus_install.log" );
    m_qsRootPassword                = "adminpass";
    m_bWampServerAlreadyInstalled   = false;
    m_bDatabaseAlreadyInstalled     = false;
    m_bRootUserExists               = false;
    m_bBelenusUserExists            = false;

    m_qsPathWampServer              = "";
    m_qsUninstallWampExec           = "";
    m_qsProcessErrorMsg             = "";
    m_qsLanguage                    = "";
    m_qsClientInstallDir            = "";

    obProcessDoc = new QDomDocument( "LanguageProcess" );

    m_obLog = new QFile( m_qsErrorReportFile );
    if( m_obLog == NULL )
    {
        QMessageBox::critical( this, tr("Error"),
                               tr("Error occured during initialization.\n"
                                  "Please contact system administrator.\n"
                                  "Error code: ErrLogCreateFail\n"
                                  "[%1]").arg( m_qsErrorReportFile ) );
        return;
    }

    _logProcess( "" );
    _logProcess( QString("========================================================================================") );
    _logProcess( QString("Kiwisun setup application started - %1\n").arg( QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm" ) ) );
    _logProcess( QString("----------------------------------------------------------------------------------------") );

    // Initialize GUI components
    ui->cmbLanguage->addItem( "English (en)" );
    ui->cmbLanguage->addItem( "Magyar (hu)" );
    ui->cmbLanguage->addItem( "Deutsch (de)" );
    ui->cmbLanguage->addItem( "Italiano (it)" );

    connect( ui->rbProcessInstall, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
//    connect( ui->rbProcessUpdate, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
    connect( ui->rbProcessRemove, SIGNAL(clicked()), this, SLOT(on_process_selected()) );

    ui->pbStart->setEnabled( false );
    ui->progressBar->setVisible( false );

    QProcessEnvironment qpeInfo = QProcessEnvironment::systemEnvironment();

    ui->ledDirectoryStartup->setText( qpeInfo.value( "BelenusStartup", "C:/Kiwisun/BelenusUpdate" ) );
    ui->ledDirectoryTarget->setText( qpeInfo.value( "BelenusTarget", "C:/Kiwisun/Belenus" ) );
    ui->ledDirectoryResource->setText( qpeInfo.value( "BelenusResource", "Download" ) );
    ui->ledDirectoryBackup->setText( qpeInfo.value( "BelenusBackup", "Backup" ) );

    on_ledDirectoryResource_editingFinished();
    on_ledDirectoryBackup_editingFinished();

    QSettings   updater( QString("%1/settings.ini").arg(ui->ledDirectoryStartup->text()), QSettings::IniFormat );

    if( updater.value( "PreProcess/Address", "" ).toString().compare("") == 0 )
    {
        ui->rbLocationLocal->setChecked( true );
    }
    else
    {
        ui->rbLocationWeb->setChecked( true );
    }

    on_pbLangHu_clicked();

    ui->cmbLanguage->setEnabled( false );
    ui->rbAppOfficial->setEnabled( false );
    ui->rbAppDemo->setEnabled( false );
    ui->rbLocationLocal->setEnabled( false );
    ui->rbLocationWeb->setEnabled( false );
    ui->ledPanelCount->setEnabled( false );
    ui->spinCombo->setEnabled( false );
    ui->pbDirectoryStartup->setEnabled( false );
    ui->pbDirectoryTarget->setEnabled( false );
    ui->ledDirectoryResource->setEnabled( false );
    ui->pbDirectoryResource->setEnabled( false );
    ui->ledDirectoryBackup->setEnabled( false );
    ui->pbDirectoryBackup->setEnabled( false );
    ui->pbDefault->setEnabled( false );
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete obProcessDoc;
    delete ui;
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbLangEn_clicked()
{
    ui->pbLangEn->setChecked( true );
    ui->pbLangIt->setChecked( false );
    ui->pbLangDe->setChecked( false );
    ui->pbLangHu->setChecked( false );
    apMainApp->removeTranslator( poTransStartup );
    apMainApp->removeTranslator( poTransQT );
    ui->retranslateUi( this );
    m_qsLangInstaller = "en";
}

void MainWindow::on_pbLangDe_clicked()
{
    ui->pbLangEn->setChecked( false );
    ui->pbLangIt->setChecked( false );
    ui->pbLangDe->setChecked( true );
    ui->pbLangHu->setChecked( false );
    apMainApp->removeTranslator( poTransStartup );
    apMainApp->removeTranslator( poTransQT );
    QString qsLangSetup = QString("%1\\lang\\startup_de.qm").arg(QDir::currentPath());
    QString qsLangQT = QString("%1\\lang\\qt_de.qm").arg(QDir::currentPath());
    poTransStartup->load( qsLangSetup );
    poTransQT->load( qsLangQT );
    apMainApp->installTranslator( poTransStartup );
    apMainApp->installTranslator( poTransQT );
    ui->retranslateUi( this );
    m_qsLangInstaller = "de";
}

void MainWindow::on_pbLangHu_clicked()
{
    ui->pbLangEn->setChecked( false );
    ui->pbLangIt->setChecked( false );
    ui->pbLangDe->setChecked( false );
    ui->pbLangHu->setChecked( true );
    apMainApp->removeTranslator( poTransStartup );
    apMainApp->removeTranslator( poTransQT );
    QString qsLangSetup = QString("%1\\lang\\startup_hu.qm").arg(QDir::currentPath());
    QString qsLangQT = QString("%1\\lang\\qt_hu.qm").arg(QDir::currentPath());
    poTransStartup->load( qsLangSetup );
    poTransQT->load( qsLangQT );
    apMainApp->installTranslator( poTransStartup );
    apMainApp->installTranslator( poTransQT );
    ui->retranslateUi( this );
    m_qsLangInstaller = "hu";
}

void MainWindow::on_pbLangIt_clicked()
{
    ui->pbLangEn->setChecked( false );
    ui->pbLangIt->setChecked( true );
    ui->pbLangDe->setChecked( false );
    ui->pbLangHu->setChecked( false );
    apMainApp->removeTranslator( poTransStartup );
    apMainApp->removeTranslator( poTransQT );
    QString qsLangSetup = QString("%1\\lang\\startup_it.qm").arg(QDir::currentPath());
    QString qsLangQT = QString("%1\\lang\\qt_it.qm").arg(QDir::currentPath());
    poTransStartup->load( qsLangSetup );
    poTransQT->load( qsLangQT );
    apMainApp->installTranslator( poTransStartup );
    apMainApp->installTranslator( poTransQT );
    ui->retranslateUi( this );
    m_qsLangInstaller = "it";
}

//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_process_selected()
{
    if( ui->rbProcessInstall->isChecked() )
    {
        m_nProcessType = PROCESS_INSTALL;
        ui->cmbLanguage->setEnabled( true );
        ui->rbAppOfficial->setEnabled( true );
        ui->rbAppDemo->setEnabled( true );
        ui->rbLocationLocal->setEnabled( true );
        ui->rbLocationWeb->setEnabled( true );
        ui->ledPanelCount->setEnabled( true );
        ui->spinCombo->setEnabled( true );
        ui->pbDirectoryStartup->setEnabled( true );
        ui->pbDirectoryTarget->setEnabled( true );
        ui->ledDirectoryResource->setEnabled( true );
        ui->pbDirectoryResource->setEnabled( true );
        ui->ledDirectoryBackup->setEnabled( true );
        ui->pbDirectoryBackup->setEnabled( true );
        ui->pbDefault->setEnabled( true );
    }
    else if( ui->rbProcessRemove->isChecked() )
    {
        m_nProcessType = PROCESS_REMOVE;
        ui->cmbLanguage->setEnabled( false );
        ui->rbAppOfficial->setEnabled( false );
        ui->rbAppDemo->setEnabled( false );
        ui->rbLocationLocal->setEnabled( false );
        ui->rbLocationWeb->setEnabled( false );
        ui->ledPanelCount->setEnabled( false );
        ui->spinCombo->setEnabled( false );
        ui->pbDirectoryStartup->setEnabled( false );
        ui->pbDirectoryTarget->setEnabled( false );
        ui->ledDirectoryResource->setEnabled( false );
        ui->pbDirectoryResource->setEnabled( false );
        ui->ledDirectoryBackup->setEnabled( false );
        ui->pbDirectoryBackup->setEnabled( false );
        ui->pbDefault->setEnabled( false );
    }
    /*else if( ui->rbProcessUpdate->isChecked() )
    {
        m_nProcessType = PROCESS_UPDATE;
        ui->cmbLanguage->setEnabled( true );
        ui->rbAppOfficial->setEnabled( false );
        ui->rbAppDemo->setEnabled( false );
        ui->rbLocationLocal->setEnabled( true );
        ui->rbLocationWeb->setEnabled( true );
        ui->ledPanelCount->setEnabled( false );
        ui->spinCombo->setEnabled( false );
        ui->pbDirectoryStartup->setEnabled( true );
        ui->pbDirectoryTarget->setEnabled( true );
        ui->ledDirectoryResource->setEnabled( true );
        ui->pbDirectoryResource->setEnabled( true );
        ui->ledDirectoryBackup->setEnabled( true );
        ui->pbDirectoryBackup->setEnabled( true );
        ui->pbDefault->setEnabled( true );
    }*/
    else
    {
        return;
    }

    ui->pbStart->setEnabled( true );
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbDefault_clicked()
{
    QProcessEnvironment qpeInfo = QProcessEnvironment::systemEnvironment();

    ui->ledDirectoryStartup->setText( qpeInfo.value( "BelenusStartup", "C:/Kiwisun/BelenusUpdate" ) );
    ui->ledDirectoryTarget->setText( qpeInfo.value( "BelenusTarget", "C:/Kiwisun/Belenus" ) );
    ui->ledDirectoryResource->setText( qpeInfo.value( "BelenusResource", "Download" ) );
    ui->ledDirectoryBackup->setText( qpeInfo.value( "BelenusBackup", "Backup" ) );

    on_ledDirectoryResource_editingFinished();
    on_ledDirectoryBackup_editingFinished();
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbDirectoryStartup_clicked()
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       ui->ledDirectoryStartup->text(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( qsDir.length() > 0 )
    {
        qsDir.replace( '\\', '/' );
        ui->ledDirectoryStartup->setText( qsDir );
        on_ledDirectoryResource_editingFinished();
        on_ledDirectoryBackup_editingFinished();
    }
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbDirectoryTarget_clicked()
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       ui->ledDirectoryTarget->text(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( qsDir.length() > 0 )
    {
        qsDir.replace( '\\', '/' );
        ui->ledDirectoryTarget->setText( qsDir );
    }
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_ledDirectoryResource_editingFinished()
{
    if( ui->ledDirectoryResource->text().mid(1,1).compare(":") == 0 )
    {
        ui->ledDirectoryResource->setToolTip( ui->ledDirectoryResource->text() );
    }
    else
    {
        QString qsTemp = QString( "%1/%2" ).arg( ui->ledDirectoryStartup->text() )
                                           .arg( ui->ledDirectoryResource->text() );
        qsTemp.replace( "\\", "/" );
        qsTemp.replace( "//", "/" );

        ui->ledDirectoryResource->setToolTip( qsTemp );
    }
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbDirectoryResource_clicked()
{
    QString qsDefault = ui->ledDirectoryResource->toolTip();

    if( ui->ledDirectoryResource->text().mid(1,1).compare(":") == 0 )
    {
        qsDefault = ui->ledDirectoryResource->text();
    }

    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       qsDefault,
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( qsDir.length() > 0 )
    {
        qsDir.replace( '\\', '/' );
        ui->ledDirectoryResource->setText( qsDir );
        ui->ledDirectoryResource->setToolTip( qsDir );
    }
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_ledDirectoryBackup_editingFinished()
{
    if( ui->ledDirectoryBackup->text().mid(1,1).compare(":") == 0 )
    {
        ui->ledDirectoryBackup->setToolTip( ui->ledDirectoryBackup->text() );
    }
    else
    {
        QString qsTemp = QString( "%1/%2" ).arg( ui->ledDirectoryStartup->text() )
                                           .arg( ui->ledDirectoryBackup->text() );
        qsTemp.replace( "\\", "/" );
        qsTemp.replace( "//", "/" );

        ui->ledDirectoryBackup->setToolTip( qsTemp );
    }
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbDirectoryBackup_clicked()
{
    QString qsDefault = ui->ledDirectoryBackup->toolTip();

    if( ui->ledDirectoryBackup->text().mid(1,1).compare(":") == 0 )
    {
        qsDefault = ui->ledDirectoryBackup->text();
    }

    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       qsDefault,
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( qsDir.length() > 0 )
    {
        qsDir.replace( '\\', '/' );
        ui->ledDirectoryBackup->setText( qsDir );
        ui->ledDirectoryBackup->setToolTip( qsDir );
    }
}
//=================================================================================================
// on_pbStart_clicked
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbStart_clicked()
{
    bool    bProcessSucceeded = true;

    m_qsProcessErrorMsg = "";

    ui->pbStart->setVisible( false );
    ui->progressBar->setVisible( true );

    m_qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

    QString qsProcessName;

    switch( m_nProcessType )
    {
        case PROCESS_INSTALL:
        {
            ui->progressBar->setValue( 1 );

            qsProcessName = tr( "installation" );

            _logProcess( "Installation started" );

            _checkInstallComponents();
            _progressStep();

            _updateEnvironmentVariables();
            _progressStep();

            _logProcess( "Create directories for updater and for Belenus if not exists" );
            if( !_createPaths() ) { return; }
            _progressStep();

            _logProcess( "Create the settings file for the updater" );
            if( !_createSettingsFile() ) { return; }
            _progressStep();

            _logProcess( "Copy the updater files" );
            if( !_copyUpdaterFiles() ) { return; }
            _progressStep();

            // Copy the belenus_loc.xml file if needed
            if( ui->rbLocationLocal->isChecked() )
            {
                if( !_copyXmlFile() ) { return; }
                _progressStep();
            }

            // Install Wamp server if needed
            if( bProcessSucceeded )
            {
                bProcessSucceeded = _installWampServer();
                _progressStep();
                _logProcess( "Process wamp install ... ", false );
                _logProcess( bProcessSucceeded?"OK":"FAILED" );
            }

            // Install database
            if( bProcessSucceeded )
            {
                bProcessSucceeded = _processDatabaseInstall();
                _logProcess( "Process database install ... ", false );
                _logProcess( bProcessSucceeded?"OK":"FAILED" );
            }

            // Install client files
            if( bProcessSucceeded )
            {
                bProcessSucceeded = _processClientInstall();
                _logProcess( "Process client install ... ", false );
                _logProcess( bProcessSucceeded?"OK":"FAILED" );
            }

            ui->progressBar->setValue( ui->progressBar->maximum() );
            break;
        }
        case PROCESS_REMOVE:
        {
            qsProcessName = tr( "uninstall" );

            if( QMessageBox::question( this, tr("Question"),
                                       tr("Are you sure you want to uninstall Belenus Application System and all of it's components?\n"
                                          "All of the data will be deleted from the computer."),
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
                ui->progressBar->setMaximum( 100 );
                ui->progressBar->setValue( 1 );
                _progressStep();

                _removeShortcuts();
                _progressStep();

                _emptyTargetDirectory( ui->ledDirectoryTarget->text() );
                _removeDirectory( ui->ledDirectoryTarget->text() );
                _progressStep();
                _emptyTargetDirectory( ui->ledDirectoryResource->toolTip() );
                _removeDirectory( ui->ledDirectoryResource->toolTip() );
                _progressStep();
                _emptyTargetDirectory( ui->ledDirectoryBackup->toolTip() );
                _removeDirectory( ui->ledDirectoryBackup->toolTip() );
                _progressStep();
                _emptyTargetDirectory( ui->ledDirectoryStartup->text() );
                _removeDirectory( ui->ledDirectoryStartup->text() );
                _progressStep();

                _executeWampUninstall();

                ui->progressBar->setValue( ui->progressBar->maximum() );
            }
            break;
        }
        /*case PROCESS_UPDATE:
        {
            _updateEnvironmentVariables();

            // Create directories for updater and for Belenus if not exists
            if( !_createPaths() ) { return; }
            ui->progressBar->setValue( 1 );

            // Refresh the settings file for the updater based on the modifications
            if( !_updateSettingsFile() ) { return; }
            ui->progressBar->setValue( 2 );

            // Copy the updater files to get the latest versions
            if( !_copyUpdaterFiles() ) { return; }

            // Copy the belenus_loc.xml file if needed
            if( ui->rbLocationLocal->isChecked() )
            {
                if( !_copyXmlFile() ) { return; }
            }

            ui->progressBar->setValue( ui->progressBar->maximum() );

            // Start the updater
            _executeUpdater();
            break;
        }*/
        default:
            return;
    }

    if( bProcessSucceeded )
    {
        QMessageBox::information( this, tr("Information"),
                                  tr( "The %1 process finished." ).arg( qsProcessName ) );
    }
    else
    {
        QMessageBox::warning( this, "Attention",
                              tr("Error occured during installing Belenus Application System.\n"
                                 "Please contact Belenus software support.\n\n"
                                 "Error code: %1").arg(m_qsProcessErrorMsg) );
    }

    close();
}
//=================================================================================================
// _updateEnvironmentVariables
//-------------------------------------------------------------------------------------------------
void MainWindow::_updateEnvironmentVariables()
{
    QProcess *qpProcess  = new QProcess(this);
    QString   qsProcess  = "";

    _logProcess( "Update environment variables" );

    qsProcess = QString( "c:/windows/system32/setx.exe BelenusStartup \"%1\" " ).arg( ui->ledDirectoryStartup->text() );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }

    qsProcess = QString( "c:/windows/system32/setx.exe BelenusTarget \"%1\" " ).arg( ui->ledDirectoryTarget->text() );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }

    qsProcess = QString( "c:/windows/system32/setx.exe BelenusResource \"%1\" " ).arg( ui->ledDirectoryResource->toolTip() );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }

    qsProcess = QString( "c:/windows/system32/setx.exe BelenusBackup \"%1\" " ).arg( ui->ledDirectoryBackup->toolTip() );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }

    delete qpProcess;

    QSettings obPref( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                      QSettings::NativeFormat );

    obPref.setValue( "BelenusStartup", ui->ledDirectoryStartup->text() );
    obPref.setValue( "BelenusTarget", ui->ledDirectoryTarget->text() );
    obPref.setValue( "BelenusResource", ui->ledDirectoryResource->toolTip() );
    obPref.setValue( "BelenusBackup", ui->ledDirectoryBackup->toolTip() );

    SendMessage( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment" );
}
//=================================================================================================
// _createPaths
//-------------------------------------------------------------------------------------------------
bool MainWindow::_createPaths()
{
    bool bRet = false;

    if( _createPath( ui->ledDirectoryStartup->text() ) &&
        _createPath( QString("%1/lang").arg(ui->ledDirectoryStartup->text()) ) &&
        _createPath( QString("%1/resources").arg(ui->ledDirectoryStartup->text()) ) &&
        _createPath( QString("%1/sqldrivers").arg(ui->ledDirectoryStartup->text()) ) &&
        _createPath( ui->ledDirectoryTarget->text() ) &&
        _createPath( ui->ledDirectoryResource->toolTip() ) &&
        _createPath( ui->ledDirectoryBackup->toolTip() ) )
    {
        bRet = true;
    }

    return bRet;
}
//=================================================================================================
// _createPath
//-------------------------------------------------------------------------------------------------
bool MainWindow::_createPath(QString p_qsPath)
{
    QDir    qdBackup( p_qsPath );

    if( !qdBackup.exists() )
    {
        if( !qdBackup.mkpath( p_qsPath ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The following directory can not be created:\n\n%1\n\n"
                                     "Please create the directory manually and restart process.") );
            return false;
        }
    }
    return true;
}
//=================================================================================================
// _createSettingsFile
//-------------------------------------------------------------------------------------------------
bool MainWindow::_createSettingsFile()
{
    QSettings obMasterCD( "system.inf", QSettings::IniFormat );

    QString qsVersion   = obMasterCD.value( "Version", "1.4.0" ).toString();
    QString qsSettings  = QString( "%1/settings.ini" ).arg( ui->ledDirectoryStartup->text() );
    QString qsAddress   = "";
    QString qsLocation  = "";
    QString qsAppType   = "";

    qsSettings.replace( "\\", "/" );
    qsSettings.replace( "//", "/" );

    QSettings obPrefFile( qsSettings, QSettings::IniFormat );

    if( ui->rbAppDemo->isChecked() )
    {
        qsAppType = "demo";
    }
    else if( ui->rbAppOfficial->isChecked() )
    {
        qsAppType = "official";
    }

    if( ui->rbLocationWeb->isChecked() )
    {
        qsAddress = QString( "http://download.bikfalvi.hu/belenus/%1" ).arg( qsAppType );
        qsLocation = "web";
    }
    else
    {
        qsLocation = "loc";
    }

    m_qsInfoFile = QString( "belenus_%1.xml" ).arg( qsLocation );

    obPrefFile.setValue( QString::fromAscii( "Language/Path" ), "lang" );
    obPrefFile.setValue( QString::fromAscii( "Language/Extension" ), m_qsLanguage );

    obPrefFile.setValue( QString::fromAscii( "Settings/WindowWidth" ), "640" );
    obPrefFile.setValue( QString::fromAscii( "Settings/WindowHeight" ), "400" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Background" ), "resources/updater.png" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Textcolor" ), "DDDDDD" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Timer" ), "250" );

    obPrefFile.setValue( QString::fromAscii( "PreProcess/Version" ), qsVersion );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/Address" ), qsAddress );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/InfoFile" ), m_qsInfoFile );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/InstallDir" ), ui->ledDirectoryTarget->text() );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/DownloadDir" ), ui->ledDirectoryResource->toolTip() );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/BackupDir" ), ui->ledDirectoryBackup->toolTip() );

    obPrefFile.setValue( QString::fromAscii( "PostProcess/HomeDir" ), "" );
    obPrefFile.setValue( QString::fromAscii( "PostProcess/File" ), "" );
/*
    QMessageBox::warning( this, tr("Warning"),
                          tr("Unable to create settings.ini on the following directory:\n\n%1\n\n"
                             "Please check your user rights on the directory and file.")
                          .arg( ui->ledDirectoryStartup->text() ) );
*/
    return true;
}
//=================================================================================================
// _updateSettingsFile
//-------------------------------------------------------------------------------------------------
/*bool MainWindow::_updateSettingsFile()
{
    QString qsSettings  = QString( "%1/settings.ini" ).arg( ui->ledDirectoryStartup->text() );
    QString qsAddress   = "";
    QString qsLocation  = "";
    QString qsAppType   = "";

    qsSettings.replace( "\\", "/" );
    qsSettings.replace( "//", "/" );

    QSettings obPrefFile( qsSettings, QSettings::IniFormat );

    if( obPrefFile.value( "Version", "" ).toString().length() == 0 )
    {
        return _createSettingsFile();
    }

    QString qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

    if( ui->rbAppDemo->isChecked() )
    {
        qsAppType = "demo";
    }
    else if( ui->rbAppOfficial->isChecked() )
    {
        qsAppType = "official";
    }

    if( ui->rbLocationWeb->isChecked() )
    {
        qsAddress = QString( "http://download.bikfalvi.hu/belenus/%1" ).arg( qsAppType );
        qsLocation = "web";
    }
    else
    {
        qsLocation = "loc";
    }

    m_qsInfoFile = QString( "belenus_%1.xml" ).arg( qsLocation );

    obPrefFile.setValue( QString::fromAscii( "Language/Extension" ), qsLanguage );

    obPrefFile.setValue( QString::fromAscii( "PreProcess/Address" ), qsAddress );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/InfoFile" ), m_qsInfoFile );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/InstallDir" ), ui->ledDirectoryTarget->text() );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/DownloadDir" ), ui->ledDirectoryResource->toolTip() );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/BackupDir" ), ui->ledDirectoryBackup->toolTip() );
    return true;
}*/
//=================================================================================================
// _copyUpdaterFiles
//-------------------------------------------------------------------------------------------------
bool MainWindow::_copyUpdaterFiles()
//-------------------------------------------------------------------------------------------------
{
    QDomDocument    *obProcessDoc   = new QDomDocument( "StartupProcess" );
    QString          qsFileName     = QString( "%1/settings/updater.xml" ).arg( QDir::currentPath() );

    qsFileName.replace("\\","/");
    qsFileName.replace("//","/");

    QFile        qfFile( qsFileName );
    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;

    qfFile.seek( 0 );
    if( !obProcessDoc->setContent( &qfFile, &qsErrorMsg, &inErrorLine ) )
    {
        _logProcess( tr( "Error occured during parsing file:\n'%1'\n\nError in line %2: %3" )
                     .arg( qsFileName )
                     .arg( inErrorLine )
                     .arg( qsErrorMsg ) );
        qfFile.close();
        QMessageBox::warning( this, tr("Warning"),
                              tr("Unable to copy updater files.\n"
                                 "Please check the error report file:\n\n"
                                 "%1")
                              .arg( m_qsErrorReportFile ) );
        return false;
    }
    qfFile.close();

    QDomElement      docRoot    = obProcessDoc->documentElement();
    QDomNodeList     obFiles    = docRoot.elementsByTagName( "files" )
                                         .at( 0 ).toElement().elementsByTagName( "file" );

    for( int i=0; i<obFiles.count(); i++ )
    {
        QString qsSrc   = obFiles.at(i).toElement().attribute("src");
        QString qsDst   = obFiles.at(i).toElement().attribute("dst");

        qsSrc.replace( "%INSTALL_DIR%", ui->ledDirectoryStartup->text() );
        qsSrc.replace( "%DOWNLOAD_DIR%", ui->ledDirectoryResource->toolTip() );
        qsSrc.replace( "%BACKUP_DIR%", ui->ledDirectoryBackup->toolTip() );
        qsSrc.replace( "%CURRENT_DIR%", QDir::currentPath() );
        qsSrc.replace("\\","/");
        qsSrc.replace("//","/");

        qsDst.replace( "%INSTALL_DIR%", ui->ledDirectoryStartup->text() );
        qsDst.replace( "%DOWNLOAD_DIR%", ui->ledDirectoryResource->toolTip() );
        qsDst.replace( "%BACKUP_DIR%", ui->ledDirectoryBackup->toolTip() );
        qsDst.replace( "%CURRENT_DIR%", QDir::currentPath() );
        qsDst.replace("\\","/");
        qsDst.replace("//","/");

        if( !_copyFile( qsSrc, qsDst ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("Unable to copy updater files.\n"
                                     "Please check the error report file:\n\n"
                                     "%1")
                                  .arg( m_qsErrorReportFile ) );
            return false;
        }
        _progressStep();
    }

    return true;
}
//=================================================================================================
// _copyXmlFile
//-------------------------------------------------------------------------------------------------
bool MainWindow::_copyXmlFile()
{
    QString qsSrc   = QString( "%1/settings/%2" ).arg( QDir::currentPath() ).arg( m_qsInfoFile );
    QString qsDst   = QString( "%1/%2" ).arg( ui->ledDirectoryResource->toolTip() ).arg( m_qsInfoFile );

    qsSrc.replace("\\","/");
    qsSrc.replace("//","/");
    qsDst.replace("\\","/");
    qsDst.replace("//","/");

    if( !_copyFile( qsSrc, qsDst ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Unable to copy updater file.\n"
                                 "Please check the error report file:\n\n"
                                 "%1")
                              .arg( m_qsErrorReportFile ) );
        return false;
    }
    _progressStep();

    return true;
}
//=================================================================================================
// _executeUpdater
//-------------------------------------------------------------------------------------------------
/*void MainWindow::_executeInstaller()
{
    QProcess *qpProcess  = new QProcess(this);
    QString   qsProcess  = QString( "%1/Setup.exe" ).arg( QDir::currentPath() );
    QString   qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

    qsProcess.replace("\\","/");
    qsProcess.replace("//","/");

    qsProcess.append( " -silent" );
    qsProcess.append( QString( " -device:%1" ).arg( ui->ledPanelCount->text().toInt() ) );
    qsProcess.append( QString( " -com:%1" ).arg( ui->spinCombo->text().replace("COM","") ) );
    qsProcess.append( QString( " -langi:%1" ).arg( m_qsLangInstaller ) );
    qsProcess.append( QString( " -langa:%1" ).arg( qsLanguage ) );
    qsProcess.append( QString( " -dir:\"%1\"" ).arg( ui->ledDirectoryTarget->text() ) );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }
    delete qpProcess;
}*/
//=================================================================================================
// _executeUpdater
//-------------------------------------------------------------------------------------------------
void MainWindow::_executeUpdater()
{
    QProcess *qpProcess = new QProcess(this);
    QString   qsProcess = QString( "%1/BelenusUpdate.exe" ).arg( ui->ledDirectoryStartup->text() );

    qsProcess.replace("\\","/");
    qsProcess.replace("//","/");

    QDir::setCurrent( ui->ledDirectoryStartup->text() );

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }
    delete qpProcess;
}
//=================================================================================================
// _executeWampUninstall
//-------------------------------------------------------------------------------------------------
void MainWindow::_executeWampUninstall()
{
    QProcess *qpProcess = new QProcess(this);
    QString   qsProcess = QString( "c:\\wamp\\unins000.exe /SILENT" );

    qsProcess.replace("\\","/");
    qsProcess.replace("//","/");

    if( !qpProcess->startDetached( qsProcess ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsProcess )
                              .arg( qpProcess->error() ) );
    }
    delete qpProcess;
}
//=================================================================================================
// _copyFile
//-------------------------------------------------------------------------------------------------
bool MainWindow::_copyFile( QString p_qsSrc, QString p_qsDst )
{
    if( QFile::exists(p_qsDst) )
    {
        QFile::remove(p_qsDst);
    }

    QDir        qdDst;
    QFileInfo   qfiDst( p_qsDst );

    qdDst.setPath( qfiDst.absolutePath() );
    if( !qdDst.exists() )
    {
        qdDst.mkpath( qfiDst.absolutePath() );
    }

    if( !QFile::copy( p_qsSrc, p_qsDst ) )
    {
        _logProcess( QString("Unable to copy file ...\n\nSource: %1\nDestination: %2").arg( p_qsSrc ).arg( p_qsDst ) );
        return false;
    }

    return true;
}
//=================================================================================================
// _progressStep
//-------------------------------------------------------------------------------------------------
void MainWindow::_progressStep()
//-------------------------------------------------------------------------------------------------
{
    if( ui->progressBar->value() == ui->progressBar->maximum() )
        return;

    ui->progressBar->setValue( ui->progressBar->value()+1 );
}
//=================================================================================================
// _logProcess
//-------------------------------------------------------------------------------------------------
void MainWindow::_logProcess( QString p_qsLog, bool p_bInsertNewLine )
//-------------------------------------------------------------------------------------------------
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
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
bool MainWindow::_emptyTargetDirectory( QString p_qsPath )
//-------------------------------------------------------------------------------------------------
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
                    _progressStep();
                }
            }
            else
            {
                qdTarget.remove( QString("%1\\%2").arg(p_qsPath).arg(qstFiles.at(nCount)) );
                _progressStep();
            }
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
//=================================================================================================
// _deleteRegistryKey
//-------------------------------------------------------------------------------------------------
void MainWindow::_deleteRegistryKey(QString p_qsRegPath, QString p_qsKey)
{
    QSettings   obReg( p_qsRegPath, QSettings::NativeFormat );

    if( p_qsKey.length() > 0 )
    {
        obReg.remove( p_qsKey );
    }
    else
    {
        obReg.clear();
    }
}
void MainWindow::_removeShortcuts()
{
    QSettings   obRegShell( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );

    QString qsPathPrograms    = obRegShell.value( "Common Programs", "" ).toString();
    QString qsPathDesktop     = obRegShell.value( "Common Desktop", "" ).toString();

    QFile obFile;

    _emptyTargetDirectory( QString("%1\\Belenus").arg(qsPathPrograms) );
    _removeDirectory( QString("%1\\Belenus").arg(qsPathPrograms) );

/*    obFile.remove( QString("%1\\Belenus\\belenus.lnk").arg(qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\ReportViewer.lnk").arg(qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\Advertisement.lnk").arg(qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\DBBackup.lnk").arg(qsPathPrograms) );*/

    obFile.remove( QString("%1\\belenus.lnk").arg(qsPathDesktop) );
    obFile.remove( QString("%1\\ReportViewer.lnk").arg(qsPathDesktop) );
    obFile.remove( QString("%1\\Advertisement.lnk").arg(qsPathDesktop) );
    obFile.remove( QString("%1\\DBBackup.lnk").arg(qsPathDesktop) );
}
void MainWindow::_removeDirectory( QString p_qsPath )
{
    QDir qdTarget( p_qsPath );

    qdTarget.rmpath( p_qsPath );
}


bool MainWindow::_installWampServer()
{
    bool bRet = false;

    QString qsMessage;

    _logProcess( "Install wamp server" );

    if( _processWampServerInstall(&qsMessage) )
    {
        _logProcess( QString(" SUCCEEDED") );

        QMessageBox::information( this, tr("Attention"),
                                  tr("Please make sure the WampServer icon appeared on taskbar\n"
                                     "and it's color is green.\n"
                                     "This grants that the database server is up and running\n"
                                     "and the installer is ready to continue it's process\n\n"
                                     "Click OK button to continue.") );

        _logProcess( QString("Register sql server service") );
        g_obReg.setKeyValueN( QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\wampmysqld"), QString("Start"), 2 );
        bRet = true;
    }
    else
    {
        _logProcess( QString(" FAILED") );
        QMessageBox::warning( this, tr("Attention"),
                              tr("Error occured during installation.\n\n%1\n\n"
                                 "If installation continuously fails please\ncontact Belenus software support.").arg(qsMessage) );
    }

    _progressStep();
    _logProcess( QString("Wamp install finished") );

    return bRet;
}

bool MainWindow::_processWampServerInstall( QString *p_qsMessage )
{
    bool    bRet            = false;
    QString qsProcessPath   = "";
    QString qsRedistPack    = "";
    QString qsWampServer    = "";
    int     nRet            = 0;

    qsProcessPath = g_qsCurrentPath;

    qsProcessPath.append( "\\wampserver" );
    qsRedistPack = "vcredist_x86.exe";
    qsWampServer = "wampserver2.2e_win32.exe";

    _logProcess( QString("Check Wamp install in registry") );

    nRet = _checkWampServer();

    _progressStep();

    if( nRet == 1 )
    {
        _logProcess( QString("Execute %1\\%2").arg(qsProcessPath).arg(qsRedistPack) );

        hide();

        QProcess *qpRedist = new QProcess();
        if( qpRedist->execute( QString("%1\\%2").arg(qsProcessPath).arg(qsRedistPack) ) )
            nRet = 3;
        delete qpRedist;

        _progressStep();
        _logProcess( QString("Execute %1\\%2").arg(qsProcessPath).arg(qsWampServer) );

        QProcess *qpWamp = new QProcess();
        if( qpWamp->execute( QString("%1\\%2 /SILENT ").arg(qsProcessPath).arg(qsWampServer) ) )
            nRet = 4;
        delete qpWamp;

        _progressStep();
        _logProcess( QString("Execute C:\\wamp\\wampmanager.exe") );

        QProcess *qpWampServer = new QProcess();
        if( qpWampServer->startDetached( "C:\\wamp\\wampmanager.exe" ) )
            nRet = 5;
        delete qpWampServer;

        _progressStep();
        _logProcess( QString("C:\\wamp\\wampmanager.exe started") );

        _logProcess( QString("Continue ...") );

        show();
        QApplication::processEvents();

        nRet = _checkWampServer();
    }

    switch( nRet )
    {
        case 0:
        {
            _logProcess( "Wamp Server installed and registered into registry" );
            bRet = true;
            break;
        }
        case 1:
        {
            _logProcess( "Wamp Server application is not registered in Windows registry" );
            *p_qsMessage = QString( "Wamp Server application is not registered in Windows registry" );
            break;
        }
        case 2:
        {
            _logProcess( "Wamp version not match." );
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
            _logProcess( "Installing Microsoft Redistributeable Package failed." );
            *p_qsMessage = QString( "Installing Microsoft Redistributeable Package failed." );
            break;
        }
        case 4:
        {
            _logProcess( "Installing Wamp Server application failed." );
            *p_qsMessage = QString( "Installing Wamp Server application failed." );
            break;
        }
        case 5:
        {
            _logProcess( "Starting Wamp Server application failed." );
            *p_qsMessage = QString( "Starting Wamp Server application failed." );
            break;
        }
        default:
        {
            _logProcess( "Unexpected error occured" );
            *p_qsMessage = QString( "Unexpected error occured." );
            break;
        }
    }

    return bRet;
}

int MainWindow::_checkWampServer()
{
    int nRet = 0;

    if( g_obReg.isRegPathExists( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1" ) )
    {
        QString qsVersion = g_obReg.keyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WampServer 2_is1",
                                              "Inno Setup: Setup Version",
                                      "" );
        if( qsVersion.length() == 0 )
        {
            _logProcess( "Wamp uninstall key not found in registry" );
            nRet = 1;
        }
        else
        {
            _logProcess( "Wamp uninstall key found in registry" );
            _logProcess( QString( "Inno Setup Setup Version is: [%1]" ).arg( qsVersion ) );
            if( qsVersion.compare( "5.4.0 (a)" ) != 0 )
            {
                nRet = 2;
            }
        }
    }
    else
    {
        _logProcess( "Wamp uninstall key not found in registry" );
        nRet = 1;
    }

    return nRet;
}

bool MainWindow::_processDatabaseInstall()
{
    if( !m_bRootUserExists )
    {
        _logProcess( QString("Creating root user ..."), false );
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
    _progressStep();

    _logProcess( QString("Creating database ..."), false );
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
    _progressStep();

    _logProcess( QString("Creating Belenus user ..."), false );
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
    _progressStep();

    _logProcess( QString("Granting privileges for Belenus user ..."), false );
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
    _progressStep();

    m_bBelenusUserExists = true;

    _logProcess( QString("Creating tables in database ..."), false );
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
    _progressStep();

    _logProcess( QString("Adding default data to tables ..."), false );
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
    _progressStep();

    _logProcess( QString("Add %1 panel to database ...").arg( ui->ledPanelCount->text().toInt() ), false );
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
    _progressStep();

    _logProcess( QString("Database creation SUCCEEDED") );
    m_bDatabaseAlreadyInstalled = true;

    return true;
}

void MainWindow::_checkInstallComponents()
{
    _logProcess( "Check install components" );

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
}

bool MainWindow::_processRootCreate()
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

bool MainWindow::_processDatabaseCreate()
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

bool MainWindow::_processBelenusUserCreate()
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
        m_poDB->close();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

bool MainWindow::_processBelenusUserRights()
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

bool MainWindow::_processBelenusTablesCreate()
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
                qsSQLCommand = "";
                _progressStep();
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

bool MainWindow::_processBelenusTablesFill()
{
    _logProcess( QString("_processBelenusTablesFill()") );

    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        QFile file( QString("%1/sql/db_fill_%2.sql").arg(g_qsCurrentPath).arg(m_qsLanguage) );
        _logProcess( QString("%1/sql/db_fill_%2.sql").arg(g_qsCurrentPath).arg(m_qsLanguage) );

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
                    _logProcess( QString("Unable to execute command: %1").arg( qsSQLCommand ) );
                    _logProcess( sqlQuery.lastError().text() );
                    bRet = false;
                }
                //Sleep(50);
                qsSQLCommand = "";
                _progressStep();
            }
        }
        file.close();

        m_poDB->close();
    }
    else
    {
        bRet = false;
        _logProcess( QString("Unable to connect to belenus database") );
    }

    return bRet;
}

bool MainWindow::_processBelenusDeviceFill()
{
    bool        bRet = true;

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        m_poDB->exec( QString("DELETE FROM `panels` WHERE `panelTypeId`>0") );

        for( int i=0; i<ui->ledPanelCount->text().toInt(); i++ )
        {
            QString qsSQL = QString("INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES"
                                    "( 1, 1, \"%1 %2\", 0, 0, 1, \"ARC\" )").arg(tr("KiwiSun device")).arg(i+1);
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

bool MainWindow::_processClientInstall()
{
    bool    bRet = false;

    m_qsClientInstallDir = ui->ledDirectoryTarget->text().replace("\"","");

    QDir    qdInstallDir( m_qsClientInstallDir );

    _logProcess( QString("Start Client install") );
    if( qdInstallDir.exists() )
    {
        if( !_emptyTargetDirectory( m_qsClientInstallDir ) )
        {
            _logProcess( QString("Belenus target directory exists. Empty directory failed.") );
            QMessageBox::information( this, tr("Attention"),
                                      tr("Unable to empty the specified directory.\n"
                                         "%1\n"
                                         "Please manually delete the directory if copying new files fails.") );
        }
    }

    _logProcess( QString("Copying files from install.xml ..."), false );
    if( (bRet = _copyInstallFiles( QString("%1/settings/install.xml").arg(g_qsCurrentPath) )) )
    {
        _logProcess( QString("OK") );
    }
    else
    {
        _logProcess( QString("FAILED") );
    }

    _logProcess( QString("Creating shortcuts ..."), false );
    if( (bRet = _createFolderShortcut()) )
        _logProcess( QString("OK") );
    else
        _logProcess( QString("FAIL") );
    _progressStep();

    _logProcess( QString("Update client language select file ..."), false );
    if( (bRet = _createClientLanguageSelectFile()) )
        _logProcess( QString("OK") );
    _progressStep();

    _logProcess( QString("Client install successfully finished") );

    return bRet;
}

bool MainWindow::_copyInstallFiles( QString p_qsFileName, bool /*p_bInstall*/ )
{
    bool             bRet           = true;
    QDomDocument    *obProcessDoc   = new QDomDocument( "StartupProcess" );
    QString          qsFileName     = p_qsFileName;

    qsFileName.replace("\\","/");
    qsFileName.replace("//","/");

    QFile        qfFile( qsFileName );
    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;

    qfFile.seek( 0 );
    if( !obProcessDoc->setContent( &qfFile, &qsErrorMsg, &inErrorLine ) )
    {
        _logProcess( tr( "Error occured during parsing file:\n'%1'\n\nError in line %2: %3" )
                     .arg( qsFileName )
                     .arg( inErrorLine )
                     .arg( qsErrorMsg ) );
        qfFile.close();
        return false;
    }
    qfFile.close();

    QDomElement      docRoot    = obProcessDoc->documentElement();
    QDomNodeList     obFiles    = docRoot.elementsByTagName( "files" )
                                         .at( 0 ).toElement().elementsByTagName( "file" );

    for( int i=0; i<obFiles.count(); i++ )
    {
        QString qsSrc   = obFiles.at(i).toElement().attribute("src");
        QString qsDst   = obFiles.at(i).toElement().attribute("dst");

        qsSrc.replace( "%INSTALL_DIR%", m_qsClientInstallDir );
        qsSrc.replace( "%CURRENT_DIR%", QDir::currentPath() );
        qsSrc.replace("\\","/");
        qsSrc.replace("//","/");

        qsDst.replace( "%INSTALL_DIR%", m_qsClientInstallDir );
        qsDst.replace( "%CURRENT_DIR%", QDir::currentPath() );
        qsDst.replace("\\","/");
        qsDst.replace("//","/");

        if( !_copyFile( qsSrc, qsDst ) )
        {
            bRet = false;
        }
        _progressStep();
    }

    return bRet;
}

bool MainWindow::_createFolderShortcut()
{
    bool        bRet = true;

    QSettings   obRegShell( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );

    QString qsPathPrograms    = obRegShell.value( "Common Programs", "" ).toString();
    QString qsPathDesktop     = obRegShell.value( "Common Desktop", "" ).toString();

    if( qsPathPrograms.contains( ":\\" ) )
    {
        if( !_createTargetDirectory( QString("%1\\Belenus").arg(qsPathPrograms) ) )
        {
            m_qsProcessErrorMsg = QString( "CreateClientFolderFailed" );
            return false;
        }

        m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Belenus\\belenus.lnk").arg(qsPathPrograms) );
        m_obFile->link( QString("%1\\Belenus\\belenus.lnk").arg(qsPathPrograms) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\ReportViewer.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Belenus\\ReportViewer.lnk").arg(qsPathPrograms) );
        m_obFile->link( QString("%1\\Belenus\\ReportViewer.lnk").arg(qsPathPrograms) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\Advertisement.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Belenus\\Advertisement.lnk").arg(qsPathPrograms) );
        m_obFile->link( QString("%1\\Belenus\\Advertisement.lnk").arg(qsPathPrograms) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\DBBackup.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Belenus\\DBBackup.lnk").arg(qsPathPrograms) );
        m_obFile->link( QString("%1\\Belenus\\DBBackup.lnk").arg(qsPathPrograms) );
        delete m_obFile;
    }

    if( qsPathDesktop.contains( ":\\" ) )
    {
        m_obFile = new QFile( QString("%1\\belenus.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\belenus.lnk").arg(qsPathDesktop) );
        m_obFile->link( QString("%1\\belenus.lnk").arg(qsPathDesktop) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\ReportViewer.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\ReportViewer.lnk").arg(qsPathDesktop) );
        m_obFile->link( QString("%1\\ReportViewer.lnk").arg(qsPathDesktop) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\Advertisement.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\Advertisement.lnk").arg(qsPathDesktop) );
        m_obFile->link( QString("%1\\Advertisement.lnk").arg(qsPathDesktop) );
        delete m_obFile;
        m_obFile = new QFile( QString("%1\\DBBackup.exe").arg(m_qsClientInstallDir) );
        m_obFile->remove( QString("%1\\DBBackup.lnk").arg(qsPathDesktop) );
        m_obFile->link( QString("%1\\DBBackup.lnk").arg(qsPathDesktop) );
        delete m_obFile;
    }

    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("DisplayIcon"), QString("%1\\resources\\belenus.ico").arg(m_qsClientInstallDir) );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("DisplayName"), tr("Belenus Application System") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("InstallLocation"), m_qsClientInstallDir );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("Publisher"), QString("Pagony Multimédia Stúdió Bt.") );
    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus", QString("URLInfoAbout"), QString("http://belenus.pagonymedia.hu") );

    g_obReg.setKeyValueS( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QString("BelenusWebSync"), QString("%1\\websync.exe").arg(m_qsClientInstallDir) );

    return bRet;
}

bool MainWindow::_createTargetDirectory( QString p_qsPath )
{
    QDir    qdInstallDir( p_qsPath );

    if( !qdInstallDir.mkpath( p_qsPath ) )
    {
        QMessageBox::critical( this, tr("System error"),
                               tr("Unable to create directory:\n\n%1").arg(p_qsPath));
        return false;
    }

    return true;
}

bool MainWindow::_createClientLanguageSelectFile()
{
    QString     qsFile = QString( "%1\\lang\\languages.inf" ).arg( m_qsClientInstallDir );
    QFile       qfFile( qsFile );

    if( !qfFile.exists() )
    {
        _logProcess( QString("FAIL") );
        _logProcess( QString("Language file [%1] missing").arg( qsFile ) );
        return false;
    }

    if( !qfFile.open(QIODevice::ReadOnly) )
    {
        _logProcess( QString("FAIL") );
        _logProcess( QString("Unable to read [%1] file").arg( qsFile ) );
        return false;
    }

    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;

    qfFile.seek( 0 );
    if( !obProcessDoc->setContent( &qfFile, &qsErrorMsg, &inErrorLine ) )
    {
        _logProcess( QString("FAIL") );
        _logProcess( QString("Parsing file [%1] failed").arg( qsFile ) );
        qfFile.close();
        return false;
    }
    qfFile.close();

    QDomElement     docRoot     = obProcessDoc->documentElement();
    QDomNodeList    obLanguage  = docRoot.elementsByTagName( "language" );;

    if( !qfFile.open(QIODevice::WriteOnly) )
    {
        _logProcess( QString("FAIL") );
        _logProcess( QString("Unable to write [%1] file").arg( qsFile ) );
        return false;
    }

    qfFile.write( "<languages>\n\r\t<!--\n\r\t<language name=\"\" shortname=\"\" current=\"no\" />\n\r\t-->\n\r\n\r\n\r" );

    for( int i=0; i<obLanguage.count(); i++ )
    {
        QString qsName  = obLanguage.at(i).toElement().attribute("name");
        QString qsShort = obLanguage.at(i).toElement().attribute("shortname");
        QString qsCurr  = "no";

        if( m_qsLanguage.compare( qsShort ) == 0 )
        {
            qsCurr = "yes";
        }

        qfFile.write( QString( "\t<language name=\"%1\" shortname=\"%2\" current=\"%3\" />\n\r" )
                                .arg( qsName )
                                .arg( qsShort )
                                .arg( qsCurr )
                                .toStdString().c_str() );
    }
    qfFile.write( "\n\r" );
    qfFile.write( "</languages>\n\r" );
    qfFile.close();

    return true;
}
