
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
    cReport::refreshReport();

    m_tcReport->insertHtml( "<html><body>" );
    m_tcReport->insertHtml( "<div>" );

    m_tcReport->insertText( m_qsReportName + "   ", *obTitleFormat );
    m_tcReport->setCharFormat( *obNormalFormat );
    m_tcReport->insertText( QString( "%1 %2" ).arg( tr( "Date :" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ) );

    m_tcReport->insertHtml( "</div>");
    m_tcReport->insertHtml( "<hr>" );
//    m_tcReport->insertHtml( "<div align=left>" );

    QString qsQueryCassa = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName FROM cassa, users "
                                    "WHERE cassa.userId=users.userId AND "
                                    "( DATE(cassa.startDateTime)=\"%1\" OR DATE(cassa.stopDateTime)=\"%1\" OR DATE(cassa.modified)=\"%1\" ) " ).arg(filterDateStart().toString( "yyyy-MM-dd" ));
    QSqlQuery *poQueryResultCassa = g_poDB->executeQTQuery( qsQueryCassa );
    //m_tcReport->insertText( qsQueryCassa, *obNormalFormat );

    while( poQueryResultCassa->next() )
    {
        cCurrency   cBalance( poQueryResultCassa->value(1).toInt() );

//        uint uiCassaId          = poQueryResultCassa->value(0).toUInt();
        QString qsBalance       = cBalance.currencyFullStringLong();
        QString qsDateStart     = poQueryResultCassa->value(2).toDateTime().toString("yyyy-MM-dd hh:mm");
        QString qsDateStop      = poQueryResultCassa->value(3).toDateTime().toString("yyyy-MM-dd hh:mm");
        QString qsLastModified  = poQueryResultCassa->value(4).toDateTime().toString("yyyy-MM-dd hh:mm");
        QString qsCassaOwner    = poQueryResultCassa->value(5).toString();

        obTableFormat->setHeaderRowCount( 0 );
        obTableFormat->setAlignment(Qt::AlignLeft);
        m_tcReport->insertTable( 5, 2, *obTableFormat );

        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Cassa owner :" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsCassaOwner, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );

        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Cassa balance :" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsBalance, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );

        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Cassa started :" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsDateStart, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );

        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Cassa stopped :" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsDateStop, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );

        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Cassa last entry :" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsLastModified, *obNormalFormat );
    }

//    m_tcReport->insertHtml( "</div>");

/*
    m_qsQuery = QString("SELECT name, totalPrice, comment, ledgerTime FROM ledger WHERE ledgerTime>\"%1 00:00:00\" AND ledgerTime<\"%1 24:00:00\"").arg(filterDateStart().toString("yyyy-MM-dd"));
    poQueryResultCassa = g_poDB->executeQTQuery( m_qsQuery );

    while( poQueryResultCassa->next() )
    {
        cCurrency   cTotalPrice( poQueryResultCassa->value(1).toInt() );
        m_tcReport->insertText( QString("%1\t%2\t%3\t%4\n").arg(poQueryResultCassa->value(0).toString()).arg(cTotalPrice.currencyFullStringShort()).arg(poQueryResultCassa->value(2).toString()).arg(poQueryResultCassa->value(3).toString()) );
    }
*/
    m_tcReport->insertHtml( QString("</body></html>") );

}
