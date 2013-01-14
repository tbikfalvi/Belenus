
#include "creportcardactive.h"

cReportCardActive::cReportCardActive(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Active patientcards ");
    m_qsReportDescription   = tr( "This report shows the active patient cards for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );
}
