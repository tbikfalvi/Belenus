#include <QMessageBox>
#include "belenus.h"
#include "dlgpanel.h"

cDlgPanel::cDlgPanel(  QWidget * p_poParent, Qt::WindowFlags p_inFlags )
    : QDialog( p_poParent, p_inFlags )
{
    cTracer obTrace( "cDlgPanel::cDlgPanel" );
}

cDlgPanel::~cDlgPanel()
{
    cTracer obTrace( "cDlgPanel::~cDlgPanel" );
}
