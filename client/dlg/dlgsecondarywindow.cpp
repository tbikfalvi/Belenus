
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
    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

/*    layoutMain = new QGridLayout( this );
    m_poParent = parent;

    layoutMain->setContentsMargins( 2, 2, 2, 2 );
    layoutMain->setSpacing( 3 );

    setLayout( layoutMain );
*/

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( g_poPrefs->getSecondaryFrame() ) ) );
    setPalette( obFramePalette );

    mdiArea->setBackground( QBrush( QColor( g_poPrefs->getSecondaryBackground() ) ) );

    frmCaption->setVisible( g_poPrefs->isSecondaryCaptionVisible() );
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
        int inPanelColumns  = g_poPrefs->getPanelsPerRow();
        int inPanelRows     = ceil( (double)g_poPrefs->getPanelCount() / (double)inPanelColumns );
        int inPanelW        = width();
        int inPanelH        = height();
        int inPanelMargin   = 4;
        int inCaptionHeight = frmCaption->height();

        if( !g_poPrefs->isSecondaryCaptionVisible() )
        {
            inCaptionHeight = 0;
        }

        inPanelW -= (inPanelColumns+1)*inPanelMargin+5;
        inPanelW /= inPanelColumns;
        inPanelH -= (inPanelRows+1)*inPanelMargin+inCaptionHeight+7;
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
    frmCaption->setVisible( g_poPrefs->isSecondaryCaptionVisible() );

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( g_poPrefs->getSecondaryFrame() ) ) );
    setPalette( obFramePalette );

    mdiArea->setBackground( QBrush( QColor( g_poPrefs->getSecondaryBackground() ) ) );

    placeSubWindows();
}

void cDlgSecondaryWindow::setCaption(QString p_qsCaption)
{
    lblCaption->setText( p_qsCaption );
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

void cDlgSecondaryWindow::keyPressEvent ( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = true;
    }
    if( p_poEvent->key() == Qt::Key_Shift )
    {
        m_bShiftPressed = true;
    }

    if( m_bCtrlPressed )
    {
        if( m_bShiftPressed )
        {
            switch( p_poEvent->key() )
            {
                case Qt::Key_Up:
                    resize( width(), height()-1 );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Down:
                    resize( width(), height()+1 );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Left:
                    resize( width()-1, height() );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Right:
                    resize( width()+1, height() );
                    p_poEvent->accept();
                    break;
                default: p_poEvent->ignore();
            }
        }
        else
        {
            switch( p_poEvent->key() )
            {
                case Qt::Key_Up:
                    move( x(), y()-1 );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Down:
                    move( x(), y()+1 );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Left:
                    move( x()-1, y() );
                    p_poEvent->accept();
                    break;
                case Qt::Key_Right:
                    move( x()+1, y() );
                    p_poEvent->accept();
                    break;
                default: p_poEvent->ignore();
            }
        }
    }
}

void cDlgSecondaryWindow::keyReleaseEvent( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_Control )
    {
        m_bCtrlPressed = false;
    }
    if( p_poEvent->key() == Qt::Key_Shift )
    {
        m_bShiftPressed = false;
    }
}

void cDlgSecondaryWindow::mousePressEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = true;

    m_nMouseX = p_poEvent->pos().x();
    m_nMouseY = p_poEvent->pos().y();

    if( p_poEvent->pos().x() > width()-7 &&
        p_poEvent->pos().x() < width()+13 )
    {
        setCursor( Qt::SizeHorCursor );
    }
    else if( p_poEvent->pos().y() > height()-7 &&
             p_poEvent->pos().y() < height()+13 )
    {
        setCursor( Qt::SizeVerCursor );
    }
    p_poEvent->accept();
}

void cDlgSecondaryWindow::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    setCursor( Qt::ArrowCursor );
    p_poEvent->accept();
}

void cDlgSecondaryWindow::mouseMoveEvent ( QMouseEvent *p_poEvent )
{
    if( m_bMousePressed )
    {
        if( cursor().shape() == Qt::SizeHorCursor )
        {
            resize( p_poEvent->pos().x(), height() );
        }
        else if( cursor().shape() == Qt::SizeVerCursor )
        {
            resize( width(), p_poEvent->pos().y() );
        }
        else
        {
            move( x() + p_poEvent->pos().x() - m_nMouseX,
                  y() + p_poEvent->pos().y() - m_nMouseY );
        }
    }
    p_poEvent->accept();
}

