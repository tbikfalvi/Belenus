#include "advertisementwindow.h"
#include "ui_advertisementwindow.h"

advertisementwindow::advertisementwindow(QWidget *parent, unsigned int id) : QDialog(parent), ui(new Ui::advertisementwindow)
{
    ui->setupUi(this);

    m_uiDBId        = id;
    m_nTimerCounter = 0;
    m_wndFlags      = windowFlags();
    m_qsCMDAction   = "";
    m_qsCMDStatus   = "STARTING";

    _updateStatus();

    m_qslCommands = QStringList() << ""
                                  << "REFRESH"
                                  << "EXIT";

    setWindowIcon( QIcon(":/advertisement.ico") );
    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    ui->lblCaption->setText( "" );

    ui->pbStart->setIcon( QIcon( "./resources/40x40_start.png" ) );
    ui->pbStop->setIcon( QIcon( "./resources/40x40_pause.png" ) );
    ui->pbSave->setIcon( QIcon( "./resources/40x40_save.png" ) );
    ui->pbRefresh->setIcon( QIcon( "./resources/40x40_refresh.png" ) );

    m_nTimer                = 0;
    m_nImageCounter         = 0;
    m_bWindowCustomizeable  = false;
    m_bPanelVisible         = false;
    m_bCtrlPressed          = false;
    m_bShiftPressed         = false;
    m_bMousePressed         = false;
    m_bImageChangeEnabled   = false;

    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    _loadPosition();
    _refreshSettings();
    _updateGui();
    if( m_nDBTimerLength > 0 )
        m_bImageChangeEnabled = true;
    _loadImage();

    ui->frmButtons->setVisible( m_bPanelVisible );

    m_nTimer = startTimer( 1000 );
}

advertisementwindow::~advertisementwindow()
{
    m_qsCMDAction   = "_";
    m_qsCMDStatus   = "EXITING";

    _updateCommand();
    _updateStatus();

    delete ui;
}

void advertisementwindow::refreshBackground()
{
    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor( m_qsDBBackground ) ) );
    setPalette( obFramePalette );
}

void advertisementwindow::keyPressEvent ( QKeyEvent *p_poEvent )
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

void advertisementwindow::keyReleaseEvent( QKeyEvent *p_poEvent )
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

void advertisementwindow::timerEvent(QTimerEvent *)
{
    m_qsCMDStatus = "RUNNING";
    _updateStatus();

    _readCommandFile();

    QCoreApplication::processEvents();

    if( m_qsCMDAction.length() > 0 )
    {
        switch( m_qslCommands.indexOf( m_qsCMDAction ) )
        {
            case 1:
                on_pbRefresh_clicked();
                break;
            case 2:
                close();
                break;
            default:
                break;
        }
    }

    QCoreApplication::processEvents();

    if( m_nDBTimerLength == 0 || m_bImageChangeEnabled == false )
        return;

    m_nTimerCounter++;

    if( m_nTimerCounter == m_nDBTimerLength )
    {
        m_nTimerCounter = 0;

        if( m_nImageCounter > m_qslImages.size()-1 )
        {
            m_nImageCounter = 0;
        }
        else
        {
            m_nImageCounter++;
        }

        _loadImage();
    }
}

void advertisementwindow::mousePressEvent ( QMouseEvent *p_poEvent )
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
void advertisementwindow::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    setCursor( Qt::ArrowCursor );
    p_poEvent->accept();
}

void advertisementwindow::mouseMoveEvent ( QMouseEvent *p_poEvent )
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

void advertisementwindow::mouseDoubleClickEvent ( QMouseEvent *p_poEvent )
{
    _showButtonPanel();
    p_poEvent->accept();
}

void advertisementwindow::on_pbStart_clicked()
{
    m_bImageChangeEnabled = true;
}

void advertisementwindow::on_pbStop_clicked()
{
    m_bImageChangeEnabled = false;
}

void advertisementwindow::on_pbRefresh_clicked()
{
    if( m_nTimer > 0 )
        killTimer( m_nTimer );

    _updateGui();

    m_nTimer = startTimer( 1000 );
}

void advertisementwindow::_showButtonPanel()
{
    if( m_bPanelVisible )
    {
        m_bPanelVisible = false;
    }
    else
    {
        m_bPanelVisible = true;
    }
//    if( g_obUser.isInGroup(cAccessGroup::ADMIN) )
    {
        ui->frmButtons->setVisible( m_bPanelVisible );
    }
}

