//====================================================================================
//
// Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2012
//
//====================================================================================
//
// Filename    : wndmain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo allomanya.
//====================================================================================

#include <QCryptographicHash>
#include <QSqlQuery>
#include <QMessageBox>

//====================================================================================

#include "wndmain.h"
#include "creportdaily.h"
#include "../framework/qtmysqlquerymodel.h"

//------------------------------------------------------------------------------------
cWndMain::cWndMain( QWidget *parent ) : QMainWindow( parent )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    m_qsRPSW                = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";
    m_bReportTabSwitching   = false;

    connect( this, SIGNAL(setCheckedReportDaily(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( this, SIGNAL(setCheckedReportLedger(bool)), this, SLOT(slotCheckReportLedger(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardType(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardInactive(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( this, SIGNAL(setCheckedReportPatientcardDetails(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );

    connect( cmbName, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );
    connect( ledPassword, SIGNAL(returnPressed()), this, SLOT(on_pbAuthenticate_clicked()) );

    _initActions();
    _initToolbar();
    _initFilterbar();
    _initTabInformation();

    on_tabReports_currentChanged( 0 );

    lblInformation1->setVisible( true );
    lblInformation2->setVisible( false );

    cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
    m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
    cmbName->setModel( m_poModel );

    ledPassword->setFocus();
}
//------------------------------------------------------------------------------------
cWndMain::~cWndMain()
//------------------------------------------------------------------------------------
{
}
//------------------------------------------------------------------------------------
void cWndMain::setLoginData(QString p_qsName, QString p_qsPassword)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::setLoginData" );

    if( p_qsName.length() > 0 )
    {
        for( int i=0; i<cmbName->count(); i++ )
        {
            if( cmbName->itemText(i).contains(p_qsName) )
            {
                cmbName->setCurrentIndex( i );
                break;
            }
        }
    }

    if( p_qsPassword.length() > 0 )
    {
        ledPassword->setText( p_qsPassword );
    }

    if( ledPassword->text().length() > 0 )
    {
        on_pbAuthenticate_clicked();
    }
}
//------------------------------------------------------------------------------------
void cWndMain::_initActions()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initActions" );

    // SIGNALS
    connect( action_Bookkeeping_Daily, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( action_Bookkeeping_Ledger, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportLedger(bool)) );

    connect( action_PatientcardTypes, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( action_Patientcards_Inactive, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( action_Patientcards_Details, SIGNAL(triggered(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );

    // ICONS
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    action_Bookkeeping_Daily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    action_Bookkeeping_Ledger->setIcon( QIcon("./resources/40x40_book_ledger.png") );

    action_PatientcardTypes->setIcon( QIcon("./resources/40x40_report_patientcardtypes.png") );
    action_Patientcards_Inactive->setIcon( QIcon("./resources/40x40_report_patientcard_inactive.png") );
    action_Patientcards_Details->setIcon( QIcon("./resources/40x40_report_patientcard_details.png") );

    // BEHAVIOUR
    action_FilterBar->setEnabled( false );

    action_Bookkeeping_Daily->setEnabled( false );
    action_Bookkeeping_Ledger->setEnabled( false );

    action_PatientcardTypes->setEnabled( false );
    action_Patientcards_Inactive->setEnabled( false );
    action_Patientcards_Details->setEnabled( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initToolbar()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initToolbar" );

    // SIGNALS
    connect( pbExit, SIGNAL(clicked()), this, SLOT(close()) );

    connect( pbBookkeepingDaily, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportDaily(bool)) );
    connect( pbBookkeepingLedger, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportLedger(bool)) );

    connect( pbPatientcardType, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardType(bool)) );
    connect( pbPatientcardsInactive, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardInactive(bool)) );
    connect( pbPatientcardsDetails, SIGNAL(clicked(bool)), this, SLOT(slotCheckReportPatientcardDetails(bool)) );

    // ICONS
    pbExit->setIcon( QIcon("./resources/40x40_shutdown.png") );

    pbBookkeepingDaily->setIcon( QIcon("./resources/40x40_book_daily.png") );
    pbBookkeepingLedger->setIcon( QIcon("./resources/40x40_book_ledger.png") );

    pbPatientcardType->setIcon( QIcon("./resources/40x40_report_patientcardtypes.png") );
    pbPatientcardsInactive->setIcon( QIcon("./resources/40x40_report_patientcard_inactive.png") );
    pbPatientcardsDetails->setIcon( QIcon("./resources/40x40_report_patientcard_details.png") );

    pbPrint->setIcon( QIcon("./resources/40x40_print.png") );

    // BEHAVIOUR
    pbBookkeepingDaily->setEnabled( false );
    pbBookkeepingLedger->setEnabled( false );

    pbPatientcardType->setEnabled( false );
    pbPatientcardsInactive->setEnabled( false );
    pbPatientcardsDetails->setEnabled( false );

    pbPrint->setEnabled( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initFilterbar()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initFilterbar" );

    // ICONS
    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );

    // BEHAVIOUR
    frameFilterbar->setVisible( false );
}
//------------------------------------------------------------------------------------
void cWndMain::_initTabInformation()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_initTabInformation" );

    // ICONS
    tabReports->setTabIcon( 0, QIcon("./resources/40x40_information.png") );
    pbAuthenticate->setIcon( QIcon("./resources/40x40_key.png") );

    // BEHAVIOUR
    _setAuthInfoType( AUTH_NEEDED );
}
//------------------------------------------------------------------------------------
void cWndMain::_setAuthInfoType(authType p_tAuthType)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setAuthInfoType" );

    cmbName->setEnabled( true );
    ledPassword->setEnabled( true );

    pbAuthenticate->setText( tr("Login") );

    if( p_tAuthType == AUTH_NEEDED )
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        lblAuthenticationInformation->setText( tr("Need authentication ...") );
    }
    else if( p_tAuthType == AUTH_OK )
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: green;}" );
        lblAuthenticationInformation->setText( tr("User successfully authenticated.") );

        cmbName->setEnabled( false );
        ledPassword->setEnabled( false );

        pbAuthenticate->setText( tr("Logout") );
    }
    else
    {
        lblAuthenticationInformation->setStyleSheet( "QLabel {font: bold; color: red;}" );
        if( p_tAuthType == AUTH_USER_NOTFOUND )
            lblAuthenticationInformation->setText( tr("The username entered not found in the Belenus database") );
        else if( p_tAuthType == AUTH_PASSWORD_INCORRECT )
            lblAuthenticationInformation->setText( tr("The password entered is incorrect") );
        else if( p_tAuthType == AUTH_CONNECTION_FAILED )
            lblAuthenticationInformation->setText( tr("Connection to Belenus database failed") );
        else
            lblAuthenticationInformation->setText( tr("Authentication failed. Please retry later ...") );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_tabReports_tabCloseRequested(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_tabReports_tabCloseRequested" );

    if( index > 0 )
    {
        if( m_repDaily && m_repDaily->index() == index )
            emit setCheckedReportDaily( false );
        else if( m_repLedger && m_repLedger->index() == index )
            emit setCheckedReportLedger( false );
        else if( m_repCardType && m_repCardType->index() == index )
            emit setCheckedReportPatientcardType( false );
        else if( m_repCardInactive && m_repCardInactive->index() == index )
            emit setCheckedReportPatientcardInactive( false );
        else if( m_repCardDetails && m_repCardDetails->index() == index )
            emit setCheckedReportPatientcardDetails( false );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_action_Toolbar_triggered(bool checked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_action_Toolbar_triggered" );

    frameToolbar->setVisible( checked );
}
//------------------------------------------------------------------------------------
void cWndMain::on_action_FilterBar_triggered(bool checked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_action_FilterBar_triggered" );

    frameFilterbar->setVisible( checked );
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbAuthenticate_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_pbAuthenticate_clicked" );

    authType    atRet = AUTH_NEEDED;

    if( pbAuthenticate->text().compare( tr("Login") ) == 0 )
    {
        atRet = _authenticateUser();
    }

    _setAuthInfoType( atRet );

    if( atRet == AUTH_OK )
    {
        _setReportsEnabled();
        ledPassword->setText( "" );
    }
    else
    {
        _setReportsEnabled( false );
    }
}
//------------------------------------------------------------------------------------
cWndMain::authType cWndMain::_authenticateUser()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_authenticateUser" );

    string  stName = cmbName->currentText().toStdString();
    stName = stName.substr( 0, stName.find( '(' ) - 1 );
    authType    atRet = AUTH_ERROR;
    QByteArray  obPwdHash = QCryptographicHash::hash( ledPassword->text().toAscii(), QCryptographicHash::Sha1 );

    if( QString::fromStdString(stName).compare( "root" ) == 0 )
    {
        if( m_qsRPSW.compare( QString( obPwdHash.toHex() ) ) == 0 )
        {
            atRet = AUTH_OK;
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
//------------------------------------------------------------------------------------
void cWndMain::_setReportsEnabled(bool p_bEnable)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setReportsEnabled" );

    lblInformation1->setVisible( !p_bEnable );
    lblInformation2->setVisible( p_bEnable );
    action_FilterBar->setEnabled( p_bEnable );
    action_FilterBar->setChecked( p_bEnable );
    on_action_FilterBar_triggered( p_bEnable );

    action_Bookkeeping_Daily->setEnabled( p_bEnable );
    action_Bookkeeping_Ledger->setEnabled( p_bEnable );

    action_PatientcardTypes->setEnabled( p_bEnable );
    action_Patientcards_Inactive->setEnabled( p_bEnable );
    action_Patientcards_Details->setEnabled( p_bEnable );

    pbBookkeepingDaily->setEnabled( p_bEnable );
    pbBookkeepingLedger->setEnabled( p_bEnable );

    pbPatientcardType->setEnabled( p_bEnable );
    pbPatientcardsInactive->setEnabled( p_bEnable );
    pbPatientcardsDetails->setEnabled( p_bEnable );

    pbPrint->setEnabled( p_bEnable );
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportDaily(bool p_bChecked)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportDaily" );

    m_bReportTabSwitching = true;

    action_Bookkeeping_Daily->setChecked( p_bChecked );
    pbBookkeepingDaily->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repDaily = new cReportDaily();

        m_qvReports.append( m_repDaily );
        m_repDaily->setIndex( tabReports->addTab( m_repDaily, QIcon("./resources/40x40_book_daily.png"), m_repDaily->name() ) );
        tabReports->setCurrentIndex( m_repDaily->index() );
    }
    else
    {
        m_qvReports.remove( m_repDaily->index()-1 );
        tabReports->removeTab( m_repDaily->index() );
        delete m_repDaily;
        m_repDaily = NULL;
        _updateReportIndexes();
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportLedger( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportLedger" );

    m_bReportTabSwitching = true;

    action_Bookkeeping_Ledger->setChecked( p_bChecked );
    pbBookkeepingLedger->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repLedger = new cReportLedger();

        m_qvReports.append( m_repLedger );
        m_repLedger->setIndex( tabReports->addTab( m_repLedger, QIcon("./resources/40x40_book_ledger.png"), m_repLedger->name() ) );
        tabReports->setCurrentIndex( m_repLedger->index() );
    }
    else
    {
        m_qvReports.remove( m_repLedger->index()-1 );
        tabReports->removeTab( m_repLedger->index() );
        delete m_repLedger;
        m_repLedger = NULL;
        _updateReportIndexes();
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardType( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardType" );

    m_bReportTabSwitching = true;

    action_PatientcardTypes->setChecked( p_bChecked );
    pbPatientcardType->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardType = new cReportPatientCardType();

        m_qvReports.append( m_repCardType );
        m_repCardType->setIndex( tabReports->addTab( m_repCardType, QIcon("./resources/40x40_report_patientcardtypes.png"), m_repCardType->name() ) );
        tabReports->setCurrentIndex( m_repCardType->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardType->index()-1 );
        tabReports->removeTab( m_repCardType->index() );
        delete m_repCardType;
        m_repCardType = NULL;
        _updateReportIndexes();
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardInactive( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardInactive" );

    m_bReportTabSwitching = true;

    action_Patientcards_Inactive->setChecked( p_bChecked );
    pbPatientcardsInactive->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardInactive = new cReportCardInactive();

        m_qvReports.append( m_repCardInactive );
        m_repCardInactive->setIndex( tabReports->addTab( m_repCardInactive, QIcon("./resources/40x40_report_patientcard_inactive.png"), m_repCardInactive->name() ) );
        tabReports->setCurrentIndex( m_repCardInactive->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardInactive->index()-1 );
        tabReports->removeTab( m_repCardInactive->index() );
        delete m_repCardInactive;
        m_repCardInactive = NULL;
        _updateReportIndexes();
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::slotCheckReportPatientcardDetails( bool p_bChecked )
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::slotCheckReportPatientcardDetails" );

    m_bReportTabSwitching = true;

    action_Patientcards_Details->setChecked( p_bChecked );
    pbPatientcardsDetails->setChecked( p_bChecked );

    if( p_bChecked )
    {
        m_repCardDetails = new cReportCardDetails();

        m_qvReports.append( m_repCardDetails );
        m_repCardDetails->setIndex( tabReports->addTab( m_repCardDetails, QIcon("./resources/40x40_report_patientcard_details.png"), m_repCardDetails->name() ) );
        tabReports->setCurrentIndex( m_repCardDetails->index() );
    }
    else
    {
        m_qvReports.remove( m_repCardDetails->index()-1 );
        tabReports->removeTab( m_repCardDetails->index() );
        delete m_repCardDetails;
        m_repCardDetails = NULL;
        _updateReportIndexes();
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::on_tabReports_currentChanged(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_tabReports_currentChanged" );

    m_bReportTabSwitching = true;

    if( index == 0 )
    {
        lblReportDescription->setText( tr("<i>Please select a report to show the related filters ...</i>") );
        _setFiltersEnabled( false );
    }
    else
    {
        cReport *obReport = m_qvReports.at(index-1);
        QString qsDescription = QString( "<b>%1</b>" ).arg( m_qvReports.at(index-1)->name() );

        if( m_qvReports.at(index-1)->description().length() > 0 )
        {
            qsDescription.append( QString( " - %1" ).arg( m_qvReports.at(index-1)->description() ) );
        }
        lblReportDescription->setText( qsDescription );
        _setFiltersEnabled( false );
        _setFiltersEnabledReport( obReport );
    }

    m_bReportTabSwitching = false;
}
//------------------------------------------------------------------------------------
void cWndMain::_setFiltersEnabled(bool p_bEnable)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setFiltersEnabled" );

    lblFilterDateStart->setVisible( p_bEnable );
    dtFilterDateStart->setEnabled( p_bEnable );
    dtFilterDateStart->setVisible( p_bEnable );

    lblFilterDateStop->setVisible( p_bEnable );
    dtFilterDateStop->setEnabled( p_bEnable );
    dtFilterDateStop->setVisible( p_bEnable );

    lblFilterDataName->setVisible( p_bEnable );
    ledFilterDataName->setEnabled( p_bEnable );
    ledFilterDataName->setVisible( p_bEnable );

    lblFilterDataType->setVisible( p_bEnable );
    cmbFilterDataTypes->setEnabled( p_bEnable );
    cmbFilterDataTypes->setVisible( p_bEnable );

    pbRefresh->setEnabled( p_bEnable );
    pbRefresh->setVisible( p_bEnable );
}
//------------------------------------------------------------------------------------
void cWndMain::_setFiltersEnabledReport(cReport *obReport)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_setFiltersEnabledReport" );

    spacerFilter1->changeSize( 0, 20 );
    spacerFilter2->changeSize( 0, 20 );

    if( obReport->isDateStartEnabled() )
    {
        lblFilterDateStart->setVisible( true );
        dtFilterDateStart->setEnabled( true );
        dtFilterDateStart->setVisible( true );
        lblFilterDateStart->setText( obReport->labelDateStartText() );
        dtFilterDateStart->setDate( obReport->filterDateStart() );
        spacerFilter1->changeSize( 20, 20 );
    }

    if( obReport->isDateStopEnabled() )
    {
        lblFilterDateStop->setVisible( true );
        dtFilterDateStop->setEnabled( true );
        dtFilterDateStop->setVisible( true );
        lblFilterDateStop->setText( obReport->labelDateStopText() );
        dtFilterDateStop->setDate( obReport->filterDateStop() );
        spacerFilter1->changeSize( 20, 20 );
    }

    if( obReport->isDataNameEnabled() )
    {
        lblFilterDataName->setVisible( true );
        ledFilterDataName->setEnabled( true );
        ledFilterDataName->setVisible( true );
        lblFilterDataName->setText( obReport->labelDataNameText() );
        ledFilterDataName->setText( obReport->filterName() );
        spacerFilter2->changeSize( 20, 20 );
    }

    if( obReport->isDataTypeEnabled() )
    {
        lblFilterDataType->setVisible( true );
        cmbFilterDataTypes->setEnabled( true );
        cmbFilterDataTypes->setVisible( true );
        lblFilterDataType->setText( obReport->labelDataTypeText() );

        QStringList qslDataTypes = obReport->filterType().split('#');

        cmbFilterDataTypes->clear();
        for( int i=0; i<qslDataTypes.count(); i++ )
        {
            QStringList qslDataType = qslDataTypes.at(i).split('|');
            cmbFilterDataTypes->addItem( qslDataType.at(1), qslDataType.at(0).toInt() );
        }
    }

    pbRefresh->setEnabled( true );
    pbRefresh->setVisible( true );
}
//------------------------------------------------------------------------------------
void cWndMain::_updateReportIndexes()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::_updateReportIndexes" );

    for( int i=0; i<m_qvReports.count(); i++ )
    {
        m_qvReports.at(i)->setIndex( i+1 );
    }
}
//------------------------------------------------------------------------------------
void cWndMain::on_dtFilterDateStart_dateChanged(const QDate &date)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_dtFilterDateStart_dateChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDateStart( date );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_dtFilterDateStop_dateChanged(const QDate &date)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_dtFilterDateStop_dateChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDateStop( date );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_ledFilterDataName_textEdited(const QString &arg1)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_ledFilterDataName_textEdited" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->setFilterDataName( arg1 );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_cmbFilterDataTypes_currentIndexChanged(int index)
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_cmbFilterDataTypes_currentIndexChanged" );

    if( m_bReportTabSwitching ) return;

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    QString qsDataType = QString( "%1|%2" ).arg( cmbFilterDataTypes->itemData(index).toInt() ).arg( cmbFilterDataTypes->itemText(index) );

    obReport->setFilterDataType( qsDataType );

    if( chkAutoRefresh->isChecked() )
        on_pbRefresh_clicked();
}
//------------------------------------------------------------------------------------
void cWndMain::on_pbRefresh_clicked()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cWndMain::on_pbRefresh_clicked" );

    cReport *obReport = m_qvReports.at( tabReports->currentIndex()-1 );

    obReport->refreshReport();
}
//------------------------------------------------------------------------------------
