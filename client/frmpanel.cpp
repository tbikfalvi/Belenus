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

    load( p_uiPanelId );
}

cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );

    delete poTitle;
    delete poSpacer1;
    delete poVerticalLayout;
}

void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    poTitle->setText( "Hello Mom!" );
}
