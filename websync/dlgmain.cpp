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

    m_bStartFinished            = false;

    m_qsRPSW                    = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";

    m_bStartTimerOnStart        = false;
    m_nTimer                    = 0;
    m_bMousePressed             = false;
    m_bReloadLanguage           = false;
    m_bShowMainWindowOnStart    = true;
    m_bHttpSuspended            = false;
    m_nLogLevel                 = 0;

    m_bSyncPCToServer           = false;
    m_bSyncPCFromServer         = false;

    m_nIndexPCStatusSync        = 0;
    m_nIndexPCOnlineSync        = 0;
    m_nIndexUpdateSyncDataCount = 0;
    m_nIndexUser                = 0;

    m_enGroup                   = GROUP_MIN;

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
    m_uiPatientCardTypeId       = obPref.value( "OnlinePatientCardType", 0 ).toUInt();
    m_uiPaymentMethodId         = obPref.value( "OnlinePaymentMethod", 0 ).toUInt();
    m_nLogLevel                 = obPref.value( "LogLevel", cSeverity::DEBUG ).toInt();

    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)m_nLogLevel);

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

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            g_poBlnsHttp->setStudioLicence( poQuery->value( 0 ).toUInt(), poQuery->value( 1 ).toString() );
        }

        cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
        m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
        ui->cmbName->setModel( m_poModel );

        ui->cmbOnlinePatientCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            if( poQuery->value(0) == 1 ) continue;

            ui->cmbOnlinePatientCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ).toUInt() );
            if( m_uiPatientCardTypeId == poQuery->value( 0 ).toUInt() )
                ui->cmbOnlinePatientCardType->setCurrentIndex( ui->cmbOnlinePatientCardType->count()-1 );
        }
        _setPCTypeForHttp();

        ui->cmbOnlinePaymentMethod->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT paymentMethodId, name FROM paymentMethods WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            ui->cmbOnlinePaymentMethod->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ).toUInt() );
            if( m_uiPaymentMethodId == poQuery->value( 0 ).toUInt() )
                ui->cmbOnlinePaymentMethod->setCurrentIndex( ui->cmbOnlinePaymentMethod->count()-1 );
        }
        g_poBlnsHttp->setOnlinePaymentMethod( m_uiPaymentMethodId );
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
    ui->pbStartStopHTTP->setIcon( QIcon(":/pause.png") );

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
    ui->sliFileLogLevel->setValue( m_nLogLevel );

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

    m_bStartFinished = true;
}
//=================================================================================================
dlgMain::~dlgMain()
{
    m_uiPatientCardTypeId = ui->cmbOnlinePatientCardType->itemData( ui->cmbOnlinePatientCardType->currentIndex() ).toUInt();
    m_uiPaymentMethodId   = ui->cmbOnlinePaymentMethod->itemData( ui->cmbOnlinePaymentMethod->currentIndex() ).toUInt();

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    obPref.setValue( "Lang",                    m_qsLang );
    obPref.setValue( "ShowMainWindowOnStart",   m_bShowMainWindowOnStart );

    obPref.setValue( "WindowPosition/Mainwindow_left", x() );
    obPref.setValue( "WindowPosition/Mainwindow_top", y() );
    obPref.setValue( "WindowPosition/Mainwindow_width", width() );
    obPref.setValue( "WindowPosition/Mainwindow_height", height() );
    obPref.setValue( "TimerPCStatusSync", m_nTimerPCStatusSync );
    obPref.setValue( "TimerPCOnlineSync", m_nTimerPCOnlineSync );
    obPref.setValue( "OnlinePatientCardType", m_uiPatientCardTypeId );
    obPref.setValue( "OnlinePaymentMethod", m_uiPaymentMethodId );
    obPref.setValue( "LogLevel", m_nLogLevel );

    delete g_poDB;

    g_obLogger(cSeverity::INFO) << "Belenus WebSync ended." << EOM;

    delete ui;
}
//=================================================================================================
void dlgMain::timerEvent(QTimerEvent *)
{
    m_nIndexPCStatusSync++;
    m_nIndexPCOnlineSync++;
    m_nIndexUpdateSyncDataCount++;
    m_nIndexUser++;

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

    //---------------------------------------------------------------------------------------------
    // Check if user logged in or out by belenus in every 5 seconds
    if( m_nIndexUser > 5 )
    {
        if( ui->ledPassword->text().length() < 1 )
        {
            QFile   fileUser( "c:/windows/system32/websync.usr" );

            // If user already logged in, check if logged out
            if( _isInGroup( GROUP_USER ) )
            {
                if( fileUser.size() < 1 )
                {
                    on_pbAuthenticate_clicked();
                }
            }
            else
            {
                if( fileUser.size() > 0 )
                {
                    char    strName[50];

                    for(int j=0;j<50;j++) strName[j]=0;

                    fileUser.open( QIODevice::ReadOnly );
                    fileUser.read( strName, 50 );
                    fileUser.close();

                    for( int i=0; i<ui->cmbName->count(); i++ )
                    {
                        QString qsName = ui->cmbName->itemText(i).left( ui->cmbName->itemText(i).indexOf("(")-1 );

                        if( qsName.compare( QString(strName) ) == 0 )
                        {
                            ui->cmbName->setCurrentIndex( i );
                            if( _loginUser( QString(strName) ) == AUTH_OK )
                            {
                                QString qsUser = ui->cmbName->itemText(i).replace('(','|').replace(')',"");

                                trayIcon->showMessage( "Belenus Websync",
                                                       tr("User '%1' logged in").arg( qsUser.section("|",1) ),
                                                       QSystemTrayIcon::Information, 3000 );
                                _setGUIEnabled();
                            }
                            break;
                        }
                    }
                }
            }
        }
        m_nIndexUser = 0;

        QString qsTooltip = "";

        qsTooltip.append( m_qsHttpStatus );
        if( _isInGroup( GROUP_SYSTEM ) )
        {
            qsTooltip.append( "<br><br>" );
            qsTooltip.append( g_poBlnsHttp->settingsInfo() );
        }
        ui->lblStatusIconWebServer->setToolTip( qsTooltip );
    }

    //---------------------------------------------------------------------------------------------
    // Check if any action is in progress
    if( m_bSyncPCToServer || m_bSyncPCFromServer )
    {
        // Synchronization process in progress, wait for next time slot
        return;
    }

    //---------------------------------------------------------------------------------------------
    // Update number of waiting patientcards to be sent in every minute
    if( m_nIndexUpdateSyncDataCount > 60 )
    {
        m_nIndexUpdateSyncDataCount = 0;
        ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    }

    //---------------------------------------------------------------------------------------------
    // Check if timer of PC data send is reached the value set
    if( m_nIndexPCStatusSync >= m_nTimerPCStatusSync )
    {
        m_nIndexPCStatusSync = 0;

        if( ui->ledNumberOfCardsWaiting->text().toInt() > 0 )
        {
            m_bSyncPCToServer = true;
            ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
            g_poBlnsHttp->processWaitingCardData();
        }
    }

    //---------------------------------------------------------------------------------------------
    // Check if timer of check online PC sold is reached the value set
    if( m_nIndexPCOnlineSync >= m_nTimerPCOnlineSync )
    {
        m_nIndexPCOnlineSync = 0;
        m_bSyncPCFromServer = true;
        ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
        g_poBlnsHttp->getPatientCardsSoldOnline();
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
void dlgMain::on_pbAuthenticate_clicked()
{
    authType    atRet = AUTH_NEEDED;

    m_enGroup = GROUP_MIN;

    if( ui->ledPassword->isEnabled() )
    {
        atRet = _authenticateUser();
    }

    _setAuthInfoType( atRet );

    if( atRet == AUTH_OK )
    {
        _setGUIEnabled();
    }
    else
    {
        _setGUIEnabled( false );
        ui->ledPassword->setText( "" );
    }
}
//=================================================================================================
void dlgMain::on_pbExit_clicked()
{
    if( _isInGroup( GROUP_SYSTEM ) )
    {
        qApp->quit();
    }
}
//=================================================================================================
void dlgMain::on_pbHide_clicked()
{
    hide();
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
void dlgMain::on_chkShowWindowOnStart_clicked()
{
    m_bShowMainWindowOnStart = ui->chkShowWindowOnStart->isChecked();
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
void dlgMain::on_pbClearPCData_clicked()
{
    g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo" );
}
//=================================================================================================
void dlgMain::on_pbSyncOnlinePC_clicked()
{
    g_poBlnsHttp->getPatientCardsSoldOnline();
}
//=================================================================================================
//
//  H T T P
//
//=================================================================================================
// Executed when any error occures during http process
void dlgMain::on_BlnsHttpErrorOccured()
{
    QString qsTooltip   = "";
    m_bSyncPCToServer   = false;
    m_bSyncPCFromServer = false;
    m_qsHttpStatus      = g_poBlnsHttp->errorMessage();

    qsTooltip.append( m_qsHttpStatus );
    if( _isInGroup( GROUP_SYSTEM ) )
    {
        qsTooltip.append( "<br><br>" );
        qsTooltip.append( g_poBlnsHttp->settingsInfo() );
    }

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_red.png" ) );
    ui->lblStatusIconWebServer->setToolTip( qsTooltip );
    ui->lblWebServerStatusText->setToolTip( g_poBlnsHttp->errorMessage() );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
    trayIcon->setIcon( QIcon( ":/websync_red.png" ) );

    trayIcon->showMessage( "Belenus Websync",
                           QString("%1").arg( g_poBlnsHttp->errorMessage() ),
                           QSystemTrayIcon::Critical, 3000 );
}
//=================================================================================================
// Executed when a http process finished
void dlgMain::on_BlnsHttpActionFinished(QString p_qsInfo)
{
    QString qsTooltip   = "";
    m_bSyncPCToServer   = false;
    m_bSyncPCFromServer = false;
    m_qsHttpStatus      = tr("HTTP Connection established");

    qsTooltip.append( m_qsHttpStatus );
    if( _isInGroup( GROUP_SYSTEM ) )
    {
        qsTooltip.append( "<br><br>" );
        qsTooltip.append( g_poBlnsHttp->settingsInfo() );
    }

//    if( p_qsInfo.left(10).compare( "HTTPMSG_01" ) == 0 )
//    {
        ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_green.png" ) );
        ui->lblStatusIconWebServer->setToolTip( qsTooltip );
        ui->lblWebServerStatusText->setToolTip( tr("HTTP Connection established") );
        trayIcon->setIcon( QIcon( ":/websync.png" ) );

//    }
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
//
// P R I V A T E - functions
//
//=================================================================================================
void dlgMain::_setGUIEnabled(bool p_bEnabled)
{
    ui->chkShowWindowOnStart->setEnabled( p_bEnabled );
    ui->ledTimerPCStatusSync->setEnabled( p_bEnabled );
    ui->ledTimerPCOnlineSync->setEnabled( p_bEnabled );
    ui->ledWebServerAddress->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
    ui->sliFileLogLevel->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );
    ui->pbSyncAllPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
    //ui->pbClearPCData->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );  // can be cleared anytime
    ui->pbSyncOnlinePC->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );
    ui->cmbOnlinePatientCardType->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );
    ui->cmbOnlinePaymentMethod->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );
    ui->pbExit->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
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
//    trayIconMenu->addSeparator();

//    trayIconMenu->addAction( actionExit );

    trayIcon->setContextMenu(trayIconMenu);
}
//=================================================================================================
void dlgMain::slotSettings()
{
    show();
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
dlgMain::authType dlgMain::_authenticateUser()
{
//    cTracer obTrace( "cWndMain::_authenticateUser" );

    string  stName = ui->cmbName->currentText().toStdString();
    stName = stName.substr( 0, stName.find( '(' ) - 1 );
    authType    atRet = AUTH_ERROR;
    QByteArray  obPwdHash = QCryptographicHash::hash( ui->ledPassword->text().toAscii(), QCryptographicHash::Sha1 );

    m_enGroup = GROUP_MIN;

    if( QString::fromStdString(stName).compare( "root" ) == 0 )
    {
        if( m_qsRPSW.compare( QString( obPwdHash.toHex() ) ) == 0 )
        {
            atRet = AUTH_OK;
            m_enGroup = GROUP_ROOT;
        }
        else
        {
            atRet = AUTH_PASSWORD_INCORRECT;
        }
    }
    else
    {
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + QString::fromStdString(stName) + "\"" );

            if( poQuery->size() != 1 )
            {
                atRet = AUTH_USER_NOTFOUND;
            }
            else
            {
                poQuery->first();
                if( poQuery->value(4).toString().compare( QString( obPwdHash.toHex() ) ) == 0 )
                {
                    atRet = AUTH_OK;
                    m_enGroup = poQuery->value(5).toInt();
//                    m_uiUserId = poQuery->value(0).toUInt();
                }
                else
                {
                    atRet = AUTH_PASSWORD_INCORRECT;
                }
            }
        }
        catch( cSevException &e )
        {
            atRet = AUTH_CONNECTION_FAILED;
        }
    }

    return atRet;
}
//=================================================================================================
dlgMain::authType dlgMain::_loginUser(QString p_qsName)
{
    authType    atRet = AUTH_ERROR;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
    {
        atRet = AUTH_USER_NOTFOUND;
    }
    else
    {
        poQuery->first();
        m_enGroup = poQuery->value(5).toInt();
        atRet = AUTH_OK;
    }
    _setAuthInfoType( atRet );

    return atRet;
}
//=================================================================================================
void dlgMain::_setAuthInfoType(authType p_tAuthType)
{
//    cTracer obTrace( "cWndMain::_setAuthInfoType" );

    ui->cmbName->setEnabled( true );
    ui->ledPassword->setEnabled( true );

    ui->pbAuthenticate->setIcon( QIcon(":/key.png") );
    ui->pbAuthenticate->setIconSize( QSize(20,20) );

    if( p_tAuthType == AUTH_NEEDED )
    {
        ui->pbAuthenticate->setToolTip( tr("Need authentication ...") );
    }
    else if( p_tAuthType == AUTH_OK )
    {
        ui->pbAuthenticate->setToolTip( tr("User successfully authenticated.") );

        ui->cmbName->setEnabled( false );
        ui->ledPassword->setEnabled( false );
        ui->pbAuthenticate->setIcon( QIcon(":/lock.png") );
        ui->pbAuthenticate->setIconSize( QSize(30,30) );
    }
    else
    {
        if( p_tAuthType == AUTH_USER_NOTFOUND )
            ui->pbAuthenticate->setToolTip( tr("The username entered not found in the Belenus database") );
        else if( p_tAuthType == AUTH_PASSWORD_INCORRECT )
            ui->pbAuthenticate->setToolTip( tr("The password entered is incorrect") );
        else if( p_tAuthType == AUTH_CONNECTION_FAILED )
            ui->pbAuthenticate->setToolTip( tr("Connection to Belenus database failed") );
        else
            ui->pbAuthenticate->setToolTip( tr("Authentication failed. Please retry later ...") );
    }
}
//=================================================================================================
bool dlgMain::_isInGroup(groupUser p_enGroup)
{
    return ( p_enGroup <= m_enGroup );
}

