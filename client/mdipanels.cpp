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
        QMessageBox::warning( this, "Panel count mismatch", QString( "There are more Panels defined in the database than supported by the current hardware. Only %1 panels will be displayed." ).arg( g_poHardware->getPanelCount() ) );
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

        connect( poFrame, SIGNAL( panelClicked( unsigned int ) ), this, SLOT( activatePanel( unsigned int ) ) );

        poPanel = new QMdiSubWindow( 0, Qt::FramelessWindowHint );
        poPanel->setWidget( poFrame );

        m_obPanels.push_back( poFrame );
        addSubWindow( poPanel );
        m_obPanels.at( i )->reset();
        m_obPanels.at( i )->show();
    }

    placeSubWindows();
    activatePanel( 0 );

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

void cMdiPanels::reset()
{
    m_obPanels.at( m_uiActivePanel )->reset();
}

void cMdiPanels::next()
{
    m_obPanels.at( m_uiActivePanel )->next();
}

int cMdiPanels::activePanel()
{
    return m_uiActivePanel;
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

bool cMdiPanels::isPanelWorking( const unsigned int p_uiPanel )
{
    if( p_uiPanel < m_obPanels.size() )
        return m_obPanels.at( p_uiPanel )->isWorking();
    else
        return false;
}

void cMdiPanels::activatePanel( unsigned int p_uiPanel )
{
    m_obPanels.at( m_uiActivePanel )->inactivate();
    m_obPanels.at( p_uiPanel )->activate();

    m_uiActivePanel = p_uiPanel;

    emit activePanelChanged( m_obPanels.at( m_uiActivePanel )->isWorking() );
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
