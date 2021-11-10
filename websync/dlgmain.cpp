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
#include "dlglineedit.h"

using namespace std;

//=================================================================================================
dlgMain::dlgMain(QWidget *parent, QString p_qsAppVersion) : QDialog(parent), ui(new Ui::dlgMain)
{
    ui->setupUi(this);

    //---------------------------------------------------------------------------------------------
    // Initialize variables
    g_obLogger(cSeverity::DEBUG) << "Initialize main window variables" << EOM;

    m_bStartFinished                    = false;

    m_qsRPSW                            = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";

    m_FlagNoHttpServer                  = false;
    m_FlagHttpFailed                    = false;
    m_FlagHttpDisabled                  = false;
    m_FlagHttpEnabled                   = false;
    m_FlagHttpSuspended                 = false;
    m_FlagHttpContinued                 = false;
//    m_FlagNoPaymentMethod             = false;
//    m_FlagNoPCType                    = false;

    m_bStartTimerOnStart                = false;
    m_nTimer                            = 0;
    m_bMousePressed                     = false;
    m_bReloadLanguage                   = false;
    m_bShowMainWindowOnStart            = true;
    m_nLogLevel                         = 0;

    m_bHttpEnabledBySetting             = true;
    m_bHttpEnabledByUser                = true;
    m_bHttpSuspendedByUser              = false;

    m_bSyncPCToServer                   = false;
    m_bSyncPCFromServer                 = false;
    m_bSendMailToServer                 = false;
    m_bRegisterLicenceKey               = false;
    m_bReactivateLicenceKey             = false;

    m_bStartRegisterLicenceKey          = false;
    m_bStartReactivateLicenceKey        = false;
    m_bValidateLicenceKey               = false;

    m_nIndexPCStatusSync                = 0;
//    m_nIndexPCOnlineSync              = 0;
    m_nIndexUpdateSyncDataCount         = 0;
    m_nIndexUser                        = 0;
    m_nIndexCheckEnablers               = 0;
    m_nIndexSendMailSync                = 0;
    m_nIndexLicenceValidation           = 0;

    m_nTimerLicenceValidationCheck      = 3600;

    m_enGroup                           = GROUP_MIN;

    m_bServerAddressChanged             = false;

    m_uiLicenceId                       = 0;

    g_poDB                              = new cQTMySQLConnection;
    g_poBlnsHttp                        = new cBlnsHttp();

    ui->pbHide->setVisible( false );

    //---------------------------------------------------------------------------------------------
    // Load settings from different files
    g_obLogger(cSeverity::DEBUG) << "Load settings from different files" << EOM;

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    m_bShowMainWindowOnStart    = obPref.value( "ShowMainWindowOnStart", false ).toBool();
    m_nIndexLicenceValidation   = obPref.value( "TimerLicenceValidation", 0 ).toInt();
    m_nTimerPCStatusSync        = obPref.value( "TimerPCStatusSync", 2 ).toInt();
    m_nTimerSendMailCheck       = obPref.value( "TimerSendMailCheck", 11 ).toInt();
    m_nLogLevel                 = obPref.value( "LogLevel", cSeverity::DEBUG ).toInt();

//    m_nTimerPCOnlineSync        = obPref.value( "TimerPCOnlineSync", 60 ).toInt();
//    m_uiPatientCardTypeId       = obPref.value( "OnlinePatientCardType", 0 ).toUInt();
//    m_uiPaymentMethodId         = obPref.value( "OnlinePaymentMethod", 0 ).toUInt();

    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)m_nLogLevel);

    //---------------------------------------------------------------------------------------------
    // Set main window settings
    g_obLogger(cSeverity::DEBUG) << "Set main window settings" << EOM;

//    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );
    ui->gbTitle->setTitle( QString(" v.%1 ").arg( p_qsAppVersion ) );

    _setActions();
    _setMenu();

    trayIcon->setIcon( QIcon( ":/websync_yellow.png" ) );
    trayIcon->setToolTip( tr("Belenus WebSync") );
    trayIcon->show();

    ui->tabWidget->setCurrentIndex( 0 );
    ui->tabLicence->setEnabled( false );
    ui->gbLicenceInformation->setVisible( false );
    ui->gbLicenceActions->setVisible( false );

    ui->lblProcessStatus->setVisible( false );
    ui->prgbProcess->setVisible( false );
    ui->lblIndexPCData->setVisible( false );
    ui->lblIndexPCOnline->setVisible( false );
    ui->lblIndexMailSendCheck->setVisible( false );

    // hide obsolete items
    ui->lblOnlinePatientCardType->setVisible( false );
    ui->cmbOnlinePatientCardType->setVisible( false );
    ui->lblOnlinePaymentMethod->setVisible( false );
    ui->cmbOnlinePaymentMethod->setVisible( false );
    ui->lblTimerPCOnlineSync->setVisible( false );
    ui->ledTimerPCOnlineSync->setVisible( false );
    ui->lblSeconds2->setVisible( false );
    ui->lblIndexPCOnline->setVisible( false );
    ui->pbSyncOnlinePC->setVisible( false );

    // resize dialog
    resize( obPref.value( "WindowPosition/Mainwindow_width", 785 ).toInt(),
            obPref.value( "WindowPosition/Mainwindow_height", 470 ).toInt() );
    move( obPref.value( "WindowPosition/Mainwindow_left", 100).toInt(),
          obPref.value( "WindowPosition/Mainwindow_top", 100).toInt() );

    //---------------------------------------------------------------------------------------------
    // Connecting to SQL Server; Set status icon
    g_obLogger(cSeverity::DEBUG) << "Connecting to SQL Server" << EOM;

    m_qsServerAddress = "0.0.0.0";

    try
    {
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_green.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("SQL Connection established") );
        actionStatusSQL->setIcon( QIcon( ":/status_green.png" ) );
        ui->lblDBServerStatusText->setToolTip( tr("SQL Connection established") );

        QSqlQuery *poQuery = NULL;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"SYNC_Enabled\" " ) );
        if( poQuery->first() )
        {
            m_bHttpEnabledByUser = poQuery->value( 0 ).toBool();
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"SERVER_Address\" " ) );
        if( poQuery->first() )
        {
            m_qsServerAddress = poQuery->value( 0 ).toString();
        }

        poQuery = g_poDB->executeQTQuery( "SELECT value FROM `settings` WHERE identifier='LICENCE_WORKTIME_COUNTER' " );
        if( poQuery->first() )
        {
            int nHour = (poQuery->value( 0 ).toInt() * 15)/60;

            ui->lblLicenceCheckValue->setText( QString( "%1 work hours" ).arg( nHour ) );
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_uiLicenceId = poQuery->value( 0 ).toUInt();
            ui->ledLicenceKeyCurrent->setText( poQuery->value( 1 ).toString() );
            ui->deValidated->setDate( poQuery->value( 9 ).toDate() );
            ui->ledCodeServer->setText( poQuery->value( 11 ).toString() );
            ui->ledCodeClient->setText( poQuery->value( 12 ).toString() );

            if( poQuery->value( 11 ).toString().compare( "VALIDATED" ) == 0 )
            {
                ui->ledLicenceStatus->setText( tr( "Validated" ) );
            }
            else if( poQuery->value( 11 ).toString().compare( "UNVERIFIED" ) == 0 )
            {
                ui->ledLicenceStatus->setText( tr( "Not activated" ) );
            }
            else if( poQuery->value( 11 ).toString().compare( "INVALID" ) == 0 )
            {
                ui->ledLicenceStatus->setText( tr( "Expired / Invalid" ) );
            }
            else
            {
                ui->ledLicenceStatus->setText( tr( "Demo" ) );
            }

            g_poBlnsHttp->setStudioLicence( m_uiLicenceId, ui->ledLicenceKeyCurrent->text() );
        }

        cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
        m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
        ui->cmbName->setModel( m_poModel );
