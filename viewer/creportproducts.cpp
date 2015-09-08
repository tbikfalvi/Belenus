
#include "../framework/qtframework.h"
#include "creportproducts.h"
#include "currency.h"

cReportProducts::cReportProducts(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Products ");
    m_qsReportDescription   = tr( "This report shows the products registered in database." );

    _setDataNameEnabled( true );
    _setDataNameLabelText( tr("Name contains :") );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Product types :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "-1|%1" ).arg( tr("All type") );
    qslDataTypes << QString( "0|%1" ).arg( tr("Products with no type") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT productTypeId, name FROM producttypes WHERE productTypeId>0 AND active=1" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( QString( "-1|%1" ).arg( tr("All type") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );

    setFilterIsVisible( false );
    _setDataIsVisibleEnabled( true );
    _setDataIsVisibleText( tr("Show producst with zero item count") );
}

void cReportProducts::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQuery;
    QString qsTitle = "";
    QString qsCondition = "";
    QStringList qslFilterType = filterType().split("|");

    if( qslFilterType.at(0).toInt() > -1 )
    {
        qsTitle.append( tr( "Product type: '%1'" ).arg( qslFilterType.at(1) ) );
    }
    if( filterName().length() > 0 )
    {
        qsTitle.append( tr( "%1Product name contains: '%2'" ).arg( qsTitle.length()?"  -  ":"" ).arg( filterName() ) );
        qsCondition.append( QString( " AND products.name LIKE \"\%%1\%\" ").arg( filterName() ) );
    }
    if( !filterIsVisible() )
    {
        qsCondition.append( " AND products.productCount>0 " );
    }

    if( qslFilterType.at(0).toInt() == -1 )
    {
        qsQuery = QString( "SELECT productId, name, barcode, netPriceSell, vatpercentSell, productCount FROM products WHERE "
                           "active=1 "
                           "%1" ).arg( qsCondition );
    }
    else if( qslFilterType.at(0).toInt() == 0 )
    {
        qsQuery = QString( "SELECT products.productId, "
                           "products.name, "
                           "products.barcode, "
                           "products.netPriceSell, "
                           "products.vatpercentSell, "
                           "products.productCount, "
                           "connectproductwithtype.productTypeId "
                           "FROM products LEFT JOIN connectproductwithtype "
                           "ON products.productId=connectproductwithtype.productId WHERE "
                           "products.active=1 AND "
                           "ISNULL(connectproductwithtype.productTypeId) "
                           "%1"
                           "GROUP BY products.productId" ).arg( qsCondition );
    }
    else
    {
        qsQuery = QString( "SELECT products.productId, "
                           "products.name, "
                           "products.barcode, "
                           "products.netPriceSell, "
                           "products.vatpercentSell, "
                           "products.productCount, "
                           "connectproductwithtype.productTypeId "
                           "FROM products LEFT JOIN connectproductwithtype "
                           "ON products.productId=connectproductwithtype.productId WHERE "
                           "products.active=1 AND "
                           "connectproductwithtype.productTypeId=%1 "
                           "%2"
                           "GROUP BY products.productId" ).arg( qslFilterType.at(0).toInt() ).arg( qsCondition );
    }

    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    if( qsTitle.length() )
        addSubTitle( qsTitle );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Name"), "bold" );
    addTableCell( tr("Barcode"), "bold" );
    addTableCell( tr("Price"), "center bold" );
    addTableCell( tr("Count"), "center bold" );

    while( poQueryResult->next() )
    {
        cCurrency   obTotalPrice( poQueryResult->value(3).toInt(), cCurrency::CURR_NET, poQueryResult->value(4).toInt() );

        addTableRow();
        addTableCell( poQueryResult->value(1).toString() );
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( obTotalPrice.currencyFullStringShort(), "right" );
        addTableCell( poQueryResult->value(5).toString(), "center" );
    }

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
