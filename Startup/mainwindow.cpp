
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
void MainWindow::on_cmbLanguage_currentIndexChanged(int /*index*/)
{
    QString qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

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

    ui->retranslateUi( this );
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
//
//-------------------------------------------------------------------------------------------------
void MainWindow::on_process_selected()
{
    if( ui->rbProcessInstall->isChecked() )
    {
        m_nProcessType = PROCESS_INSTALL;
    }
    else if( ui->rbProcessRemove->isChecked() )
    {
        m_nProcessType = PROCESS_REMOVE;
    }
    else if( ui->rbProcessUpdate->isChecked() )
    {
        m_nProcessType = PROCESS_UPDATE;
    }
    else
    {
        return;
    }

    ui->pbStart->setEnabled( true );
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
            if( !_createPaths() ) { return; }
            ui->progressBar->setValue( 1 );

            if( !_createSettingsFile() ) { return; }
            ui->progressBar->setValue( 2 );

            if( !_copyUpdaterFiles() ) { return; }

            if( ui->rbLocationLocal->isChecked() )
            {
                if( !_copyXmlFile() ) { return; }
            }

            _executeUpdater();
            break;
        }
        case PROCESS_REMOVE:
        {
            break;
        }
        case PROCESS_UPDATE:
        {
            if( !_createPaths() ) { return; }
            ui->progressBar->setValue( 1 );

            if( !_updateSettingsFile() ) { return; }
            ui->progressBar->setValue( 2 );

            if( !_copyUpdaterFiles() ) { return; }

            if( ui->rbLocationLocal->isChecked() )
            {
                if( !_copyXmlFile() ) { return; }
            }

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

    QString qsVersion   = obMasterCD.value( "Version", "1.0.0" ).toString();
    QString qsSettings  = QString( "%1/settings.ini" ).arg( ui->ledDirectoryStartup->text() );
    QString qsAddress   = "";
    QString qsInfoFile  = "";
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

    qsInfoFile = QString( "belenus_%1.xml" ).arg( qsLocation );

    obPrefFile.setValue( QString::fromAscii( "Language/Path" ), "lang" );
    obPrefFile.setValue( QString::fromAscii( "Language/Extension" ), qsLanguage );

    obPrefFile.setValue( QString::fromAscii( "Settings/WindowWidth" ), "640" );
    obPrefFile.setValue( QString::fromAscii( "Settings/WindowHeight" ), "400" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Background" ), "resources/updater.png" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Textcolor" ), "DDDDDD" );
    obPrefFile.setValue( QString::fromAscii( "Settings/Timer" ), "250" );

    obPrefFile.setValue( QString::fromAscii( "PreProcess/Version" ), qsVersion );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/Address" ), qsAddress );
    obPrefFile.setValue( QString::fromAscii( "PreProcess/InfoFile" ), qsInfoFile );
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

    m_qsInfoFile = QString( "belenus_%1_%2.xml" ).arg( qsLocation ).arg( qsLanguage );

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
    QString qsSrc   = QString( "%1/%2" ).arg( QDir::currentPath() ).arg( m_qsInfoFile );
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
void MainWindow::_executeUpdater()
{
    QProcess *qpProcess = new QProcess(this);
    QString   qsUpdater = QString( "%1/BelenusUpdate.exe" ).arg( ui->ledDirectoryTarget->text() );

    qsUpdater.replace("\\","/");
    qsUpdater.replace("//","/");

    if( !qpProcess->startDetached( qsUpdater ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.")
                              .arg( qsUpdater )
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
        _logProcess( tr("Unable to copy file ...\n\nSource: %1\nDestination: %2").arg( p_qsSrc ).arg( p_qsDst ) );
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

