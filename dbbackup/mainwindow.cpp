
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //---------------------------------------------------------------
    // Initialize the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //---------------------------------------------------------------
    // Initialize the variables
    m_nTimer            = 0;
    m_bProcessFinished  = false;

    //---------------------------------------------------------------
    // Start the application process with the timer
    m_nTimer = startTimer( 1000 );
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
    QString     qsProcess       = QString("%1/mysqldump.exe").arg(qsMysqlPath);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > backup/belenus_backup_%1.sql").arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
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
