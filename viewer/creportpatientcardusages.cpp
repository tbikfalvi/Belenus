
#include "../framework/qtframework.h"
#include "creportpatientcardusages.h"
#include "currency.h"

cReportPCUsages::cReportPCUsages(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Patientcard usages ");
    m_qsReportDescription   = tr( "This report shows the patientcard usages for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate().addMonths(-1);

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportPCUsages::refreshReport()
//------------------------------------------------------------------------------------
{
    // General report init
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    QString qsQueryCards = QString( "SELECT dateTimeUsed, barcode, COUNT(dateTimeUsed) "
                                    "FROM patientcardunits "
                                    "JOIN patientcards ON patientcardunits.patientCardId=patientcards.patientCardId "
                                    "WHERE dateTimeUsed>=\"%1 00:00:00\" "
                                    "AND dateTimeUsed<=\"%2 24:00:00\" "
                                    "AND patientcardunits.active=0 "
                                    "GROUP BY dateTimeUsed" ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));
    QSqlQuery *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

    m_dlgProgress.setProgressValue( 10 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Date of usage" ), "center bold" );
    addTableCell( tr( "Patientcard barcode" ), "center bold" );
    addTableCell( tr( "Unit count" ), "center bold" );

    m_dlgProgress.setProgressMax( poQueryResultCards->size()+10 );

    while( poQueryResultCards->next() )
    {
        addTableRow();
        addTableCell( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm" ), "center" );
        addTableCell( poQueryResultCards->value(1).toString(), "center" );
        addTableCell( poQueryResultCards->value(2).toString(), "center" );
        m_dlgProgress.increaseProgressValue();
    }
    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
