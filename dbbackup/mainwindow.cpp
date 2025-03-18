
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

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

    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon( QIcon( ":/logo.png" ) );
    trayIcon->setToolTip( tr("Belenus Database Manager") );
    trayIcon->show();

    setControlsEnabled( false );

    //---------------------------------------------------------------
    // Initialize the variables
    m_nTimer            = 0;
    m_teAction          = p_teAction;
    m_qsFileName        = p_qsFileName;
    m_qsDirDbBinaries   = "";
    m_qsDirDbBackup     = "";
    g_poDB              = new cQTMySQLConnection;

    // remove binary logs to save time for db backup
    try
    {
        g_poDB->executeQTQuery( QString( "PURGE BINARY LOGS BEFORE '%1'; " ).arg( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

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

    if( m_qsDirDbBinaries.isEmpty() )
    {
        // MessageBox megjelenítése
        QMessageBox::warning(this, tr("Error"), tr("The binary folder for Wampserver MySQL can not be found in database!\nPlease define the location manually!") );

        // Könyvtárválasztó dialógus megnyitása
        QString selectedDir = QFileDialog::getExistingDirectory(0,
                                                                tr( "Select the location of the MySQL binary folder" ),
                                                                "C:/",
                                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        // Ha választott a felhasználó, frissítjük a qsDir változót
        if( !selectedDir.isEmpty() )
        {
            m_qsDirDbBinaries = selectedDir;
        }
    }

    if( m_qsDirDbBackup.isEmpty() )
    {
        // MessageBox megjelenítése
        QMessageBox::warning(this, tr("Error"), tr("The backup folder for Belenus can not be found in database!\nPlease define the location manually!") );

        // Könyvtárválasztó dialógus megnyitása
        QString selectedDir = QFileDialog::getExistingDirectory(0,
                                                                tr( "Select the location of the backup folder" ),
                                                                "C:/",
                                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        // Ha választott a felhasználó, frissítjük a qsDir változót
        if( !selectedDir.isEmpty() )
        {
            m_qsDirDbBackup = selectedDir;
        }
    }

    if( m_qsDirDbBinaries.isEmpty() || m_qsDirDbBackup.isEmpty() )
    {
        QMessageBox::warning(this, tr("Error"), tr("The required folders are not set!\nExiting the application ...") );
        m_teAction = ACT_FINISHED;
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
        case ACT_UPDATE:
        {
            ui->lblCaption->setText( tr("Update database file") );
            ui->lblInfo->setText( tr("Please select desired database and click on Start") );
            setControlsEnabled( true );
            break;
        }
        default:
        {
            trayIcon->showMessage( tr("Belenus Database Manager"),
                                   tr("Saving database process started."),
                                   QSystemTrayIcon::Information, 1000 );
            m_nTimer = startTimer( 1000 );
        }
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

    switch( m_teAction )
    {
        case ACT_BACKUP:
        {
            processBackup();
            break;
        }

        case ACT_RESTORE:
        {
            trayIcon->showMessage( tr("Belenus Database Manager"),
                                   tr("Restoring database process started."),
                                   QSystemTrayIcon::Information, 1000 );
            processRestore();
            break;
        }

        case ACT_EXECUTE:
        {
            trayIcon->showMessage( tr("Belenus Database Manager"),
                                   tr("Executing database modification started."),
                                   QSystemTrayIcon::Information, 1000 );
            processExecute();
            break;
        }

        case ACT_UPDATE:
        {
            // Mysql 5.5.24 dump utáni FOREIGN KEY javitas
            trayIcon->showMessage( tr("Belenus Database Manager"),
                                   tr("Database backup file correction for forward compatibility."),
                                   QSystemTrayIcon::Information, 1000 );
            updateBackupFile();
            break;
        }

        default:
        {
            trayIcon->showMessage( tr("Belenus Database Manager"),
                                   tr("Process finished."),
                                   QSystemTrayIcon::Information, 1000 );
            close();
        }
    }
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processBackup()
//-------------------------------------------------------------------------------------------------
{
//    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );
    QString     qsCurrentDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    QString     qsProcess       = QString( "\"%1/mysqldump.exe\"" ).arg(m_qsDirDbBinaries);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > \"%1/belenus_backup_%2.sql\"" ).arg(m_qsDirDbBackup).arg( qsCurrentDateTime );
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
    m_nTimer = startTimer( 3000 );
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
        m_qsFileName = ui->ledDatabase->toolTip();
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

void MainWindow::updateBackupFile()
{
    QFile file( m_qsFileName );

    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to open the source file\n\n%1").arg( m_qsFileName ) );
        m_teAction = ACT_FINISHED;
        m_nTimer = startTimer( 500 );
        return;
    }

    QString qsUpdatedFile = QString( "belenus_backup_%1_updated.sql" ).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );

    QFile outFile( qsUpdatedFile );

    if( !outFile.open( QIODevice::WriteOnly ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to open the destination file\n\n%1").arg( qsUpdatedFile ) );
        m_teAction = ACT_FINISHED;
        m_nTimer = startTimer( 500 );
        return;
    }

    QTextStream in( &file );
    QTextStream out( &outFile );

    // Kereső kifejezés: (`xxx`)
    QRegExp keyPattern( "\\(`([^`]+)`\\)" );

    while( !in.atEnd() )
    {
        QString line = in.readLine();

        // Ha a sor tartalmazza a "FOREIGN KEY"-t, de nincs benne a `licenceId`
        if( line.contains("FOREIGN KEY") && !line.contains("`licenceId`") )
        {
            if( keyPattern.indexIn(line) != -1 )
            {
                QString columnId = keyPattern.cap(1); // Az első oszlop neve (xxx)
                QString replacement = QString( "(`%1`,`licenceId`)" ).arg( columnId );
                line.replace( keyPattern, replacement );
            }
        }

        out << line << "\n";
    }

    file.close();
    outFile.close();

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}

