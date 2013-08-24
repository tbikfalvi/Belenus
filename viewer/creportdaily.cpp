
#include "../framework/qtframework.h"
#include "currency.h"
#include "creportdaily.h"

//------------------------------------------------------------------------------------
cReportDaily::cReportDaily(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
//------------------------------------------------------------------------------------
{
    m_qsReportName          = tr( " Daily ledger " );
    m_qsReportDescription   = tr( "This report shows the daily bookkeepings the cash book and every accounting data for the selected day. "
                                  "Please select the day you interested in." );

  cReportDaily::_setDateStartEnabled( true );
  cReportDaily::_setDateStartLabelText( tr("Date :") );
    m_qdStartDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportDaily::refreshReport()
//------------------------------------------------------------------------------------
{
    // General report init
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2" ).arg( tr( "Date :" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    QString qsQuery;

    // Main data about cassa entries of the selected date
    _reportPartGeneral();

    // Product sold of the selected date
    addSeparator();
    addSubTitle( tr( "Products sold" ) );

        startSection();
        addTable();
        addTableRow();
        addTableCell( "" );
        addTableCell( tr("Product name"), "bold" );
        addTableCell( tr("Count"), "center bold" );
        addTableCell( tr("Price"), "right bold" );

        // Data from morning
        unsigned int uiTotalAm = _reportPartProductSellMorning();

        // Data from afternoon
        unsigned int uiTotalPm = _reportPartProductSellAfternoon();

        // Total of product sold
        cCurrency   obTotalPrice( uiTotalAm + uiTotalPm );

        addTableRow();
        addTableCell( tr("Sum total"), "bold" );
        addTableCell( "" );
        addTableCell( "" );
        addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

        finishTable();
        finishSection();

    // Product sold of the selected date
    addSeparator();
    addSubTitle( tr( "Patientcards sold" ) );

        startSection();
        addTable();

        unsigned int uiTotalPatientCards = _reportPartPatientCardSell();

        finishTable();
        finishSection();

    // Panel related data of the selected date
    addSeparator();
    addSubTitle( tr( "Device usages" ) );

        startSection();
        addTable();

        unsigned int uiTotalPanelUse = _reportPartPanelUse();

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

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartGeneral()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    qsQuery = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName, cassa.userId "
                       "FROM cassa, users "
                       "WHERE cassa.userId=users.userId AND "
                       "( DATE(cassa.startDateTime)=\"%1\" OR DATE(cassa.stopDateTime)=\"%1\" OR DATE(cassa.modified)=\"%1\" ) " ).arg(filterDateStart().toString( "yyyy-MM-dd" ));
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

    while( poQueryResult->next() )
    {
        cCurrency       cBalance( poQueryResult->value(1).toInt() );
        unsigned int    uiUserId    = poQueryResult->value(6).toUInt();
        QString         qsStart     = poQueryResult->value(2).toDateTime().toString("yyyy-MM-dd hh:mm");
        QString         qsStop      = poQueryResult->value(3).toDateTime().toString("yyyy-MM-dd hh:mm");
        cCurrency       obIncome( _sumUserIncome(uiUserId,qsStart,qsStop) );

        addTableRow();
        addTableCell( poQueryResult->value(5).toString() );
        addTableCell( cBalance.currencyFullStringShort(), "right" );
        addTableCell( obIncome.currencyFullStringShort(), "right" );
        addTableCell( qsStart, "center" );
        addTableCell( qsStop, "center" );
        addTableCell( poQueryResult->value(4).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartProductSellMorning()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    qsQuery = QString("SELECT name, SUM(itemCount), SUM(totalPrice) "
                      "FROM ledger WHERE "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%1 12:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "active=1 "
                      "GROUP BY productId ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_PROD_SELL);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    addTableRow();
    addTableCell( tr("Morning"), "bold" );
    addTableCell( "" );
    addTableCell( "" );
    addTableCell( "" );

    unsigned int uiTotalAm = 0;

    while( poQueryResult->next() )
    {
        cCurrency   obPrice( poQueryResult->value(2).toInt() );
        uiTotalAm += poQueryResult->value(2).toInt();

        addTableRow();
        addTableCell( "" );
        addTableCell( poQueryResult->value(0).toString() );
        addTableCell( poQueryResult->value(1).toString(), "center" );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }
    cCurrency   obTotalPriceAm( uiTotalAm );

    addTableRow();
    addTableCell( "" );
    addTableCell( tr("Sum"), "bold" );
    addTableCell( "" );
    addTableCell( obTotalPriceAm.currencyFullStringShort(), "right bold" );

    return uiTotalAm;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartProductSellAfternoon()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    qsQuery = QString("SELECT name, SUM(itemCount), SUM(totalPrice) "
                      "FROM ledger WHERE "
                      "ledgerTime>\"%1 12:00:00\" AND "
                      "ledgerTime<\"%1 24:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "active=1 "
                      "GROUP BY productId ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_PROD_SELL);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    addTableRow();
    addTableCell( tr("Afternoon"), "bold" );
    addTableCell( "" );
    addTableCell( "" );
    addTableCell( "" );

    unsigned int uiTotalPm = 0;

    while( poQueryResult->next() )
    {
        cCurrency   obPrice( poQueryResult->value(2).toInt() );
        uiTotalPm += poQueryResult->value(2).toInt();

        addTableRow();
        addTableCell( "" );
        addTableCell( poQueryResult->value(0).toString() );
        addTableCell( poQueryResult->value(1).toString(), "center" );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }
    cCurrency   obTotalPricePm( uiTotalPm );

    addTableRow();
    addTableCell( "" );
    addTableCell( tr("Sum"), "bold" );
    addTableCell( "" );
    addTableCell( obTotalPricePm.currencyFullStringShort(), "right bold" );

    return uiTotalPm;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    unsigned int    uiTotalCardSell = 0;

    addTableRow();
    addTableCell( "" );
    addTableCell( tr("Morning"), "center bold" );
    addTableCell( tr("Afternoon"), "center bold" );
    addTableCell( tr("Sum"), "center bold" );

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
        addTableCell( _countPatientCardTypeSellMorning(poQueryResult->value(0).toUInt()), "center" );
        addTableCell( _countPatientCardTypeSellAfternoon(poQueryResult->value(0).toUInt()), "center" );
        addTableCell( qsSumPrice, "right" );
    }

    cCurrency   obTotalPrice( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( "" );
    addTableCell( "" );
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    return uiTotalCardSell;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartPanelUse()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    unsigned int    uiTotalPanelUse = 0;

    addTableRow();
    addTableCell( "" );
    addTableCell( tr("Morning"), "center bold" );
    addTableCell( tr("Afternoon"), "center bold" );
    addTableCell( tr("Sum"), "center bold" );

    qsQuery = "SELECT * FROM paneltypes WHERE panelTypeId>0";
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int nPriceMorning   = _sumPanelUseMorning( poQueryResult->value(0).toUInt() );
        int nPriceAfternoon = _sumPanelUseAfternoon( poQueryResult->value(0).toUInt() );

        uiTotalPanelUse += nPriceMorning+nPriceAfternoon;

        QString qsPanelUseMorning   = "";
        QString qsPanelUseAfternoon = "";
        QString qsPanelUseFullday   = "";

        if( nPriceMorning > 0 )
        {
            cCurrency   obPanelUseMorning( nPriceMorning );

            qsPanelUseMorning = obPanelUseMorning.currencyFullStringShort();
        }
        if( nPriceAfternoon > 0 )
        {
            cCurrency   obPanelUseAfternoon( nPriceAfternoon );

            qsPanelUseAfternoon = obPanelUseAfternoon.currencyFullStringShort();
        }
        if( nPriceMorning+nPriceAfternoon > 0 )
        {
            cCurrency   obPanelUseFullday( nPriceMorning+nPriceAfternoon );

            qsPanelUseFullday = obPanelUseFullday.currencyFullStringShort();
        }

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( qsPanelUseMorning, "right" );
        addTableCell( qsPanelUseAfternoon, "right" );
        addTableCell( qsPanelUseFullday, "right" );
    }

    cCurrency   obTotalPanelUse( uiTotalPanelUse );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( "" );
    addTableCell( "" );
    addTableCell( obTotalPanelUse.currencyFullStringShort(), "right bold" );

    return uiTotalPanelUse;
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartPaymentMethods()
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    addTableRow();
    addTableCell();

    // SELECT SUM(actionValue) FROM cassahistory WHERE modified>"2013-08-24 00:00:00" AND modified<"2013-08-24 24:00:00" AND ledgerId>0 AND cassaId>0 GROUP BY cassaId
    // ha a ledgerId=0 akkor nyitás, zárás; ha a cassaId=0, akkor meg nem készpénzes fizetés
}
//------------------------------------------------------------------------------------
QString cReportDaily::_countPatientCardTypeSellMorning( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";

    qsQuery = QString("SELECT COUNT(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%1 12:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "ledger.patientCardTypeId=%3" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
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
QString cReportDaily::_countPatientCardTypeSellAfternoon( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";

    qsQuery = QString("SELECT COUNT(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 12:00:00\" AND "
                      "ledgerTime<\"%1 24:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "ledger.patientCardTypeId=%3" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
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
int cReportDaily::_sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nRet = 0;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%1 24:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "ledger.patientCardTypeId=%3" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        nRet = poQueryResult->value(0).toInt();
    }

    return nRet;
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumUserIncome( unsigned int p_uiUserId, QString p_qsStart, QString p_qsStop )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    if( p_qsStop.length() == 0 )
    {
        p_qsStop = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
    }

    qsQuery = QString( "SELECT SUM(totalPrice) "
                       "FROM ledger WHERE "
                       "userId=%1 AND "
                       "ledgerTime>\"%2\" AND "
                       "ledgerTime<\"%3\" " ).arg(p_uiUserId).arg(p_qsStart).arg(p_qsStop);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPanelUseMorning( unsigned int p_uiPanelTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, panels WHERE "
                      "ledger.panelId=panels.panelId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%1 12:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "panelTypeId=%3" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_DEVICE_USAGE).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPanelUseAfternoon( unsigned int p_uiPanelTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, panels WHERE "
                      "ledger.panelId=panels.panelId AND "
                      "ledgerTime>\"%1 12:00:00\" AND "
                      "ledgerTime<\"%1 24:00:00\" AND "
                      "ledgerTypeId=%2 AND "
                      "panelTypeId=%3" ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(LT_DEVICE_USAGE).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
