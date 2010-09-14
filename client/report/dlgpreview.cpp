#include <QPushButton>

#include "../../framework/qtframework.h"
#include "dlgpreview.h"

cDlgPreview::cDlgPreview( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );

    m_qsReportName = "";

    QPushButton *poPrintButton = btbButtons->addButton( tr( "&Print ..." ), QDialogButtonBox::ActionRole );
    btbButtons->addButton( tr( "&Close" ), QDialogButtonBox::RejectRole );

    connect( btnRefresh, SIGNAL(clicked()), this, SLOT(refreshReport()) );
    connect( poPrintButton, SIGNAL(clicked()), this, SLOT(printReport()) );

    tedPreview->setDocument( &m_tdReport );
}

cDlgPreview::~cDlgPreview()
{
}

void cDlgPreview::setReportTitle( const QString &p_qsTitle )
{
    m_qsReportName = p_qsTitle;
    setWindowTitle( windowTitle() + " - " + m_qsReportName );
}

void cDlgPreview::refreshReport()
{
}

void cDlgPreview::printReport()
{
}
