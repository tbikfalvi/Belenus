#include <QMessageBox>
#include "belenus.h"
#include "dlgpanel.h"

cDlgPanel::cDlgPanel( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanel::cDlgPanel" );

    setupUi( this );

    m_uiTimeLeft = 0;
    m_obTimer.setInterval( 1000 );
    connect( &m_obTimer, SIGNAL( timeout() ), this, SLOT( timeout() ) );
}

cDlgPanel::~cDlgPanel()
{
    cTracer obTrace( "cDlgPanel::~cDlgPanel" );

    m_obTimer.stop();
}

void cDlgPanel::timeout()
{
    QString stTime = QString( "%1:%2" ).arg( (--m_uiTimeLeft) / 60 ).arg( m_uiTimeLeft % 60 );
    lblCountDown->setText( stTime );

    if( !m_uiTimeLeft )
    {
        m_obTimer.stop();
    }
}
