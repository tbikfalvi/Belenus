
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>

#include "../framework/qtmysqlquerymodel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString p_qsVersion, teAction p_teAction, QString p_qsFileName) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //---------------------------------------------------------------
    // Initialize the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->gbVersion->setTitle( QString(" v.%1 ").arg( p_qsVersion ) );

    setControlsEnabled( false );

    //---------------------------------------------------------------
    // Initialize the variables
    m_nTimer        = 0;
    m_teAction      = p_teAction;
    m_qsFileName    = p_qsFileName;
    g_poDB          = new cQTMySQLConnection;

    // remove binary logs to save time for db backup
    try
    {
        g_poDB->executeQTQuery( QString( "PURGE BINARY LOGS BEFORE '%1'; " ).arg( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

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
            if( m_qsFileName.length() == 0 )
            {
                ui->lblInfo->setText( tr("Please select desired database and click on Start") );
                setControlsEnabled( true );
            }
            else
            {
                m_nTimer = startTimer( 1000 );
            }
            break;
        }
        default:
            m_nTimer = startTimer( 1000 );
    }
}

MainWindow::~MainWindow()
{
    delete g_poDB;

    delete ui;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    killTimer( m_nTimer );
    m_nTimer = 0;

    try
    {
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        m_qsDirDbBinaries = loadSetting( "BACKUP_DirDbBinaries", "C:/wamp/bin/mysql/mysql5.5.24/bin" );
        m_qsDirDbBackup   = loadSetting( "BACKUP_DirDbBackup", "" );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }


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
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QString     qsProcess       = QString( "\"%1/mysqldump.exe\"" ).arg(m_qsDirDbBinaries);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > \"%1/belenus_backup_%2.sql\" ").arg(m_qsDirDbBackup).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsParameters );

    QDir    qdBackup( m_qsDirDbBackup );

    if( !qdBackup.exists() )
    {
        qdBackup.mkpath( m_qsDirDbBackup );
    }

    if( m_qsDirDbBinaries.length() > 0 )
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
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(m_qsDirDbBinaries);
    QString     qsCommand;
    QString     qsImport        = QString( " \"%1\" ").arg(ui->ledDatabase->toolTip());

    //-------------------------------------------------------------------------------------
    // Recreate belenus database with temporary file 'dbrecreate.sql'
    QFile qfRecreate( "dbrecreate.sql" );

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

    qsCommand = QString( "cmd /c %1 dbrecreate.sql " ).arg( qsProcess );

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

    //-------------------------------------------------------------------------------------
    // Execute database import
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

    //-------------------------------------------------------------------------------------
    // Clear activation random code with temporary file 'dbreactivate.sql'
    QFile qfReactivate( "dbreactivate.sql" );

    if( qfReactivate.open( QIODevice::WriteOnly ) )
    {
        qfReactivate.write( "UPDATE licences SET Cod=\"\";\n" );
        qfReactivate.close();
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to create temporary file: dbreactivate.sql") );
        setControlsEnabled( true );
        return;
    }

    qsCommand = QString( "cmd /c %1 dbreactivate.sql " ).arg( qsProcess );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }
    qfReactivate.remove();

    delete qpRestore;

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processExecute()
//-------------------------------------------------------------------------------------------------
{
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(m_qsDirDbBinaries);
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
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );
    QString qsFile = "";

    if( m_qsDirDbBackup.length() == 0 )    m_qsDirDbBackup = QDir::currentPath();

    QFileDialog dlgFileOpen( this );

    dlgFileOpen.setDirectory( m_qsDirDbBackup );
    dlgFileOpen.setFileMode( QFileDialog::ExistingFile );
    dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
    dlgFileOpen.setViewMode( QFileDialog::Detail );

    if( dlgFileOpen.exec() )
    {
        QString qsDir  = dlgFileOpen.directory().absolutePath();
        qsDir.replace( '\\', '/' );
        if( qsDir.right(1).compare("/") == 0 )
        {
            qsDir = qsDir.left(qsDir.length()-1);
        }
        qsFile = dlgFileOpen.selectedFiles().at(0).right( dlgFileOpen.selectedFiles().at(0).length()-qsDir.length()-1 );

        ui->ledDatabase->setText( qsFile );
        ui->ledDatabase->setToolTip( QString("%1/%2").arg( qsDir ).arg( qsFile ) );
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

QString MainWindow::loadSetting( QString p_Identifier, QString p_Default ) throw (cSevException)
{
    QString value = "";
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"%1\" " ).arg( p_Identifier ) );
        if( poQuery->first() )
        {
            value = poQuery->value( 0 ).toString();
        }
        else
        {
            g_poDB->executeQTQuery( QString("INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2') " )
                                            .arg( p_Identifier )
                                            .arg( p_Default ) );
            value = p_Default;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return value;
}