/*
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
*/
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_red.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("Error in connection: %1").arg( e.what() ) );
        actionStatusSQL->setIcon( QIcon( ":/status_green.png" ) );
        ui->lblDBServerStatusText->setToolTip( tr("Error in connection: %1").arg( e.what() ) );
    }

    //---------------------------------------------------------------------------------------------
    // Connecting to HTTP Server; Set status icon
    g_obLogger(cSeverity::DEBUG) << "Connecting to HTTP Server" << EOM;

    connect( g_poBlnsHttp, SIGNAL(signalErrorOccured()),                    this, SLOT(on_BlnsHttpErrorOccured()) );
    connect( g_poBlnsHttp, SIGNAL(signalActionProcessed(QString)),          this, SLOT(on_BlnsHttpActionFinished(QString)) );
    connect( g_poBlnsHttp, SIGNAL(signalStepProgress()),                    this, SLOT(on_BlnsHttpStepProgress()) );
    connect( g_poBlnsHttp, SIGNAL(signalPatientCardUpdated(uint,QString)),  this, SLOT(on_PatientCardUpdated(uint,QString)) );
    connect( g_poBlnsHttp, SIGNAL(signalDisplayNotification(QString)),      this, SLOT(slotShowModuleNotification(QString)) );
    connect( g_poBlnsHttp, SIGNAL(signalHideProgress(QString)),             this, SLOT(on_BlnsHttpActionFinished(QString)) );

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
    ui->chkHttpCommunicationEnabled->setChecked( m_bHttpEnabledByUser );
    g_poBlnsHttp->setServerAddress( m_qsServerAddress );
    g_poBlnsHttp->setTimeout( 12000 );
    ui->pbStartStopHTTP->setIcon( QIcon(":/pause.png") );
    m_bHttpEnabledBySetting = m_bHttpEnabledByUser;

    if( m_bHttpEnabledBySetting && m_bHttpEnabledByUser )
    {
        g_poBlnsHttp->setCommunicationEnabled( true );
        g_poBlnsHttp->checkHttpServerAvailability();
    }
    else
    {
        g_poBlnsHttp->setCommunicationEnabled( false );
    }

    //---------------------------------------------------------------------------------------------
    // Web Connection settings
    g_obLogger(cSeverity::DEBUG) << "Web Connection settings" << EOM;

    ui->ledWebServerAddress->setText( m_qsServerAddress );

    //---------------------------------------------------------------------------------------------
    // Application settings
    g_obLogger(cSeverity::DEBUG) << "Application settings" << EOM;

    int lngindex = g_obLanguage.setLanguageCombo( ui->cmbLang );
    g_obLogger(cSeverity::INFO) << "Language index: [" << lngindex << "]" << EOM;

    ui->chkShowWindowOnStart->setChecked( m_bShowMainWindowOnStart );
    ui->ledTimerPCStatusSync->setText( QString::number( m_nTimerPCStatusSync ) );
//    ui->ledTimerPCOnlineSync->setText( QString::number( m_nTimerPCOnlineSync ) );
    ui->ledTimerMailSendCheck->setText( QString::number( m_nTimerSendMailCheck ) );
    ui->sliFileLogLevel->setValue( m_nLogLevel );

    //---------------------------------------------------------------------------------------------
    // Patientcard status synchronization
    g_obLogger(cSeverity::DEBUG) << "Patientcard status synchronization" << EOM;

    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );

    _setGUIEnabled( false );

    //---------------------------------------------------------------------------------------------
    // Start sync process
    g_obLogger(cSeverity::DEBUG) << "Start main timer process" << EOM;

    ui->lblIndexPCData->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->lblIndexPCOnline->setVisible( false/*_isInGroup( GROUP_SYSTEM )*/ );
    ui->lblIndexMailSendCheck->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->pbTest->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->pbTest->setEnabled( _isInGroup( GROUP_SYSTEM ) );
    ui->tabLicence->setEnabled( _isInGroup( GROUP_SYSTEM ) );
    ui->gbLicenceInformation->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->gbLicenceActions->setVisible( _isInGroup( GROUP_SYSTEM ) );

    m_bStartTimerOnStart = true;
    m_nTimer = startTimer( 500 );
    m_bStartFinished = true;
}

//=================================================================================================
dlgMain::~dlgMain()
{
    _saveSettings();

    delete g_poDB;

    g_obLogger(cSeverity::INFO) << "Belenus WebSync ended." << EOM;

    delete ui;
}

