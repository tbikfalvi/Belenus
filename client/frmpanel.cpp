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

    m_poWorking = false;

    setAutoFillBackground( true );

    load( p_uiPanelId );
}

cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );
}

bool cFrmPanel::isWorking() const
{
    return m_poWorking;
}

void cFrmPanel::start()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::start", ssTrace.str() );

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::red ) );
    setPalette( obNewPalette );

    m_poWorking = true;
}

void cFrmPanel::pause()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::pause", ssTrace.str() );

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::yellow ) );
    setPalette( obNewPalette );

    m_poWorking = false;
}

void cFrmPanel::reset()
{
    stringstream ssTrace;
    ssTrace << "Id: " << m_uiId;
    cTracer obTrace( "cFrmPanel::reset", ssTrace.str() );

    QPalette  obNewPalette = palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( Qt::green ) );
    setPalette( obNewPalette );

    m_poWorking = false;
}

void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    poTitle->setText( "Hello Mom!" );
}

