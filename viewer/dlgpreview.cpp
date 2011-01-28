#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "../../framework/qtframework.h"
#include "dlgpreview.h"

cDlgPreview::cDlgPreview( QWidget *parent )
    : QDialog( parent, Qt::WindowMaximizeButtonHint )
{
    cTracer obTrace( "cDlgPreview::cDlgPreview" );

    setupUi( this );

    m_qsReportName = "";
    m_qsReportFileName = tr( "report" );

    QPushButton *poSaveToFileButton = btbButtons->addButton( tr( "&Save to html" ), QDialogButtonBox::ActionRole );
    QPushButton *poPrintButton = btbButtons->addButton( tr( "&Print ..." ), QDialogButtonBox::ActionRole );
    btbButtons->addButton( tr( "&Close" ), QDialogButtonBox::RejectRole );

    connect( btnRefresh, SIGNAL(clicked()), this, SLOT(refreshReport()) );
    connect( poPrintButton, SIGNAL(clicked()), this, SLOT(printReport()) );
    connect( poSaveToFileButton, SIGNAL(clicked()), this, SLOT(saveReportToFile()) );

    tedPreview->setDocument( &m_tdReport );
}

cDlgPreview::~cDlgPreview()
{
    cTracer obTrace( "cDlgPreview::~cDlgPreview" );
}

void cDlgPreview::keyPressEvent( QKeyEvent *p_poEvent )
{
    if( p_poEvent->key() == Qt::Key_F5 )
    {
        refreshReport();
    }

    QDialog::keyPressEvent( p_poEvent );
}

void cDlgPreview::setReportTitle( const QString &p_qsTitle )
{
    m_qsReportName = p_qsTitle;
    setWindowTitle( windowTitle() + " - " + m_qsReportName );
}

void cDlgPreview::setReportFileName( const QString &p_qsFileName )
{
    m_qsReportFileName = p_qsFileName;
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

void cDlgPreview::saveReportToFile()
{
    QString     qsFileName = QString( "%1_%2.html" ).arg( m_qsReportFileName ).arg( QDate::currentDate().toString("yyyyMMdd") );

    QFile   obReportFile( qsFileName );

    obReportFile.open( QIODevice::WriteOnly );
    obReportFile.write( m_tdReport.toHtml("utf8_general_ci").toStdString().c_str() );
    obReportFile.close();

    QMessageBox::information( this, tr("Information"),
                              tr("Exporting report to HTML file finished.\n"
                                 "Name of the file is: %1").arg(qsFileName) );
}

QString cDlgPreview::intTimeToString( const int p_inTime )
{
    QTime   qtTemp;

    qtTemp = QTime( p_inTime/3600, (p_inTime%3600)/60, (p_inTime%3600)%60, 0 );

    return qtTemp.toString("hh:mm:ss");
}

