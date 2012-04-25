
#include <QPoint>
#include <QSize>

#include "belenus.h"
#include "dlgsecondarywindow.h"
#include "ui_dlgsecondarywindow.h"

cDlgSecondaryWindow::cDlgSecondaryWindow(QWidget *parent) : QDialog(parent)
{
    cTracer obTrace( "cDlgSecondaryWindow::cDlgSecondaryWindow" );

    setupUi(this);

    setWindowIcon( QIcon("./resources/belenus.ico") );

    layoutMain = new QGridLayout( this );

    setLayout( layoutMain );
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

    int nRow = 0;
    int nColumn = 0;

    cDspPanel   *pDspPanel;

    for( int i=0; i<inPanelCount; i++ )
    {
        pDspPanel = new cDspPanel( i+1 );

        m_obPanels.push_back( pDspPanel );

        layoutMain->addWidget( pDspPanel, nRow, nColumn++ );

        if( nColumn > g_poPrefs->getPanelsPerRow()-1 )
        {
            nRow++;
            nColumn = 0;
        }
    }
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

void cDlgSecondaryWindow::setPanelCounter( unsigned int p_uiPanelId, const unsigned int p_uiCounter )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelCounter( p_uiCounter );
}

void cDlgSecondaryWindow::setPanelWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime )
{
    if( p_uiPanelId < m_obPanels.size() )
        m_obPanels.at( p_uiPanelId )->setPanelWaitTime( p_uiWaitTime );
}

