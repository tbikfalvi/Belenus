
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

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    addSeparator();
    addSubTitle( tr( "Cassa list" ) );
    _reportPartCassaList();

    // Product sold
    addSeparator();
    addSubTitle( tr( "Products sold" ) );
    cCurrency   obProductTotal( _reportPartProductSell() );

    // Patientcards sold
    addSeparator();
    addSubTitle( tr( "Patientcards sold" ) );
    cCurrency obPatientCardTotal( _reportPartPatientCardSell() );

    // Panel related data of the selected date
    addSeparator();
    addSubTitle( tr( "Device usages income" ) );
    cCurrency obDeviceUsagesTotal( _reportPartPanelUse() );

    // Panel usage by patientcard units
    addSeparator();
    addSubTitle( tr( "Device usages by patientcard units" ) );
    _reportPartPanelUseType( PU_USE_WITH_CARD );

    // Panel usage by cash
    addSeparator();
    addSubTitle( tr( "Device usages by cash" ) );
    _reportPartPanelUseType( PU_USE_WITH_CASH );

    // Panel usage by cash and units
    addSeparator();
    addSubTitle( tr( "Device usages by patientcard units and cash" ) );
    _reportPartPanelUseType( PU_USE_COMBINED );

    // Income based on payment methods
    addSeparator();
    addSubTitle( tr( "Income by payment methods" ) );
    _reportPartPaymentMethods();

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
void cReportLedger::_reportPartCassaList()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    qsQuery = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName, cassa.userId "
                       "FROM cassa, users "
                       "WHERE cassa.userId=users.userId AND ("
                       " (DATE(cassa.startDateTime)>=\"%1\" AND DATE(cassa.startDateTime)<=\"%2\") OR "
                       " (DATE(cassa.stopDateTime)>=\"%1\" AND DATE(cassa.stopDateTime)<=\"%2\") OR "
                       " (DATE(cassa.modified)>=\"%1\" AND DATE(cassa.modified)<=\"%2\") "
                       ") ORDER BY startDateTime" ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Owner" ), "bold" );
    addTableCell( tr( "Balance" ), "center bold" );
    addTableCell( tr( "Income" ), "center bold" );
    addTableCell( tr( "Cassa started" ), "center bold" );
    addTableCell( tr( "Cassa closed" ), "center bold" );
    addTableCell( tr( "Last entry" ), "center bold" );
    m_dlgProgress.increaseProgressValue();

    int nSumTotal = 0;

    while( poQueryResult->next() )
    {
        cCurrency       cBalance( poQueryResult->value(1).toInt() );
        unsigned int    uiCassaId   = poQueryResult->value(0).toUInt();
        int             nSum = _sumCassaIncome(uiCassaId);
        cCurrency       obIncome( nSum );

        nSumTotal += nSum;

        addTableRow();
        addTableCell( poQueryResult->value(5).toString() );
        addTableCell( cBalance.currencyFullStringShort(), "right" );
        addTableCell( obIncome.currencyFullStringShort(), "right" );
        addTableCell( poQueryResult->value(2).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResult->value(3).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResult->value(4).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        m_dlgProgress.increaseProgressValue();
    }

    cCurrency obSumTotal( nSumTotal );

    addTableRow();
    addTableCell( tr( "Sum total" ), "bold" );
    addTableCell();
    addTableCell( obSumTotal.currencyFullStringShort(), "right bold" );
    addTableCell();
    addTableCell();
    addTableCell();

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartProductSell()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    startSection();
    addTable();

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
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Amount"), "right bold" );

    unsigned int uiTotal = 0;

    while( poQueryResult->next() )
    {
        cCurrency   obPrice( poQueryResult->value(2).toInt() );
        uiTotal += poQueryResult->value(2).toInt();

        addTableRow();
        addTableCell( poQueryResult->value(0).toString() );
        addTableCell( poQueryResult->value(1).toString(), "center" );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }
    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotal;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
    unsigned int    uiTotalCardSell = 0;
    QSqlQuery      *poQueryResult;

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Amount"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM patientcardtypes WHERE patientCardTypeId>1" );

    while( poQueryResult->next() )
    {
        int nTotalPrice = _sumPatientCardTypeSell(poQueryResult->value(0).toUInt());

        uiTotalCardSell += nTotalPrice;

        if( nTotalPrice > 0 )
        {
            cCurrency obPrice( nTotalPrice );

            addTableRow();
            addTableCell( poQueryResult->value(2).toString() );
            addTableCell( _countPatientCardTypeSell(poQueryResult->value(0).toUInt()), "center" );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
    }

    cCurrency   obTotalPrice( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotalCardSell;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPanelUse()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    unsigned int    uiTotalPanelUse = 0;

    startSection();
    addTable();

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

    finishTable();
    finishSection();

    return uiTotalPanelUse;
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartPanelUseType(tePanelUse p_tePanelUse)
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nCountTotal = 0;

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    addTableCell( tr("Count"), "center bold" );

    qsQuery = QString( "SELECT * FROM paneltypes WHERE panelTypeId>0" );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int nCount = _countPanelUse( poQueryResult->value(0).toUInt(), p_tePanelUse );

        nCountTotal += nCount;

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( QString::number( nCount ), "center" );
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( QString::number(nCountTotal), "center bold" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartPaymentMethods()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nSumPayment = 0;

    startSection();
    addTable();

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

    finishTable();
    finishSection();
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
                      "ledger.patientCardTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
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
                      "ledger.patientCardTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
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
                      "panelTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_DEVICE_USAGE).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_countPanelUse(unsigned int p_uiPanelTypeId, tePanelUse p_tePanelUse)
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;
    QString     qsCond;

    switch( p_tePanelUse )
    {
        case PU_USE_WITH_CARD:
            qsCond = "units>0 AND cash=0 AND ";
            break;
        case PU_USE_WITH_CASH:
            qsCond = "units=0 AND cash>0 AND ";
            break;
        case PU_USE_COMBINED:
            qsCond = "units>0 AND cash>0 AND ";
            break;
    }

    qsQuery = QString("SELECT COUNT(ledgerDeviceId) FROM "
                      "ledgerdevice, panels WHERE "
                      "ledgerdevice.panelId = panels.panelId AND "
                      "%1 "
                      "panelTypeId=%2 AND "
                      "ledgerTime>\"%3 00:00:00\" AND "
                      "ledgerTime<\"%4 24:00:00\" ").arg( qsCond ).arg( p_uiPanelTypeId ).arg( filterDateStart().toString("yyyy-MM-dd") ).arg( filterDateStop().toString("yyyy-MM-dd") );
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
                      "paymentMethodId=%3 AND "
                      "ledger.active=1 ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(p_uiPaymentMethodId);
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
