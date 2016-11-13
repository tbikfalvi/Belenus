//    QMessageBox::information( this, "", "" );

//#include <windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QProcessEnvironment>
#include <QMouseEvent>
#include <iostream>

#include "dlgmain.h"
#include "ui_dlgmain.h"

using namespace std;

//=================================================================================================
dlgMain::dlgMain(QWidget *parent) : QDialog(parent), ui(new Ui::dlgMain)
{
    ui->setupUi(this);

    //---------------------------------------------------------------------------------------------
    // Initialize variables
    g_obLogger(cSeverity::DEBUG) << "Initialize main window variables" << EOM;

    m_nTimer                    = 0;
    m_bMousePressed             = false;
    m_bReloadLanguage           = false;
    m_bShowMainWindowOnStart    = true;

    m_bSyncPCToServer           = false;
    m_bSyncPCFromServer         = false;

    trayIcon                    = new QSystemTrayIcon(this);
    trayIconMenu                = new QMenu(this);
    menuConnection              = new QMenu(this);

    g_poDB                      = new cQTMySQLConnection;
    g_poBlnsHttp                = new cBlnsHttp();

    ui->pbHide->setVisible( false );

    //---------------------------------------------------------------------------------------------
    // Load settings from different files
    g_obLogger(cSeverity::DEBUG) << "Load settings from different files" << EOM;

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );
    QSettings   obBelenus( QString( "%1/belenus.ini" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    m_bShowMainWindowOnStart = obPref.value( "ShowMainWindowOnStart", false ).toBool();

    //---------------------------------------------------------------------------------------------
    // Set main window settings
    g_obLogger(cSeverity::DEBUG) << "Set main window settings" << EOM;

//    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    trayIcon->setIcon( QIcon( ":/websync.png" ) );
    trayIcon->setToolTip( tr("Belenus WebSync") );
    trayIcon->show();

    _setActions();
    _setMenu();

    resize( obPref.value( "WindowPosition/Mainwindow_width", 900 ).toInt(),
            obPref.value( "WindowPosition/Mainwindow_height", 600 ).toInt() );
    move( obPref.value( "WindowPosition/Mainwindow_left", 100).toInt(),
          obPref.value( "WindowPosition/Mainwindow_top", 100).toInt() );

    //---------------------------------------------------------------------------------------------
    // Connecting to SQL Server; Set status icon
    g_obLogger(cSeverity::DEBUG) << "Connecting to SQL Server" << EOM;

    try
    {
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_green.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("SQL Connection established") );
        ui->lblDBServerStatusText->setToolTip( tr("SQL Connection established") );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT serial FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            g_poBlnsHttp->setStudioLicenceString( poQuery->value( 0 ).toString() );
        }
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_red.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("Error in connection: %1").arg( e.what() ) );
        ui->lblDBServerStatusText->setToolTip( tr("Error in connection: %1").arg( e.what() ) );
    }

    //---------------------------------------------------------------------------------------------
    // Connecting to HTTP Server; Set status icon
    g_obLogger(cSeverity::DEBUG) << "Connecting to HTTP Server" << EOM;

    connect( g_poBlnsHttp, SIGNAL(signalErrorOccured()),            this, SLOT(on_BlnsHttpErrorOccured()) );
    connect( g_poBlnsHttp, SIGNAL(signalActionProcessed(QString)),  this, SLOT(on_BlnsHttpActionFinished(QString)) );
    connect( g_poBlnsHttp, SIGNAL(signalStepProgress()),            this, SLOT(on_BlnsHttpStepProgress()) );

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
    g_poBlnsHttp->setServerAddress( obBelenus.value( "Server/Address", "0.0.0.0" ).toString() );
    g_poBlnsHttp->setTimeout( obBelenus.value( QString::fromAscii( "BlnsHttp/MessageWaitTime" ), 12 ).toInt() * 1000 );
    g_poBlnsHttp->checkHttpServerAvailability();

    //---------------------------------------------------------------------------------------------
    // Web Connection settings
    g_obLogger(cSeverity::DEBUG) << "Web Connection settings" << EOM;

    ui->ledWebServerAddress->setText( obBelenus.value( "Server/Address", "0.0.0.0" ).toString() );

    //---------------------------------------------------------------------------------------------
    // Application settings
    g_obLogger(cSeverity::DEBUG) << "Application settings" << EOM;

    m_qsLang            = obPref.value( "Lang", "en" ).toString();
    int nCurrentIndex   = ui->cmbLang->findText( QString("%1 (").arg(m_qsLang), Qt::MatchContains );

    ui->cmbLang->setCurrentIndex( nCurrentIndex );

    ui->chkShowWindowOnStart->setChecked( m_bShowMainWindowOnStart );

    //---------------------------------------------------------------------------------------------
    // Patientcard status synchronization
    g_obLogger(cSeverity::DEBUG) << "Patientcard status synchronization" << EOM;

    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );

    //---------------------------------------------------------------------------------------------
    // Start sync process
    g_obLogger(cSeverity::DEBUG) << "Start main timer process" << EOM;

    m_bStartTimerOnStart = true;

    m_nTimer = startTimer( 500 );
}
//=================================================================================================
dlgMain::~dlgMain()
{
    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    obPref.setValue( "Lang",                    m_qsLang );
    obPref.setValue( "ShowMainWindowOnStart",   m_bShowMainWindowOnStart );

    obPref.setValue( "WindowPosition/Mainwindow_left", x() );
    obPref.setValue( "WindowPosition/Mainwindow_top", y() );
    obPref.setValue( "WindowPosition/Mainwindow_width", width() );
    obPref.setValue( "WindowPosition/Mainwindow_height", height() );

    delete g_poDB;

    g_obLogger(cSeverity::INFO) << "Belenus WebSync ended." << EOM;

    delete ui;
}
//=================================================================================================
void dlgMain::timerEvent(QTimerEvent *)
{
    //---------------------------------------------------------------------------------------------
    // Executed only at the beginning
    if( m_bStartTimerOnStart )
    {
        killTimer( m_nTimer );
        if( m_bShowMainWindowOnStart )
        {
            show();
        }
        m_bStartTimerOnStart = false;
        m_nTimer = startTimer( 3000 );
    }

    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );

    if( m_bSyncPCToServer || m_bSyncPCFromServer )
    {
        // Synchronization process in progress, wait for next time slot
        return;
    }

    if( ui->ledNumberOfCardsWaiting->text().toInt() > 0 )
    {
        m_bSyncPCToServer = true;
        ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
        g_poBlnsHttp->processWaitingCardData();
    }
}
//=================================================================================================
void dlgMain::mousePressEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = true;

    m_nMouseX = p_poEvent->pos().x();
    m_nMouseY = p_poEvent->pos().y();

    p_poEvent->accept();
}
//=================================================================================================
void dlgMain::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    p_poEvent->accept();
}
//=================================================================================================
void dlgMain::mouseMoveEvent ( QMouseEvent *p_poEvent )
{
    if( m_bMousePressed )
    {
        move( x() + p_poEvent->pos().x() - m_nMouseX,
              y() + p_poEvent->pos().y() - m_nMouseY );
    }
    p_poEvent->accept();
}
//=================================================================================================
void dlgMain::_setActions()
{
    actionSettings = new QAction(tr("&Open main window"), this);
    actionSettings->setIcon( QIcon( ":/websync.png" ) );
    connect( actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()) );

    actionResetSQL = new QAction( tr("Reset &SQL connection"), this );
    actionResetSQL->setIcon( QIcon( ":/reset_sql.png" ) );

    actionResetHTTP = new QAction( tr("Reset &HTTP connection"), this );
    actionResetHTTP->setIcon( QIcon( ":/reset_http.png" ) );

    actionExit = new QAction(tr("&Exit application"), this);
    actionExit->setIcon( QIcon( ":/exit.png" ) );
    connect( actionExit, SIGNAL(triggered()), qApp, SLOT(quit()) );
}
//=================================================================================================
void dlgMain::_setMenu()
{
    menuConnection->setIcon( QIcon( ":/settings.png" ) );
    menuConnection->setTitle( tr("Connection") );
    menuConnection->addAction( actionResetSQL );
    menuConnection->addAction( actionResetHTTP );

    trayIconMenu->addAction( actionSettings );
    trayIconMenu->addSeparator();

    trayIconMenu->addMenu( menuConnection );
    trayIconMenu->addSeparator();

    trayIconMenu->addAction( actionExit );

    trayIcon->setContextMenu(trayIconMenu);
}
//=================================================================================================
void dlgMain::slotSettings()
{
    show();
}
//=================================================================================================
void dlgMain::on_pbHide_clicked()
{
    hide();
}
//=================================================================================================
void dlgMain::on_pbExit_clicked()
{
    qApp->quit();
}
//=================================================================================================
void dlgMain::on_cmbLang_currentIndexChanged(const QString &arg1)
{
    if( !m_bReloadLanguage )
    {
        m_qsLang = arg1.left(2);
    }
}
//=================================================================================================
void dlgMain::on_pbRetranslate_clicked()
{
    m_bReloadLanguage = true;

    apMainApp->removeTranslator( poTransApp );
    apMainApp->removeTranslator( poTransQT );

    poTransApp->load( QString("%1\\lang\\websync_%2.qm").arg( QDir::currentPath() ).arg(m_qsLang) );
    poTransQT->load( QString("%1\\lang\\qt_%2.qm").arg( QDir::currentPath() ).arg(m_qsLang) );

    apMainApp->installTranslator( poTransApp );
    apMainApp->installTranslator( poTransQT );

    ui->retranslateUi( this );

    int nCurrentIndex   = ui->cmbLang->findText( QString("%1 (").arg(m_qsLang), Qt::MatchContains );

    ui->cmbLang->setCurrentIndex( nCurrentIndex );

    m_bReloadLanguage = false;
}
//=================================================================================================
// Executed when any error occures during http process
void dlgMain::on_BlnsHttpErrorOccured()
{
    m_bSyncPCToServer = false;

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_red.png" ) );
    ui->lblStatusIconWebServer->setToolTip( g_poBlnsHttp->errorMessage() );
    ui->lblWebServerStatusText->setToolTip( g_poBlnsHttp->errorMessage() );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
}
//=================================================================================================
// Executed when a http process finished
void dlgMain::on_BlnsHttpActionFinished(QString p_qsInfo)
{
    m_bSyncPCToServer = false;

    if( p_qsInfo.left(10).compare( "HTTPMSG_01" ) == 0 )
    {
        ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_green.png" ) );
        ui->lblStatusIconWebServer->setToolTip( tr("HTTP Connection established") );
        ui->lblWebServerStatusText->setToolTip( tr("HTTP Connection established") );
    }
    g_obLogger(cSeverity::INFO) << p_qsInfo << EOM;
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
}
//=================================================================================================
// Executed when patientcard data updated on server
void dlgMain::on_BlnsHttpStepProgress()
{
    m_bSyncPCToServer = false;

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_green.png" ) );
    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
}
//=================================================================================================
void dlgMain::on_pbResetSQL_clicked()
{

}
//=================================================================================================
void dlgMain::on_pbResetHTTP_clicked()
{
    g_poBlnsHttp->checkHttpServerAvailability();
    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
}
//=================================================================================================
void dlgMain::on_chkShowWindowOnStart_clicked()
{
    m_bShowMainWindowOnStart = ui->chkShowWindowOnStart->isChecked();
}
