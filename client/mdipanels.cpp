#include <QMessageBox>
#include <QMdiSubWindow>

#include "mdipanels.h"
#include "belenus.h"

cMdiPanels::cMdiPanels( QWidget *p_poParent )
    : QMdiArea( p_poParent )
{
    m_uiActivePanel = 0;
    m_bHWTimerStarted = false;
    m_nTimer = 0;
}

cMdiPanels::~cMdiPanels()
{
    if( m_bHWTimerStarted )
        killTimer( m_nTimer );
}

void cMdiPanels::initPanels()
{
    cTracer obTrace( "cMdiPanels::initPanels" );

    int inPanelCount = g_poPrefs->getPanelCount();

    if( g_poHardware->getPanelCount() < inPanelCount )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr( "There are more Panels defined in the database\n"
                                  "than supported by the current hardware.\n"
                                  "Only %1 panels will be displayed." ).arg( g_poHardware->getPanelCount() ) );
        inPanelCount = g_poHardware->getPanelCount();
    }

    m_obPanels.reserve( g_poPrefs->getPanelCount() );

    cFrmPanel *poFrame;
    QMdiSubWindow *poPanel;
    for( int i = 0; i < inPanelCount; i++ )
    {
        poFrame = new cFrmPanel( i + 1 );
        poFrame->setFrameShape( QFrame::Panel);
        poFrame->setFrameShadow( QFrame::Sunken );
        poFrame->setLineWidth( 3 );

        connect( poFrame, SIGNAL( panelClicked( unsigned int ) ),           this, SLOT( activatePanel( unsigned int ) ) );
        connect( poFrame, SIGNAL( signalOpenShoppingCart( uint ) ),         this, SLOT( openShoppingCart( uint ) ) );
        connect( poFrame, SIGNAL( signalPaymentActivated(uint) ),           this, SLOT( slotPaymentActivated(uint) ) );
        connect( poFrame, SIGNAL( signalOpenScheduleTable(uint) ),          this, SLOT( slotOpenScheduleTable(uint) ) );
        connect( poFrame, SIGNAL( signalStatusChanged(uint,uint,QString)),  this, SLOT( slotStatusChanged(uint,uint,QString) ) );
        connect( poFrame, SIGNAL( signalSetCounterText(uint,QString)),      this, SLOT( slotSetCounterText(uint,QString) ) );
        connect( poFrame, SIGNAL( signalSetWaitTime(uint,uint) ),           this, SLOT( slotSetWaitTime(uint,uint) ) );
        connect( poFrame, SIGNAL( signalSetInfoText(uint,QString) ),        this, SLOT( slotSetInfoText(uint,QString) ) );
        connect( poFrame, SIGNAL( signalSelectedFromWaitingQueue()),        this, SLOT( slotSelectedFromWaitingQueue()) );

        poPanel = new QMdiSubWindow( 0, Qt::FramelessWindowHint );
        poPanel->setWidget( poFrame );

        m_obPanels.push_back( poFrame );
        addSubWindow( poPanel );
        m_obPanels.at( i )->reset();
        m_obPanels.at( i )->show();
    }

    placeSubWindows();
    activatePanel( 0 );

    slotSelectedFromWaitingQueue();

    if( g_poHardware->isHardwareConnected() )
    {
        m_nTimer = startTimer( 300 );
    }
}

void cMdiPanels::placeSubWindows()
{
    QList<QMdiSubWindow*> obSubWindowList = subWindowList();
    if( obSubWindowList.size() )
    {
        int inPanelColumns = g_poPrefs->getPanelsPerRow();
        int inPanelRows    = ceil( (double)g_poPrefs->getPanelCount() / (double)inPanelColumns );
        int inPanelW       = width();
        int inPanelH       = height();
        int inPanelMargin  = 4;

        inPanelW -= (inPanelColumns+1)*inPanelMargin;
        inPanelW /= inPanelColumns;
        inPanelH -= (inPanelRows+1)*inPanelMargin;
        inPanelH /= inPanelRows;

        obSubWindowList.first();
        int inPosX = inPanelMargin;
        int inPosY = inPanelMargin;
        for( int i = 0; i < obSubWindowList.size(); i++ )
        {
            QMdiSubWindow *poSubWindow = obSubWindowList.at( i );
            poSubWindow->move( inPosX, inPosY );
            poSubWindow->resize( inPanelW, inPanelH );

            inPosX += inPanelMargin + inPanelW;
            if( inPosX + inPanelW > width() )
            {
                inPosX = inPanelMargin;
                inPosY += inPanelMargin + inPanelH;
            }
        }
    }
}

