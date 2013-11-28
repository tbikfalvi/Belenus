
#include <QPoint>
#include <QSize>
#include <QMdiSubWindow>

#include "belenus.h"
#include "dlgsecondarywindow.h"
#include "ui_dlgsecondarywindow.h"

cDlgSecondaryWindow::cDlgSecondaryWindow(QWidget *parent) : QDialog(parent)
{
    cTracer obTrace( "cDlgSecondaryWindow::cDlgSecondaryWindow" );

    setupUi(this);

    setWindowIcon( QIcon("./resources/belenus.ico") );

/*    layoutMain = new QGridLayout( this );
    m_poParent = parent;

    layoutMain->setContentsMargins( 2, 2, 2, 2 );
    layoutMain->setSpacing( 3 );

    setLayout( layoutMain );

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( g_poPrefs->getSecondaryBackground() ) ) );
    setPalette( obFramePalette );
*/
}

cDlgSecondaryWindow::~cDlgSecondaryWindow()
{
    cTracer obTrace( "cDlgSecondaryWindow::~cDlgSecondaryWindow" );

    g_poPrefs->setSecondaryWindowPosition( QPoint( x(), y() ), true );
    g_poPrefs->setSecondaryWindowSize( QSize( width(), height() ), true );
}

void cDlgSecondaryWindow::initPanels()
{
    cTracer obTrace( "cDlgSecondaryWindow::initPanels" );

    int inPanelCount = g_poPrefs->getPanelCount();

    m_obPanels.reserve( inPanelCount );

//    int nRow = 0;
//    int nColumn = 0;

    cDspPanel   *pDspPanel;
    QMdiSubWindow *poPanel;

    for( int i=0; i<inPanelCount; i++ )
    {
        pDspPanel = new cDspPanel( i+1 );
        pDspPanel->setPanelStatus( 1 );

        poPanel = new QMdiSubWindow( 0, Qt::FramelessWindowHint );
        poPanel->setWidget( pDspPanel );

        m_obPanels.push_back( pDspPanel );
        mdiArea->addSubWindow( poPanel );

/*        layoutMain->addWidget( pDspPanel, nRow, nColumn++, 1, 1 );

        if( nColumn > (int)g_poPrefs->getPanelsPerRow()-1 )
        {
            nRow++;
            nColumn = 0;
        }*/
    }
    placeSubWindows();
}

void cDlgSecondaryWindow::placeSubWindows()
{
    QList<QMdiSubWindow*> obSubWindowList = mdiArea->subWindowList();

    if( obSubWindowList.size() )
    {
        int inPanelColumns = g_poPrefs->getPanelsPerRow();
        int inPanelRows    = ceil( (double)g_poPrefs->getPanelCount() / (double)inPanelColumns );
        int inPanelW       = width();
        int inPanelH       = height();
        int inPanelMargin  = 2;

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

void cDlgSecondaryWindow::refreshBackground()
{
    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( g_poPrefs->getSecondaryBackground() ) ) );
    setPalette( obFramePalette );
}

void cDlgSecondaryWindow::refreshTitle( unsigned int p_uiPanelId )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->refreshTitle();
}

void cDlgSecondaryWindow::setPanelStatus( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelStatus( p_uiPanelStatusId );
}

void cDlgSecondaryWindow::setPanelStatusText( unsigned int p_uiPanelId, const QString &p_qsStatus )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelStatusText( p_qsStatus );
}

void cDlgSecondaryWindow::setPanelInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelInfoText( p_qsInfo );
}

void cDlgSecondaryWindow::setCounterText(unsigned int p_uiPanelId, const QString &p_qsCounter)
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setCounterText( p_qsCounter );
}

void cDlgSecondaryWindow::setPanelWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelWaitTime( p_uiWaitTime );
}

//====================================================================================
void cDlgSecondaryWindow::resizeEvent ( QResizeEvent *p_poEvent )
{
    placeSubWindows();
    p_poEvent->accept();
}

void cDlgSecondaryWindow::keyPressEvent ( QKeyEvent */*p_poEvent*/ )
{
    return;
}
