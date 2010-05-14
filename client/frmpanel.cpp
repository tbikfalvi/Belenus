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