void cMdiPanels::timerEvent(QTimerEvent *)
{
    m_bHWTimerStarted = true;

    g_poHardware->HW_Kezel();
}

void cMdiPanels::start()
{
    m_obPanels.at( m_uiActivePanel )->start();
}

void cMdiPanels::continueStoppedDevice()
{
    m_obPanels.at( m_uiActivePanel )->continueStoppedDevice();
}

void cMdiPanels::reset()
{
    m_obPanels.at( m_uiActivePanel )->reset();
}

void cMdiPanels::clear()
{
    m_obPanels.at( m_uiActivePanel )->clear();
}

void cMdiPanels::next()
{
    m_obPanels.at( m_uiActivePanel )->next();
}

void cMdiPanels::clean()
{
    m_obPanels.at( m_uiActivePanel )->clean();
}

bool cMdiPanels::isNeedToBeCleaned()
{
    return m_obPanels.at( m_uiActivePanel )->isNeedToBeCleaned();
}

bool cMdiPanels::isDeviceStopped()
{
    return m_obPanels.at( m_uiActivePanel )->isDeviceStopped();
}

void cMdiPanels::reload()
{
    m_obPanels.at( m_uiActivePanel )->reload();
}

bool cMdiPanels::isMainProcess()
{
    return m_obPanels.at( m_uiActivePanel )->isMainProcess();
}

int cMdiPanels::mainProcessTime()
{
    return m_obPanels.at( m_uiActivePanel )->mainProcessTime();
}

void cMdiPanels::setMainProcessTime( const int p_inLength )
{
    m_obPanels.at( m_uiActivePanel )->setMainProcessTime( p_inLength );
}

void cMdiPanels::setMainProcessTime( const int p_inLength, const int p_inPrice )
{
    m_obPanels.at( m_uiActivePanel )->setMainProcessTime( p_inLength, p_inPrice );
}

/*bool cMdiPanels::isTimeIntervallValid( const int p_inLength, int *p_inPrice, int *p_inCount )
{
    return m_obPanels.at( m_uiActivePanel )->isTimeIntervallValid( p_inLength, p_inPrice, p_inCount );
}*/

void cMdiPanels::setMainProcessTime( const unsigned int p_uiPatientCardId, const QStringList p_qslUnitIds, const int p_inLength )
{
    m_obPanels.at( m_uiActivePanel )->setMainProcessTime( p_uiPatientCardId, p_qslUnitIds, p_inLength );
}

void cMdiPanels::getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice, int *p_inDiscount )
{
    m_obPanels.at( m_uiActivePanel )->getPanelCashData( p_uiPatientId, p_inPrice, p_inDiscount );
}

bool cMdiPanels::isHasToPay()
{
    return m_obPanels.at( m_uiActivePanel )->isHasToPay();
}

int cMdiPanels::activePanel()
{
    return m_uiActivePanel;
}

unsigned int cMdiPanels::activePanelId()
{
    return m_obPanels.at( m_uiActivePanel )->panelId();
}

