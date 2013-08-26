
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

    QString qsQueryCards = QString( "SELECT pc.barcode, pct.name as cardtype, pc.units, p.name as owner, pc.comment "
                                    "FROM patientcards as pc, patientcardtypes as pct, patients as p "
                                    "WHERE pc.patientCardTypeId=pct.patientCardTypeId "
                                    "AND pc.patientId=p.patientId "
                                    "AND patientCardId>1 "
                                    "AND pc.patientCardTypeId<>1 "
                                    "AND pc.active=0 "
                                    );

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
    addTableCell( tr( "Barcode" ), "center bold" );
    addTableCell( tr( "Card type" ), "bold" );
    addTableCell( tr( "No. units" ), "center bold" );
    addTableCell( tr( "Card owner" ), "bold" );
    addTableCell( tr( "Comment" ), "bold" );

    while( poQueryResultCards->next() )
    {
        addTableRow();
        addTableCell( poQueryResultCards->value(0).toString(), "center" );
        addTableCell( poQueryResultCards->value(1).toString() );
        addTableCell( poQueryResultCards->value(2).toString(), "center" );
        addTableCell( poQueryResultCards->value(3).toString() );
        addTableCell( poQueryResultCards->value(4).toString() );
    }
    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
