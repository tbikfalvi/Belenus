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
#include "crud/dlgwaitlist.h"
#include "db/dbwaitlist.h"
#include "db/dbpaneluses.h"
#include "db/dbdiscount.h"

#include <iostream>

//====================================================================================
cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId ) : QFrame()
{
    verticalLayout      = new QVBoxLayout( this );
    lblTitle            = new QLabel( this );
    lblCurrStatus       = new QLabel( this );
    lblCurrTimer        = new QLabel( this );
    lblNextStatusLen    = new QLabel( this );
    lblInfo             = new QLabel( this );
    lblImage            = new QLabel( this );
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
    verticalLayout->addWidget( lblImage );
    verticalLayout->addItem( spacer4 );
    verticalLayout->addLayout( layoutIcons );
    verticalLayout->addWidget( prgUsageMonitor );

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );
    lblTitle->setStyleSheet( QString("QLabel {background-color: %1;font: bold; color: %2; font-size:14px;}")
                                    .arg( g_poPrefs->getActiveCaptionBackground() )
                                    .arg( g_poPrefs->getActiveCaptionColor() ) );

    lblImage->setMinimumHeight( 100 );
    lblImage->setScaledContents( true );
    // Fo ablakon nem latszodnak a kepek egyelore
    lblImage->setVisible( false );

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
    prgUsageMonitor->setFixedHeight( 11 );
    prgUsageMonitor->setStyleSheet( "QProgressBar { border: 2px solid grey; border-radius: 0.5px; } QProgressBar::chunk { background-color: #0000FF; }" );
    prgUsageMonitor->setVisible( g_poPrefs->isUsageVisibleOnMain() );

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
    m_uiShoppingCartItemId  = 0;

    m_bIsItemInShoppingCart     = false;
    m_bIsPatientWaiting         = false;
    m_bIsNeedToBeCleaned        = false;
    m_bIsDeviceStopped          = false;
    m_bIsTubeReplaceNeeded      = false;
    m_bIsTubeCleanupNeeded      = false;
    m_bDeviceHasCoolingProcess  = false;

    m_qsCashToPay               = "";
    m_qsTransactionId           = "";

    m_nMinuteOfPanel            = 0;

    m_nForceTimeSendCounter     = g_poPrefs->getForceTimeSendCounter();

    m_vrPatientCards.clear();