bool cMdiPanels::isPanelWorking()
{
    bool    bRet = false;

    for( int i=0; i<(int)m_obPanels.size(); i++ )
    {
        if( m_obPanels.at( i )->isWorking() )
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

void cMdiPanels::refreshDisplay()
{
    for( int i=0; i<(int)m_obPanels.size(); i++ )
    {
        m_obPanels.at(i)->refreshDisplay();
    }
}

void cMdiPanels::setTextInformation(QString p_qsInfoText)
{
    m_obPanels.at( m_uiActivePanel )->setTextInformation( p_qsInfoText, true );
}

bool cMdiPanels::isPanelWorking( const unsigned int p_uiPanel )
{
    if( p_uiPanel < m_obPanels.size() )
        return m_obPanels.at( p_uiPanel )->isWorking();
    else
        return false;
}

bool cMdiPanels::isStatusCanBeSkipped( const unsigned int p_uiPanel )
{
    return m_obPanels.at( p_uiPanel )->isStatusCanBeSkipped();
}

bool cMdiPanels::isStatusCanBeReseted()
{
    return m_obPanels.at( m_uiActivePanel )->isStatusCanBeReseted();
}

void cMdiPanels::activatePanel( unsigned int p_uiPanel )
{
    m_obPanels.at( m_uiActivePanel )->inactivate();
    m_obPanels.at( p_uiPanel )->activate();

    m_uiActivePanel = p_uiPanel;

//    emit activePanelChanged( m_obPanels.at( m_uiActivePanel )->isWorking() );
    emit activePanelChanged();
}

void cMdiPanels::resizeEvent ( QResizeEvent *p_poEvent )
{
    placeSubWindows();
    p_poEvent->accept();
}

void cMdiPanels::keyPressEvent ( QKeyEvent *p_poEvent )
{
    int inNewPanel = m_uiActivePanel;

    switch( p_poEvent->key() )
    {
    case Qt::Key_Up:
        inNewPanel -= g_poPrefs->getPanelsPerRow();
        p_poEvent->accept();
        break;
    case Qt::Key_Down:
        inNewPanel += g_poPrefs->getPanelsPerRow();
        p_poEvent->accept();
        break;
    case Qt::Key_Left:
        if( inNewPanel % g_poPrefs->getPanelsPerRow() != 0 ) inNewPanel--;
        p_poEvent->accept();
        break;
    case Qt::Key_Right:
        if( inNewPanel % g_poPrefs->getPanelsPerRow() < g_poPrefs->getPanelsPerRow() - 1 ) inNewPanel++;
        p_poEvent->accept();
        break;
    default: p_poEvent->ignore();
    }

    if( inNewPanel >= 0 && inNewPanel < (int)m_obPanels.size() && inNewPanel != (int)m_uiActivePanel ) activatePanel( (unsigned int)inNewPanel );
}

void cMdiPanels::cashPayed( const unsigned int p_uiLedgerId )
{
    m_obPanels.at( m_uiActivePanel )->cashPayed( p_uiLedgerId );
}

void cMdiPanels::cashPayed( const unsigned int p_uiPanelId, const unsigned int p_uiLedgerId )
{
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            m_obPanels.at(i)->cashPayed( p_uiLedgerId );
            break;
        }
    }
}

QString cMdiPanels::getActivePanelCaption()
{
    return m_obPanels.at( m_uiActivePanel )->getPanelName();
}

QString cMdiPanels::getPanelCaption( const unsigned int p_uiPanelId )
{
    QString qsCaption = "";

    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            qsCaption = m_obPanels.at(i)->getPanelName();
            break;
        }
    }
    return qsCaption;
}

bool cMdiPanels::isCanBeStartedByTime()
{
    return m_obPanels.at( m_uiActivePanel )->isCanBeStartedByTime();
}

bool cMdiPanels::isCanBeStartedByCard()
{
    return m_obPanels.at( m_uiActivePanel )->isCanBeStartedByCard();
}

void cMdiPanels::setPaymentMethod( const unsigned int p_uiPaymentMethodId )
{
    m_obPanels.at( m_uiActivePanel )->setPaymentMethod( p_uiPaymentMethodId );
}

void cMdiPanels::setPaymentMethod( const unsigned int p_uiPanelId, const unsigned int p_uiPaymentMethodId )
{
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            m_obPanels.at(i)->setPaymentMethod( p_uiPaymentMethodId );
            break;
        }
    }
}

