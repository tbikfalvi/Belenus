
#include "../framework/qtframework.h"
#include "creportproducthistory.h"
#include "currency.h"

cReportProductHistory::cReportProductHistory(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Product history ");
    m_qsReportDescription   = tr( "This report shows the product history. Please select the product you are interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Products :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("<No product selected>") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT productId, name FROM products WHERE productId>0 AND active=1 ORDER BY name" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( QString( "0|%1" ).arg( tr("<No product selected>") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}

void cReportProductHistory::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QStringList qslFilterType = filterType().split("|");

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    if( qslFilterType.at(0).toInt() == 0 )
    {
        startSection();
        addTable();
        addTableRow();
        addTableCell( tr("No product selected"), "center italic" );
        finishTable();
        finishSection();
    }
    else
    {
        QString qsQuery = QString( "SELECT "
                                   "ledger.ledgerTypeId, "
                                   "ledger.name, "
                                   "ledger.itemCount, "
                                   "totalPrice, "
                                   "ledgerTime, "
                                   "productItemCount, "
                                   "cassaActionIndication "
                                   "FROM ledger "
                                   "LEFT JOIN producthistory ON ledger.ledgerId=producthistory.ledgerId "
                                   "LEFT JOIN productactiontype ON productactiontype.productActionTypeId=producthistory.productActionTypeId "
                                   "WHERE "
                                   "ledger.productId=%1 AND "
                                   "DATE(ledgerTime)>=\"%2\" AND "
                                   "DATE(ledgerTime)<=\"%3\" AND "
                                   "ledger.active=1" ).arg( qslFilterType.at(0).toInt() ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));
        QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

        m_dlgProgress.setProgressValue( 90 );

        m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
        m_dlgProgress.setProgressValue( 0 );

        addSeparator();
        addSubTitle( qslFilterType.at(1) );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr("Action description"), "bold" );
        addTableCell( tr("Count"), "center bold" );
        addTableCell( tr("Action amount"), "right bold" );
        addTableCell( tr("Date"), "center bold" );

        int nSumCount = 0;
        int nSumPrice = 0;

        while( poQueryResult->next() )
        {
            int nPrice = poQueryResult->value(3).toInt();

            if( poQueryResult->value(6).toString().compare(tr("Negative")) == 0 )
            {
                nPrice *= (-1);
            }
            int nCount = 0;

            if( poQueryResult->value(0).toInt() == LT_PROD_STORAGE_CHANGE )
            {
                nCount = poQueryResult->value(5).toInt();
            }
            else
            {
                nCount = poQueryResult->value(2).toInt() * (-1);
            }

            nSumPrice += nPrice;
            nSumCount += nCount;

            cCurrency   cPrice( nPrice );

            addTableRow();
            if( poQueryResult->value(0).toInt() == LT_PROD_STORAGE_CHANGE )
            {
                addTableCell( poQueryResult->value(1).toString() );
            }
            else
            {
                addTableCell( tr("Product sold") );
            }
            if( poQueryResult->value(0).toInt() == LT_PROD_STORAGE_CHANGE )
            {
                addTableCell( poQueryResult->value(5).toString(), "center" );
            }
            else
            {
                addTableCell( poQueryResult->value(2).toString(), "center" );
            }
            addTableCell( cPrice.currencyFullStringShort(), "right" );
            addTableCell( poQueryResult->value(4).toDateTime().toString("yyyy-MM-dd hh:mm") );
        }

        cCurrency   cPriceSum( nSumPrice );

        addTableRow();
        addTableCell( tr("Sum"), "bold" );
        addTableCell( QString::number( nSumCount ), "center bold" );
        addTableCell( cPriceSum.currencyFullStringShort(), "right bold" );
        addTableCell();

        finishTable();
        finishSection();
    }

    finishReport();

    m_dlgProgress.hide();
}