//=================================================================================================
void dlgMain::timerEvent(QTimerEvent *)
{
    m_nIndexPCStatusSync++;
//    m_nIndexPCOnlineSync++;
    m_nIndexUpdateSyncDataCount++;
    m_nIndexUser++;
    m_nIndexCheckEnablers++;
    m_nIndexSendMailSync++;
    m_nIndexLicenceValidation++;

    ui->lblIndexPCData->setText( QString::number(m_nIndexPCStatusSync) );
//    ui->lblIndexPCOnline->setText( QString::number(m_nIndexPCOnlineSync) );
    ui->lblIndexMailSendCheck->setText( QString::number(m_nIndexSendMailSync ) );
    ui->lblWorktimeCounterValue->setText( QString( "%1:%2" ).arg( (m_nTimerLicenceValidationCheck-m_nIndexLicenceValidation)/60 ).arg( (m_nTimerLicenceValidationCheck-m_nIndexLicenceValidation)%60 ) );

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
        return;
    }

    QFile   fileCheck( "websync.chk" );

    if( fileCheck.size() > 0 )
    {
        fileCheck.open( QIODevice::WriteOnly );
        fileCheck.write( "" );
        fileCheck.close();
    }

    //---------------------------------------------------------------------------------------------
    // If Licence management tab is selected but not Kiwisun system admin logged in jump back to
    // status tab
    if( ui->tabWidget->currentIndex() == 3 && !_isInGroup( GROUP_SYSTEM ) )
    {
        ui->tabWidget->setCurrentIndex( 0 );
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

                                _displayUserNotification( INFO_Custom,
                                                          tr("User '%1' logged in").arg( qsUser.section("|",1) ) );
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
    if( m_bSyncPCToServer || m_bSyncPCFromServer || m_bSendMailToServer || m_bRegisterLicenceKey || m_bReactivateLicenceKey || m_bValidateLicenceKey )
    {
        // Synchronization process in progress, wait for next time slot
        g_obLogger(cSeverity::DEBUG) << "Processes: " << m_bSyncPCToServer
                                     << "|" << m_bSyncPCFromServer
                                     << "|" << m_bSendMailToServer
                                     << "|" << m_bRegisterLicenceKey
                                     << "|" << m_bReactivateLicenceKey
                                     << "|" << m_bValidateLicenceKey
                                     << EOM;
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
    // Check if application settings are ok for http processes
    if( m_nIndexCheckEnablers > 10 )
    {
        m_bHttpEnabledBySetting = true;
        m_nIndexCheckEnablers   = 0;

        // Read enabler from belenus.ini
        _checkIfHttpDisabledByUser();
        _checkIfHttpChangedByUser();
        _setGUIEnabled();

        // Check web server address
        if( ui->ledWebServerAddress->text().length() == 0 )
        {
            _disableHttpBySetting();
            _displayUserNotification( INFO_NoHttpServer );
            ui->lblServerAddress->setStyleSheet( "QLabel {font: bold; color: red;}" );
            return;
        }
        // Check if PatientCardType is selected
        /*if( m_uiPatientCardTypeId == 0 )
        {
            _disableHttpBySetting();
            _displayUserNotification( INFO_NoPCType );
            ui->lblOnlinePatientCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
            return;
        }
        // Check if PaymentMethod is selected
        if( m_uiPaymentMethodId == 0 )
        {
            _disableHttpBySetting();
            _displayUserNotification( INFO_NoPaymentMethod );
            ui->lblOnlinePaymentMethod->setStyleSheet( "QLabel {font: bold; color: red;}" );
            return;
        }*/
    }

    //---------------------------------------------------------------------------------------------
    // Check if Licence key registration is needed
    if( m_bStartRegisterLicenceKey )
    {
        m_bStartRegisterLicenceKey = false;
        m_bRegisterLicenceKey = true;
        g_poBlnsHttp->registerLicenceKey( ui->ledLicenceKeyNew->text(), ui->ledCodeClient->text() );
        return;
    }

    //---------------------------------------------------------------------------------------------
    // Check if Licence key reactivation is needed
    if( m_bStartReactivateLicenceKey )
    {
        m_bStartReactivateLicenceKey = false;
        m_bReactivateLicenceKey = true;
        g_poBlnsHttp->reactivateLicenceKey( ui->ledLicenceKeyCurrent->text(), ui->ledCodeClient->text() );
        return;
    }

    //---------------------------------------------------------------------------------------------
    // Check every hour if the licence key validetion has to be checked or not
    if( m_nIndexLicenceValidation > m_nTimerLicenceValidationCheck )
    {
        m_nIndexLicenceValidation = 0;

        // Retrieve worktime counter from database, if it reached zero, then validation needed
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT value FROM `settings` WHERE identifier='LICENCE_WORKTIME_COUNTER' " );

            if( poQuery->first() )
            {
                int nWorktimeCounter = poQuery->value( 0 ).toInt();
                int nHour = (nWorktimeCounter * 15)/60;

                ui->lblLicenceCheckValue->setText( QString( "%1 work hours" ).arg( nHour ) );

                if( nWorktimeCounter < 1 )
                {
                    m_bValidateLicenceKey = true;
                    g_poBlnsHttp->validateLicenceKey( ui->ledLicenceKeyCurrent->text(), ui->ledCodeClient->text(), ui->ledCodeServer->text() );
                    return;
                }
            }
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    //---------------------------------------------------------------------------------------------
    // Check if timer of PC data send is reached the value set
    if( m_nIndexPCStatusSync >= m_nTimerPCStatusSync && !m_bSyncPCFromServer && !m_bSyncPCToServer && !m_bSendMailToServer )
    {
        m_nIndexPCStatusSync = 0;

        if( ui->ledNumberOfCardsWaiting->text().toInt() > 0 )
        {
            m_bSyncPCToServer = true;
            g_obLogger(cSeverity::DEBUG) << "Process started: sending patientcard data." << EOM;
            ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
            trayIcon->setIcon( QIcon( ":/hourglass.png" ) );
            g_poBlnsHttp->processWaitingCardData();
        }
    }

    //---------------------------------------------------------------------------------------------
    // Check if timer of check online PC sold is reached the value set
/*    if( m_nIndexPCOnlineSync >= m_nTimerPCOnlineSync && !m_bSyncPCFromServer && !m_bSyncPCToServer && !m_bSendMailToServer )
    {
        m_nIndexPCOnlineSync = 0;
        m_bSyncPCFromServer = true;
        g_obLogger(cSeverity::DEBUG) << "Process started: retrieving patientcard data sold online." << EOM;
        ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
        trayIcon->setIcon( QIcon( ":/hourglass.png" ) );
        g_poBlnsHttp->getPatientCardsSoldOnline();
    }
*/
    //---------------------------------------------------------------------------------------------
    // Check if timer of mail send is reached the value set
    if( m_nIndexSendMailSync >= m_nTimerSendMailCheck && !m_bSyncPCFromServer && !m_bSyncPCToServer && !m_bSendMailToServer )
    {
        m_nIndexSendMailSync = 0;
        m_bSendMailToServer = true;
        g_obLogger(cSeverity::DEBUG) << "Process started: sending mail." << EOM;
        ui->lblStatusSync->setPixmap( QPixmap( ":/hourglass.png" ) );
        trayIcon->setIcon( QIcon( ":/hourglass.png" ) );
        g_poBlnsHttp->processWaitingMails();
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
    if( m_bHttpEnabledByUser )
    {
        g_poBlnsHttp->setCommunicationEnabled();
        g_poBlnsHttp->checkHttpServerAvailability();
        ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
        actionStatusHttp->setIcon( QIcon( ":/status_yellow.png" ) );
        _setGUIEnabled();
    }
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
    if( _isInGroup( GROUP_USER ) &&
        QMessageBox::question( this, "Belenus Websync",
                               tr("Are you sure you want to close this application?\n\n"
                                  "With closing the application the data of the patientcards "
                                  "will not be synchronized with the KiwiSun server."),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
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

    g_obLanguage.reloadLanguage( m_qsLang );
    ui->retranslateUi( this );

    m_bReloadLanguage = false;
}

//=================================================================================================
void dlgMain::on_pbSaveSettings_clicked()
{
    _saveSettings();
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
//    m_nTimerPCOnlineSync = ui->ledTimerPCOnlineSync->text().toInt();
}
//=================================================================================================
void dlgMain::on_ledTimerMailSendCheck_textEdited(const QString &/*arg1*/)
{
    m_nTimerSendMailCheck = ui->ledTimerMailSendCheck->text().toInt();
}
//=================================================================================================
void dlgMain::on_pbSyncAllPatientCard_clicked()
{
    if( QMessageBox::question( this, "Belenus Websync",
                           tr("Are you sure you want send all patientcard data to KiwiSun Web server?\n\n"
                              "Sending all patientcard data could take long time.\n"
                              "Please consider if you have enough time to wait process to finish."),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
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
}

//=================================================================================================
void dlgMain::on_pbClearPCData_clicked()
{
    g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo" );
}

//=================================================================================================
void dlgMain::on_pbSyncOnlinePC_clicked()
{
//    g_poBlnsHttp->getPatientCardsSoldOnline();
}

//=================================================================================================
void dlgMain::on_pbClearPatientCard_clicked()
{
    dlgLineEdit obDlgLineEdit( this );

    obDlgLineEdit.setTitle( tr("Enter barcode of the patientcard:") );
    if( obDlgLineEdit.exec() == QDialog::Accepted )
    {
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT barcode "
                                                                 "FROM patientcards WHERE "
                                                                 "barcode = \"%1\" " )
                                                         .arg( obDlgLineEdit.value().trimmed() ) );
            if( poQuery->size() < 1 )
            {
                QMessageBox::warning( this, "Belenus Websync",
                                      tr("The defined barcode not found in the database.") );
                return;
            }
            g_poBlnsHttp->sendPatientCardData( obDlgLineEdit.value().trimmed(), "", false );
            m_nIndexUpdateSyncDataCount = 0;
            ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

//=================================================================================================
void dlgMain::on_pbClearAllPatientCard_clicked()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT barcode "
                                                 "FROM patientcards WHERE "
                                                 "active = 0 AND "
                                                 "patientCardId>1 AND "
                                                 "barcode<>\"\" ");

    ui->lblProcessStatus->setVisible( true );
    ui->prgbProcess->setVisible( true );
    ui->prgbProcess->setMaximum( poQuery->size() );
    ui->prgbProcess->setValue( 0 );

    while( poQuery->next() )
    {
        try
        {
            g_poBlnsHttp->sendPatientCardData( poQuery->value(0).toString(), "", false );
            ui->prgbProcess->setValue( ui->prgbProcess->value()+1 );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
    ui->lblProcessStatus->setVisible( false );
    ui->prgbProcess->setVisible( false );
    m_nIndexUpdateSyncDataCount = 0;
    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
}

//=================================================================================================
//
//  H T T P
//
//=================================================================================================
// Executed when any error occures during http process
void dlgMain::on_BlnsHttpErrorOccured()
{
    QString qsTooltip       = "";
    m_bSyncPCToServer       = false;
    m_bSyncPCFromServer     = false;
    m_bSendMailToServer     = false;
    m_bRegisterLicenceKey   = false;
    m_bReactivateLicenceKey = false;
    m_bValidateLicenceKey   = false;
    m_qsHttpStatus          = g_poBlnsHttp->errorMessage();

    qsTooltip.append( m_qsHttpStatus );
    if( _isInGroup( GROUP_SYSTEM ) )
    {
        qsTooltip.append( "<br><br>" );
        qsTooltip.append( g_poBlnsHttp->settingsInfo() );
    }

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_red.png" ) );
    ui->lblStatusIconWebServer->setToolTip( qsTooltip );
    actionStatusHttp->setIcon( QIcon( ":/status_red.png" ) );
    ui->lblWebServerStatusText->setToolTip( g_poBlnsHttp->errorMessage() );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
    trayIcon->setIcon( QIcon( ":/websync_red.png" ) );
    _displayUserNotification( INFO_HttpFailed, g_poBlnsHttp->errorMessage() );
}
//=================================================================================================
// Executed when a http process finished
void dlgMain::on_BlnsHttpActionFinished(QString p_qsInfo)
{
    if( m_bRegisterLicenceKey )
    {
        _licenceRegistrationAdmin();
        _resetLicenceValidationTimers();
    }
    else if( m_bReactivateLicenceKey )
    {
        _licenceReactivationAdmin();
        _resetLicenceValidationTimers();
    }
    else if( m_bValidateLicenceKey )
    {
        _resetLicenceValidationTimers();
    }

    QString qsTooltip       = "";
    m_bSyncPCToServer       = false;
    m_bSyncPCFromServer     = false;
    m_bSendMailToServer     = false;
    m_bRegisterLicenceKey   = false;
    m_bReactivateLicenceKey = false;
    m_bValidateLicenceKey   = false;
    m_qsHttpStatus          = tr("HTTP Connection established");

    qsTooltip.append( m_qsHttpStatus );
    if( _isInGroup( GROUP_SYSTEM ) )
    {
        qsTooltip.append( "<br><br>" );
        qsTooltip.append( g_poBlnsHttp->settingsInfo() );
    }

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_green.png" ) );
    ui->lblStatusIconWebServer->setToolTip( qsTooltip );
    actionStatusHttp->setIcon( QIcon( ":/status_green.png" ) );
    ui->lblWebServerStatusText->setToolTip( tr("HTTP Connection established") );
    trayIcon->setIcon( QIcon( ":/websync.png" ) );
    m_FlagHttpFailed = false;
    g_obLogger(cSeverity::INFO) << p_qsInfo << EOM;
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
}
//=================================================================================================
// Executed when patientcard data updated on server
void dlgMain::on_BlnsHttpStepProgress()
{
    m_bSyncPCToServer   = false;
    m_bSendMailToServer = false;

    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_green.png" ) );
    actionStatusHttp->setIcon( QIcon( ":/status_green.png" ) );
    ui->ledNumberOfCardsWaiting->setText( QString::number( g_poBlnsHttp->getNumberOfWaitingRecords() ) );
    ui->lblStatusSync->setPixmap( QPixmap( ":/ok.png" ) );
    trayIcon->setIcon( QIcon( ":/websync.png" ) );
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
    ui->ledTimerPCOnlineSync->setEnabled( false/*p_bEnabled*/ );
    ui->ledTimerMailSendCheck->setEnabled( p_bEnabled );
    ui->ledWebServerAddress->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
    ui->sliFileLogLevel->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );

    ui->pbSyncAllPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );
    actionUserSendAllPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );

    ui->pbSyncOnlinePC->setEnabled( false/*p_bEnabled && _isInGroup( GROUP_USER ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser*/ );
//    actionUserGetOnlinePatientCards->setEnabled( false/*p_bEnabled && _isInGroup( GROUP_USER ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser*/ );

    ui->pbClearPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );
    actionClearPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );

    ui->pbClearAllPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );
    actionClearAllPatientCard->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) && m_bHttpEnabledBySetting && m_bHttpEnabledByUser );

    ui->cmbOnlinePatientCardType->setEnabled( false /*p_bEnabled && _isInGroup( GROUP_USER )*/ );
    ui->cmbOnlinePaymentMethod->setEnabled( false /*p_bEnabled && _isInGroup( GROUP_USER )*/ );

    ui->tabLicence->setEnabled( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
    ui->gbLicenceInformation->setVisible( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );
    ui->gbLicenceActions->setVisible( p_bEnabled && _isInGroup( GROUP_SYSTEM ) );

    ui->pbExit->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );
    actionExit->setEnabled( p_bEnabled && _isInGroup( GROUP_USER ) );

    ui->lblIndexPCData->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->lblIndexPCOnline->setVisible( false/*_isInGroup( GROUP_SYSTEM )*/ );
    ui->lblIndexMailSendCheck->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->pbTest->setVisible( _isInGroup( GROUP_SYSTEM ) );
    ui->pbTest->setEnabled( _isInGroup( GROUP_SYSTEM ) );
}
//=================================================================================================
void dlgMain::_setActions()
{
    actionStatusSQL = new QAction(tr("Database connection"), this);
    actionStatusSQL->setIcon( QIcon( ":/status_yellow.png" ) );

    actionStatusHttp = new QAction(tr("Http connection"), this);
    actionStatusHttp->setIcon( QIcon( ":/status_yellow.png" ) );

    actionShowMainWindow = new QAction("Belenus WebSync", this);
    actionShowMainWindow->setIcon( QIcon( ":/websync.png" ) );
    connect( actionShowMainWindow, SIGNAL(triggered()), this, SLOT(slotShowMainWindow()) );

    actionUserSendAllPatientCard = new QAction(tr("Send all patientcard"), this);
    actionUserSendAllPatientCard->setIcon( QIcon( ":/patientcards.png" ) );
    connect( actionUserSendAllPatientCard, SIGNAL(triggered()), this, SLOT(on_pbSyncAllPatientCard_clicked()) );

    actionUserWaitingClearPatientCards = new QAction(tr("Clear waiting queue"), this);
    actionUserWaitingClearPatientCards->setIcon( QIcon( ":/trash.png" ) );
    connect( actionUserWaitingClearPatientCards, SIGNAL(triggered()), this, SLOT(on_pbClearPCData_clicked()) );

//    actionUserGetOnlinePatientCards = new QAction(tr("Get online sold patientcards"), this);
//    actionUserGetOnlinePatientCards->setIcon( QIcon( ":/onlinepcsync.png" ) );
//    connect( actionUserGetOnlinePatientCards, SIGNAL(triggered()), this, SLOT(on_pbSyncOnlinePC_clicked()) );

    actionClearPatientCard = new QAction(tr("Clear patientcard"), this);
    actionClearPatientCard->setIcon( QIcon( ":/patientcard_delete.png" ) );
    connect( actionClearPatientCard, SIGNAL(triggered()), this, SLOT(on_pbClearPatientCard_clicked()) );

    actionClearAllPatientCard = new QAction(tr("Clear all inactive patientcards"), this);
    actionClearAllPatientCard->setIcon( QIcon( ":/patientcards_delete.png" ) );
    connect( actionClearAllPatientCard, SIGNAL(triggered()), this, SLOT(on_pbClearAllPatientCard_clicked()) );

    actionExit = new QAction(tr("&Exit application"), this);
    actionExit->setIcon( QIcon( ":/exit.png" ) );
    connect( actionExit, SIGNAL(triggered()), this, SLOT(on_pbExit_clicked()) );
}
//=================================================================================================
void dlgMain::_setMenu()
{
    trayIcon                    = new QSystemTrayIcon(this);
    trayIconMenu                = new QMenu(this);
    menuConnection              = new QMenu(this);
    menuUserActions             = new QMenu(this);

    menuUserActions->setTitle( tr("User actions") );
    menuUserActions->setIcon( QIcon( ":/user.png" ) );
    menuUserActions->addAction( actionUserSendAllPatientCard );
    menuUserActions->addAction( actionUserWaitingClearPatientCards );
//    menuUserActions->addAction( actionUserGetOnlinePatientCards );
    menuUserActions->addAction( actionClearPatientCard );
    menuUserActions->addAction( actionClearAllPatientCard );

    trayIconMenu->addAction( actionShowMainWindow );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction( actionStatusSQL );
    trayIconMenu->addAction( actionStatusHttp );
    trayIconMenu->addSeparator();
    trayIconMenu->addMenu( menuUserActions );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction( actionExit );

    trayIcon->setContextMenu(trayIconMenu);
}
//=================================================================================================
void dlgMain::slotShowMainWindow()
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
    if( m_bHttpSuspendedByUser )
    {
        m_bHttpSuspendedByUser = false;
        ui->pbStartStopHTTP->setIcon( QIcon(":/pause.png") );
        ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
        actionStatusHttp->setIcon( QIcon( ":/status_yellow.png" ) );
        trayIcon->setIcon( QIcon( ":/websync_yellow.png" ) );
        g_poBlnsHttp->setCommunicationSuspended( false );
        _displayUserNotification( INFO_HttpContinued );
        g_poBlnsHttp->checkHttpServerAvailability();
        m_FlagHttpSuspended = false;
    }
    else
    {
        m_bHttpSuspendedByUser = true;
        ui->pbStartStopHTTP->setIcon( QIcon(":/start.png") );
        g_poBlnsHttp->setCommunicationSuspended();
        _displayUserNotification( INFO_HttpSuspended );
        m_FlagHttpContinued = false;
    }
}

