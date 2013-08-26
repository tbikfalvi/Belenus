
#include "../framework/qtframework.h"
#include "creportcassahistory.h"
#include "currency.h"

cReportCassaHistory::cReportCassaHistory(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    cTracer obTrace( "cReportCassaHistory::cReportCassaHistory" );

    m_qsReportName          = tr(" Cassa history ");
    m_qsReportDescription   = tr( "This report shows all of the events and action for the selected owner and date intervall. "
                                  "Please select the first and last day of the date intervall and the cassa owner you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Cassa owner :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All users") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT userId, realName FROM users WHERE userId>1 AND active=1" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( qslDataTypes.join("#") );

    _setDataIsVisibleEnabled( true );
    _setDataIsVisibleText( tr("Hide storno entries") );
}

void cReportCassaHistory::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString      qsCondition = "";

    if( filterType().left(1).toInt() > 0 )
    {
        qsCondition = QString( "cassahistory.userId=%1 AND " ).arg( filterType().left(1).toInt() );
    }

    bool         bIsStornoEntriesHidden = filterIsVisible();
    QStringList  qslEntriesToHide = QStringList();

    QString      qsQuery = QString( "SELECT users.realName, actionTime, actionValue, cassahistory.comment, cassaHistoryId, parentId "
                                    "FROM cassahistory, users WHERE "
                                    "cassahistory.userId=users.userId AND "
                                    "cassaId>0 AND "
                                    "%1"
                                    "actionTime>\"%2 00:00:00\" AND "
                                    "actionTime<\"%3 24:00:00\" "
                                    "ORDER BY cassahistory.actionTime" ).arg( qsCondition ).arg( filterDateStart().toString( "yyyy-MM-dd" ) ).arg( filterDateStop().toString( "yyyy-MM-dd" ) );
    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

//    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
//    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Cassa owner" ), "bold" );
    addTableCell( tr( "Date/time" ), "center bold" );
    addTableCell( tr( "Amount" ), "center bold" );
    addTableCell( tr( "Description" ), "bold" );

    int nTotalAmount = 0;

    while( poQueryResult->next() )
    {
        if( bIsStornoEntriesHidden && poQueryResult->value(5).toInt() > 0 )
        {
            qslEntriesToHide << poQueryResult->value(5).toString();
        }
        else if( bIsStornoEntriesHidden && qslEntriesToHide.contains( poQueryResult->value(4).toString() ) )
        {
            // this is the parent of a storno entry
        }
        else
        {
            cCurrency   obCassaActionValue( poQueryResult->value(2).toInt() );

            nTotalAmount += poQueryResult->value(2).toInt();

            addTableRow();
            addTableCell( poQueryResult->value(0).toString() );
            addTableCell( poQueryResult->value(1).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
            addTableCell( obCassaActionValue.currencyFullStringShort(), "right" );
            addTableCell( poQueryResult->value(3).toString() );
        }
    }

    cCurrency   obTotalAmount( nTotalAmount );

    addTableRow();
    addTableCell( tr( "Sum total" ), "bold" );
    addTableCell();
    addTableCell( obTotalAmount.currencyFullStringShort(), "right bold" );
    addTableCell();

    finishReport();

    m_dlgProgress.hide();
}
