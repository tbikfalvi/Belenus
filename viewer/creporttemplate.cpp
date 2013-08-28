
#include "../framework/qtframework.h"
#include "creporttemplate.h"

cReport__Template_ClassName__::cReport__Template_ClassName__(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Inactive patientcards ");
    m_qsReportDescription   = tr( "This report shows the __template_description__." );
}

void cReport__Template_ClassName__::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQuery = QString( "" );
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
    m_dlgProgress.setProgressValue( 0 );


    finishReport();

    m_dlgProgress.hide();
}
