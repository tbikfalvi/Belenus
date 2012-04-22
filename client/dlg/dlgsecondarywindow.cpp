
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

void cDlgSecondaryWindow::placePanels()
{
    cTracer obTrace( "cDlgSecondaryWindow::placePanels" );

}
