
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, teAction p_teAction, QString p_qsFileName) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //---------------------------------------------------------------
    // Initialize the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setControlsEnabled( false );

    //---------------------------------------------------------------
    // Initialize the variables
    m_nTimer        = 0;
    m_teAction      = p_teAction;
    m_qsFileName    = p_qsFileName;

    switch( m_teAction )
    {
        case ACT_RESTORE:
        {
            ui->lblCaption->setText( tr("Restore database") );
            ui->lblInfo->setText( tr("Please select desired database and click on Start") );
            setControlsEnabled( true );
            break;
        }
        case ACT_EXECUTE:
        {
            ui->lblCaption->setText( tr("Update database") );
            m_nTimer = startTimer( 1000 );
            break;
        }
        default:
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
    m_nTimer = 0;

    switch( m_teAction )
    {
        case ACT_BACKUP:
        {
            processBackup();
            break;
        }

        case ACT_RESTORE:
        {
            processRestore();
            break;
        }

        case ACT_EXECUTE:
        {
            processExecute();
            break;
        }

        default:
            close();
    }
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processBackup()
//-------------------------------------------------------------------------------------------------
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsBackupPath    = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysqldump.exe\"" ).arg(qsMysqlPath);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > \"%1\\belenus_backup_%2.sql\" ").arg(qsBackupPath).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsParameters );

    QDir    qdBackup( qsBackupPath );

    if( !qdBackup.exists() )
    {
        qdBackup.mkpath( qsBackupPath );
    }

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

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processRestore()
//-------------------------------------------------------------------------------------------------
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(qsMysqlPath);
    QString     qsCommand;
    QString     qsDBRecreate    = QString( "dbrecreate.sql" );
    QString     qsImport        = QString( " \"%1\" ").arg(ui->ledDatabase->toolTip());

    QFile qfRecreate(qsDBRecreate );

    if( qfRecreate.open( QIODevice::WriteOnly ) )
    {
        qfRecreate.write( "DROP DATABASE belenus;\nCREATE DATABASE belenus;\n" );
        qfRecreate.close();
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to create temporary file: dbrecreate.sql") );
        setControlsEnabled( true );
        return;
    }

    QProcess *qpRestore = new QProcess();

    qsCommand = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsDBRecreate );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }
    qfRecreate.remove();

    qsCommand = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsImport );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }

    delete qpRestore;

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processExecute()
//-------------------------------------------------------------------------------------------------
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(qsMysqlPath);
    QString     qsExecute       = QString( " \"%1\" ").arg(m_qsFileName);
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsExecute );

    QProcess *qpExecute = new QProcess();

    if( qpExecute->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpExecute;
        setControlsEnabled( true );
        return;
    }

    delete qpExecute;

    m_teAction = ACT_FINISHED;
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
    QFile   fileDb( ui->ledDatabase->toolTip() );

    if( fileDb.exists() )
    {
        setControlsEnabled( false );
        ui->lblInfo->setText( tr("Please wait until the process finish.") );
        m_nTimer = startTimer( 1000 );
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("The selected file not exists or not readable\n\n%1").arg( ui->ledDatabase->toolTip() ) );
        setControlsEnabled( true );
        return;
    }
}

void MainWindow::setControlsEnabled(bool p_bEnable)
{
    ui->ledDatabase->setVisible( p_bEnable );
    ui->ledDatabase->setEnabled( p_bEnable );
    ui->pbSelect->setVisible( p_bEnable );
    ui->pbSelect->setEnabled( p_bEnable );
    ui->pbStart->setVisible( p_bEnable );
    ui->pbStart->setEnabled( p_bEnable );
}
