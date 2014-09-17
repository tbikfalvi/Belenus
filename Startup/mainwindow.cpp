
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize GUI components
    ui->cmbLanguage->addItem( "Magyar (hu)" );
    ui->cmbLanguage->addItem( "English (en)" );
    ui->cmbLanguage->addItem( "Deutsch (de)" );
    ui->cmbLanguage->addItem( "Slovensky (sk)" );

    ui->ledDirectoryResource->setToolTip( "c:/BelenusUpdate/Download" );
    ui->ledDirectoryBackup->setToolTip( "c:/BelenusUpdate/Backup" );

    connect( ui->rbProcessInstall, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
    connect( ui->rbProcessUpdate, SIGNAL(clicked()), this, SLOT(on_process_selected()) );
    connect( ui->rbProcessRemove, SIGNAL(clicked()), this, SLOT(on_process_selected()) );

    ui->pbStart->setEnabled( false );
    ui->progressBar->setVisible( false );
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::on_pbDefault_clicked()
{
    ui->ledDirectoryStartup->setText( "c:/BelenusUpdate" );
    ui->ledDirectoryTarget->setText( "c:/Program Files/Belenus" );
    ui->ledDirectoryResource->setText( "Download" );
    ui->ledDirectoryBackup->setText( "Backup" );

    ui->ledDirectoryResource->setToolTip( "c:/BelenusUpdate/Download" );
    ui->ledDirectoryBackup->setToolTip( "c:/BelenusUpdate/Backup" );
}

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
    }
}

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

void MainWindow::on_ledDirectoryBackup_editingFinished()
{
    if( ui->ledDirectoryBackup->text().mid(1,1).compare(":") == 0 )
    {
        ui->ledDirectoryBackup->setToolTip( ui->ledDirectoryBackup->text() );
    }
    else
    {
        QString qsTemp = QString( "%1/%2" ).arg( ui->ledDirectoryBackup->text() )
                                           .arg( ui->ledDirectoryBackup->text() );
        qsTemp.replace( "\\", "/" );
        qsTemp.replace( "//", "/" );

        ui->ledDirectoryBackup->setToolTip( qsTemp );
    }
}

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

void MainWindow::on_process_selected()
{
    ui->pbStart->setEnabled( true );
}

void MainWindow::on_pbStart_clicked()
{
    ui->pbStart->setVisible( false );
    ui->progressBar->setVisible( true );

    if( _createPath( ui->ledDirectoryStartup->text() ) &&
        _createPath( ui->ledDirectoryTarget->text() ) &&
        _createPath( ui->ledDirectoryResource->toolTip() ) &&
        _createPath( ui->ledDirectoryBackup->toolTip() ) )
    {
        ui->progressBar->setValue( 4 );
        _createSettingsFile();
        ui->progressBar->setValue( 5 );
        //close();
    }
}

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

bool MainWindow::_createSettingsFile()
{
    QString qsSettings = QString( "%1/settings.ini" ).arg( ui->ledDirectoryStartup->text() );

    qsSettings.replace( "\\", "/" );
    qsSettings.replace( "//", "/" );

    QSettings obPrefFile( qsSettings, QSettings::IniFormat );

    QString qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

    obPrefFile.setValue( QString::fromAscii( "Language/Path" ), "lang" );
    obPrefFile.setValue( QString::fromAscii( "Language/Extension" ), qsLanguage );

    return true;
}
