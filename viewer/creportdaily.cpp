
#include "creportdaily.h"

cReportDaily::cReportDaily(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr( " Daily ledger " );
    m_qsReportDescription   = tr( "This report shows the daily bookkeepings the cash book and every accounting data for the selected day. "
                                  "Please select the day you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("Date :") );
    m_qdStartDate = QDate::currentDate();
}
