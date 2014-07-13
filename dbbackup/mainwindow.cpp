
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, bool p_bDbRestore) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //---------------------------------------------------------------
    // Initialize the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->ledDatabase->setVisible( false );
    ui->ledDatabase->setEnabled( false );
    ui->pbSelect->setVisible( false );
    ui->pbSelect->setEnabled( false );
    ui->pbStart->setVisible( false );
    ui->pbStart->setEnabled( false );

    //---------------------------------------------------------------
    // Initialize the variables
    m_nTimer            = 0;
    m_bProcessFinished  = false;

    if( p_bDbRestore )
    {
        ui->lblCaption->setText( tr("Restore database") );
        ui->lblInfo->setText( tr("Please select desired database and click on Start") );

        ui->ledDatabase->setVisible( true );
        ui->ledDatabase->setEnabled( true );
        ui->pbSelect->setVisible( true );
        ui->pbSelect->setEnabled( true );
        ui->pbStart->setVisible( true );
        ui->pbStart->setEnabled( true );
    }
    else
    {
        //---------------------------------------------------------------
        // Start the application process with the timer
        m_nTimer = startTimer( 1000 );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    killTimer( m_nTimer );

    if( m_bProcessFinished )
    {
        close();
    }
    else
    {
        processMain();
    }
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processMain()
//-------------------------------------------------------------------------------------------------
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsBackupPath    = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ), "" ).toString();
    QString     qsProcess       = QString("%1/mysqldump.exe").arg(qsMysqlPath);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > \"%1\\belenus_backup_%2.sql\" ").arg(qsBackupPath).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsParameters );

    if( qsMysqlPath.length() > 0 )
    {
        QProcess *qpBackup = new QProcess();

        if( qpBackup->execute( qsCommand ) )
        {
            QMessageBox::warning( this, tr("Error"),
                                  tr("Unable to execute the following command:\n"
                                     "%1").arg(qsCommand) );
        }

        delete qpBackup;
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Location of the MySQL archive application is not set.\n"
                                 "Please check your settings.") );
    }

    m_bProcessFinished = true;
    m_nTimer = startTimer( 500 );
}

void MainWindow::on_pbExit_clicked()
{
    close();
}

void MainWindow::on_pbSelect_clicked()
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsBackupPath = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ), "" ).toString();
    QString     qsFile = "";

    if( qsBackupPath.length() == 0 )    qsBackupPath = QDir::currentPath();

    QFileDialog dlgFileOpen( this );

    dlgFileOpen.setDirectory( qsBackupPath );
    dlgFileOpen.setFileMode( QFileDialog::ExistingFile );
    dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
    dlgFileOpen.setViewMode( QFileDialog::Detail );

    if( dlgFileOpen.exec() )
    {
        QString qsDir  = dlgFileOpen.directory().absolutePath();
        qsDir.replace( '/', '\\' );
        if( qsDir.right(1).compare("\\") == 0 )
        {
            qsDir = qsDir.left(qsDir.length()-1);
        }
        qsFile = dlgFileOpen.selectedFiles().at(0).right( dlgFileOpen.selectedFiles().at(0).length()-qsDir.length()-1 );

        ui->ledDatabase->setText( qsFile );
        ui->ledDatabase->setToolTip( QString("%1\\%2").arg( qsDir ).arg( qsFile ) );
    }
}

void MainWindow::on_pbStart_clicked()
{
    QFile   fileDb( ui->ledDatabase->text() );

    if( fileDb.exists() )
    {
        QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
        QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
        QString     qsProcess       = QString("%1/mysql.exe").arg(qsMysqlPath);
        QString     qsParameters    = QString( "-u belenus -pbelenus belenus < \"%1\" ").arg(ui->ledDatabase->toolTip());

    }
}