//=================================================================================================
void dlgMain::on_cmbOnlinePatientCardType_currentIndexChanged(int /*index*/)
{/*
    if( m_bStartFinished )
    {
        m_uiPatientCardTypeId = ui->cmbOnlinePatientCardType->itemData( index ).toUInt();
        _setPCTypeForHttp();
        if( m_uiPatientCardTypeId > 0 )
        {
            m_FlagNoPCType = false;
            ui->lblOnlinePatientCardType->setStyleSheet( "QLabel {font: normal;}" );
        }
        else
        {
            ui->lblOnlinePatientCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }
*/}

//=================================================================================================
void dlgMain::_setPCTypeForHttp()
{/*
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT price, units, unitTime "
                                                          "FROM patientCardTypes WHERE "
                                                          "active=1 AND archive<>\"DEL\" AND patientCardTypeId=%1 " )
                                                 .arg( m_uiPatientCardTypeId ) );

    poQuery->first();

    g_poBlnsHttp->setOnlinePCType( m_uiPatientCardTypeId,
                                   poQuery->value( 0 ).toInt(),
                                   poQuery->value( 1 ).toInt(),
                                   poQuery->value( 2 ).toInt() );
*/}

//=================================================================================================
void dlgMain::on_cmbOnlinePaymentMethod_currentIndexChanged(int /*index*/)
{/*
    if( m_bStartFinished )
    {
        m_uiPaymentMethodId = ui->cmbOnlinePaymentMethod->itemData( index ).toUInt();
        g_poBlnsHttp->setOnlinePaymentMethod( m_uiPaymentMethodId );
        if( m_uiPaymentMethodId > 0 )
        {
            m_FlagNoPaymentMethod = false;
            ui->lblOnlinePaymentMethod->setStyleSheet( "QLabel {font: normal;}" );
        }
        else
        {
            ui->lblOnlinePaymentMethod->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }
*/}