void advertisementwindow::on_pbSave_clicked()
{
    _savePosition();
}

void advertisementwindow::_refreshSettings()
{
    m_qslImages.clear();

    m_poDB->open();

    QSqlQuery poQuery = m_poDB->exec( QString( "SELECT * FROM advertisements WHERE advertisementId=%1 " ).arg( m_uiDBId ) );

    if( poQuery.first() )
    {
        m_qsDBCaption       = poQuery.value( 3 ).toString();
        m_qsDBBackground    = poQuery.value( 4 ).toString();
        m_qsDBPath          = poQuery.value( 5 ).toString();
        m_nDBTimerLength    = poQuery.value( 7 ).toInt();
        m_qslImages         = poQuery.value( 6 ).toString().split("#");
    }

    m_poDB->close();
}

void advertisementwindow::_loadPosition()
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );

    QPoint  qpDlgPos;
    QPoint  qpDlgSize;

    qpDlgPos.setX( obPrefFile.value( QString::fromAscii( "Dialogs/Ad%1_left" ).arg(m_uiDBId), 0 ).toInt() );
    qpDlgPos.setY( obPrefFile.value( QString::fromAscii( "Dialogs/Ad%1_top" ).arg(m_uiDBId), 0 ).toInt() );
    qpDlgSize.setX( obPrefFile.value( QString::fromAscii( "Dialogs/Ad%1_width" ).arg(m_uiDBId), 400 ).toInt() );
    qpDlgSize.setY( obPrefFile.value( QString::fromAscii( "Dialogs/Ad%1_height" ).arg(m_uiDBId), 400 ).toInt() );

    resize( qpDlgSize.x(), qpDlgSize.y() );
    move( qpDlgPos );
}

void advertisementwindow::_savePosition()
{
    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );

    obPrefFile.setValue( QString::fromAscii( "Dialogs/Ad%1_left" ).arg(m_uiDBId), x() );
    obPrefFile.setValue( QString::fromAscii( "Dialogs/Ad%1_top" ).arg(m_uiDBId), y() );
    obPrefFile.setValue( QString::fromAscii( "Dialogs/Ad%1_width" ).arg(m_uiDBId), width() );
    obPrefFile.setValue( QString::fromAscii( "Dialogs/Ad%1_height" ).arg(m_uiDBId), height() );
}

void advertisementwindow::_loadImages()
{
    m_obAdImages.clear();

    for( int i=0; i<m_qslImages.size(); i++ )
    {
        QString qsFile      = m_qslImages.at( i );
        QString qsFileName  = QString( "%1/%2" ).arg( m_qsDBPath ).arg( qsFile );

        qsFileName.replace( "\\", "/" );
        qsFileName.replace( "//", "/" );

        QPixmap *qpAd = new QPixmap( qsFileName );

        m_obAdImages.push_back( qpAd );
    }
    m_nImageCounter = 0;
}

void advertisementwindow::_loadImage()
{
    if( m_nImageCounter > (int)m_obAdImages.size()-1 )
        m_nImageCounter = 0;

    ui->lblImage->setPixmap( *m_obAdImages.at( m_nImageCounter ) );
}

void advertisementwindow::_updateGui()
{
    if( m_qsDBCaption.length() > 0 )
    {
        ui->lblCaption->setVisible( true );
        ui->lblCaption->setText( QString("   ")+m_qsDBCaption );
    }
    else
    {
        ui->lblCaption->setVisible( false );
    }
    refreshBackground();

    _loadImages();
}

void advertisementwindow::_readCommandFile()
{
    QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

    QString qsCommand = obPrefFile.value( QString::fromAscii( "Advertisement%1/Command" ).arg(m_uiDBId), "" ).toString();

    if( qsCommand.length() > 0 )
    {
        m_qsCMDAction = qsCommand;
    }
}

void advertisementwindow::_updateCommand()
{
    QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

    obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Command" ).arg(m_uiDBId), m_qsCMDAction );
}

void advertisementwindow::_updateStatus()
{
    QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

    obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Status" ).arg(m_uiDBId), m_qsCMDStatus );
}
