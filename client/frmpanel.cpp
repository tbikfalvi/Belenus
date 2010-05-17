#include <QPalette>

#include "belenus.h"
#include "frmpanel.h"

cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId )
    : QFrame()
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );

    poVerticalLayout = new QVBoxLayout( this );
    poTitle = new QLabel( this );
    poSpacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );

    poVerticalLayout->addWidget( poTitle );
    poVerticalLayout->addItem( poSpacer1 );

    setAutoFillBackground( true );

    poTitle->setAutoFillBackground( true );
    poTitle->setContentsMargins( 0, 5, 0, 5 );

    load( p_uiPanelId );

    m_uiStatus = 0;
    inactivate();
    displayStatus();
}

cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );
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

    m_uiStatus = 1;
    displayStatus();
}

void cFrmPanel::reset()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::reset", ssTrace.str() );

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::green ) );
    setPalette( obNewPalette );

    m_uiStatus = 0;
    displayStatus();
}

void cFrmPanel::inactivate()
{
    setFrameShadow( QFrame::Sunken );

    QPalette  obNewPalette = poTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#b9b9b9") ) );
    poTitle->setPalette( obNewPalette );
}

void cFrmPanel::activate()
{
    setFrameShadow( QFrame::Raised );

    QPalette  obNewPalette = poTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#4387cb" ) ) );
    poTitle->setPalette( obNewPalette );
}

void cFrmPanel::mousePressEvent ( QMouseEvent * p_poEvent )
{
    emit panelClicked( m_uiId - 1 );
    p_poEvent->ignore();
}

void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
    if( poQuery->size() )
    {
        poQuery->first();
        poTitle->setText( poQuery->value( 1 ).toString() );
    }
    else
    {
        poTitle->setText( "Panel Not Found in Database" );
    }
}

void cFrmPanel::displayStatus()
{
    poTitle->setAlignment( Qt::AlignCenter );
}
