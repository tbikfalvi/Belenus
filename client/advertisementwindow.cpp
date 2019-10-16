
#include "advertisementwindow.h"
#include "edit/dlgadvertisementedit.h"
#include "db/dbadvertisements.h"

cDlgAdvertisementWindow::cDlgAdvertisementWindow(QWidget *parent, unsigned int id) : QDialog(parent)
{
    cTracer obTrace( "cDlgAdvertisementWindow::cDlgAdvertisementWindow" );

    setupUi(this);

    m_wndFlags = windowFlags();

    setWindowIcon( QIcon("./resources/belenus.ico") );
    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    lblCaption->setText( "" );

    pbStart->setIcon( QIcon( "./resources/40x40_start.png" ) );
    pbStop->setIcon( QIcon( "./resources/40x40_pause.png" ) );
    pbSettings->setIcon( QIcon( "./resources/40x40_settings.png" ) );
    pbSave->setIcon( QIcon( "./resources/40x40_save.png" ) );
    pbRefresh->setIcon( QIcon( "./resources/40x40_refresh.png" ) );

    m_obAdvertisement.createNew();

    m_nTimer                = 0;
    m_nImageCounter         = 0;
    m_bWindowCustomizeable  = false;
    m_bPanelVisible         = false;
    m_bCtrlPressed          = false;
    m_bShiftPressed         = false;
    m_bMousePressed         = false;

    if( id > 0 )
    {
        try
        {
            m_obAdvertisement.load( id );

            QPoint  qpDlgSize   = g_poPrefs->getDialogSize( QString("Ad%1").arg(m_obAdvertisement.id()), QPoint(400,400) );
            QPoint  qpDlgPos    = g_poPrefs->dialogPosition( QString("Ad%1").arg(m_obAdvertisement.id()) );

            resize( qpDlgSize.x(), qpDlgSize.y() );
            move( qpDlgPos );

            setWindowTitle( m_obAdvertisement.caption() );
            lblCaption->setText( QString("   ")+m_obAdvertisement.caption() );
            refreshBackground();

            m_qslImages = m_obAdvertisement.filenames().split("#");

            _loadImage();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }

    if( m_obAdvertisement.caption().length() == 0 )
        lblCaption->setVisible( false );

    frmButtons->setVisible( m_bPanelVisible );

    if( m_obAdvertisement.timerLength() > 0 )
        m_nTimer = startTimer( m_obAdvertisement.timerLength() * 1000 );
}

cDlgAdvertisementWindow::~cDlgAdvertisementWindow()
{
    cTracer obTrace( "cDlgAdvertisementWindow::~cDlgAdvertisementWindow" );
}

void cDlgAdvertisementWindow::refreshBackground()
{
    cTracer obTrace( "cDlgAdvertisementWindow::refreshBackground" );

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( m_obAdvertisement.backgroundColor() ) ) );
    setPalette( obFramePalette );
}

void cDlgAdvertisementWindow::keyPressEvent ( QKeyEvent *p_poEvent )
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

void cDlgAdvertisementWindow::keyReleaseEvent( QKeyEvent *p_poEvent )
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

void cDlgAdvertisementWindow::timerEvent(QTimerEvent *)
{
    killTimer( m_nTimer );
    m_nTimer = 0;

    if( m_nImageCounter == m_qslImages.size()-1 )
    {
        m_nImageCounter = 0;
    }
    else
    {
        m_nImageCounter++;
    }

    QCoreApplication::processEvents();

    _loadImage();

    if( m_obAdvertisement.timerLength() > 0 )
        m_nTimer = startTimer( m_obAdvertisement.timerLength() * 1000 );
}

void cDlgAdvertisementWindow::mousePressEvent ( QMouseEvent *p_poEvent )
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
void cDlgAdvertisementWindow::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    setCursor( Qt::ArrowCursor );
    p_poEvent->accept();
}

void cDlgAdvertisementWindow::mouseMoveEvent ( QMouseEvent *p_poEvent )
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

void cDlgAdvertisementWindow::mouseDoubleClickEvent ( QMouseEvent *p_poEvent )
{
    _showButtonPanel();
    p_poEvent->accept();
}

void cDlgAdvertisementWindow::_loadImage()
{
    cTracer obTrace( "cDlgAdvertisementWindow::_loadImage" );

    QString qsFile      = m_qslImages.at( m_nImageCounter );
    QString qsFileName  = QString( "%1/%2" ).arg( m_obAdvertisement.path() )
                                            .arg( qsFile );

    qsFileName.replace( "\\", "/" );
    qsFileName.replace( "//", "/" );

    lblImage->setPixmap( QPixmap( qsFileName ) );
}

void cDlgAdvertisementWindow::on_pbStart_clicked()
{
    if( m_obAdvertisement.timerLength() > 0 )
        m_nTimer = startTimer( m_obAdvertisement.timerLength() * 1000 );
}

void cDlgAdvertisementWindow::on_pbStop_clicked()
{
    killTimer( m_nTimer );
    m_nTimer = 0;
}

void cDlgAdvertisementWindow::on_pbRefresh_clicked()
{
    unsigned int id = m_obAdvertisement.id();

    m_obAdvertisement.createNew();
    m_obAdvertisement.load( id );

    setWindowTitle( m_obAdvertisement.caption() );
    lblCaption->setText( QString("   ")+m_obAdvertisement.caption() );
    refreshBackground();

    if( m_obAdvertisement.caption().length() == 0 )
        lblCaption->setVisible( false );

    m_qslImages = m_obAdvertisement.filenames().split("#");

    _loadImage();
}

void cDlgAdvertisementWindow::on_pbSettings_clicked()
{
    cDlgAdvertisementEdit  obDlgEdit( this, &m_obAdvertisement, false );

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        on_pbRefresh_clicked();
    }
}

void cDlgAdvertisementWindow::_showButtonPanel()
{
    if( m_bPanelVisible )
    {
        m_bPanelVisible = false;
//        setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );
//        show();
    }
    else
    {
        m_bPanelVisible = true;
//        setWindowFlags( m_wndFlags );
//        show();
    }
    if( g_obUser.isInGroup(cAccessGroup::ADMIN) )
    {
        frmButtons->setVisible( m_bPanelVisible );
    }
}

void cDlgAdvertisementWindow::on_pbSave_clicked()
{
    g_poPrefs->setDialogSize( QString("Ad%1").arg(m_obAdvertisement.id()), QPoint( width(), height() ) );
    g_poPrefs->setDialogPosition( QString("Ad%1").arg(m_obAdvertisement.id()), QPoint( x(), y() ) );
}
