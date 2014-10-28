
#include <QProcessEnvironment>
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

//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_pbLangEn_clicked();

    // Initialize GUI components
    ui->cmbLanguage->addItem( "Magyar (hu)" );
    ui->cmbLanguage->addItem( "English (en)" );
    ui->cmbLanguage->addItem( "Deutsch (de)" );
//    ui->cmbLanguage->addItem( "Slovensky (sk)" );

    connect( ui->rbProcessInstall, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
    connect( ui->rbProcessUpdate, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
    connect( ui->rbProcessRemove, SIGNAL(clicked()), this, SLOT(on_process_selected()) );

    ui->pbStart->setEnabled( false );
    ui->progressBar->setVisible( false );

    QProcessEnvironment qpeInfo = QProcessEnvironment::systemEnvironment();

    ui->ledDirectoryStartup->setText( qpeInfo.value( "BelenusStartup", "C:/BelenusUpdate" ) );
    ui->ledDirectoryTarget->setText( qpeInfo.value( "BelenusTarget", "C:/Program Files/Belenus" ) );
    ui->ledDirectoryResource->setText( qpeInfo.value( "BelenusResource", "Download" ) );
    ui->ledDirectoryBackup->setText( qpeInfo.value( "BelenusBackup", "Backup" ) );

    on_ledDirectoryResource_editingFinished();
    on_ledDirectoryBackup_editingFinished();

    QSettings   belenus( QString("%1/belenus.ini").arg(ui->ledDirectoryTarget->text()), QSettings::IniFormat );

    QString qsLangApp = belenus.value( "Lang", "" ).toString();

    ui->cmbLanguage->setCurrentIndex( ui->cmbLanguage->findText( QString(" (%1)").arg(qsLangApp), Qt::MatchContains ) );

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
    delete ui;
}
//=================================================================================================
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_pbLangEn_clicked()
{
    ui->pbLangEn->setChecked( true );
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
void MainWindow::on_cmbLanguage_currentIndexChanged(int /*index*/)
{
//    QString qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);
/*
    apMainApp->removeTranslator( poTransStartup );
    apMainApp->removeTranslator( poTransQT );

    if( qsLanguage.compare("en") )
    {
        QString qsLangSetup = QString("%1\\lang\\startup_%2.qm").arg(QDir::currentPath()).arg( qsLanguage );
        QString qsLangQT = QString("%1\\lang\\qt_%2.qm").arg(QDir::currentPath()).arg( qsLanguage );

        poTransStartup->load( qsLangSetup );
        poTransQT->load( qsLangQT );

        apMainApp->installTranslator( poTransStartup );
        apMainApp->installTranslator( poTransQT );
    }

    ui->retranslateUi( this );*/
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
    else if( ui->rbProcessUpdate->isChecked() )
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
    }
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

    ui->ledDirectoryStartup->setText( qpeInfo.value( "BelenusStartup", "C:/BelenusUpdate" ) );
    ui->ledDirectoryTarget->setText( qpeInfo.value( "BelenusTarget", "C:/Program Files/Belenus" ) );
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
    _updateEnvironmentVariables();

    ui->pbStart->setVisible( false );
    ui->progressBar->setVisible( true );

    switch( m_nProcessType )
    {
        case PROCESS_INSTALL:
        {
            // Create directories for updater and for Belenus if not exists
            if( !_createPaths() ) { return; }
            ui->progressBar->setValue( 1 );

            // Create the settings file for the updater
            if( !_createSettingsFile() ) { return; }
            ui->progressBar->setValue( 2 );

            // Copy the updater files
            if( !_copyUpdaterFiles() ) { return; }

            // Copy the belenus_loc.xml file if needed
            if( ui->rbLocationLocal->isChecked() )
            {
                if( !_copyXmlFile() ) { return; }
            }

            ui->progressBar->setValue( ui->progressBar->maximum() );
            // Start Belenus installer
            _executeInstaller();
            break;
        }
        case PROCESS_REMOVE:
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Are you sure you want to uninstall Belenus Application System and all of it's components?\n"
                                          "All of the data will be deleted from the computer."),
                                       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            {
                ui->progressBar->setMaximum( 100 );
                ui->progressBar->setValue( 1 );
                _deleteRegistryKey( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" );
                _deleteRegistryKey( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                    "BelenusStartup" );
                _deleteRegistryKey( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                    "BelenusTarget" );
                _deleteRegistryKey( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                    "BelenusResource" );
                _deleteRegistryKey( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                    "BelenusBackup" );
                _progressStep();

                SendMessage( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment" );
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
        case PROCESS_UPDATE:
        {
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
        }
        default:
            return;
    }

    close();
}
//=================================================================================================
// _updateEnvironmentVariables
//-------------------------------------------------------------------------------------------------
void MainWindow::_updateEnvironmentVariables()
{
    QSettings obPref( "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment",
                      QSettings::NativeFormat );

    obPref.setValue( "BelenusStartup", ui->ledDirectoryStartup->text() );
    obPref.setValue( "BelenusTarget", ui->ledDirectoryTarget->text() );
    obPref.setValue( "BelenusResource", ui->ledDirectoryResource->toolTip() );
    obPref.setValue( "BelenusBackup", ui->ledDirectoryBackup->toolTip() );

    SendMessage( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment" );

    m_qsErrorReportFile = QString( "%1/error.log" ).arg( ui->ledDirectoryStartup->text() );

    m_qsErrorReportFile.replace( "\\", "/" );
    m_qsErrorReportFile.replace( "//", "/" );

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

    obPrefFile.setValue( QString::fromAscii( "Language/Path" ), "lang" );
    obPrefFile.setValue( QString::fromAscii( "Language/Extension" ), qsLanguage );

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
bool MainWindow::_updateSettingsFile()
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
/*
    QMessageBox::warning( this, tr("Warning"),
                          tr("Unable to update settings.ini on the following directory:\n\n%1\n\n"
                             "Please check your user rights on the directory and file.")
                          .arg( ui->ledDirectoryStartup->text() ) );
*/
    return true;
}
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
void MainWindow::_executeInstaller()
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

    QString m_qsPathPrograms    = obRegShell.value( "Common Programs", "" ).toString();;
    QString m_qsPathDesktop     = obRegShell.value( "Common Desktop", "" ).toString();

    QFile obFile;

    _emptyTargetDirectory( QString("%1\\Belenus").arg(m_qsPathPrograms) );
    _removeDirectory( QString("%1\\Belenus").arg(m_qsPathPrograms) );

/*    obFile.remove( QString("%1\\Belenus\\belenus.lnk").arg(m_qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\ReportViewer.lnk").arg(m_qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\Advertisement.lnk").arg(m_qsPathPrograms) );
    obFile.remove( QString("%1\\Belenus\\DBBackup.lnk").arg(m_qsPathPrograms) );*/

    obFile.remove( QString("%1\\belenus.lnk").arg(m_qsPathDesktop) );
    obFile.remove( QString("%1\\ReportViewer.lnk").arg(m_qsPathDesktop) );
    obFile.remove( QString("%1\\Advertisement.lnk").arg(m_qsPathDesktop) );
    obFile.remove( QString("%1\\DBBackup.lnk").arg(m_qsPathDesktop) );
}
void MainWindow::_removeDirectory( QString p_qsPath )
{
    QDir qdTarget( p_qsPath );

    qdTarget.rmpath( p_qsPath );
}