//=================================================================================================
void dlgMain::on_sliFileLogLevel_valueChanged(int value)
{
    m_nLogLevel = value;
    ui->lblFileLogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)m_nLogLevel ) );
    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)m_nLogLevel);
}

//=================================================================================================
void dlgMain::_displayUserNotification(userInfo p_tUserInfo, QString p_qsInfoText, QSystemTrayIcon::MessageIcon icon)
{
    switch( p_tUserInfo )
    {
        case INFO_NoHttpServer:
            if( !m_FlagNoHttpServer )
            {
                m_FlagNoHttpServer = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http server address is not defined!\n"
                                          "Http connection is going to be disabled."),
                                       QSystemTrayIcon::Critical, 2000 );
            }
            break;

        case INFO_HttpFailed:
            if( !m_FlagHttpFailed )
            {
                m_FlagHttpFailed = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http error occured!\n%1").arg(p_qsInfoText),
                                       QSystemTrayIcon::Critical, 2000 );
            }
            break;

        case INFO_HttpDisabled:
            if( !m_FlagHttpDisabled )
            {
                m_FlagHttpDisabled = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http communication disabled."),
                                       QSystemTrayIcon::Warning, 2000 );
            }
            break;

        case INFO_HttpEnabled:
            if( !m_FlagHttpEnabled )
            {
                m_FlagHttpEnabled = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http communication enabled."),
                                       QSystemTrayIcon::Information, 2000 );
            }
            break;

        case INFO_HttpSuspended:
            if( !m_FlagHttpSuspended )
            {
                m_FlagHttpSuspended = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http communication suspended."),
                                       QSystemTrayIcon::Warning, 2000 );
            }
            break;

        case INFO_HttpContinued:
            if( !m_FlagHttpContinued )
            {
                m_FlagHttpContinued = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("Http communication continued."),
                                       QSystemTrayIcon::Information, 2000 );
            }
            break;
