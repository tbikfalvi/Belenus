//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : frmpanel.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
// Panelok kezeleset vegzo osztaly
//====================================================================================

#include <QPalette>
#include <QMessageBox>

//====================================================================================

#include "belenus.h"
#include "frmpanel.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardhistory.h"
#include "db/dbledger.h"
//#include "db/dbattendance.h"
#include "db/dbpanelstatussettings.h"

#include <iostream>

//====================================================================================
cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId )
    : QFrame()
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );

    verticalLayout   = new QVBoxLayout( this );
    lblTitle         = new QLabel( this );
    lblCurrStatus    = new QLabel( this );
    lblCurrTimer     = new QLabel( this );
    lblNextStatusLen = new QLabel( this );
    lblInfo          = new QLabel( this );
    spacer1          = new QSpacerItem( 20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer2          = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer3          = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer4          = new QSpacerItem( 20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding );

    verticalLayout->setContentsMargins( 0, 0, 0, 0 );
    verticalLayout->addWidget( lblTitle );
    verticalLayout->addItem( spacer1 );
    verticalLayout->addWidget( lblCurrStatus );
    verticalLayout->addItem( spacer2 );
    verticalLayout->addWidget( lblCurrTimer );
    verticalLayout->addWidget( lblNextStatusLen );
    verticalLayout->addItem( spacer3 );
    verticalLayout->addWidget( lblInfo );
    verticalLayout->addItem( spacer4 );

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );

    m_uiId                  = 0;
    m_uiType                = 0;
    m_uiStatus              = 0;
    m_uiCounter             = 0;
    m_inTimerId             = 0;

    m_inMainProcessLength   = 0;
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
//    m_uiPatientToPay        = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;

    m_uiPaymentMethodId     = 0;

    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.inCountUnits     = 0;
    m_vrPatientCard.inUnitTime       = 0;

//    m_uiAttendanceId        = 0;
    m_uiLedgerId            = 0;

    m_pDBLedgerDevice       = new cDBLedgerDevice();

    m_pDBLedgerDevice->createNew();

    load( p_uiPanelId );

    inactivate();
    displayStatus();
}
//====================================================================================
cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );

    if( m_pDBLedgerDevice ) delete m_pDBLedgerDevice;

    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ ) if( m_obStatuses.at( i ) ) delete m_obStatuses.at( i );
}
//====================================================================================
bool cFrmPanel::isWorking() const
{
    return (m_uiStatus > 0);
}
//====================================================================================
bool cFrmPanel::isStatusCanBeSkipped()
{
    bool bRet = true;

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == 3 ||
        m_uiStatus == 0 ||
        (m_obStatuses.at(m_uiStatus)->activateCommand() == 4 && !g_obUser.isInGroup( cAccessGroup::ADMIN )) )
    {
        bRet = false;
    }

    return bRet;
}
//====================================================================================
bool cFrmPanel::isStatusCanBeReseted()
{
    bool bRet = false;

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == 1 ||
        m_uiStatus == 0 )
    {
        bRet = true;
    }

    return bRet;
}
//====================================================================================
void cFrmPanel::start()
{
    if( m_inMainProcessLength == 0 )
        return;

    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ )
    {
        if( m_obStatuses.at( i )->activateCommand() == 3 )
        {
            m_obStatuses.at( i )->setLength( m_inMainProcessLength );
        }
    }

    m_pDBLedgerDevice->setLicenceId( g_poPrefs->getLicenceId() );
    m_pDBLedgerDevice->setUserId( g_obUser.id() );
    m_pDBLedgerDevice->setPanelId( m_uiId );
//    m_pDBLedgerDevice->setPatientId( g_obPatient.id() );
    m_pDBLedgerDevice->setActive( true );

    g_poHardware->setMainActionTime( m_uiId-1, m_inMainProcessLength );

