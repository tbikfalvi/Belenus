#include <QMessageBox>
#include "belenus.h"
#include "frmpanel.h"

cFrmPanel::cFrmPanel(  QWidget * p_poParent, Qt::WindowFlags p_inFlags )
    : QFrame( p_poParent, p_inFlags )
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );
}

cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );
}
