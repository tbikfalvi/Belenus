#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelsettings.h"

cDlgPanelSettings::cDlgPanelSettings( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Validate Serial key") );
    setWindowIcon( QIcon("./resources/40x40_key.png") );

}

cDlgPanelSettings::~cDlgPanelSettings()
{
}

