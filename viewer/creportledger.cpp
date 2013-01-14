
#include "creportledger.h"

cReportLedger::cReportLedger(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Main ledger ");
    m_qsReportDescription   = tr( "This report shows the bookkeepings, the cash book and every accounting data for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate().addMonths(-1);

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
}