/*
        case INFO_NoPaymentMethod:
            if( !m_FlagNoPaymentMethod )
            {
                m_FlagNoPaymentMethod = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("No payment method set for online transactions.\n"
                                          "Http connection is going to be disabled."),
                                       QSystemTrayIcon::Critical, 2000 );
            }
            break;

        case INFO_NoPCType:
            if( !m_FlagNoPCType )
            {
                m_FlagNoPCType = true;
                trayIcon->showMessage( "Belenus Websync",
                                       tr("No patientcard type set for onlince transactions.\n"
                                          "Http connection is going to be disabled."),
                                       QSystemTrayIcon::Critical, 2000 );
            }
            break;
*/
        default:
            if( p_qsInfoText.length() > 0 )
            {
                trayIcon->showMessage( "Belenus Websync", p_qsInfoText, icon, 2000 );
            }
    }
}

//=================================================================================================
void dlgMain::_checkIfHttpDisabledByUser()
{
    bool bHttpEnabled = m_bHttpEnabledByUser;

    try
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"SYNC_Enabled\" " ) );
        if( poQuery->first() )
        {
            bHttpEnabled = poQuery->value( 0 ).toBool();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    if( m_bHttpEnabledByUser != bHttpEnabled )
    {
        m_bHttpEnabledByUser = bHttpEnabled;
        ui->chkHttpCommunicationEnabled->setChecked( m_bHttpEnabledByUser );

        if( m_bHttpEnabledByUser )
        {
            // Http just enabled
            g_poBlnsHttp->setCommunicationEnabled( true );
            _displayUserNotification( INFO_HttpEnabled );
            m_FlagHttpDisabled = false;
            _setGUIEnabled();
        }
        else
        {
            // Http just disabled
            ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
            actionStatusHttp->setIcon( QIcon( ":/status_yellow.png" ) );
            trayIcon->setIcon( QIcon( ":/websync_yellow.png" ) );
            g_poBlnsHttp->setCommunicationEnabled( false );
            _displayUserNotification( INFO_HttpDisabled );
            m_FlagHttpEnabled = false;
        }
    }
}