//    m_uiAttendanceId        = 0;
    m_uiLedgerId            = 0;

    m_pDBLedgerDevice       = new cDBLedgerDevice();

    m_pDBLedgerDevice->createNew();

    load( p_uiPanelId );

    if( !g_poPrefs->isPanelSterile( m_uiId ) )
    {
        m_bIsNeedToBeCleaned = true;
    }

    inactivate();
    displayStatus();
}
//====================================================================================
cFrmPanel::~cFrmPanel()
{
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
    return ( m_obStatuses.at(m_uiStatus)->allowedToSkip() && g_obUser.isInGroup((cAccessGroup::teAccessGroup)(m_obStatuses.at(m_uiStatus)->skipLevel())) );
}
//====================================================================================
bool cFrmPanel::isStatusCanBeStopped()
{
    return ( m_obStatuses.at(m_uiStatus)->allowedToStop() && g_obUser.isInGroup((cAccessGroup::teAccessGroup)(m_obStatuses.at(m_uiStatus)->stopLevel())) );
}
//====================================================================================
bool cFrmPanel::isStatusCanBeReseted()
{
    bool bRet = false;

    if( m_obStatuses.at(m_uiStatus)->activateCommand() == STATUS_VETKOZES ||
        m_obStatuses.at(m_uiStatus)->activateCommand() == STATUS_ALAP )
    {
        bRet = true;
    }

    return bRet;
}
//====================================================================================
bool cFrmPanel::isDeviceHasCoolingProcess()
{
    return m_bDeviceHasCoolingProcess;
}
//====================================================================================
void cFrmPanel::start()
{
    if( m_inTimerId != 0 )
    {
        g_obLogger(cSeverity::INFO) << "Device Id [" << m_uiId-1 << "] already started." << EOM;
        return;
    }

    m_bIsDeviceStopped = false;
    icoPanelStart->setVisible( false );
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

    unsigned char byStatus;

    g_poHardware->setMainActionTime( m_uiId-1, m_inMainProcessLength, &byStatus );

    QStringList qslPCUsed = QStringList();

    for( unsigned int j=0;j<m_vrPatientCards.size(); j++ )
    {
        stUsedPatientCards *stTemp = m_vrPatientCards.at(j);

        qslPCUsed.append( QString("%1#%2").arg( stTemp->uiPatientCardId )
                                          .arg( stTemp->qslUnitIds.join("|") ) );
    }

    g_obLogger(cSeverity::INFO) << "Device started Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "] Time ["
                                << m_inMainProcessLength/60
                                << "] CardIds ["
                                << qslPCUsed.join(" ")
                                << "] CashLength ["
                                << m_inCashLength/60
                                << "] CardLength ["
                                << (m_inMainProcessLength-m_inCashLength)/60
                                << "]"
                                << EOM;

    m_nMinuteOfPanel = m_inMainProcessLength/60;

    m_nForceTimeSendCounter = g_poPrefs->getForceTimeSendCounter();

    activateNextStatus();
    m_inTimerId = startTimer( 1000 );
}
//====================================================================================
void cFrmPanel::continueStoppedDevice()
{
    g_poHardware->continueStoppedDevice( m_uiId-1 );
}
//====================================================================================
void cFrmPanel::reset()
{
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

    g_obLogger(cSeverity::INFO) << "Device stopped Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "]"
                                << EOM;

    if( !isMainProcess() )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        m_inTimerId = 0;
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }

    for( unsigned int i=0; i<m_vrPatientCards.size(); i++ )
    {
        stUsedPatientCards *stTemp = m_vrPatientCards.at(i);

        for( int j=0; j<stTemp->qslUnitIds.count(); j++ )
        {
            cDBPatientcardUnit obDBPatientcardUnit;

            if( stTemp->qslUnitIds.at(j).toInt() > 0 )
            {
                obDBPatientcardUnit.load( stTemp->qslUnitIds.at(j).toInt() );
                obDBPatientcardUnit.setPrepared( false );
                obDBPatientcardUnit.save();
            }
        }
    }

    m_vrPatientCards.clear();

    m_inMainProcessLength   = 0;
    m_inCashLength          = 0;
    m_inCashTimeRemains     = 0;
    m_inCardTimeRemains     = 0;

    emit signalSetCounterText( m_uiId-1, "" );
    emit signalSetWaitTime( m_uiId-1, 0 );

    if( m_uiShoppingCartItemId > 0 )
    {
        g_obLogger(cSeverity::INFO) << "Remove payment from shopping cart" << EOM;
        QString qsQuery = QString( "SELECT shoppingCartItemId FROM shoppingcartitems WHERE shoppingCartItemId=%1" ).arg(m_uiShoppingCartItemId);

        QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->first() )
        {
            cDBShoppingCart obDBShoppingCart;

            obDBShoppingCart.load( poQuery->value( 0 ).toUInt() );
            obDBShoppingCart.remove();
        }
        itemRemovedFromShoppingCart();
    }

    if( m_inCashToPay == 0 )
    {
        if( m_pDBLedgerDevice->cash() > 0 )
        {
            g_obLogger(cSeverity::INFO) << "Device usage already payed, revoke payment" << EOM;
//            int inPriceTotal = m_pDBLedgerDevice->cash();

            if( m_uiLedgerId > 0 )
            {
                g_obLogger(cSeverity::DEBUG) << "Payment revoked by ledgerId: " << m_uiLedgerId << EOM;
                g_obCassa.cassaProcessRevokeDeviceUse( m_uiLedgerId );
            }
        }
    }
    m_inCashToPay           = 0;
    m_inCashNetToPay        = 0;
    m_qsCashToPay           = "";
    m_inCashDiscountToPay   = 0;
    m_uiPatientToPay        = 0;
    m_uiCurrentPatient      = 0;
    m_uiLedgerId            = 0;
    m_uiPaymentMethodId     = 0;
    m_uiShoppingCartItemId  = 0;
    m_qsTransactionId       = "";
    m_nMinuteOfPanel        = 0;
    m_pDBLedgerDevice->createNew();

    setTextInformation( "" );

    m_bIsDeviceStopped = false;
    icoPanelStart->setVisible( false );

    displayStatus();
}
//====================================================================================
void cFrmPanel::next()
{
    activateNextStatus();
}
//====================================================================================
void cFrmPanel::clean()
{
    m_bIsNeedToBeCleaned = false;
    displayStatus();
    g_poPrefs->setPanelSterile( m_uiId, true );
}
//====================================================================================
void cFrmPanel::cool()
{
    if( m_uiStatus == 0 )
    {
        for( int i=0; i<(int)m_obStatuses.size(); i++ )
        {
            if( m_obStatuses.at( i )->activateCommand() == STATUS_UTOHUTES )
            {
                m_uiStatus = i-1;
                break;
            }
        }
        if( m_uiStatus )
        {
            m_bIsDeviceStopped = false;
            icoPanelStart->setVisible( false );
            activateNextStatus();
            g_obLogger(cSeverity::INFO) << "Cooling process started by user [" << m_uiStatus << "] [" << m_uiCounter << "]" << m_uiLedgerId << EOM;
            m_inTimerId = startTimer( 1000 );
        }
    }
}
//====================================================================================
void cFrmPanel::inactivate()
{
    setFrameShadow( QFrame::Sunken );

    lblTitle->setStyleSheet( QString("QLabel {background-color: %1;font: bold; color: %2; font-size:14px;}")
                                    .arg( g_poPrefs->getInactiveCaptionBackground() )
                                    .arg( g_poPrefs->getInactiveCaptionColor() ) );
/*
    g_obLogger( cSeverity::DEBUG ) << "Inactivate panel ["
                                   << m_uiId
                                   << "] "
                                   << g_poPrefs->getInactiveCaptionBackground()
                                   << " - "
                                   << g_poPrefs->getInactiveCaptionColor()
                                   << EOM;
*/}
//====================================================================================
void cFrmPanel::activate()
{
    setFrameShadow( QFrame::Raised );

    lblTitle->setStyleSheet( QString("QLabel {background-color: %1;font: bold; color: %2; font-size:14px;}")
                                    .arg( g_poPrefs->getActiveCaptionBackground() )
                                    .arg( g_poPrefs->getActiveCaptionColor() ) );
/*
    g_obLogger( cSeverity::DEBUG ) << "Activate panel ["
                                   << m_uiId
                                   << "] "
                                   << g_poPrefs->getActiveCaptionBackground()
                                   << " - "
                                   << g_poPrefs->getActiveCaptionColor()
                                   << EOM;
*/}
//====================================================================================
int cFrmPanel::mainProcessTime()
{
    return m_inMainProcessLength;
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength )
{
    g_obLogger(cSeverity::INFO) << "Device set main time Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "] from ["
                                << m_inMainProcessLength/60
                                << "] to ["
                                << (m_inMainProcessLength+p_inLength)/60
                                << "]"
                                << EOM;
    m_inMainProcessLength += p_inLength;
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()+p_inLength );

    displayStatus();
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength, const int p_inPrice )
{
    if( p_inPrice > 0 && !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( NULL, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    int inPriceTotal        = p_inPrice;
    int nTimezoneDiscount   = 0;

    try
    {
        cDBDiscount obDiscount;

        obDiscount.loadTimeZone();
        nTimezoneDiscount = obDiscount.discount( inPriceTotal );
    }
    catch( cSevException &e )
    {
//        g_obLogger(e.severity()) << e.what() << EOM;
//        g_obGen.showTrayError( e.what() );
    }

    g_obLogger(cSeverity::INFO) << "Device set cash time Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "] time ["
                                << p_inLength/60
                                << "] cash ["
                                << p_inPrice
                                << "] discount_guest ["
                                << g_obGuest.getDiscountedPrice( inPriceTotal )
                                << "] discount_timezone ["
                                << nTimezoneDiscount
                                << "]"
                                << EOM;

    m_inCashLength += p_inLength;
    m_inCashTimeRemains = m_inCashLength;
    m_inCashNetToPay += p_inPrice;
    m_inCashToPay += inPriceTotal;
    m_inCashDiscountToPay += inPriceTotal - g_obGuest.getDiscountedPrice( inPriceTotal ) + nTimezoneDiscount;
    m_uiPatientToPay = m_uiCurrentPatient = g_obGuest.id();

    m_pDBLedgerDevice->setCash( m_inCashToPay );
    m_pDBLedgerDevice->setTimeCash( m_pDBLedgerDevice->timeCash()+p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const unsigned int p_uiPatientCardId, const QStringList p_qslUnitIds, const int p_inLength )
{
    g_obLogger(cSeverity::INFO) << "Device set card time Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "] time ["
                                << p_inLength/60
                                << "] units ["
                                << p_qslUnitIds.join("|")
                                << "]"
                                << EOM;

    stUsedPatientCards *stTemp = new stUsedPatientCards;

    stTemp->uiPatientCardId = p_uiPatientCardId;
    stTemp->qslUnitIds      = p_qslUnitIds;
    stTemp->inUnitTime      = p_inLength;

    m_vrPatientCards.push_back( stTemp );

    m_inCardTimeRemains    += p_inLength;

    for( int i=0; i<p_qslUnitIds.count(); i++ )
    {
        cDBPatientcardUnit obDBPatientcardUnit;

        if( p_qslUnitIds.at(i).toInt() > 0 )
        {
            obDBPatientcardUnit.load( p_qslUnitIds.at(i).toInt() );
            obDBPatientcardUnit.setPanelId( m_uiId );
            obDBPatientcardUnit.setPrepared( true );
            obDBPatientcardUnit.save();
        }
    }

    if( p_uiPatientCardId > 1 )
    {
       m_pDBLedgerDevice->setUnits( m_pDBLedgerDevice->units() + p_qslUnitIds.count() );
    }
    m_pDBLedgerDevice->setTimeCard( m_inCardTimeRemains );

    setMainProcessTime( p_inLength );
}
//====================================================================================
bool cFrmPanel::isTimeIntervallValid(const int p_inLength)
{
    QSqlQuery   *poQuery;
    bool         bRet = false;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelUses WHERE panelIds LIKE '\%%1\%' AND useTime=%2" ).arg(m_uiId).arg(p_inLength) );
    if( poQuery->first() )
    {
        g_obLogger(cSeverity::DEBUG) << "No record found" << EOM;
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
void cFrmPanel::mousePressEvent ( QMouseEvent * p_poEvent )
{
    emit signalMainWindowActivated();
    emit panelClicked( m_uiId - 1 );
    p_poEvent->ignore();
}
//====================================================================================
void cFrmPanel::timerEvent ( QTimerEvent * )
{
    try
    {
        if( g_poHardware->isHardwareMovedNextStatus( m_uiId-1 ) )
        {
            g_poHardware->setHardwareMovedNextStatus( m_uiId-1 );
            m_uiCounter = 0;
        }
        if( g_poHardware->isHardwareStopped( m_uiId-1 ) )
        {
            QString qsStatus = m_obStatuses.at( m_uiStatus )->name();

            if( g_poPrefs->isStopInLine() )
            {
                qsStatus.append( " " );
            }
            else
            {
                qsStatus.append( "<br>" );
            }
            qsStatus.append( QString( "!! %1 !!" ).arg( tr("PAUSED") ) );
            formatStatusString( qsStatus );
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
        bool bUpdatePanelTimer = false;

        if( g_poPrefs->isForceModuleSendTime() && m_nForceTimeSendCounter >= 0 )
        {
    //        if( (m_inMainProcessLength/60 + 1) != m_nMinuteOfPanel )
    //        {
                bUpdatePanelTimer = true;
    //            m_nMinuteOfPanel = m_inMainProcessLength/60 + 1;
    //        }
                m_nForceTimeSendCounter--;
        }

        g_poHardware->setCounter( m_uiId-1, (int)m_uiCounter, bUpdatePanelTimer );

        if( !isWorking() && mainProcessTime() > 0 && !isHasToPay() )
        {
            icoPanelStart->setVisible( true );
        }
        else
        {
            if( m_bIsDeviceStopped )
            {
                icoPanelStart->setVisible( true );
            }
            else
            {
                icoPanelStart->setVisible( false );
            }
        }
        icoPanelNext->setVisible( isStatusCanBeSkipped() );
        icoPanelStop->setVisible( isStatusCanBeStopped()/*isMainProcess()*/ );
        icoPanelCassa->setVisible( isHasToPay() );
        icoShoppingCart->setVisible( m_bIsItemInShoppingCart );
        icoScheduledGuest->setVisible( m_bIsPatientWaiting );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}
//====================================================================================
void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    QString qsImageFilename = "";

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title, workTime, maxWorkTime, cleanTime, maxCleanTime, imagePathFileName FROM panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            m_uiType = poQuery->value( 0 ).toInt();
            lblTitle->setText( poQuery->value( 1 ).toString() );
            qsImageFilename = poQuery->value( 6 ).toString();
        }
        else
        {
            lblTitle->setText( tr("Panel Not Found in Database") );
        }

        lblImage->clear();
        if( qsImageFilename.length() > 0 )
        {
            QPixmap *qpAd = new QPixmap( qsImageFilename );

            lblImage->setPixmap( *qpAd );
        }
        g_obLogger(cSeverity::DEBUG) << "Image for panel [" << qsImageFilename << "]" << EOM;

        m_bIsTubeReplaceNeeded = false;
        prgUsageMonitor->setMaximum( poQuery->value(3).toInt() );
        prgUsageMonitor->setValue( poQuery->value(2).toInt()/3600 );
        g_obLogger(cSeverity::DEBUG) << "Worktime for panel [" << poQuery->value( 1 ).toString() << "] is \'" << poQuery->value(2).toInt()/3600 << "\' [" << prgUsageMonitor->minimum() << "-" << prgUsageMonitor->maximum() << "]" << EOM;
        if( poQuery->value(2).toInt()/3600 >= prgUsageMonitor->maximum() )
        {
            prgUsageMonitor->setValue( prgUsageMonitor->maximum() );
            m_bIsTubeReplaceNeeded = true;
        }
        prgUsageMonitor->setVisible( g_poPrefs->isUsageVisibleOnMain() );

        unsigned int uiCleanTime = poQuery->value(4).toUInt();
        unsigned int uiCleanTimeMax = poQuery->value(5).toUInt();

        m_bIsTubeCleanupNeeded = false;
        g_obLogger(cSeverity::DEBUG) << "Cleantime for panel [" << poQuery->value( 1 ).toString() << "] is \'" << uiCleanTime/3600 << "\'" << EOM;
        if( uiCleanTime/3600 >= uiCleanTimeMax )
        {
            m_bIsTubeCleanupNeeded = true;
        }

        delete poQuery;
        poQuery = NULL;

        m_uiProcessWaitTime = 0;
        m_bDeviceHasCoolingProcess = false;
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

            if( poStatus->activateCommand() == STATUS_UTOHUTES )
            {
                m_bDeviceHasCoolingProcess = true;
            }

            cDBPanelStatusSettings  *poStatusSettings = new cDBPanelStatusSettings();
            poStatusSettings->loadStatus( poStatus->id() );
            m_obStatusSettings.push_back( poStatusSettings );
        }

        formatInfoString();

        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::reload()
{
    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title, workTime, maxWorkTime, cleanTime, maxCleanTime, imagePathFileName FROM panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            m_bIsTubeReplaceNeeded = false;
            poQuery->first();

            lblTitle->setText( poQuery->value( 1 ).toString() );
            prgUsageMonitor->setMaximum( poQuery->value(3).toInt() );
            prgUsageMonitor->setValue( poQuery->value(2).toInt()/3600 );
            g_obLogger(cSeverity::DEBUG) << "Worktime for panel [" << poQuery->value( 1 ).toString() << "] is \'" << poQuery->value(2).toInt()/3600 << "\' [" << prgUsageMonitor->minimum() << "-" << prgUsageMonitor->maximum() << "]" << EOM;
            if( poQuery->value(2).toInt()/3600 >= prgUsageMonitor->maximum() )
            {
                prgUsageMonitor->setValue( prgUsageMonitor->maximum() );
                m_bIsTubeReplaceNeeded = true;
            }

            unsigned int uiCleanTime = poQuery->value(4).toUInt();
            unsigned int uiCleanTimeMax = poQuery->value(5).toUInt();

            m_bIsTubeCleanupNeeded = false;
            g_obLogger(cSeverity::DEBUG) << "Cleantime for panel [" << poQuery->value( 1 ).toString() << "] is \'" << uiCleanTime/3600 << "\'" << EOM;
            if( uiCleanTime/3600 >= uiCleanTimeMax )
            {
                m_bIsTubeCleanupNeeded = true;
            }

            formatInfoString();
            prgUsageMonitor->setVisible( g_poPrefs->isUsageVisibleOnMain() );

            QString qsImageFilename = "";
            lblImage->clear();

            qsImageFilename = poQuery->value( 6 ).toString();
            if( qsImageFilename.length() > 0 )
            {
                QPixmap *qpAd = new QPixmap( qsImageFilename );

                lblImage->setPixmap( *qpAd );
            }
            g_obLogger(cSeverity::DEBUG) << "New image for panel [" << qsImageFilename << "]" << EOM;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::refreshDisplay()
//====================================================================================
{
    prgUsageMonitor->setVisible( g_poPrefs->isUsageVisibleOnMain() );
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

        // Fo ablakon nem latszodnak a kepek egyelore
//        lblImage->setVisible( false );
    }
    else
    {
        m_qsStatus = "";
        if( m_inMainProcessLength > 0 )
            m_qsTimer = QString( "%1:%2" ).arg( m_inMainProcessLength / 60, 2, 10, QChar( '0' ) ).arg( m_inMainProcessLength % 60, 2, 10, QChar( '0' ) );
        else
            m_qsTimer = "";
        m_qsTimerNextStatus = "";

        // Fo ablakon nem latszodnak a kepek egyelore
//        lblImage->setVisible( true );
    }

    if( m_inCashToPay > 0 )
    {
        cCurrency   cPrice( m_inCashToPay-m_inCashDiscountToPay );

        m_qsCashToPay = QString( tr("Cash to pay: ") + cPrice.currencyFullStringShort() );
    }

    QString     qsBackgroundColor = m_obStatusSettings.at(m_uiStatus)->backgroundColor();

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor(qsBackgroundColor) ) );
    setPalette( obFramePalette );

    formatStatusString( m_qsStatus );
    formatTimerString( m_qsTimer );
    formatNextLengthString( m_qsTimerNextStatus );
    formatInfoString();

    g_obLogger(cSeverity::DEBUG) << "signalStatusChanged - " << m_obStatuses.at(m_uiStatus)->id() << " | " << m_qsStatus << EOM;
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
        if( m_bIsDeviceStopped )
        {
            icoPanelStart->setVisible( true );
        }
        else
        {
            icoPanelStart->setVisible( false );
        }
    }
    icoPanelNext->setVisible( isStatusCanBeSkipped() );
    icoPanelStop->setVisible( isStatusCanBeStopped()/*isMainProcess()*/ );
    icoPanelCassa->setVisible( isHasToPay() );
    icoShoppingCart->setVisible( m_bIsItemInShoppingCart );
    icoScheduledGuest->setVisible( m_bIsPatientWaiting );
}
//====================================================================================
void cFrmPanel::formatStatusString( QString p_qsStatusText )
{
    if( p_qsStatusText.length() == 0 )
    {
        lblCurrStatus->setVisible( false );
    }
    else
    {
        lblCurrStatus->setVisible( true );
    }

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
    if( p_qsTimerText.length() == 0 )
    {
        lblCurrTimer->setVisible( false );
    }
    else
    {
        lblCurrTimer->setVisible( true );
    }

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
    if( p_qsNextLengthText.length() == 0 )
    {
        lblNextStatusLen->setVisible( false );
    }
    else
    {
        lblNextStatusLen->setVisible( true );
    }

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
void cFrmPanel::formatInfoString()
{
    g_obLogger(cSeverity::DEBUG) << "PanelId: "
                                 << m_uiId
                                 << " Tube: "
                                 << m_bIsTubeReplaceNeeded
                                 << " Clean: "
                                 << m_bIsNeedToBeCleaned
                                 << " Text: "
                                 << m_qsInfo
                                 << EOM;

    QFont   obFont;

    obFont = lblInfo->font();
    obFont.setFamily( m_obStatusSettings.at(m_uiStatus)->infoFontName() );
    obFont.setPixelSize( m_obStatusSettings.at(m_uiStatus)->infoFontSize() );
    obFont.setBold( true );

    QString qsMainInfoText      = "";
    QString qsSecondaryInfoText = "";

    if( m_bIsTubeReplaceNeeded )
    {
        qsMainInfoText.append( g_poPrefs->getPanelTextTubeReplace() );

        if( g_poPrefs->isTextTubeReplaceVisible() )
        {
            qsSecondaryInfoText.append( g_poPrefs->getPanelTextTubeReplace() );
        }
    }

    if( m_bIsTubeCleanupNeeded )
    {
        if( qsMainInfoText.length() > 0 ) qsMainInfoText.append( "<br>" );
        qsMainInfoText.append( g_poPrefs->getPanelTextTubeCleanup() );

        if( g_poPrefs->isTextTubeCleanupVisible() )
        {
            if( qsSecondaryInfoText.length() > 0 ) qsSecondaryInfoText.append( "<br>" );
            qsSecondaryInfoText.append( g_poPrefs->getPanelTextTubeCleanup() );
        }
    }

    if( m_bIsNeedToBeCleaned )
    {
        if( qsMainInfoText.length() > 0 ) qsMainInfoText.append( "<br>" );
        qsMainInfoText.append( g_poPrefs->getPanelTextSteril() );

        if( g_poPrefs->isTextSterilVisible() )
        {
            if( qsSecondaryInfoText.length() > 0 ) qsSecondaryInfoText.append( "<br>" );
            qsSecondaryInfoText.append( g_poPrefs->getPanelTextSteril() );
        }
    }

    if( m_qsCashToPay.length() > 0 )
    {
        if( qsMainInfoText.length() > 0 ) qsMainInfoText.append( "<br>" );
        qsMainInfoText.append( m_qsCashToPay );
    }

    if( m_qsInfo.length() > 0 )
    {
        if( qsMainInfoText.length() > 0 ) qsMainInfoText.append( "<br>" );
        qsMainInfoText.append( m_qsInfo );

        if( qsSecondaryInfoText.length() > 0 ) qsSecondaryInfoText.append( "<br>" );
        qsSecondaryInfoText.append( m_qsInfo );
    }

    lblInfo->setAlignment( Qt::AlignCenter );
    lblInfo->setFont( obFont );
    lblInfo->setText( QString("<font color=%1>%2</font>")
                      .arg(QColor( m_obStatusSettings.at(m_uiStatus)->infoFontColor()).name())
                      .arg(qsMainInfoText) );

    g_obLogger(cSeverity::DEBUG) << "MainInfo: " << qsMainInfoText << EOM;
    g_obLogger(cSeverity::DEBUG) << "SecondaryInfo: " << qsSecondaryInfoText << EOM;
    g_obLogger(cSeverity::DEBUG) << "Set info text for m_uiID=" << m_uiId-1 << " Text: " << qsSecondaryInfoText << EOM;
    emit signalSetInfoText( m_uiId-1, qsSecondaryInfoText );
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
    m_bIsDeviceStopped = false;

    if( m_uiStatus == 0 )
    {
        // Gep hasznalat inditas
        setTextInformation( "" );
    }

    if( isMainProcess() )
    {
        // Kezeles vege
        closeAttendance();
        m_bIsNeedToBeCleaned = true;
        g_poPrefs->setPanelSterile( m_uiId, false );
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
        m_inTimerId = 0;
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
    m_inCashNetToPay        = 0;
    m_inCashDiscountToPay   = 0;
    m_uiPatientToPay        = 0;
    m_uiLedgerId            = p_uiLedgerId;
    m_qsCashToPay           = "";

    displayStatus();
}
//====================================================================================
bool cFrmPanel::isMainProcess()
{
    int nCommand = m_obStatuses.at(m_uiStatus)->activateCommand();

    return ( nCommand==STATUS_BARNULAS ? true : false );
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

    QSqlQuery *poQuery;
    poQuery = g_poDB->executeQTQuery( QString( "SELECT workTime, cleanTime, maxCleanTime FROM panels WHERE panelId=%1" ).arg(m_uiId) );
    poQuery->first();

    unsigned int uiWorkTime = poQuery->value( 0 ).toUInt() + m_pDBLedgerDevice->timeReal();
    unsigned int uiCleanTime = poQuery->value( 1 ).toUInt() + m_pDBLedgerDevice->timeReal();
    unsigned int uiCleanTimeMax = poQuery->value(2).toUInt();

    g_obLogger(cSeverity::DEBUG) << "Worktime for panel  [" << lblTitle->text() << "] is \'" << uiWorkTime/3600 << "\' [" << prgUsageMonitor->minimum() << "-" << prgUsageMonitor->maximum() << "]" << EOM;
    g_obLogger(cSeverity::DEBUG) << "Cleantime for panel [" << lblTitle->text() << "] is \'" << uiCleanTime/3600 << "\' [" << prgUsageMonitor->minimum() << "-" << prgUsageMonitor->maximum() << "]" << EOM;

    m_bIsTubeCleanupNeeded = false;
    if( uiCleanTime/3600 >= uiCleanTimeMax )
    {
        m_bIsTubeCleanupNeeded = true;
    }

    m_bIsTubeReplaceNeeded = false;
    prgUsageMonitor->setValue( uiWorkTime/3600 );
    if( prgUsageMonitor->value() >= prgUsageMonitor->maximum() )
    {
        prgUsageMonitor->setValue( prgUsageMonitor->maximum() );
        m_bIsTubeReplaceNeeded = true;
    }
    formatInfoString();

    QString  qsQuery;
    try
    {
        qsQuery = "UPDATE panels SET ";

        qsQuery += QString( "workTime = \"%1\", " ).arg( uiWorkTime );
        qsQuery += QString( "cleanTime = \"%1\", " ).arg( uiCleanTime );
        qsQuery += QString( "archive = \"%1\" " ).arg( "MOD" );
        qsQuery += QString( " WHERE panelId = %1" ).arg( m_uiId );

        poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    int             nPatientHistoryType = PATIENTHISTORY_USEDDEVICEWITHCASH;
    unsigned int    uiPatientCardId     = 0;

    if( m_vrPatientCards.size() > 0 )
    {
        for( unsigned int i=0; i<m_vrPatientCards.size(); i++ )
        {
            stUsedPatientCards  *stTemp = m_vrPatientCards.at(i);
            cDBPatientCard       obDBPatientCard;

            uiPatientCardId = stTemp->uiPatientCardId;
            obDBPatientCard.load( stTemp->uiPatientCardId );

            // Szerviz csoportba tartozo kartyanal nem kell levonni az egyseget es idot
            if( !obDBPatientCard.isServiceCard() )
            {
                for( int j=0; j<stTemp->qslUnitIds.count(); j++ )
                {
                    cDBPatientcardUnit obDBPatientcardUnit;

                    obDBPatientcardUnit.load( stTemp->qslUnitIds.at(j).toInt() );

                    obDBPatientcardUnit.setPanelId( m_uiId );
                    obDBPatientcardUnit.setDateTime( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") );
                    obDBPatientcardUnit.setPrepared( false );
                    obDBPatientcardUnit.setActive( false );
                    obDBPatientcardUnit.save();
                }

                obDBPatientCard.synchronizeUnits();
                obDBPatientCard.synchronizeTime();
                obDBPatientCard.save();
                obDBPatientCard.sendDataToWeb();

                if( g_poPrefs->isAutoMailOnPCUse() || (g_poPrefs->isCardyGoSync() && obDBPatientCard.isCardOwnerRegisteredOnCardy()) )
                {
/*                    int nDestination = AUTO_MAIL_DESTINATION_MAIL_CARDY;

                    if( !g_poPrefs->isCardyGoSync() )           nDestination = AUTO_MAIL_DESTINATION_MAIL;
                    else if( !g_poPrefs->isAutoMailOnPCSell() ) nDestination = AUTO_MAIL_DESTINATION_CARDY;
*/
                    g_obLogger(cSeverity::INFO) << "PatientCard used, send auto mail about usage" << EOM;
                    obDBPatientCard.sendAutoMail( AUTO_MAIL_ON_PCUSE,
                                                  AUTO_MAIL_DESTINATION_MAIL,
                                                  QDate::currentDate().toString("yyyy-MM-dd"),
                                                  stTemp->qslUnitIds.count(),
                                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") );
                }
            }

            cDBPatientCardHistory   obDBPatientCardHistory;

            QTime m_qtTemp = QTime( 0, stTemp->inUnitTime/60, stTemp->inUnitTime%60, 0 );

            obDBPatientCardHistory.createNew();
            obDBPatientCardHistory.setLicenceId( g_poPrefs->getLicenceId() );
            obDBPatientCardHistory.setPatientCardId( obDBPatientCard.id() );
            obDBPatientCardHistory.setUnits( stTemp->qslUnitIds.count() );
            obDBPatientCardHistory.setTime( m_qtTemp.toString("hh:mm:ss") );
            obDBPatientCardHistory.setActive( true );

            obDBPatientCardHistory.save();

            m_uiCurrentPatient  = obDBPatientCard.patientId();
            nPatientHistoryType = PATIENTHISTORY_USEDDEVICEWITHCARD;
        }
    }

    if( m_uiCurrentPatient > 0 )
    {
        try
        {
            qsQuery = "INSERT INTO patienthistory SET ";

            qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
            qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiCurrentPatient );
            qsQuery += QString( "patientHistoryTypeId = \"%1\", " ).arg( nPatientHistoryType );
            qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiId );
            qsQuery += QString( "patientCardId = \"%1\" " ).arg( uiPatientCardId );

            poQuery = g_poDB->executeQTQuery( qsQuery );

            if( poQuery ) delete poQuery;
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }

    g_obLogger(cSeverity::INFO) << "Device action finished Id ["
                                << m_uiId
                                << "] TrId ["
                                << _transactionId()
                                << "] ledgerId ["
                                << m_uiLedgerId
                                << "] ledgerDeviceId ["
                                << m_pDBLedgerDevice->id()
                                << "]"
                                << EOM;

    m_vrPatientCards.clear();
    m_inCashToPay                   = 0;
    m_inCashNetToPay                = 0;
    m_inCashDiscountToPay           = 0;
    m_uiPaymentMethodId             = 0;
    m_uiCurrentPatient              = 0;
    m_uiLedgerId                    = 0;
    m_inMainProcessLength           = 0;
    m_inCashLength                  = 0;
    m_inCashTimeRemains             = 0;
    m_inCardTimeRemains             = 0;
    m_qsTransactionId               = "";
    m_pDBLedgerDevice->createNew();
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
    return true;  // Panel can be started with multiple cards
/*
    bool    bRet = true;

    if( m_vrPatientCard.uiPatientCardId > 0 )
    {
        bRet = false;
    }

    return bRet;
*/
}

//====================================================================================
void cFrmPanel::setPaymentMethod( const unsigned int p_uiPaymentMethodId )
{
    m_uiPaymentMethodId = p_uiPaymentMethodId;
    m_pDBLedgerDevice->setPaymentMethod( m_uiPaymentMethodId );
}
//====================================================================================
bool cFrmPanel::isItemInShoppingCart()
{
    return m_bIsItemInShoppingCart;
}
//====================================================================================
void cFrmPanel::itemAddedToShoppingCart(const unsigned int p_uiShoppingCardItemId)
{
    m_bIsItemInShoppingCart = true;
    if( p_uiShoppingCardItemId > 0 )
    {
        g_obLogger(cSeverity::INFO) << "Device usage payment moved to shopping cart ["
                                    << p_uiShoppingCardItemId
                                    << "]"
                                    << EOM;
        m_uiShoppingCartItemId = p_uiShoppingCardItemId;
    }
    icoShoppingCart->setVisible( true );
}
//====================================================================================
void cFrmPanel::itemRemovedFromShoppingCart()
{
    m_bIsItemInShoppingCart = false;
    m_uiShoppingCartItemId = 0;
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

    emit signalMainWindowActivated();

    if( isDeviceStopped() )
    {
        continueStoppedDevice();
    }
    else
    {
        start();
    }
}
//====================================================================================
void cFrmPanel::slotPanelNextClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelNextClicked" );

    emit signalMainWindowActivated();
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

    emit signalMainWindowActivated();
    reset();
}
//====================================================================================
void cFrmPanel::slotPanelCassaClicked()
{
    cTracer obTrace( "cFrmPanel::slotPanelCassaClicked" );
    g_obLogger(cSeverity::DEBUG) << "PanelID [" << m_uiId << "]" << EOM;

    emit signalPaymentActivated( m_uiId );
}
//====================================================================================
void cFrmPanel::slotScheduledGuestClicked()
{
    cTracer obTrace( "cFrmPanel::slotScheduledGuestClicked" );

    emit signalOpenScheduleTable( m_uiId );
}
//====================================================================================
//void cFrmPanel::addPatientToWaitingQueue( int p_inLengthCash, int p_inPrice, unsigned int p_uiPatientCardId, QString p_qsUnitIds, int p_inLenghtCard, unsigned int p_uiLedgerId, int p_inPayType )
void cFrmPanel::addPatientToWaitingQueue( bool p_bIsPatientWaiting )
{
    cTracer obTrace( "cFrmPanel::addPatientToWaitingQueue" );
/*
    stWaitingQueue  *poTemp = new stWaitingQueue;

    poTemp->inLengthCash    = p_inLengthCash;
    poTemp->inPrice         = p_inPrice;
    poTemp->uiPatientCardId = p_uiPatientCardId;
    poTemp->qsUnitIds       = p_qsUnitIds;
    poTemp->inLengthCard    = p_inLenghtCard;
    poTemp->uiLedgerId      = p_uiLedgerId;
    poTemp->inPayType       = p_inPayType;

    m_vrWaitingQueue.push_back( poTemp );
*/
    m_bIsPatientWaiting = p_bIsPatientWaiting;
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

    cDlgWaitlist    obDlgWaitlist( this );

    if( obDlgWaitlist.exec() == QDialog::Accepted )
    {
        try
        {
            cDBWaitlist obDBWaitlist;

            obDBWaitlist.load( obDlgWaitlist.selectedId() );

            if( obDBWaitlist.LengthCash() > 0 && !isTimeIntervallValid( obDBWaitlist.LengthCash()/60 ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("This time period did not saved in the database\n"
                                         "for the actually selected device.\n"
                                         "Please select valid value from the list.") );
                return;
            }
            if( obDBWaitlist.PanelTypeId() != m_uiType )
            {
                if( QMessageBox::question( this, tr("Warning"),
                                           tr( "The selected device usage has been scheduled\n"
                                               "for a different device type.\n\n"
                                               "Are you sure you want to start the device with the selected usage?"),
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
            }

            m_uiShoppingCartItemId = obDBWaitlist.ShoppingCartItemId();

            setMainProcessTime( obDBWaitlist.LengthCash(), obDBWaitlist.UsePrice() );
            if( obDBWaitlist.UnitIds().length() > 0 )
            {
                setMainProcessTime( obDBWaitlist.PatientCardId(), obDBWaitlist.UnitIds().split('|'), obDBWaitlist.LengthCard() );
            }

            if( obDBWaitlist.LedgerId() > 0 || m_uiShoppingCartItemId > 0 )
            {
                setPaymentMethod( obDBWaitlist.PayType() );
                cashPayed( obDBWaitlist.LedgerId() );
            }
            obDBWaitlist.remove();

            emit signalSelectedFromWaitingQueue();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }

/*    if( m_vrWaitingQueue.size() > 0 )
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
    }*/

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
void cFrmPanel::setTextInformation(QString p_qsInfoText, bool p_bCallDisplayStatus)
{
//    cTracer obTrace( "cFrmPanel::setTextInformation" );

    m_qsInfo = p_qsInfoText;

    if( p_bCallDisplayStatus )
        displayStatus();
}
//====================================================================================
QString cFrmPanel::_transactionId()
//------------------------------------------------------------------------------------
{
    if( m_qsTransactionId.length() == 0 )
    {
        m_qsTransactionId = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    }

    return m_qsTransactionId;
}
//====================================================================================