//=================================================================================================
void dlgMain::on_pbStartStopHTTP_clicked()
{
    if( m_bHttpSuspended )
    {
        m_bHttpSuspended = false;
        ui->pbStartStopHTTP->setIcon( QIcon(":/pause.png") );
    }
    else
    {
        m_bHttpSuspended = true;
        ui->pbStartStopHTTP->setIcon( QIcon(":/start.png") );
    }
}

//=================================================================================================
void dlgMain::on_cmbOnlinePatientCardType_currentIndexChanged(int index)
{
    if( m_bStartFinished )
    {
        m_uiPatientCardTypeId = ui->cmbOnlinePatientCardType->itemData( index ).toUInt();

        _setPCTypeForHttp();
    }
}

//=================================================================================================
void dlgMain::_setPCTypeForHttp()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT price, units, unitTime "
                                                          "FROM patientCardTypes WHERE "
                                                          "active=1 AND archive<>\"DEL\" AND patientCardTypeId=%1 " )
                                                 .arg( m_uiPatientCardTypeId ) );

    poQuery->first();

    g_poBlnsHttp->setOnlinePCType( m_uiPatientCardTypeId,
                                   poQuery->value( 0 ).toInt(),
                                   poQuery->value( 1 ).toInt(),
                                   poQuery->value( 2 ).toInt() );
}

//=================================================================================================
void dlgMain::on_cmbOnlinePaymentMethod_currentIndexChanged(int index)
{
    if( m_bStartFinished )
    {
        m_uiPaymentMethodId = ui->cmbOnlinePaymentMethod->itemData( index ).toUInt();
        g_poBlnsHttp->setOnlinePaymentMethod( m_uiPaymentMethodId );
    }
}

//=================================================================================================
void dlgMain::on_sliFileLogLevel_valueChanged(int value)
{
    m_nLogLevel = value;
    ui->lblFileLogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)m_nLogLevel ) );
    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)m_nLogLevel);
}