//====================================================================================
bool cMdiPanels::isItemInShoppingCart()
{
    return m_obPanels.at( m_uiActivePanel )->isItemInShoppingCart();
}
//====================================================================================
void cMdiPanels::itemAddedToShoppingCart()
{
    m_obPanels.at( m_uiActivePanel )->itemAddedToShoppingCart();
}
//====================================================================================
void cMdiPanels::itemRemovedFromShoppingCart()
{
    m_obPanels.at( m_uiActivePanel )->itemRemovedFromShoppingCart();
}
//====================================================================================
void cMdiPanels::itemRemovedFromShoppingCart( const unsigned int p_uiPanelId )
{
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            m_obPanels.at(i)->itemRemovedFromShoppingCart();
            break;
        }
    }
}
//====================================================================================
//void cMdiPanels::addPatientToWaitingQueue( int p_inLengthCash, int p_inPrice, unsigned int p_uiPatientCardId, QString p_qsUnitIds, int p_inLenghtCard, unsigned int p_uiLedgerId, int p_inPayType )
void cMdiPanels::addPatientToWaitingQueue( bool p_bIsPatientWaiting )
{
//    m_obPanels.at( m_uiActivePanel )->addPatientToWaitingQueue( p_inLengthCash, p_inPrice, p_uiPatientCardId, p_qsUnitIds, p_inLenghtCard, p_uiLedgerId, p_inPayType );
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        m_obPanels.at(i)->addPatientToWaitingQueue( p_bIsPatientWaiting );
    }
}
//====================================================================================
bool cMdiPanels::isPatientWaiting()
{
    return m_obPanels.at( m_uiActivePanel )->isPatientWaiting();
}
//====================================================================================
void cMdiPanels::setUsageFromWaitingQueue()
{
    m_obPanels.at( m_uiActivePanel )->setUsageFromWaitingQueue();
}
//====================================================================================
void cMdiPanels::openShoppingCart( unsigned int p_uiPanelId )
{
    emit signalOpenShoppingCart( p_uiPanelId );
}
//====================================================================================
void cMdiPanels::slotPaymentActivated( unsigned int p_uiPanelId )
{
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            activatePanel( i );
            break;
        }
    }

    emit signalPaymentActivated();
}
//====================================================================================
void cMdiPanels::slotOpenScheduleTable( unsigned int p_uiPanelId )
{
    emit signalOpenScheduleTable( p_uiPanelId );
}
//====================================================================================
void cMdiPanels::activatePanelId( unsigned int p_uiPanelId )
{
    for( unsigned int i=0; i<m_obPanels.size(); i++ )
    {
        if( m_obPanels.at(i)->panelId() == p_uiPanelId )
        {
            activatePanel( i );
            break;
        }
    }
}
//====================================================================================
void cMdiPanels::slotStatusChanged( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId, const QString p_qsStatus )
{
    emit signalStatusChanged( p_uiPanelId, p_uiPanelStatusId, p_qsStatus );
}
void cMdiPanels::slotSetCounterText(unsigned int p_uiPanelId, const QString &p_qsCounter)
{
    emit signalSetCounterText( p_uiPanelId, p_qsCounter );
}
//====================================================================================
void cMdiPanels::slotSetWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime )
{
    emit signalSetWaitTime( p_uiPanelId, p_uiWaitTime );
}
//====================================================================================
void cMdiPanels::slotSetInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo )
{
    emit signalSetInfoText( p_uiPanelId, p_qsInfo );
}
//====================================================================================
void cMdiPanels::slotSelectedFromWaitingQueue()
{
    QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString( "SELECT * from waitlist" ) );

    if( poQuery->size() == 0 )
    {
        for( unsigned int i=0; i<m_obPanels.size(); i++ )
        {
            m_obPanels.at(i)->addPatientToWaitingQueue( false );
        }
    }
    else
    {
        addPatientToWaitingQueue( true );
    }
}
