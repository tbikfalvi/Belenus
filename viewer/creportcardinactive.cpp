
#include "creportcardinactive.h"

cReportCardInactive::cReportCardInactive(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Inactive patientcards ");
    m_qsReportDescription   = tr( "This report shows the inactive patient cards." );
}
