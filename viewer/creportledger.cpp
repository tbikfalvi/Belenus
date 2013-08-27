
#include "../framework/qtframework.h"
#include "creportledger.h"
#include "currency.h"

cReportLedger::cReportLedger(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Main ledger ");
    m_qsReportDescription   = tr( "This report shows the bookkeepings, the cash book and every accounting data for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate().addMonths(-1);

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportLedger::refreshReport()
//------------------------------------------------------------------------------------
{
    // General report init
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    QString      qsQuery = QString( "" );
//    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    // Product sold
    addSeparator();
    addSubTitle( tr( "Products sold" ) );

        startSection();
        addTable();
        addTableRow();
        addTableCell();
        addTableCell( tr("Product name"), "bold" );
        addTableCell( tr("Count"), "center bold" );
        addTableCell( tr("Amount"), "right bold" );

        // Total of product sold
        cCurrency   obProductTotal( _reportPartProductSell() );

        addTableRow();
        addTableCell( tr("Sum total"), "bold" );
        addTableCell();
        addTableCell();
        addTableCell( obProductTotal.currencyFullStringShort(), "right bold" );

        finishTable();
        finishSection();

    // Patientcards sold
    addSeparator();
    addSubTitle( tr( "Patientcards sold" ) );

        startSection();
        addTable();

        cCurrency obPatientCardTotal( _reportPartPatientCardSell() );

        finishTable();
        finishSection();

    // Panel related data of the selected date
    addSeparator();
    addSubTitle( tr( "Device usages" ) );

        startSection();
        addTable();

        cCurrency obDeviceUsagesTotal( _reportPartPanelUse() );

        finishTable();
        finishSection();

    // Panel usage by patientcard units
    addSeparator();
    addSubTitle( tr( "Device usages by patientcard units" ) );

        startSection();
        addTable();

        _reportPartPanelUseUnits();

        finishTable();
        finishSection();

    // Income based on payment methods
    addSeparator();
    addSubTitle( tr( "Income by payment methods" ) );

        startSection();
        addTable();

        _reportPartPaymentMethods();

        finishTable();
        finishSection();

    // Income summary
    addSeparator();
    addSubTitle( tr( "Income summary" ) );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr("Products sold") );
        addTableCell( ":", "bold");
        addTableCell( obProductTotal.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Patientcards sold") );
        addTableCell( ":", "bold");
        addTableCell( obPatientCardTotal.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Device usages") );
        addTableCell( ":", "bold");
        addTableCell( obDeviceUsagesTotal.currencyFullStringShort(), "right" );

        cCurrency   obTotal( obProductTotal.currencyValue().toInt()+obPatientCardTotal.currencyValue().toInt()+obDeviceUsagesTotal.currencyValue().toInt() );
        addTableRow();
        addTableCell( tr("Sum total"), "bold" );
        addTableCell( ":", "bold");
        addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartProductSell()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    qsQuery = QString("SELECT name, SUM(itemCount), SUM(totalPrice) "
                      "FROM ledger WHERE "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "active=1 "
                      "GROUP BY productId ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PROD_SELL);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    addTableRow();
    addTableCell( tr("Morning"), "bold" );
    addTableCell();
    addTableCell();
    addTableCell();

    unsigned int uiTotal = 0;

    while( poQueryResult->next() )
    {
        cCurrency   obPrice( poQueryResult->value(2).toInt() );
        uiTotal += poQueryResult->value(2).toInt();

        addTableRow();
        addTableCell();
        addTableCell( poQueryResult->value(0).toString() );
        addTableCell( poQueryResult->value(1).toString(), "center" );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }
    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell();
    addTableCell( tr("Sum"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    return uiTotal;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    unsigned int    uiTotalCardSell = 0;

    addTableRow();
    addTableCell();
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Amount"), "center bold" );

    qsQuery = "SELECT * FROM patientcardtypes WHERE patientCardTypeId>1";
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        QString qsSumPrice  = "";
        int     nTotalPrice = _sumPatientCardTypeSell(poQueryResult->value(0).toUInt());

        uiTotalCardSell += nTotalPrice;

        if( nTotalPrice > 0 )
        {
            cCurrency obPrice( nTotalPrice );

            qsSumPrice = obPrice.currencyFullStringShort();
        }

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( _countPatientCardTypeSell(poQueryResult->value(0).toUInt()), "center" );
        addTableCell( qsSumPrice, "right" );
    }

    cCurrency   obTotalPrice( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    return uiTotalCardSell;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPanelUse()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    unsigned int    uiTotalPanelUse = 0;

    addTableRow();
    addTableCell();
    addTableCell( tr("Amount"), "center bold" );

    qsQuery = "SELECT * FROM paneltypes WHERE panelTypeId>0";
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int nPrice = _sumPanelUse( poQueryResult->value(0).toUInt() );

        uiTotalPanelUse += nPrice;

        QString qsPanelUse = "";

        if( nPrice > 0 )
        {
            cCurrency   obPanelUse( nPrice );

            qsPanelUse = obPanelUse.currencyFullStringShort();
        }

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( qsPanelUse, "right" );
    }

    cCurrency   obTotalPanelUse( uiTotalPanelUse );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( obTotalPanelUse.currencyFullStringShort(), "right bold" );

    return uiTotalPanelUse;
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartPanelUseUnits()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nCountTotal = 0;

    addTableRow();
    addTableCell();
    addTableCell( tr("Count"), "center bold" );

    qsQuery = QString( "SELECT * FROM paneltypes WHERE panelTypeId>0" );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int nCount = _countPanelUseUnits( poQueryResult->value(0).toUInt() );

        nCountTotal += nCount;

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( QString::number( nCount ), "center" );
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( QString::number(nCountTotal), "center bold" );
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartPaymentMethods()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nSumPayment = 0;

    addTableRow();
    addTableCell();
    addTableCell( tr("Amount"), "center bold");

    qsQuery = QString( "SELECT * FROM paymentmethods WHERE paymentMethodId>0" );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int     nPayment = _sumPaymentMethod( poQueryResult->value(0).toInt() );
        QString qsPayment = "";

        nSumPayment += nPayment;

        if( nPayment > 0 )
        {
            cCurrency   obSum( nPayment );
            qsPayment = obSum.currencyFullStringShort();
        }

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( qsPayment, "right" );
    }

    cCurrency   obSumTotal( nSumPayment );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( obSumTotal.currencyFullStringShort(), "right bold");
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nRet = 0;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "ledger.patientCardTypeId=%4" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        nRet = poQueryResult->value(0).toInt();
    }

    return nRet;
}
//------------------------------------------------------------------------------------
QString cReportLedger::_countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";

    qsQuery = QString("SELECT COUNT(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "ledger.patientCardTypeId=%4" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        if( poQueryResult->value(0).toInt() > 0 )
        {
            qsRet = QString::number( poQueryResult->value(0).toInt() );
        }
    }

    return qsRet;
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPanelUse( unsigned int p_uiPanelTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, panels WHERE "
                      "ledger.panelId=panels.panelId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "panelTypeId=%4" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_DEVICE_USAGE).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_countPanelUseUnits( unsigned int p_uiPanelTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT COUNT(patientCardUnitId) "
                      "FROM patientcardunits, panels, paneltypes WHERE "
                      "patientcardunits.panelId=panels.panelId AND "
                      "panels.panelTypeId=paneltypes.panelTypeId AND "
                      "patientcardunits.panelId>0 AND "
                      "dateTimeUsed>\"%1 00:00:00\" AND "
                      "dateTimeUsed<\"%2 24:00:00\" AND "
                      "paneltypes.panelTypeId=%3 "
                      "GROUP BY paneltypes.panelTypeId ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPaymentMethod( unsigned int p_uiPaymentMethodId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger WHERE "
                      "modified>\"%1 00:00:00\" AND "
                      "modified<\"%2 24:00:00\" AND "
                      "ledgerTypeId>0 AND "
                      "ledgerTypeId<9 AND "
                      "paymentMethodId=%3 ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(p_uiPaymentMethodId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumCassaIncome( unsigned int p_uiCassaId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(actionValue) "
                      "FROM cassahistory WHERE "
                      "cassaId>0 AND "
                      "ledgerId>0 AND "
                      "cassaId=%1 ").arg(p_uiCassaId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
