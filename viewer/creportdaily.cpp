
#include "../framework/qtframework.h"
#include "currency.h"
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

void cReportDaily::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2" ).arg( tr( "Date :" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    QString qsQueryCassa = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName FROM cassa, users "
                                    "WHERE cassa.userId=users.userId AND "
                                    "( DATE(cassa.startDateTime)=\"%1\" OR DATE(cassa.stopDateTime)=\"%1\" OR DATE(cassa.modified)=\"%1\" ) " ).arg(filterDateStart().toString( "yyyy-MM-dd" ));
    QSqlQuery *poQueryResultCassa = g_poDB->executeQTQuery( qsQueryCassa );

    m_dlgProgress.setProgressMax( poQueryResultCassa->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Owner" ), "bold" );
    addTableCell( tr( "Balance" ), "bold" );
    addTableCell( tr( "Cassa started" ), "center bold" );
    addTableCell( tr( "Cassa closed" ), "center bold" );
    addTableCell( tr( "Last entry" ), "center bold" );
    m_dlgProgress.increaseProgressValue();

    while( poQueryResultCassa->next() )
    {
        cCurrency   cBalance( poQueryResultCassa->value(1).toInt() );

        addTableRow();
        addTableCell( poQueryResultCassa->value(5).toString() );
        addTableCell( cBalance.currencyFullStringLong() );
        addTableCell( poQueryResultCassa->value(2).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResultCassa->value(3).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResultCassa->value(4).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();

    addSeparator();
    addSubTitle( tr( "Products sold" ) );

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    QString qsQuery = QString("SELECT name, SUM(itemCount), SUM(totalPrice) FROM ledger WHERE ledgerTime>\"%1 00:00:00\" AND ledgerTime<\"%1 24:00:00\" AND productId>0 AND active=1 GROUP BY productId ").arg(filterDateStart().toString("yyyy-MM-dd"));
    poQueryResultCassa = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResultCassa->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Price"), "right bold" );

    unsigned int uiTotal = 0;

    while( poQueryResultCassa->next() )
    {
        cCurrency   obPrice( poQueryResultCassa->value(2).toInt() );
        uiTotal += poQueryResultCassa->value(2).toInt();

        addTableRow();
        addTableCell( poQueryResultCassa->value(0).toString() );
        addTableCell( poQueryResultCassa->value(1).toString(), "center" );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }
    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell( tr("Sum"), "bold" );
    addTableCell( "" );
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
