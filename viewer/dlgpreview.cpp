#include <QPushButton>

#include "../framework/qtframework.h"
#include "dlgpreview.h"

cDlgPreview::cDlgPreview( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );

    QPushButton *poPrintButton = btbButtons->addButton( tr( "&Print ..." ), QDialogButtonBox::ActionRole );
    btbButtons->addButton( tr( "&Close" ), QDialogButtonBox::RejectRole );

    connect( poPrintButton, SIGNAL(clicked()), this, SLOT(printReport()) );
}

cDlgPreview::~cDlgPreview()
{
}

void cDlgPreview::printReport()
{
}