//    m_uiAttendanceId = g_uiPatientAttendanceId;

    activateNextStatus();
    m_inTimerId = startTimer( 1000 );
}
//====================================================================================
void cFrmPanel::reset()
{
    if( !isMainProcess() )
        return;

    activateNextStatus();
}
//====================================================================================
void cFrmPanel::clear()
{
/*
    cDBAttendance   obDBAttendance;

    obDBAttendance.load( m_uiAttendanceId );
    obDBAttendance.setLength( 0 );
    obDBAttendance.save();

    m_uiAttendanceId        = 0;
*/
    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.inCountUnits     = 0;
    m_vrPatientCard.inUnitTime       = 0;

    m_inMainProcessLength   = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;
    if( m_inCashToPay == 0 )
    {
        if( m_pDBLedgerDevice->cash() > 0 )
        {
            int inPriceTotal = m_pDBLedgerDevice->cash();
            QString qsComment = tr( "Revoking device (%1) usage." ).arg(getPanelName());

/*            if( QMessageBox::warning( this,
                                      tr("Question"),
                                      tr("The device usage has been payed before.\n"
                                         "Do you want to revoke the payment from the cassa?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )*/
            if( m_uiPaymentMethodId == 1 )
            {
                g_obCassa.cassaDecreaseMoney( inPriceTotal, qsComment );
            }

            cDBLedger   obDBLedger;

            obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
            obDBLedger.setLedgerTypeId( 1 );
            obDBLedger.setPaymentMethod( m_uiPaymentMethodId );
            obDBLedger.setUserId( g_obUser.id() );
            obDBLedger.setProductId( 0 );
            obDBLedger.setPatientCardTypeId( 0 );
            obDBLedger.setPatientCardId( 0 );
            obDBLedger.setPanelId( m_uiId );
            obDBLedger.setName( getPanelName() );
            obDBLedger.setNetPrice( -(m_inCashNetToPay-m_inCashDiscountToPay) );
            obDBLedger.setDiscount( m_inCashDiscountToPay );
            obDBLedger.setVatpercent( g_poPrefs->getDeviceUseVAT() );
            obDBLedger.setComment( qsComment );
            obDBLedger.setActive( true );
            obDBLedger.save();
        }
    }
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
//    m_uiPatientToPay        = 0;
    m_uiLedgerId            = 0;
    m_uiPaymentMethodId     = 0;
    m_pDBLedgerDevice->createNew();

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == 1 )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }

    displayStatus();
}
//====================================================================================
void cFrmPanel::next()
{
    activateNextStatus();
}
//====================================================================================
void cFrmPanel::inactivate()
{
    setFrameShadow( QFrame::Sunken );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#b9b9b9") ) );
    lblTitle->setPalette( obNewPalette );
}
//====================================================================================
void cFrmPanel::activate()
{
    setFrameShadow( QFrame::Raised );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#4387cb" ) ) );
    lblTitle->setPalette( obNewPalette );
}
//====================================================================================
int cFrmPanel::mainProcessTime()
{
    return m_inMainProcessLength;
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength )
{
    m_inMainProcessLength += p_inLength;
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()+p_inLength );

    displayStatus();
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength, const int p_inPrice )
{
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( NULL, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

// 'SOLARIUM GUEST'
    m_inCashLength += p_inLength;
    m_inCashTimeRemains = m_inCashLength;
    m_inCashNetToPay += p_inPrice;
//    m_inCashDiscountToPay += p_inPrice - g_obPatient.getDiscountPrice( p_inPrice );
    m_inCashToPay += /*g_obPatient.getDiscountPrice(*/p_inPrice/*) + (g_obPatient.getDiscountPrice(p_inPrice)/100)*g_poPrefs->getDeviceUseVAT()*/;
//    m_uiPatientToPay = g_obPatient.id();

    m_pDBLedgerDevice->setCash( m_inCashToPay );
    m_pDBLedgerDevice->setTimeCash( m_pDBLedgerDevice->timeCash()+p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength )
{
    m_vrPatientCard.uiPatientCardId  = p_uiPatientCardId;
    m_vrPatientCard.inCountUnits     = p_inCountUnits;
    m_vrPatientCard.inUnitTime       = p_inLength;
    m_inCardTimeRemains              = p_inLength;

    m_pDBLedgerDevice->setUnits( p_inCountUnits );
    m_pDBLedgerDevice->setTimeCard( p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
bool cFrmPanel::isTimeIntervallValid( const int p_inLength, int *p_inPrice, int *p_inCount )
{
    QSqlQuery   *poQuery;
    bool         bRet = false;

    *p_inPrice = 0;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT usePrice, COUNT(usePrice) FROM panelUses WHERE panelId=%1 AND useTime=%2" ).arg(m_uiId).arg(p_inLength) );
    if( poQuery->first() )
    {
        *p_inCount  = poQuery->value( 1 ).toInt();
        if( *p_inCount > 0 )
        {
            *p_inPrice  = poQuery->value( 0 ).toInt();
        }
        else
        {
            *p_inPrice  = 0;
        }

        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
void cFrmPanel::mousePressEvent ( QMouseEvent * p_poEvent )
{
    emit panelClicked( m_uiId - 1 );
    p_poEvent->ignore();
}
//====================================================================================
void cFrmPanel::timerEvent ( QTimerEvent * )
{
    if( g_poHardware->isHardwareMovedNextStatus( m_uiId-1 ) )
    {
        g_poHardware->setHardwareMovedNextStatus( m_uiId-1 );
        m_uiCounter = 0;
    }
    if( g_poHardware->isHardwareStopped( m_uiId-1 ) )
    {
        formatStatusString( m_obStatuses.at( m_uiStatus )->name() + tr("\n<< STOPPED >>") );
    }
    else
    {
        formatStatusString( m_obStatuses.at( m_uiStatus )->name() );
    }

    if( m_uiCounter )
    {
        if( !g_poHardware->isHardwareStopped( m_uiId-1 ) )
            m_uiCounter--;

        if( isMainProcess() )
        {
            m_inMainProcessLength--;
            if( m_inCashTimeRemains > 0 )
            {
                m_inCashTimeRemains--;
            }
            else
            {
                m_inCardTimeRemains--;
            }
        }

        formatTimerString( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
    }
    else
    {
        activateNextStatus();
    }
    g_poHardware->setCounter( m_uiId-1, (int)m_uiCounter );
}
//====================================================================================
void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            m_uiType = poQuery->value( 0 ).toInt();
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        else
        {
            lblTitle->setText( tr("Panel Not Found in Database") );
        }

        delete poQuery;
        poQuery = NULL;

        m_obStatuses.clear();
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, panelTypeId, seqNumber from panelStatuses WHERE panelTypeId=%1 ORDER BY seqNumber" ).arg( m_uiType ) );
        while( poQuery->next() )
        {
            unsigned int uiStatusId = poQuery->value( 0 ).toInt();

            cDBPanelStatuses  *poStatus = new cDBPanelStatuses();
            poStatus->load( uiStatusId );
            m_obStatuses.push_back( poStatus );
        }

        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::reload()
{
    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::displayStatus()
{
    if( m_uiStatus )
    {
        m_qsStatus = m_obStatuses.at( m_uiStatus )->name();

        m_qsTimer = QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) );
        unsigned int uiNextLen = 0;
        if( m_uiStatus != m_obStatuses.size() - 1 )
        {
            uiNextLen = m_obStatuses.at( m_uiStatus + 1 )->length();
            m_qsTimerNextStatus = QString( "%1:%2" ).arg( uiNextLen / 60, 2, 10, QChar( '0' ) ).arg( uiNextLen % 60, 2, 10, QChar( '0' ) );
        }
        else
        {
            m_qsTimerNextStatus = "";
        }
    }
    else
    {
        m_qsStatus = "";
        if( m_inMainProcessLength > 0 )
            m_qsTimer = QString( "%1:%2" ).arg( m_inMainProcessLength / 60, 2, 10, QChar( '0' ) ).arg( m_inMainProcessLength % 60, 2, 10, QChar( '0' ) );
        else
            m_qsTimer = "";
        m_qsTimerNextStatus = "";
    }

    QString qsInfo = "";

    if( m_inCashToPay > 0 )
    {
        qsInfo += tr("Cash to pay: ") + convertCurrency( m_inCashToPay, g_poPrefs->getCurrencyShort() );
    }

    QString     qsBackgroundColor;

    cDBPanelStatusSettings  obDBPanelStatusSettings;

    try
    {
        obDBPanelStatusSettings.loadStatus( m_uiStatus+1 );
        qsBackgroundColor = obDBPanelStatusSettings.backgroundColor();
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            switch( m_uiStatus )
            {
                case 0:
                    qsBackgroundColor = "#00ff00";
                    break;
                case 1:
                    qsBackgroundColor = "#ffff00";
                    break;
                case 2:
                    qsBackgroundColor = "#ff0000";
                    break;
                case 3:
                    qsBackgroundColor = "#ffff00";
                    break;
            }
        }
    }

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor(qsBackgroundColor) ) );
    setPalette( obFramePalette );

    formatStatusString( m_qsStatus );
    formatTimerString( m_qsTimer );
    formatNextLengthString( m_qsTimerNextStatus );
    formatInfoString( qsInfo );
}
//====================================================================================
void cFrmPanel::formatStatusString( QString p_qsStatusText )
{
    cDBPanelStatusSettings  obDBPanelStatusSettings;

    try
    {
        obDBPanelStatusSettings.loadStatus( m_uiStatus+1 );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            obDBPanelStatusSettings.createNew();
            obDBPanelStatusSettings.setStatusFontName( "Arial" );
            obDBPanelStatusSettings.setStatusFontSize( 18 );
            obDBPanelStatusSettings.setStatusFontColor( "#000000" );
        }
    }

    QFont   obFont;

    obFont = lblCurrStatus->font();
    obFont.setFamily( obDBPanelStatusSettings.statusFontName() );
    obFont.setPixelSize( obDBPanelStatusSettings.statusFontSize() );
    obFont.setBold( true );
    obFont.setCapitalization( QFont::AllUppercase );

    lblCurrStatus->setAlignment( Qt::AlignCenter );
    lblCurrStatus->setFont( obFont );
    lblCurrStatus->setText( QString("<font color=%1>%2</font>").arg(QColor( obDBPanelStatusSettings.statusFontColor()).name()).arg(p_qsStatusText) );
}
//====================================================================================
void cFrmPanel::formatTimerString( QString p_qsTimerText )
{
    cDBPanelStatusSettings  obDBPanelStatusSettings;

    try
    {
        obDBPanelStatusSettings.loadStatus( m_uiStatus+1 );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            obDBPanelStatusSettings.createNew();
            obDBPanelStatusSettings.setTimerFontName( "Arial" );
            obDBPanelStatusSettings.setTimerFontSize( 30 );
            obDBPanelStatusSettings.setTimerFontColor( "#000000" );
        }
    }

    QFont   obFont;

    obFont = lblCurrTimer->font();
    obFont.setFamily( obDBPanelStatusSettings.timerFontName() );
    obFont.setPixelSize( obDBPanelStatusSettings.timerFontSize() );
    obFont.setBold( true );

    lblCurrTimer->setAlignment( Qt::AlignCenter );
    lblCurrTimer->setFont( obFont );
    lblCurrTimer->setText( QString("<font color=%1>%2</font>").arg(QColor( obDBPanelStatusSettings.timerFontColor()).name()).arg(p_qsTimerText) );
}
//====================================================================================
void cFrmPanel::formatNextLengthString( QString p_qsNextLengthText )
{
    cDBPanelStatusSettings  obDBPanelStatusSettings;

    try
    {
        obDBPanelStatusSettings.loadStatus( m_uiStatus+1 );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            obDBPanelStatusSettings.createNew();
            obDBPanelStatusSettings.setNextFontName( "Arial" );
            obDBPanelStatusSettings.setNextFontSize( 18 );
            obDBPanelStatusSettings.setNextFontColor( "#000000" );
        }
    }

    QFont   obFont;

    obFont = lblNextStatusLen->font();
    obFont.setFamily( obDBPanelStatusSettings.nextFontName() );
    obFont.setPixelSize( obDBPanelStatusSettings.nextFontSize() );
    obFont.setBold( true );

    lblNextStatusLen->setAlignment( Qt::AlignCenter );
    lblNextStatusLen->setFont( obFont );
    lblNextStatusLen->setText( QString("<font color=%1>%2</font>").arg(QColor( obDBPanelStatusSettings.nextFontColor()).name()).arg(p_qsNextLengthText) );
}
//====================================================================================
void cFrmPanel::formatInfoString( QString p_qsInfoText )
{
    cDBPanelStatusSettings  obDBPanelStatusSettings;

    try
    {
        obDBPanelStatusSettings.loadStatus( m_uiStatus+1 );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            obDBPanelStatusSettings.createNew();
            obDBPanelStatusSettings.setInfoFontName( "Arial" );
            obDBPanelStatusSettings.setInfoFontSize( 10 );
            obDBPanelStatusSettings.setInfoFontColor( "#000000" );
        }
    }

    QFont   obFont;

    obFont = lblInfo->font();
    obFont.setFamily( obDBPanelStatusSettings.infoFontName() );
    obFont.setPixelSize( obDBPanelStatusSettings.infoFontSize() );
    obFont.setBold( true );

    lblInfo->setAlignment( Qt::AlignCenter );
    lblInfo->setFont( obFont );
    lblInfo->setText( QString("<font color=%1>%2</font>").arg(QColor( obDBPanelStatusSettings.infoFontColor()).name()).arg(p_qsInfoText) );
}
//====================================================================================
QString cFrmPanel::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}
//====================================================================================
void cFrmPanel::activateNextStatus()
{
    if( isMainProcess() )
    {
        // Kezeles vege
        closeAttendance();
    }

    m_uiStatus++;

    if( m_uiStatus == m_obStatuses.size() )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }
    else
    {
        m_uiCounter = m_obStatuses.at( m_uiStatus )->length();
        g_poHardware->setCurrentCommand( m_uiId-1, m_obStatuses.at( m_uiStatus )->activateCommand() );
    }

    displayStatus();
}
//====================================================================================
void cFrmPanel::cashPayed( const unsigned int p_uiLedgerId )
{
    m_inCashToPay           = 0;
//    m_inCashNetToPay        = 0;
//    m_inCashDiscountToPay   = 0;
//    m_uiPatientToPay        = 0;
    m_uiLedgerId = p_uiLedgerId;

    displayStatus();
}
//====================================================================================
bool cFrmPanel::isMainProcess()
{
    return ( m_obStatuses.at(m_uiStatus)->activateCommand()==3 ? true : false );
}
//====================================================================================
void cFrmPanel::closeAttendance()
{
    m_pDBLedgerDevice->setTimeLeft( m_inMainProcessLength );
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()-m_inMainProcessLength );
    if( m_inMainProcessLength > 0 )
    {
        m_pDBLedgerDevice->setComment( tr("Device usage stopped after %1 minutes. Unused time: %2 minutes.").arg(m_pDBLedgerDevice->timeReal()).arg(m_pDBLedgerDevice->timeLeft()) );
    }
    m_pDBLedgerDevice->save();

    if( m_uiLedgerId > 0 )
    {
        try
        {
            cDBLedger   obDBLedger;

            obDBLedger.load( m_uiLedgerId );
            obDBLedger.setLedgerDeviceId( m_pDBLedgerDevice->id() );
            obDBLedger.save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        m_uiLedgerId = 0;
    }

    QSqlQuery *poQuery;
    poQuery = g_poDB->executeQTQuery( QString( "SELECT workTime FROM panels WHERE panelId=%1" ).arg(m_uiId) );
    poQuery->first();

    unsigned int uiWorkTime = poQuery->value( 0 ).toUInt() + m_pDBLedgerDevice->timeReal();

    QString  qsQuery;

    qsQuery = "UPDATE panels SET ";

    qsQuery += QString( "workTime = \"%1\", " ).arg( uiWorkTime );
    qsQuery += QString( "archive = \"%1\" " ).arg( "MOD" );
    qsQuery += QString( " WHERE panelId = %1" ).arg( m_uiId );

    poQuery = g_poDB->executeQTQuery( qsQuery );

    if( poQuery ) delete poQuery;

    if( m_vrPatientCard.uiPatientCardId != 0 )
    {
        cDBPatientCard          obDBPatientCard;
        cDBPatientCardHistory   obDBPatientCardHistory;

        obDBPatientCard.load( m_vrPatientCard.uiPatientCardId );

        // Szerviz csoportba tartozo kartyanal nem kell levonni az egyseget es idot
        if( obDBPatientCard.patientCardTypeId() > 1 )
        {
            obDBPatientCard.setUnits( obDBPatientCard.units()-m_vrPatientCard.inCountUnits );
            obDBPatientCard.setTimeLeft( obDBPatientCard.timeLeft()-m_vrPatientCard.inUnitTime+m_inCardTimeRemains );

            obDBPatientCard.save();
        }

        QTime m_qtTemp = QTime( 0, m_vrPatientCard.inUnitTime/60, m_vrPatientCard.inUnitTime%60, 0 );

        obDBPatientCardHistory.createNew();
        obDBPatientCardHistory.setLicenceId( g_poPrefs->getLicenceId() );
        obDBPatientCardHistory.setPatientCardId( obDBPatientCard.id() );
        obDBPatientCardHistory.setUnits( m_vrPatientCard.inCountUnits );
        obDBPatientCardHistory.setTime( m_qtTemp.toString("hh:mm:ss") );
        obDBPatientCardHistory.setActive( true );

        obDBPatientCardHistory.save();
    }

    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.inCountUnits     = 0;
    m_vrPatientCard.inUnitTime       = 0;
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
    m_uiPaymentMethodId     = 0;
    m_pDBLedgerDevice->createNew();

    m_inMainProcessLength   = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;
}
//====================================================================================
void cFrmPanel::getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice, int *p_inDiscount )
{
    *p_uiPatientId  = 0//m_uiPatientToPay;
    *p_inPrice      = m_inCashNetToPay;
    *p_inDiscount   = m_inCashDiscountToPay;
}
//====================================================================================
bool cFrmPanel::isHasToPay()
{
    return ( m_inCashToPay > 0 ? true : false );
}
//====================================================================================
QString cFrmPanel::getPanelName()
{
    return lblTitle->text();
}
//====================================================================================
bool cFrmPanel::isCanBeStartedByTime()
{
    bool    bRet = true;

    if( /*g_obPatient.id() == 0 ||
        g_uiPatientAttendanceId == 0 ||*/
        m_inCashLength > 0 )
    {
        bRet = false;
    }

    return bRet;
}

bool cFrmPanel::isCanBeStartedByCard()
{
    bool    bRet = true;

    if( /*g_obPatient.id() == 0 ||
        g_uiPatientAttendanceId == 0 ||*/
        m_vrPatientCard.uiPatientCardId > 0 )
    {
        bRet = false;
    }

    return bRet;
}

void cFrmPanel::setPaymentMethod( const unsigned int p_uiPaymentMethodId )
{
    m_uiPaymentMethodId = p_uiPaymentMethodId;
    m_pDBLedgerDevice->setPaymentMethod( m_uiPaymentMethodId );
}