//=================================================================================================
void dlgMain::_checkIfHttpChangedByUser()
{
    QString qsAddress = "";

    try
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"SERVER_Address\" " ) );
        if( poQuery->first() )
        {
            qsAddress = poQuery->value( 0 ).toString();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    if( qsAddress.compare( ui->ledWebServerAddress->text() ) )
    {
        m_bServerAddressChanged = true;
        ui->ledWebServerAddress->setText( qsAddress );
        ui->lblServerAddress->setStyleSheet( "QLabel {font: normal;}" );
    }
}

//=================================================================================================
void dlgMain::_disableHttpBySetting()
{
    ui->lblStatusIconWebServer->setPixmap( QPixmap( ":/status_yellow.png" ) );
    actionStatusHttp->setIcon( QIcon( ":/status_yellow.png" ) );
    trayIcon->setIcon( QIcon( ":/websync_yellow.png" ) );
    g_poBlnsHttp->setCommunicationEnabled( false );
    m_bHttpEnabledBySetting = false;
}

//=================================================================================================
void dlgMain::on_ledWebServerAddress_textEdited(const QString &/*arg1*/)
//-------------------------------------------------------------------------------------------------
{
    ui->lblServerAddress->setStyleSheet( "QLabel {font: normal;}" );

    if( !m_bServerAddressChanged )
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"SERVER_Address\" " ).arg( ui->ledWebServerAddress->text().replace("\\\\","/") ) );
    }
    else
    {
        m_bServerAddressChanged = false;
    }

    if( ui->ledWebServerAddress->text().length() == 0 )
    {
        ui->lblServerAddress->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else
    {
        m_FlagNoHttpServer = false;
    }
}

//=================================================================================================
void dlgMain::on_chkHttpCommunicationEnabled_clicked()
//-------------------------------------------------------------------------------------------------
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"SYNC_Enabled\" " ).arg( ui->chkHttpCommunicationEnabled->isChecked() ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

//====================================================================================
QString dlgMain::_bytearrayToString(QString p_qsString)
//------------------------------------------------------------------------------------
{
    return p_qsString;
}

//====================================================================================
void dlgMain::on_ledPassword_returnPressed()
//------------------------------------------------------------------------------------
{
    on_pbAuthenticate_clicked();
}

//====================================================================================
bool dlgMain::_isAppicationRunning(QString p_qsAppName)
//------------------------------------------------------------------------------------
{
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");

    process.waitForStarted(1000);
    process.waitForFinished(1000);

    QString listOfRunningApps = QString( process.readAll() );

    g_obLogger(cSeverity::DEBUG) << "Check if [" << p_qsAppName << "] is running" << EOM;
//    g_obLogger(cSeverity::DEBUG) << listOfRunningApps << EOM;

    if( listOfRunningApps.contains( p_qsAppName) )
    {
        g_obLogger(cSeverity::DEBUG) << "RUNNING" << EOM;
        return true;
    }

    g_obLogger(cSeverity::DEBUG) << "NOT RUNNING" << EOM;
    return false;
}

//====================================================================================
void dlgMain::on_PatientCardUpdated(unsigned int p_uiPatientCardId, QString p_qsBarcode)
//------------------------------------------------------------------------------------
{
    _sendPCData( p_uiPatientCardId, p_qsBarcode );
}

//====================================================================================
void dlgMain::slotShowModuleNotification(QString p_qsMessage)
//------------------------------------------------------------------------------------
{
    _displayUserNotification( INFO_Custom, p_qsMessage );
}

//====================================================================================
void dlgMain::_saveSettings()
//------------------------------------------------------------------------------------
{
//    m_uiPatientCardTypeId = ui->cmbOnlinePatientCardType->itemData( ui->cmbOnlinePatientCardType->currentIndex() ).toUInt();
//    m_uiPaymentMethodId   = ui->cmbOnlinePaymentMethod->itemData( ui->cmbOnlinePaymentMethod->currentIndex() ).toUInt();

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    obPref.setValue( "Lang",                    m_qsLang );
    obPref.setValue( "ShowMainWindowOnStart",   m_bShowMainWindowOnStart );

    obPref.setValue( "WindowPosition/Mainwindow_left", x() );
    obPref.setValue( "WindowPosition/Mainwindow_top", y() );
    obPref.setValue( "WindowPosition/Mainwindow_width", width() );
    obPref.setValue( "WindowPosition/Mainwindow_height", height() );
    obPref.setValue( "TimerLicenceValidation", m_nIndexLicenceValidation );
    obPref.setValue( "TimerPCStatusSync", m_nTimerPCStatusSync );
    obPref.setValue( "TimerSendMailCheck", m_nTimerSendMailCheck );
    obPref.setValue( "LogLevel", m_nLogLevel );

//    obPref.setValue( "TimerPCOnlineSync", m_nTimerPCOnlineSync );
//    obPref.setValue( "OnlinePatientCardType", m_uiPatientCardTypeId );
//    obPref.setValue( "OnlinePaymentMethod", m_uiPaymentMethodId );

    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"SERVER_Address\" " ).arg( ui->ledWebServerAddress->text().replace("\\\\","/") ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

//=================================================================================================
void dlgMain::_licenceRegistrationAdmin()
//-------------------------------------------------------------------------------------------------
{
    QSqlQuery   *poQuery = NULL;

    QString qsLicenceId     = "";
    QString qsLicenceText   = ui->ledLicenceKeyNew->text();

    for( int i=0; i<qsLicenceText.length(); i++ )
    {
        if( qsLicenceText.at(i).isDigit() )
        {
            qsLicenceId.append( qsLicenceText.at(i) );
        }
    }

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "INSERT INTO licences ( `licenceId`, `serial`, `lastValidated`, `Type`, `Act`, `Cod` ) "
                                                   "VALUES ( %1, '%2', '%3' )" ).arg( qsLicenceId.toUInt() )
                                                                                .arg( qsLicenceText )
                                                                                .arg( QDate::currentDate().toString("yyyy-MM-dd") )
                                                                                .arg( "VALIDATED" )
                                                                                .arg( ui->ledCodeServer->text() )
                                                                                .arg( ui->ledCodeClient->text() )
                                         );
        if( poQuery )
        {
            m_uiLicenceId = poQuery->lastInsertId().toUInt();
        }

        ui->ledLicenceKeyCurrent->setText( qsLicenceText );
        ui->ledLicenceKeyCurrent->setText( "" );
        ui->ledCodeServer->setText( g_poBlnsHttp->licenceServerCode() );

        g_poBlnsHttp->setStudioLicence( m_uiLicenceId, ui->ledLicenceKeyCurrent->text() );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

//=================================================================================================
void dlgMain::_licenceReactivationAdmin()
//-------------------------------------------------------------------------------------------------
{
    ui->ledCodeServer->setText( g_poBlnsHttp->licenceServerCode() );

    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE licences SET Cod=\"%1\" WHERE licenceId=%2" ).arg( ui->ledCodeServer->text() ).arg( m_uiLicenceId ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

//=================================================================================================
void dlgMain::_resetLicenceValidationTimers()
//-------------------------------------------------------------------------------------------------
{

}

//=================================================================================================
void dlgMain::on_pbRegisterLicence_clicked()
//-------------------------------------------------------------------------------------------------
{
    if( ui->ledCodeServer->text().length() > 0 && ui->ledCodeClient->text().length() > 0 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("This licence key already registered at server.\n"
                                                      "If you want to re-register it, please use the\n"
                                                      "'Reactivate licence key' button.") );
        ui->pbActivateLicence->setFocus();
        return;
    }
    if( g_poBlnsHttp->licenceId() > 1 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("There is a valid licence key already registered for this client.\n"
                                                      "If you want to change it, please use the\n"
                                                      "'Change current key to new' button.") );
        ui->pbChangeLicence->setFocus();
        return;
    }
    if( ui->ledLicenceKeyNew->text().length() != 13 )
    {
        QMessageBox::critical( this, tr("Error"), tr("The licence key is invalid.\n"
                                                      "Please check and retype it again.") );
        ui->ledLicenceKeyNew->setFocus();
        return;
    }

    ui->lblStatusIconLicenceAction->setPixmap( QPixmap( ":/hourglass.png" ) );

    ui->ledCodeServer->setText( "" );
    ui->ledCodeClient->setText( QTime::currentTime().toString( "hhmmss" ) );

    m_bStartRegisterLicenceKey = true;
}

