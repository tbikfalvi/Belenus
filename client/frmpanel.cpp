#include <QPalette>

#include "belenus.h"
#include "frmpanel.h"

cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId )
    : QFrame()
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );

    verticalLayout   = new QVBoxLayout( this );
    lblTitle         = new QLabel( this );
    lblCurrStatus    = new QLabel( this );
    lblCurrTimer     = new QLabel( this );
    lblNextStatusLen = new QLabel( this );
    lblInfo          = new QLabel( this );
    spacer1          = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );

    verticalLayout->setContentsMargins( 0, 0, 0, 0 );
    verticalLayout->addWidget( lblTitle );
    verticalLayout->addWidget( lblCurrStatus );
    verticalLayout->addWidget( lblCurrTimer );
    verticalLayout->addWidget( lblNextStatusLen );
    verticalLayout->addWidget( lblInfo );
    verticalLayout->addItem( spacer1 );

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );

    m_uiId      = 0;
    m_uiType    = 0;
    m_uiStatus  = 0;
    m_uiCounter = 0;
    m_inTimerId = 0;

    load( p_uiPanelId );

    inactivate();
    displayStatus();
}

cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );

    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ ) if( m_obStatuses.at( i ) ) delete m_obStatuses.at( i );
}

bool cFrmPanel::isWorking() const
{
    return (m_uiStatus > 0);
}

void cFrmPanel::start()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::start", ssTrace.str() );

    activateNextStatus();
    m_inTimerId = startTimer( 1000 );
}

void cFrmPanel::reset()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::reset", ssTrace.str() );

    m_uiStatus = m_obStatuses.size() - 1;
    activateNextStatus();
}

void cFrmPanel::next()
{
    activateNextStatus();
}

void cFrmPanel::inactivate()
{
    setFrameShadow( QFrame::Sunken );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#b9b9b9") ) );
    lblTitle->setPalette( obNewPalette );
}

void cFrmPanel::activate()
{
    setFrameShadow( QFrame::Raised );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#4387cb" ) ) );
    lblTitle->setPalette( obNewPalette );
}

void cFrmPanel::mousePressEvent ( QMouseEvent * p_poEvent )
{
    emit panelClicked( m_uiId - 1 );
    p_poEvent->ignore();
}

void cFrmPanel::timerEvent ( QTimerEvent * )
{
    if( g_poHardware->isHardwareMovedNextStatus( m_uiId-1 ) )
    {
        g_poHardware->setHardwareMovedNextStatus( m_uiId-1 );
        m_uiCounter = 0;
    }

    if( m_uiCounter )
    {
        m_uiCounter--;
        lblCurrTimer->setText( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
    }
    else
    {
        activateNextStatus();
        displayStatus();
    }
    g_poHardware->setCounter( m_uiId-1, (int)m_uiCounter );
}

void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            m_uiType = poQuery->value( 0 ).toInt();
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        else
        {
            lblTitle->setText( "Panel Not Found in Database" );
        }

        delete poQuery;
        poQuery = NULL;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, panelTypeId, seqNumber from panelStatuses WHERE panelTypeId=%1 ORDER BY seqNumber" ).arg( m_uiType ) );
        while( poQuery->next() )
        {
            unsigned int uiStatusId = poQuery->value( 0 ).toInt();

            cDBPanelStatuses  *poStatus = new cDBPanelStatuses();
            poStatus->load( uiStatusId );
            m_obStatuses.push_back( poStatus );
        }

        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity() << e.what() << cQTLogger::EOM;

        if( poQuery ) delete poQuery;
    }

}

void cFrmPanel::displayStatus()
{
    if( m_uiStatus )
    {
        lblCurrStatus->setText( QString::fromStdString( m_obStatuses.at( m_uiStatus )->name() ) );

        lblCurrTimer->setText( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
        unsigned int uiNextLen = 0;
        if( m_uiStatus != m_obStatuses.size() - 1 )
        {
            uiNextLen = m_obStatuses.at( m_uiStatus + 1 )->length();
        }
        lblNextStatusLen->setText( QString( "%1:%2" ).arg( uiNextLen / 60, 2, 10, QChar( '0' ) ).arg( uiNextLen % 60, 2, 10, QChar( '0' ) ) );
    }
    else
    {
        lblCurrStatus->setText( "" );
        lblCurrTimer->setText( "" );
        lblNextStatusLen->setText( "" );
    }
    lblInfo->setText( QString( "Additional Info for status %1" ).arg( QString::fromStdString( m_obStatuses.at( m_uiStatus )->name() ) ) );

    // A kovetkezo reszt at kell irni, ha keszen lesz a dinamikus
    // stilus valtas statuszonkent
    QPalette  obFramePalette = palette();
    switch( m_uiStatus )
    {
        case 0:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::green ) );
            break;
        case 1:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::yellow ) );
            break;
        case 2:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::red ) );
            break;
        case 3:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::cyan ) );
            break;
        case 4:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::blue ) );
            break;
    }
    setPalette( obFramePalette );

    lblCurrStatus->setAlignment( Qt::AlignCenter );
    lblCurrTimer->setAlignment( Qt::AlignCenter );
    lblNextStatusLen->setAlignment( Qt::AlignCenter );
    lblInfo->setAlignment( Qt::AlignCenter );
}

void cFrmPanel::activateNextStatus()
{
    m_uiStatus++;
    if( m_uiStatus == m_obStatuses.size() )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }
    else
    {
        m_uiCounter = m_obStatuses.at( m_uiStatus )->length();
        g_poHardware->setCurrentCommand( m_uiId-1, m_obStatuses.at( m_uiStatus )->activateCommand() );
    }

    displayStatus();
}
