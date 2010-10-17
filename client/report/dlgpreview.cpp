#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>

#include "../../framework/qtframework.h"
#include "dlgpreview.h"

cDlgPreview::cDlgPreview( QWidget *parent )
    : QDialog( parent, Qt::WindowMaximizeButtonHint )
{
    cTracer obTrace( "cDlgPreview::cDlgPreview" );

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
    cTracer obTrace( "cDlgPreview::~cDlgPreview" );
}

/*void cDlgPreview::keyPressEvent( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_F5 )
    {
        refreshReport();
    }

    QDialog::keyPressEvent( p_poEvent );
}*/

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
    QPrinter obPrinter( QPrinter::HighResolution );
    QPrintDialog obPrintDlg( &obPrinter, this );

    if( obPrintDlg.exec() == QDialog::Accepted )
    {
        m_tdReport.print( &obPrinter );
    }
}