//=================================================================================================
void dlgMain::on_pbActivateLicence_clicked()
//-------------------------------------------------------------------------------------------------
{

    ui->lblStatusIconLicenceAction->setPixmap( QPixmap( ":/hourglass.png" ) );

    ui->ledCodeServer->setText( "" );
    ui->ledCodeClient->setText( QTime::currentTime().toString( "hhmmss" ) );

    m_bStartReactivateLicenceKey = true;
}

//=================================================================================================
void dlgMain::on_pbChangeLicence_clicked()
//-------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to replace the licence key?\n\n"
                                  "Please note this modification affect all data connected "
                                  "to this licence key. If you use a currently active licence "
                                  "key, the related data will be corrupted!"),
                               QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        ui->lblStatusIconLicenceAction->setPixmap( QPixmap( ":/hourglass.png" ) );

        try
        {
            g_poDB->executeQTQuery( QString( "UPDATE licences SET `serial`=\"%1\" WHERE licenceId=%2" ).arg( ui->ledLicenceKeyNew->text() ).arg( m_uiLicenceId ) );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        ui->ledLicenceKeyCurrent->setText( ui->ledLicenceKeyNew->text() );
        ui->ledLicenceKeyNew->setText( "" );
        ui->ledCodeServer->setText( "" );
        ui->ledCodeClient->setText( QTime::currentTime().toString( "hhmmss" ) );

        m_bStartReactivateLicenceKey = true;
    }
}

//=================================================================================================
void dlgMain::on_pbTest_clicked()
//-------------------------------------------------------------------------------------------------
{
//    QString qsResponse = "LICENCE_REGISTRATION_OK - 700919";
//    QMessageBox::information( this, "Teszt", qsResponse.right( qsResponse.length() - qsResponse.indexOf( '-' ) - 2 ) );

    ui->lblStatusIconLicenceAction->setPixmap( QPixmap( ":/status_green.png" ) );
//    _displayUserNotification( INFO_Custom, "This button is for testing purpose.\nNo current action to be tested." );

/*
    // Read text from database and test sha1
    try
    {
        QString      qsQuery            = "SELECT * FROM "
                                          "httpsendmail WHERE "
                                          "dateOfSending=\"" + QDate::currentDate().toString( "yyyy-MM-dd" ) + "\" AND "
                                          "active=1 AND "
                                          "archive='NEW' "
                                          "LIMIT 1 ";
        QSqlQuery   *poQuery            = g_poDB->executeQTQuery( qsQuery );
        QByteArray   qbaSha1Base        = "";

        poQuery->first();
        qbaSha1Base.append( poQuery->value(6).toString().toUtf8() );

        QString qsSha1Gen = QString(QCryptographicHash::hash(qbaSha1Base,QCryptographicHash::Sha1).toHex());

        _displayUserNotification( INFO_Custom, "Check logs for sha1 test values" );

        g_obLogger(cSeverity::DEBUG) << "qbaSha1Base:  [" << _bytearrayToString(qbaSha1Base) << "]" << EOM;
        g_obLogger(cSeverity::DEBUG) << "qsSha1Gen:    [" << qsSha1Gen << "]" << EOM;
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
*/


/*    QFile   file( "ansi.php" );

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString     sha1    = file.readLine();
    QByteArray  qbaTest = file.readLine();
    QString     gen = QString(QCryptographicHash::hash(qbaTest,QCryptographicHash::Sha1).toHex());

    _displayUserNotification( INFO_Custom, qbaTest+"\n"+sha1+"\n"+gen );
    g_obLogger(cSeverity::DEBUG) << "TEST: [" << _bytearrayToString(qbaTest) << "]" << EOM;
    g_obLogger(cSeverity::DEBUG) << "sha1: [" << sha1.left(40) << "]" << EOM;
    g_obLogger(cSeverity::DEBUG) << "gen:  [" << gen << "]" << EOM;

    g_poDB->executeQTQuery( QString("INSERT INTO settings SET identifier=\"Ekezet teszt\", value=\"%1\" ").arg( _bytearrayToString(qbaTest) ) );

    _displayUserNotification( INFO_Custom, tr("árvíztűrő tükörfúrógép\nÁRVÍZTŰRŐ TÜKÖRFÚRÓGÉP") );

    dlgLineEdit obDlgLineEdit(this);

    if( obDlgLineEdit.exec() == QDialog::Accepted )
    {
        _isAppicationRunning( obDlgLineEdit.value() );
    }

    _displayUserNotification( INFO_Custom, tr("") );
*/
}

