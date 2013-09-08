
#include "../framework/qtframework.h"
#include "creportproductstatus.h"

cReportProductStatus::cReportProductStatus(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Product status ");
    m_qsReportDescription   = tr( "This report shows the product status on the selected date. "
                                  "Please select the date you are interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("Date :") );
    m_qdStartDate = QDate::currentDate();
}

void cReportProductStatus::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQuery = QString( "SELECT productTypeId, name FROM producttypes WHERE active=1 ORDER BY name" );
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    _reportPartProductTypeGroup( tr("Products with no type"), 0 );

    while( poQueryResult->next() )
    {
        _reportPartProductTypeGroup( poQueryResult->value(1).toString(), poQueryResult->value(0).toUInt() );
    }

    finishReport();

    m_dlgProgress.hide();
}

void cReportProductStatus::_reportPartProductTypeGroup(QString p_qsSubTitle, unsigned int p_uiProductTypeId)
{
    QString qsQuery;

    if( p_uiProductTypeId == 0 )
    {
        qsQuery = QString( "SELECT products.productId, "
                           "products.name, "
                           "products.barcode, "
                           "products.productCount, "
                           "connectproductwithtype.productTypeId "
                           "FROM products LEFT JOIN connectproductwithtype "
                           "ON products.productId=connectproductwithtype.productId WHERE "
                           "products.active=1 AND "
                           "ISNULL(connectproductwithtype.productTypeId) "
                           "GROUP BY products.productId" );
    }
    else
    {
        qsQuery = QString( "SELECT products.productId, name, barcode, productCount "
                           "FROM products, connectproductwithtype WHERE "
                           "products.productId = connectproductwithtype.productId AND "
                           "productTypeId=%1 AND "
                           "products.active=1 ").arg( p_uiProductTypeId );
    }
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->numRowsAffected() == 0 )
        return;

    addSeparator();
    addSubTitle( p_qsSubTitle );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Barcode"), "bold" );
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Actual count"), "center bold" );

    int nCountDateSum   = 0;
    int nCountActSum    = 0;

    while( poQueryResult->next() )
    {
        int nCountDate = _countProductToDate( poQueryResult->value(0).toUInt(), filterDateStart() );

        addTableRow();
        addTableCell( poQueryResult->value(1).toString() );
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( QString::number( nCountDate ), "center" );
        addTableCell( poQueryResult->value(3).toString(), "center" );

        nCountDateSum += nCountDate;
        nCountActSum  += poQueryResult->value(3).toInt();
    }

    addTableRow();
    addTableCell( tr("Sum"), "bold" );
    addTableCell();
    addTableCell( QString::number( nCountDateSum ), "center bold" );
    addTableCell( QString::number( nCountActSum ), "center bold" );

    finishTable();
    finishSection();
}

int cReportProductStatus::_countProductToDate(unsigned int p_uiProductId, const QDate p_qdFilterDate)
{
    int         nCountProduct = 0;
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    qsQuery = QString( "SELECT productItemCount, increaseProductCount, decreaseProductCount, actionDateTime FROM "
                       "producthistory, productactiontype WHERE "
                       "producthistory.productActionTypeId=productactiontype.productActionTypeId AND "
                       "producthistory.productId=%1 AND "
                       "actionDateTime<\"%2 24:00:00\" " ).arg( p_uiProductId ).arg( p_qdFilterDate.toString("yyyy-MM-dd") );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        if( poQueryResult->value(1).toInt() == 1 || poQueryResult->value(2).toInt() == 1 )
        {
            nCountProduct += poQueryResult->value(0).toInt();
        }
    }

    qsQuery = QString( "SELECT itemCount, ledgerTime FROM "
                       "ledger WHERE "
                       "productId=%1 AND "
                       "ledgerTime<\"%2 24:00:00\" AND "
                       "ledgerTypeId=%3 AND "
                       "active=1" ).arg( p_uiProductId ).arg( p_qdFilterDate.toString("yyyy-MM-dd") ).arg(LT_PROD_SELL);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        nCountProduct -= poQueryResult->value(0).toInt();
    }

    return nCountProduct;
}

