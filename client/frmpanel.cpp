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
#include <QPixmap>

//====================================================================================

#include "belenus.h"
#include "frmpanel.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardhistory.h"
#include "db/dbledger.h"
//#include "db/dbattendance.h"
#include "db/dbshoppingcart.h"
#include "crud/dlgshoppingcart.h"
#include "db/dbpatientcardunits.h"

#include <iostream>

//====================================================================================
cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId ) : QFrame()
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );

    verticalLayout      = new QVBoxLayout( this );
    lblTitle            = new QLabel( this );
    lblCurrStatus       = new QLabel( this );
    lblCurrTimer        = new QLabel( this );
    lblNextStatusLen    = new QLabel( this );
    lblInfo             = new QLabel( this );
    spacer1             = new QSpacerItem( 20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer2             = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer3             = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer4             = new QSpacerItem( 20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layoutIcons         = new QHBoxLayout( this );
    spacerIcons         = new QSpacerItem( 100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    icoPanelStart       = new QPushButton( this );
    icoPanelNext        = new QPushButton( this );
    icoPanelStop        = new QPushButton( this );
    icoPanelCassa       = new QPushButton( this );
    icoShoppingCart     = new QPushButton( this );
    icoScheduledGuest   = new QPushButton( this );
    prgUsageMonitor     = new QProgressBar( this );

    lblCurrStatus->setWordWrap( true );
    lblInfo->setWordWrap( true );

    layoutIcons->setContentsMargins( 5, 0, 5, 5 );
    layoutIcons->setSpacing( 2 );
    layoutIcons->addWidget( icoPanelStart );
    layoutIcons->addWidget( icoPanelNext );
    layoutIcons->addWidget( icoPanelStop );
    layoutIcons->addWidget( icoPanelCassa );
    layoutIcons->addItem( spacerIcons );
    layoutIcons->addWidget( icoShoppingCart );
    layoutIcons->addWidget( icoScheduledGuest );

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
    verticalLayout->addLayout( layoutIcons );
    verticalLayout->addWidget( prgUsageMonitor );

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );
    lblTitle->setStyleSheet( "QLabel {font: bold; color: white; font-size:14px;}" );

    icoPanelStart->setIconSize( QSize(20,20) );
    icoPanelStart->setIcon( QIcon(QString("./resources/40x40_start.png")) );
    icoPanelStart->setFocusPolicy( Qt::NoFocus );
    connect( icoPanelStart, SIGNAL(clicked()), this, SLOT(slotPanelStartClicked()) );

    icoPanelNext->setIconSize( QSize(20,20) );
    icoPanelNext->setIcon( QIcon(QString("./resources/40x40_next.png")) );
    icoPanelNext->setFocusPolicy( Qt::NoFocus );
    connect( icoPanelNext, SIGNAL(clicked()), this, SLOT(slotPanelNextClicked()) );

    icoPanelStop->setIconSize( QSize(20,20) );
    icoPanelStop->setIcon( QIcon(QString("./resources/40x40_stop.png")) );
    icoPanelStop->setFocusPolicy( Qt::NoFocus );
    connect( icoPanelStop, SIGNAL(clicked()), this, SLOT(slotPanelStopClicked()) );

    icoPanelCassa->setIconSize( QSize(20,20) );
    icoPanelCassa->setIcon( QIcon(QString("./resources/40x40_cassa.png")) );
    icoPanelCassa->setFocusPolicy( Qt::NoFocus );
    connect( icoPanelCassa, SIGNAL(clicked()), this, SLOT(slotPanelCassaClicked()) );

    icoShoppingCart->setIconSize( QSize(20,20) );
    icoShoppingCart->setIcon( QIcon(QString("./resources/40x40_shoppingcart.png")) );
    icoShoppingCart->setFocusPolicy( Qt::NoFocus );
    connect( icoShoppingCart, SIGNAL(clicked()), this, SLOT(slotShoppingCartClicked()) );

    icoScheduledGuest->setIconSize( QSize(20,20) );
    icoScheduledGuest->setIcon( QIcon(QString("./resources/40x40_hourglass.png")) );
    icoScheduledGuest->setFocusPolicy( Qt::NoFocus );
    connect( icoScheduledGuest, SIGNAL(clicked()), this, SLOT(slotScheduledGuestClicked()) );

    prgUsageMonitor->setTextVisible( false );
    prgUsageMonitor->setFormat( "" );
    prgUsageMonitor->setMinimum( 0 );
    prgUsageMonitor->setValue( 0 );
    prgUsageMonitor->setFixedHeight( 8 );

    m_uiId                  = 0;
    m_uiType                = 0;
    m_uiStatus              = 0;
    m_uiCounter             = 0;
    m_inTimerId             = 0;

    m_inMainProcessLength   = 0;
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
    m_uiPatientToPay        = 0;
    m_uiCurrentPatient      = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;

    m_uiPaymentMethodId     = 0;

    m_bIsItemInShoppingCart = false;
    m_bIsPatientWaiting     = false;
    m_bIsNeedToBeCleaned    = false;
    m_bIsDeviceStopped      = false;

    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.qslUnitIds       = QStringList();
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
    return (m_obStatuses.at(m_uiStatus)->activateCommand() > 0);
}
//====================================================================================
bool cFrmPanel::isStatusCanBeSkipped()
{
    bool bRet = true;

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == 3 ||
        m_obStatuses.at(m_uiStatus)->activateCommand() == 0 ||
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
        m_obStatuses.at(m_uiStatus)->activateCommand() == 0 )
    {
        bRet = true;
    }

    return bRet;
}
//====================================================================================
void cFrmPanel::start()
{
    cTracer obTrace( "cFrmPanel::start" );

    m_bIsDeviceStopped = false;
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
    m_pDBLedgerDevice->setPatientId( m_uiCurrentPatient );
    m_pDBLedgerDevice->setActive( true );

    g_poHardware->setMainActionTime( m_uiId-1, m_inMainProcessLength );

    g_obLogger(cSeverity::INFO) << "Start device ID[" << m_uiId-1 << "] Time[" << m_inMainProcessLength << "]" << EOM;

    activateNextStatus();
    m_inTimerId = startTimer( 1000 );
}
//====================================================================================
void cFrmPanel::continueStoppedDevice()
{
    cTracer obTrace( QString("cFrmPanel::continueStoppedDevice - %1").arg(m_uiId-1) );

    g_poHardware->continueStoppedDevice( m_uiId-1 );
}
//====================================================================================
void cFrmPanel::reset()
{
    cTracer obTrace( "cFrmPanel::reset" );

    if( !isMainProcess() )
    {
        clear();
    }
    else
    {
        emit signalSetCounterText( m_uiId-1, "" );
        activateNextStatus();
    }
}
//====================================================================================
void cFrmPanel::clear()
{
    cTracer obTrace( "cFrmPanel::clear" );

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == 1 ||
        m_obStatuses.at(m_uiStatus)->activateCommand() == 4 )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }

    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.qslUnitIds       = QStringList();
    m_vrPatientCard.inUnitTime       = 0;

    m_inMainProcessLength   = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;

    emit signalSetCounterText( m_uiId-1, "" );
    emit signalSetWaitTime( m_uiId-1, 0 );

    if( m_inCashToPay == 0 )
    {
        if( m_pDBLedgerDevice->cash() > 0 )
        {
            g_obLogger(cSeverity::INFO) << "Device usage already payed, revoke payment" << EOM;
            int inPriceTotal = m_pDBLedgerDevice->cash();

            if( isItemInShoppingCart() )
            {
                g_obLogger(cSeverity::INFO) << "Payment removed from shopping cart" << EOM;
                QString qsQuery = QString( "SELECT shoppingCartItemId FROM shoppingcartitems WHERE panelId =%1 AND itemSumPrice=%2" ).arg(m_uiId).arg(inPriceTotal);

                QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

                if( poQuery->first() )
                {
                    cDBShoppingCart obDBShoppingCart;

                    obDBShoppingCart.load( poQuery->value( 0 ).toUInt() );
                    obDBShoppingCart.remove();
                    itemRemovedFromShoppingCart();
                }
            }
            else if( m_uiLedgerId > 0 )
            {
                g_obLogger(cSeverity::INFO) << "Payment revoked by ledgerId: " << m_uiLedgerId << EOM;
                g_obCassa.cassaProcessRevokeDeviceUse( m_uiLedgerId );
            }
        }
    }
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
    m_uiPatientToPay        = 0;
    m_uiCurrentPatient      = 0;
    m_uiLedgerId            = 0;
    m_uiPaymentMethodId     = 0;
    m_pDBLedgerDevice->createNew();

    if( m_qsInfo.compare( tr("NOT STERILE") ) )
    {
        setTextInformation( "" );
    }

    displayStatus();
}
//====================================================================================
void cFrmPanel::next()
{
    cTracer obTrace( "cFrmPanel::next" );

    activateNextStatus();
}
//====================================================================================
void cFrmPanel::clean()
//====================================================================================
{
    cTracer obTrace( "cFrmPanel::clean" );

    m_bIsNeedToBeCleaned = false;
    if( m_qsInfo.compare( tr("NOT STERILE") ) == 0 )
    {
        setTextInformation( "" );
    }
}
//====================================================================================
void cFrmPanel::inactivate()
{
    cTracer obTrace( "cFrmPanel::inactivate" );

    setFrameShadow( QFrame::Sunken );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#00003C") ) );
    lblTitle->setPalette( obNewPalette );
}
//====================================================================================
void cFrmPanel::activate()
{
    cTracer obTrace( "cFrmPanel::activate" );

    setFrameShadow( QFrame::Raised );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#0000FF" ) ) );
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
    cTracer obTrace( "cFrmPanel::setMainProcessTime(int)" );

    g_obLogger(cSeverity::INFO) << "Main process time set from [" << m_inMainProcessLength << "] to [" << m_inMainProcessLength + p_inLength << "]" << EOM;
    m_inMainProcessLength += p_inLength;
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()+p_inLength );

    displayStatus();
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength, const int p_inPrice )
{
    cTracer obTrace( "cFrmPanel::setMainProcessTime(int, int)" );

    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( NULL, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    g_obLogger(cSeverity::INFO) << "Device prepared with cash usage" << EOM;

    int inPriceTotal = p_inPrice;

    m_inCashLength += p_inLength;
    m_inCashTimeRemains = m_inCashLength;
    m_inCashNetToPay += p_inPrice;
    m_inCashToPay += inPriceTotal;
    m_inCashDiscountToPay += inPriceTotal - g_obGuest.getDiscountedPrice( inPriceTotal );
    m_uiPatientToPay = m_uiCurrentPatient = g_obGuest.id();

    m_pDBLedgerDevice->setCash( m_inCashToPay );
    m_pDBLedgerDevice->setTimeCash( m_pDBLedgerDevice->timeCash()+p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const unsigned int p_uiPatientCardId, const QStringList p_qslUnitIds, const int p_inLength )
{
    cTracer obTrace( "cFrmPanel::setMainProcessTime(uint,stringlist,int)" );

    m_vrPatientCard.uiPatientCardId  = p_uiPatientCardId;
    m_vrPatientCard.qslUnitIds       = p_qslUnitIds;
    m_vrPatientCard.inUnitTime       = p_inLength;
    m_inCardTimeRemains              = p_inLength;

    m_pDBLedgerDevice->setUnits( m_vrPatientCard.qslUnitIds.count() );
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
        formatStatusString( QString( "%1<br>!! %2 !!" ).arg( m_obStatuses.at( m_uiStatus )->name() ).arg( tr("PAUSED") ) );
        m_bIsDeviceStopped = true;
    }
    else
    {
        formatStatusString( m_obStatuses.at( m_uiStatus )->name() );
        m_bIsDeviceStopped = false;
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
    cTracer obTrace( "cFrmPanel::load" );

    m_uiId = p_uiPanelId;

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title, workTime, maxWorkTime FROM panels WHERE panelId=%1" ).arg( m_uiId ) );
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

        prgUsageMonitor->setValue( poQuery->value(2).toInt()/3600 );
        prgUsageMonitor->setMaximum( poQuery->value(3).toInt() );

        delete poQuery;
        poQuery = NULL;

        m_uiProcessWaitTime = 0;
        m_obStatuses.clear();
        m_obStatusSettings.clear();
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, length, panelTypeId, seqNumber from panelStatuses WHERE panelTypeId=%1 ORDER BY seqNumber" ).arg( m_uiType ) );
        while( poQuery->next() )
        {
            unsigned int uiStatusId = poQuery->value( 0 ).toInt();
            m_uiProcessWaitTime += poQuery->value( 1 ).toUInt();

            cDBPanelStatuses  *poStatus = new cDBPanelStatuses();
            poStatus->load( uiStatusId );
            m_obStatuses.push_back( poStatus );

            cDBPanelStatusSettings  *poStatusSettings = new cDBPanelStatusSettings();
            poStatusSettings->loadStatus( poStatus->id() );
            m_obStatusSettings.push_back( poStatusSettings );
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
    cTracer obTrace( "cFrmPanel::reload" );

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title, workTime, maxWorkTime FROM panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            lblTitle->setText( poQuery->value( 1 ).toString() );
            prgUsageMonitor->setValue( poQuery->value(2).toInt()/3600 );
            prgUsageMonitor->setMaximum( poQuery->value(3).toInt() );
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
void cFrmPanel::refreshDisplay()
//====================================================================================
{
    displayStatus();
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

    if( m_inCashToPay > 0 )
    {
        cCurrency   cPrice( m_inCashToPay );

        m_qsInfo = tr("Cash to pay: ") + cPrice.currencyFullStringShort();
    }

    QString     qsBackgroundColor = m_obStatusSettings.at(m_uiStatus)->backgroundColor();

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor(qsBackgroundColor) ) );
    setPalette( obFramePalette );

    formatStatusString( m_qsStatus );
    formatTimerString( m_qsTimer );
    formatNextLengthString( m_qsTimerNextStatus );
    formatInfoString( m_qsInfo );

    emit signalSetInfoText( m_uiId-1, m_qsInfo );
    emit signalStatusChanged( m_uiId-1, m_obStatuses.at(m_uiStatus)->id(), m_qsStatus );

    if( m_uiStatus == 0 && m_inMainProcessLength == 0 )
    {
        emit signalSetWaitTime( m_uiId-1, 0 );
    }
    else
    {
        emit signalSetWaitTime( m_uiId-1, _calculateWaitTime() );
    }

    if( !isWorking() && mainProcessTime() > 0 && !isHasToPay() )
    {
        icoPanelStart->setVisible( true );
    }
    else
    {
        icoPanelStart->setVisible( false );
    }
    icoPanelNext->setVisible( isStatusCanBeSkipped() );
    icoPanelStop->setVisible( isMainProcess() );
    icoPanelCassa->setVisible( isHasToPay() );
    icoShoppingCart->setVisible( m_bIsItemInShoppingCart );
    icoScheduledGuest->setVisible( m_bIsPatientWaiting );
}
//====================================================================================
void cFrmPanel::formatStatusString( QString p_qsStatusText )
{
    QFont   obFont;

    obFont = lblCurrStatus->font();
    obFont.setFamily( m_obStatusSettings.at(m_uiStatus)->statusFontName() );
    obFont.setPixelSize( m_obStatusSettings.at(m_uiStatus)->statusFontSize() );
    obFont.setBold( true );
    obFont.setCapitalization( QFont::AllUppercase );

    lblCurrStatus->setAlignment( Qt::AlignCenter );
    lblCurrStatus->setFont( obFont );
    lblCurrStatus->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(m_uiStatus)->statusFontColor()).name()).arg(p_qsStatusText) );
}
//====================================================================================
void cFrmPanel::formatTimerString( QString p_qsTimerText )
{
    QFont   obFont;

    obFont = lblCurrTimer->font();
    obFont.setFamily( m_obStatusSettings.at(m_uiStatus)->timerFontName() );
    obFont.setPixelSize( m_obStatusSettings.at(m_uiStatus)->timerFontSize() );
    obFont.setBold( true );

    emit signalSetCounterText( m_uiId-1, p_qsTimerText );

    lblCurrTimer->setAlignment( Qt::AlignCenter );
    lblCurrTimer->setFont( obFont );
    lblCurrTimer->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(m_uiStatus)->timerFontColor()).name()).arg(p_qsTimerText) );
}
//====================================================================================
void cFrmPanel::formatNextLengthString( QString p_qsNextLengthText )
{
    QFont   obFont;

    obFont = lblNextStatusLen->font();
    obFont.setFamily( m_obStatusSettings.at(m_uiStatus)->nextFontName() );
    obFont.setPixelSize( m_obStatusSettings.at(m_uiStatus)->nextFontSize() );
    obFont.setBold( true );

    lblNextStatusLen->setAlignment( Qt::AlignCenter );
    lblNextStatusLen->setFont( obFont );
    lblNextStatusLen->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(m_uiStatus)->nextFontColor()).name()).arg(p_qsNextLengthText) );
}
//====================================================================================
void cFrmPanel::formatInfoString( QString p_qsInfoText )
{
    QFont   obFont;

    obFont = lblInfo->font();
    obFont.setFamily( m_obStatusSettings.at(m_uiStatus)->infoFontName() );
    obFont.setPixelSize( m_obStatusSettings.at(m_uiStatus)->infoFontSize() );
    obFont.setBold( true );

    lblInfo->setAlignment( Qt::AlignCenter );
    lblInfo->setFont( obFont );
    lblInfo->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(m_uiStatus)->infoFontColor()).name()).arg(p_qsInfoText) );
}
//====================================================================================
/*QString cFrmPanel::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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
}*/
//====================================================================================
void cFrmPanel::activateNextStatus()
{
    cTracer obTrace( "cFrmPanel::activateNextStatus" );

    if( m_uiStatus == 0 )
    {
        // Gep hasznalat inditas
        m_qsInfo = "";
    }

    if( isMainProcess() )
    {
        // Kezeles vege
        closeAttendance();
        m_qsInfo = tr( "NOT STERILE" );
        m_bIsNeedToBeCleaned = true;
    }

    m_uiStatus++;

    if( m_uiStatus < m_obStatuses.size() && isMainProcess() )
    {
        if( m_uiLedgerId > 0 )
        {
            m_pDBLedgerDevice->save();
            g_obCassa.cassaConnectLedgerWithLedgerDevice( m_uiLedgerId, m_pDBLedgerDevice->id() );
        }
    }

    if( m_uiStatus > m_obStatuses.size()-1 )
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
    cTracer obTrace( "cFrmPanel::cashPayed" );

    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
    m_uiPatientToPay        = 0;
    m_uiLedgerId            = p_uiLedgerId;
    m_qsInfo                = "";

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
    cTracer obTrace( "cFrmPanel::closeAttendance" );

    m_pDBLedgerDevice->setTimeLeft( m_inMainProcessLength );
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()-m_inMainProcessLength );
    if( m_inMainProcessLength > 0 )
    {
        m_pDBLedgerDevice->setComment( tr("Device usage stopped after %1 minutes. Unused time: %2 minutes.").arg(m_pDBLedgerDevice->timeReal()).arg(m_pDBLedgerDevice->timeLeft()) );
    }
    m_pDBLedgerDevice->save();

    if( m_uiLedgerId > 0 )
    {
//        g_obCassa.cassaConnectLedgerWithLedgerDevice( m_uiLedgerId, m_pDBLedgerDevice->id() );
        m_uiLedgerId = 0;
    }

    QSqlQuery *poQuery;
    poQuery = g_poDB->executeQTQuery( QString( "SELECT workTime FROM panels WHERE panelId=%1" ).arg(m_uiId) );
    poQuery->first();

    unsigned int uiWorkTime = poQuery->value( 0 ).toUInt() + m_pDBLedgerDevice->timeReal();

    prgUsageMonitor->setValue( uiWorkTime/3600 );

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
            obDBPatientCard.setUnits( obDBPatientCard.units()-m_vrPatientCard.qslUnitIds.count() );
            obDBPatientCard.setTimeLeft( obDBPatientCard.timeLeft()-m_vrPatientCard.inUnitTime+m_inCardTimeRemains );

            obDBPatientCard.save();

            for( int i=0; i<m_vrPatientCard.qslUnitIds.count(); i++ )
            {
                cDBPatientcardUnit obDBPatientcardUnit;

                obDBPatientcardUnit.load( m_vrPatientCard.qslUnitIds.at(i).toInt() );
                obDBPatientcardUnit.setPanelId( m_uiId );
                obDBPatientcardUnit.setDateTime( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm::ss") );
                obDBPatientcardUnit.setActive( false );
                obDBPatientcardUnit.save();
            }
        }

        QTime m_qtTemp = QTime( 0, m_vrPatientCard.inUnitTime/60, m_vrPatientCard.inUnitTime%60, 0 );

        obDBPatientCardHistory.createNew();
        obDBPatientCardHistory.setLicenceId( g_poPrefs->getLicenceId() );
        obDBPatientCardHistory.setPatientCardId( obDBPatientCard.id() );
        obDBPatientCardHistory.setUnits( m_vrPatientCard.qslUnitIds.count() );
        obDBPatientCardHistory.setTime( m_qtTemp.toString("hh:mm:ss") );
        obDBPatientCardHistory.setActive( true );

        obDBPatientCardHistory.save();
    }

    m_vrPatientCard.uiPatientCardId  = 0;
    m_vrPatientCard.qslUnitIds       = QStringList();
    m_vrPatientCard.inUnitTime       = 0;
    m_inCashToPay                    = 0;
    m_inCashNetToPay                 = 0;
    m_inCashDiscountToPay            = 0;
    m_uiPaymentMethodId              = 0;
    m_uiCurrentPatient               = 0;
    m_pDBLedgerDevice->createNew();

    m_inMainProcessLength            = 0;
    m_inCashLength                   = 0;
    m_inCashTimeRemains              = 0;
    m_inCardTimeRemains              = 0;
}
//====================================================================================
void cFrmPanel::getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice, int *p_inDiscount )
{
    *p_uiPatientId  = m_uiPatientToPay;
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

//====================================================================================
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

//====================================================================================
void cFrmPanel::setPaymentMethod( const unsigned int p_uiPaymentMethodId )
{
    cTracer obTrace( "cFrmPanel::setPaymentMethod" );

    m_uiPaymentMethodId = p_uiPaymentMethodId;
    m_pDBLedgerDevice->setPaymentMethod( m_uiPaymentMethodId );
}
//====================================================================================
bool cFrmPanel::isItemInShoppingCart()
{
    return m_bIsItemInShoppingCart;
}
//====================================================================================
void cFrmPanel::itemAddedToShoppingCart()
{
    cTracer obTrace( "cFrmPanel::itemAddedToShoppingCart" );

    m_bIsItemInShoppingCart = true;
    icoShoppingCart->setVisible( true );
}
//====================================================================================
void cFrmPanel::itemRemovedFromShoppingCart()
{
    cTracer obTrace( "cFrmPanel::itemRemovedFromShoppingCart" );

    m_bIsItemInShoppingCart = false;
    icoShoppingCart->setVisible( false );
}
//====================================================================================
unsigned int cFrmPanel::panelId()
{
    return m_uiId;
}
//====================================================================================
void cFrmPanel::slotShoppingCartClicked()
{
    cTracer obTrace( "cFrmPanel::slotShoppingCartClicked" );

    emit signalOpenShoppingCart( m_uiId );
}
//====================================================================================
void cFrmPanel::slotPanelStartClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelStartClicked" );

    start();
}
//====================================================================================
void cFrmPanel::slotPanelNextClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelNextClicked" );

    if( QMessageBox::question( this, tr("Question"),
                               tr("Do you want to jump to the next status of the device?"),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
    {
        next();
    }
}
//====================================================================================
void cFrmPanel::slotPanelStopClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelStopClicked" );

    reset();
}
//====================================================================================
void cFrmPanel::slotPanelCassaClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelCassaClicked" );

    emit signalPaymentActivated( m_uiId );
}
//====================================================================================
void cFrmPanel::slotScheduledGuestClicked()
{
    cTracer obTrace( "cFrmPanel::slotScheduledGuestClicked" );

    emit signalOpenScheduleTable( m_uiId );
}
//====================================================================================
void cFrmPanel::addPatientToWaitingQueue( int p_inLengthCash, int p_inPrice, unsigned int p_uiPatientCardId, QString p_qsUnitIds, int p_inLenghtCard, unsigned int p_uiLedgerId, int p_inPayType )
{
    cTracer obTrace( "cFrmPanel::addPatientToWaitingQueue" );

    stWaitingQueue  *poTemp = new stWaitingQueue;

    poTemp->inLengthCash    = p_inLengthCash;
    poTemp->inPrice         = p_inPrice;
    poTemp->uiPatientCardId = p_uiPatientCardId;
    poTemp->qsUnitIds       = p_qsUnitIds;
    poTemp->inLengthCard    = p_inLenghtCard;
    poTemp->uiLedgerId      = p_uiLedgerId;
    poTemp->inPayType       = p_inPayType;

    m_vrWaitingQueue.push_back( poTemp );

    m_bIsPatientWaiting = true;
    displayStatus();
}
//====================================================================================
bool cFrmPanel::isPatientWaiting()
{
    return m_bIsPatientWaiting;
}
//====================================================================================
void cFrmPanel::setUsageFromWaitingQueue()
{
    cTracer obTrace( "cFrmPanel::setUsageFromWaitingQueue" );

    if( m_vrWaitingQueue.size() > 0 )
    {
        stWaitingQueue  *poTemp = m_vrWaitingQueue.at(m_vrWaitingQueue.size()-1);
        setMainProcessTime( poTemp->inLengthCash, poTemp->inPrice );
        setMainProcessTime( poTemp->uiPatientCardId, poTemp->qsUnitIds.split('|'), poTemp->inLengthCard );
        if( poTemp->inPrice == 0 )
        {
            setPaymentMethod( poTemp->inPayType );
            cashPayed( poTemp->uiLedgerId );
        }

        delete poTemp;
        m_vrWaitingQueue.pop_back();
    }
    if( m_vrWaitingQueue.size() < 1 )
    {
        m_bIsPatientWaiting = false;
    }

    displayStatus();
}
//====================================================================================
unsigned int cFrmPanel::_calculateWaitTime()
{
    unsigned int uiWaitTime = 0;

    for( int i=m_uiStatus+1; i<(int)m_obStatuses.size(); i++ )
    {
        uiWaitTime += m_obStatuses.at(i)->length();
        break;
    }

    return uiWaitTime;
}
//====================================================================================
void cFrmPanel::setTextInformation(QString p_qsInfoText)
//====================================================================================
{
    cTracer obTrace( "cFrmPanel::setTextInformation" );

    m_qsInfo = p_qsInfoText;
    displayStatus();
}
//====================================================================================
