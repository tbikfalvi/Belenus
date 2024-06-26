
#include "../framework/qtframework.h"
#include "creportcassahistory.h"
#include "currency.h"

cReportCassaHistory::cReportCassaHistory(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin, unsigned int p_uiUserId) : cReport(parent,p_qsReportName)
{
    cTracer obTrace( "cReportCassaHistory::cReportCassaHistory" );

    m_qsReportName          = tr(" Cassa history (list of actions)");
    m_qsReportDescription   = tr( "This report shows all of the events and action for the selected owner and date intervall. "
                                  "Please select the first and last day of the date intervall and the cassa owner you interested in." );

    _setDateStartEnabled( p_bIsAdmin );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( p_bIsAdmin );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Cassa owner :") );

    QStringList qslDataTypes;

    if( p_bIsAdmin )
    {
        qslDataTypes << QString( "0|%1" ).arg( tr("All users") );

        QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT userId, realName FROM users WHERE userId>1 AND active=1" );

        while( poQueryResult->next() )
        {
            qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
        }

        setFilterDataType( QString( "0|%1" ).arg( tr("All users") ) );
    }
    else
    {
        QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( QString("SELECT userId, realName FROM users WHERE userId=%1 AND active=1").arg(p_uiUserId) );

        poQueryResult->first();
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }
    setFilterDataTypeList( qslDataTypes.join("#") );

    setFilterIsVisible( false );
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

    QStringList qslFilterType = filterType().split("|");

    if( qslFilterType.at(0).toInt() > 0 )
    {
        qsCondition = QString( "cassahistory.userId=%1 AND " ).arg( qslFilterType.at(0).toInt() );
    }

    bool         bIsStornoEntriesHidden = filterIsVisible();
    QStringList  qslEntriesToHide = QStringList();

    QString      qsQuery = QString( "SELECT users.realName, actionTime, actionCash, actionCard, actionValue, cassahistory.comment, cassaHistoryId, parentId "
                                    "FROM cassahistory, users WHERE "
                                    "cassahistory.userId=users.userId AND "
                                    "cassaId>0 AND "
                                    "%1"
                                    "actionTime>\"%2 00:00:00\" AND "
                                    "actionTime<\"%3 24:00:00\" "
                                    "ORDER BY cassahistory.actionTime" ).arg( qsCondition ).arg( filterDateStart().toString( "yyyy-MM-dd" ) ).arg( filterDateStop().toString( "yyyy-MM-dd" ) );
    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    startReport();

    QString qsFilter;

    qsFilter.append( QString( " %1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );

    if( qslFilterType.at(0).toInt() > 0 )
    {
        qsFilter.append( tr( "  -  Related to user: %1" ).arg( qslFilterType.at(1) ) );
    }

    addTitle( m_qsReportName );
    addSubTitle( qsFilter );
    addHorizontalLine();

//    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
//    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Cassa owner" ), "bold" );
    addTableCell( tr( "Date/time" ), "center bold" );
    addTableCell( tr( "Cash" ), "right bold" );
    addTableCell( tr( "Card" ), "right bold" );
    addTableCell( tr( "Action amount" ), "center bold" );
    addTableCell( tr( "Description" ), "bold" );

    int nTotalCash   = 0;
    int nTotalCard   = 0;
    int nTotalAmount = 0;

    while( poQueryResult->next() )
    {
        if( bIsStornoEntriesHidden && poQueryResult->value(7).toInt() > 0 )
        {
            qslEntriesToHide << poQueryResult->value(7).toString();
        }
        else if( bIsStornoEntriesHidden && qslEntriesToHide.contains( poQueryResult->value(6).toString() ) )
        {
            // this is the parent of a storno entry
        }
        else
        {
            cCurrency   obCassaActionCash( poQueryResult->value(2).toInt() );
            cCurrency   obCassaActionCard( poQueryResult->value(3).toInt() );
            cCurrency   obCassaActionValue( poQueryResult->value(4).toInt() );

            nTotalCash   += poQueryResult->value(2).toInt();
            nTotalCard   += poQueryResult->value(3).toInt();
            nTotalAmount += poQueryResult->value(4).toInt();

            addTableRow();
            addTableCell( poQueryResult->value(0).toString() );
            addTableCell( poQueryResult->value(1).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
            addTableCell( obCassaActionCash.currencyFullStringShort(), "right" );
            addTableCell( obCassaActionCard.currencyFullStringShort(), "right" );
            addTableCell( obCassaActionValue.currencyFullStringShort(), "right" );
            addTableCell( poQueryResult->value(5).toString() );
        }
    }

    cCurrency   obTotalCash( nTotalCash );
    cCurrency   obTotalCard( nTotalCard );
    cCurrency   obTotalAmount( nTotalAmount );

    addTableRow();
    addTableCell( tr( "Sum" ), "bold" );
    addTableCell();
    addTableCell( obTotalCash.currencyFullStringShort(), "right bold" );
    addTableCell( obTotalCard.currencyFullStringShort(), "right bold" );
    addTableCell( obTotalAmount.currencyFullStringShort(), "right bold" );
    addTableCell();

    finishReport();

    m_dlgProgress.hide();
}
