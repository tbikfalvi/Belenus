//    QMessageBox::information( this, "", "" );

//#include <windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QProcessEnvironment>
#include <QMouseEvent>
#include <iostream>
#include <QCryptographicHash>
#include <QSqlQuery>

#include "../framework/qtmysqlquerymodel.h"
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

    m_nIndexPCStatusSync        = 0;
    m_nIndexPCOnlineSync        = 0;

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

    m_bShowMainWindowOnStart    = obPref.value( "ShowMainWindowOnStart", false ).toBool();
    m_nTimerPCStatusSync        = obPref.value( "TimerPCStatusSync", 2 ).toInt();
    m_nTimerPCOnlineSync        = obPref.value( "TimerPCOnlineSync", 60 ).toInt();

    //---------------------------------------------------------------------------------------------
    // Set main window settings
    g_obLogger(cSeverity::DEBUG) << "Set main window settings" << EOM;

//    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    trayIcon->setIcon( QIcon( ":/websync.png" ) );
    trayIcon->setToolTip( tr("Belenus WebSync") );
    trayIcon->show();

    ui->lblProcessStatus->setVisible( false );
    ui->prgbProcess->setVisible( false );
    ui->lblIndexPCData->setVisible( false );
    ui->lblIndexPCOnline->setVisible( false );

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

        cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
        m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
        ui->cmbName->setModel( m_poModel );
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
    ui->ledTimerPCStatusSync->setText( QString::number( m_nTimerPCStatusSync ) );
    ui->ledTimerPCOnlineSync->setText( QString::number( m_nTimerPCOnlineSync ) );

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
    obPref.setValue( "TimerPCStatusSync", m_nTimerPCStatusSync );
    obPref.setValue( "TimerPCOnlineSync", m_nTimerPCOnlineSync );

    delete g_poDB;

    g_obLogger(cSeverity::INFO) << "Belenus WebSync ended." << EOM;

    delete ui;
}
//=================================================================================================
void dlgMain::timerEvent(QTimerEvent *)
{
    m_nIndexPCStatusSync++;
    m_nIndexPCOnlineSync++;

    ui->lblIndexPCData->setText( QString::number(m_nIndexPCStatusSync) );
    ui->lblIndexPCOnline->setText( QString::number(m_nIndexPCOnlineSync) );

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
        m_nTimer = startTimer( 1000 );
    }

    if( m_bSyncPCToServer || m_bSyncPCFromServer )
    {
        // Synchronization process in progress, wait for next time slot
        return;
    }

    if( m_nIndexPCStatusSync >= m_nTimerPCStatusSync )
    {
        ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );

        m_nIndexPCStatusSync = 0;

        if( ui->ledNumberOfCardsWaiting->text().toInt() > 0 )
        {
            m_bSyncPCToServer = true;
            ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
            g_poBlnsHttp->processWaitingCardData();
        }
    }

    if( m_nIndexPCOnlineSync >= m_nTimerPCOnlineSync )
    {
        m_nIndexPCOnlineSync = 0;
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
//=================================================================================================
void dlgMain::on_pbSyncAllPatientCard_clicked()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientCardId, barcode "
                                                 "FROM patientcards WHERE "
                                                 "active = 1");

    ui->lblProcessStatus->setVisible( true );
    ui->prgbProcess->setVisible( true );
    ui->prgbProcess->setMaximum( poQuery->size() );
    ui->prgbProcess->setValue( 0 );

    while( poQuery->next() )
    {
        try
        {
            _sendPCData( poQuery->value(0).toUInt(), poQuery->value(1).toString() );
            ui->prgbProcess->setValue( ui->prgbProcess->value()+1 );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
    ui->lblProcessStatus->setVisible( false );
    ui->prgbProcess->setVisible( false );
}
//=================================================================================================
void dlgMain::_sendPCData(unsigned int p_uiId , QString p_qsBarcode)
{
    try
    {
        QString qsMessageData = "";

        qsMessageData.append( "<div id='unit'><label>T202</label></div>" );

        QString qsQuery = QString( "SELECT patientCardUnitId, "
                                   "patientCardTypeId, "
                                   "unitTime, "
                                   "validDateFrom, "
                                   "validDateTo, "
                                   "COUNT(unitTime) "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                   "AND prepared=0 "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" )
                                 .arg( p_uiId );
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            while( poQuery->next() )
            {
                QString qsValid = QString( "%1 -> %2" ).arg( poQuery->value( 3 ).toString() )
                                                       .arg( poQuery->value( 4 ).toString() );
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                qsMessageData.append( "<div class='validType'><span class='cardName'>" );
                qsMessageData.append( poQuery->value( 5 ).toString() );
                qsMessageData.append( " T219 (" );
                qsMessageData.append( poQuery->value( 2 ).toString() );
                qsMessageData.append( "T204) (" );
                qsMessageData.append( _getPatientCardTypeName( uiPCTId ) );
                qsMessageData.append( ")</span><span class='cardDays'>" );
                qsMessageData.append( qsValid );
                qsMessageData.append( "</span></div>" );
            }
        }
        else
        {
            qsMessageData.append( "<div class='validType'><span class='cardName'>" );
            qsMessageData.append( "0 T219 (" );
            qsMessageData.append( "0 T204" );
            qsMessageData.append( ")</span><span class='cardDays'>" );
            qsMessageData.append( "</span></div>" );
        }

        g_poBlnsHttp->sendPatientCardData( p_qsBarcode, qsMessageData, false );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//=================================================================================================
QString dlgMain::_getPatientCardTypeName( unsigned int p_uiId )
{
    QString qsRet = "";

    if( p_uiId > 0 )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT name "
                                                              "FROM patientcardtypes WHERE "
                                                              "patientcardtypeid=%1 AND "
                                                              "active = 1" ).arg( p_uiId ) );
        poQuery->first();
        qsRet = poQuery->value( 0 ).toString();
    }

    return qsRet;
}
//=================================================================================================
void dlgMain::on_ledTimerPCStatusSync_textEdited(const QString &/*arg1*/)
{
    m_nTimerPCStatusSync = ui->ledTimerPCStatusSync->text().toInt();
}
//=================================================================================================
void dlgMain::on_ledTimerPCOnlineSync_textEdited(const QString &/*arg1*/)
{
    m_nTimerPCOnlineSync = ui->ledTimerPCOnlineSync->text().toInt();
}
//=================================================================================================
void dlgMain::on_pbClearPCData_clicked()
{
    g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo" );
}
//=================================================================================================
void dlgMain::on_pbAuthenticate_clicked()
{

}
