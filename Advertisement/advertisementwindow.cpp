
#include "advertisementwindow.h"
#include "dlgadvertisementedit.h"
#include "dbadvertisements.h"

cDlgAdvertisementWindow::cDlgAdvertisementWindow(QWidget *parent, unsigned int id) : QDialog(parent)
{
    cTracer obTrace( "cDlgAdvertisementWindow::cDlgAdvertisementWindow" );

    setupUi(this);

    setWindowIcon( QIcon("./resources/belenus.ico") );

    pbStart->setIcon( QIcon( "./resources/40x40_start.png" ) );
    pbStop->setIcon( QIcon( "./resources/40x40_pause.png" ) );
    pbSettings->setIcon( QIcon( "./resources/40x40_settings.png" ) );
    pbRefresh->setIcon( QIcon( "./resources/40x40_refresh.png" ) );

    m_obAdvertisement.createNew();

    m_nTimer            = 0;
    m_nImageCounter     = 0;

    m_bPanelVisible     = false;

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
            refreshBackground();

            m_qslImages = m_obAdvertisement.filenames().split("#");

            _loadImage();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    frmButtons->setVisible( m_bPanelVisible );

    if( m_obAdvertisement.timerLength() > 0 )
        m_nTimer = startTimer( m_obAdvertisement.timerLength() * 1000 );
}

cDlgAdvertisementWindow::~cDlgAdvertisementWindow()
{
    cTracer obTrace( "cDlgAdvertisementWindow::~cDlgAdvertisementWindow" );

    g_poPrefs->setDialogSize( QString("Ad%1").arg(m_obAdvertisement.id()), QPoint( width(), height() ) );
    g_poPrefs->setDialogPosition( QString("Ad%1").arg(m_obAdvertisement.id()), QPoint( x(), y() ) );
}

void cDlgAdvertisementWindow::refreshBackground()
{
    cTracer obTrace( "cDlgAdvertisementWindow::refreshBackground" );

    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( m_obAdvertisement.backgroundColor() ) ) );
    setPalette( obFramePalette );
}

void cDlgAdvertisementWindow::keyPressEvent ( QKeyEvent */*p_poEvent*/ )
{
    return;
}

void cDlgAdvertisementWindow::timerEvent(QTimerEvent *)
{
    if( m_nImageCounter == m_qslImages.size()-1 )
    {
        m_nImageCounter = 0;
    }
    else
    {
        m_nImageCounter++;
    }
    _loadImage();
}

void cDlgAdvertisementWindow::mousePressEvent ( QMouseEvent *p_poEvent )
{
    _showButtonPanel();
    p_poEvent->ignore();
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
    refreshBackground();

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
    if( g_obUser.isInGroup(cAccessGroup::ADMIN) )
    {
        if( m_bPanelVisible )
        {
            m_bPanelVisible = false;
        }
        else
        {
            m_bPanelVisible = true;
        }
        frmButtons->setVisible( m_bPanelVisible );
    }
}
