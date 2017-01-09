
#include "../framework/qtframework.h"
#include "creportmonthclose.h"
#include "currency.h"

cReportMonthClose::cReportMonthClose(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Monthly close ");
    m_qsReportDescription   = tr( "This report shows the product and patientcard related summaries for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate( QDate::currentDate().year(), QDate::currentDate().month(), 1 );

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportMonthClose::refreshReport()
//------------------------------------------------------------------------------------
{
    m_qslCassaIds.clear();

    // General report init
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    _reportPartCassaList();

    // Product sold
    addSeparator();
    addSubTitle( tr( "Products sold" ) );
    _reportPartProductSell();

    // Patientcards sold
    addSeparator();
    addSubTitle( tr( "Patientcards sold / refilled / attached" ) );
    _reportPartPatientCardSell();

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
void cReportMonthClose::_reportPartCassaList()
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

    m_dlgProgress.increaseProgressValue();

    while( poQueryResult->next() )
    {
        m_qslCassaIds << poQueryResult->value( 0 ).toString();
        m_dlgProgress.increaseProgressValue();
    }
}
//------------------------------------------------------------------------------------
unsigned int cReportMonthClose::_reportPartProductSell()
//------------------------------------------------------------------------------------
{
//    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Count"), "center bold" );
//    addTableCell( tr("Amount"), "right bold" );

    QSqlQuery  *poQueryProducts = g_poDB->executeQTQuery( QString( "SELECT name, SUM(itemCount), SUM(totalPrice) "
                                                                   "FROM cassahistory, ledger WHERE "
                                                                   "cassahistory.ledgerId=ledger.ledgerId AND "
                                                                   "cassahistory.ledgerId>0 AND "
                                                                   "cassaId IN( %1 ) AND "
                                                                   "ledgerTypeId=%2 AND "
                                                                   "ledger.active=1 "
                                                                   "GROUP BY productId" ).arg( m_qslCassaIds.join(",") ).arg( LT_PROD_SELL ) );
    unsigned int uiTotal = 0;
    while( poQueryProducts->next() )
    {
//        cCurrency   obPrice( poQueryProducts->value(2).toInt() );
//        uiTotalCassa += poQueryProducts->value(2).toInt();

        addTableRow();
        addTableCell( poQueryProducts->value(0).toString() );
        addTableCell( poQueryProducts->value(1).toString(), "center" );
        uiTotal += poQueryProducts->value(1).toUInt();
//        addTableCell( obPrice.currencyFullStringShort(), "right" );
    }
//    cCurrency   obTotalPriceAm( uiTotalCassa );

    addTableRow();
    addTableCell( tr("Sum"), "bold" );
    addTableCell( QString::number(uiTotal), "center bold" );

    finishTable();
    finishSection();

    return uiTotal;
}
//------------------------------------------------------------------------------------
unsigned int cReportMonthClose::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
//    unsigned int    uiTotalCardSell = 0;
    QSqlQuery      *poQueryResult;

    startSection();
    addTable();

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM patientcardtypes WHERE patientCardTypeId>1" );

    unsigned int    uiCountPCSum = 0;

    while( poQueryResult->next() )
    {
        unsigned int    uiCountPCTSum = 0;
        unsigned int    uiPricePCTSum = 0;
        QString         qsPricePCTSum = "";

        unsigned int uiPricePCSell = 0;
        QString      qsCountPCSell = _countsumPatientCardTypeSell( m_qslCassaIds.join(","), poQueryResult->value(0).toUInt(), &uiPricePCSell );

        uiCountPCTSum += qsCountPCSell.toInt();
        uiPricePCTSum += uiPricePCSell;

        if( uiPricePCTSum > 0 || qsCountPCSell.toInt() > 0 )
        {
            addTableRow();
            addTableCell( poQueryResult->value(2).toString() );

//            cCurrency   obPricePCTSum( uiPricePCTSum );

//            qsPricePCTSum = obPricePCTSum.currencyFullStringShort();
//            addTableCell( QString( "%1 / %2" ).arg( uiCountPCTSum ).arg( qsPricePCTSum ) );
            addTableCell( QString( "%1" ).arg( uiCountPCTSum ) );
        }
        uiCountPCSum += uiCountPCTSum;

//        uiTotalCardSell += uiPricePCTSum;
    }

//    cCurrency obTotalCardSell( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum"), "bold" );
    addTableCell( QString::number( uiCountPCSum ), "center bold" );

    finishTable();
    finishSection();

    return 0;//uiTotalCardSell;
}
//------------------------------------------------------------------------------------
int cReportMonthClose::_sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
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
QString cReportMonthClose::_countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
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
QString cReportMonthClose::_countsumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId, unsigned int *p_uiPricePCSell )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";
    QString         qsPCCondition = QString( "(ledgerTypeId=%1 OR ledgerTypeId=%2 OR ledgerTypeId=%3 OR ledgerTypeId=%4 OR ledgerTypeId=%5 OR ledgerTypeId=%6)" )
                                            .arg(LT_PC_SELL)
                                            .arg(LT_PC_REFILL)
                                            .arg(LT_PC_LOST_REPLACE)
                                            .arg(LT_PC_ASSIGN_PARTNER)
                                            .arg(LT_PC_ONLINE_SELL)
                                            .arg(LT_PC_ONLINE_REFILL);

    qsQuery = QString("SELECT COUNT(totalPrice), SUM(totalPrice) "
                      "FROM cassahistory, ledger, patientCardTypes WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "cassahistory.cassaId IN( %1 ) AND "
                      "%2 AND "
                      "ledger.patientCardTypeId=%3 AND "
                      "ledger.active=1 " ).arg(p_qsCassaId).arg(qsPCCondition).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        if( poQueryResult->value(0).toInt() > 0 )
        {
            qsRet = QString::number( poQueryResult->value(0).toInt() );
            *p_uiPricePCSell = poQueryResult->value(1).toUInt();
        }
    }

    return qsRet;
}
//------------------------------------------------------------------------------------
