
#include "creportcardusage.h"

cReportCardUsage::cReportCardUsage(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Patientcards usages ");
    m_qsReportDescription   = tr( "This report shows the usage of patient cards for the selected date intervall. "
                                  "Please select the barcode of the patient card or first and last day of the date intervall you interested in." );
}
