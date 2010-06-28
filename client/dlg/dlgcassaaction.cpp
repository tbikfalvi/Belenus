#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaaction.h"

cDlgCassaAction::cDlgCassaAction( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa action") );

}

cDlgCassaAction::~cDlgCassaAction()
{
}

