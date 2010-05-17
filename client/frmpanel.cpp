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

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );

    m_uiId      = 0;
    m_uiType    = 0;
    m_uiStatus  = 0;
    m_uiCounter = 0;

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

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::red ) );
    setPalette( obNewPalette );

    activateNextStatus();
}

void cFrmPanel::reset()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::reset", ssTrace.str() );

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::green ) );
    setPalette( obNewPalette );

    m_uiStatus = m_obStatuses.size() - 1;
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
    lblCurrStatus->setText( QString::fromStdString( m_obStatuses.at( m_uiStatus )->name() ) );
    lblCurrStatus->setAlignment( Qt::AlignCenter );
    lblCurrTimer->setText( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
    lblCurrTimer->setAlignment( Qt::AlignCenter );
}

void cFrmPanel::activateNextStatus()
{
    m_uiStatus++;
    if( m_uiStatus == m_obStatuses.size() )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
    }
    else
    {
        // Update m_uiCounter here and start a new timer
    }

    displayStatus();
}
