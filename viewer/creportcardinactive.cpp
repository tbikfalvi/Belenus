
#include "../framework/qtframework.h"
#include "creportcardinactive.h"

cReportCardInactive::cReportCardInactive(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Inactive patientcards ");
    m_qsReportDescription   = tr( "This report shows the currently inactive patient cards that registered into database." );
}

void cReportCardInactive::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQueryCards = QString( "SELECT barcode FROM patientcards WHERE "
                                    "patientCardId>1 AND "
                                    "patientCardTypeId<>1 AND "
                                    "pincode!=\"LOST\" AND "
                                    "active=0 " );

    m_dlgProgress.increaseProgressValue();

    QSqlQuery *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Barcode" ), "bold" );
    addTableRow();

    QString qsBarcodes = "";

    if( poQueryResultCards->first() )
    {
        qsBarcodes.append( poQueryResultCards->value(0).toString() );
        while( poQueryResultCards->next() )
        {
            qsBarcodes.append( QString( ", %1" ).arg( poQueryResultCards->value(0).toString() ) );
        }
    }
    addTableCell( qsBarcodes );
    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
