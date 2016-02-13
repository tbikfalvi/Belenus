
#include "../framework/qtframework.h"
#include "creportpatientcarddebts.h"
#include "currency.h"

//------------------------------------------------------------------------------------
cReportPatientcardDebts::cReportPatientcardDebts(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
//------------------------------------------------------------------------------------
{
    m_qsReportName          = tr(" Value of active patientcards ");
    m_qsReportDescription   = tr( "This report shows the summary price of valid patientcards on the selected date. "
                                  "Please select the day you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("Date :") );
    m_qdStartDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportPatientcardDebts::refreshReport()
//------------------------------------------------------------------------------------
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQuery = QString( "SELECT patientcards.patientCardId, "
                               "patientcards.patientCardTypeId, "
                               "barcode, "
                               "SUM(patientcardunits.unitPrice) AS price, "
                               "COUNT(patientcardunits.unitPrice) AS units, "
                               "patientcardunits.unitTime "
                               "FROM patientcards LEFT JOIN patientcardunits ON "
                               "patientcards.patientCardId=patientcardunits.patientCardId WHERE "
                               "(patientcardunits.active=1 OR patientcardunits.dateTimeUsed>\"%1 00:00:00\") AND "
                               "patientcardunits.validDateFrom<\"%1 24:00:00\" AND "
                               "patientcardunits.validDateTo>\"%1 00:00:00\" AND "
                               "patientcards.active=1 AND "
                               "patientcards.patientCardId>1 "
                               "GROUP BY barcode ORDER BY barcode " ).arg( filterDateStart().toString("yyyy-MM-dd") );
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( tr("On date: %1").arg( filterDateStart().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Barcode"), "bold" );
    addTableCell( tr("Patientcard type"), "bold" );
    addTableCell( tr("Unit time"), "center bold" );
    addTableCell( tr("Units"), "center bold" );
    addTableCell( tr("Unit price"), "right bold" );
    addTableCell( tr("Sum"), "right bold" );

    int nTotalUnits = 0;
    int nTotalPrice = 0;

    while( poQueryResult->next() )
    {
        int         nUnitPrice = poQueryResult->value(3).toInt() / poQueryResult->value(4).toInt(); //_unitPrice( poQueryResult->value(0).toUInt(), poQueryResult->value(3).toUInt() );
        int         nUnitTotalPrice = poQueryResult->value(3).toInt();

        nTotalUnits += poQueryResult->value(4).toInt();
        nTotalPrice += nUnitTotalPrice;

        cCurrency   unitPrice( nUnitPrice );
        cCurrency   totalPrice( nUnitTotalPrice );

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( _patientCardType( poQueryResult->value(1).toUInt() ) );
        addTableCell( tr( "%1 minute(s)" ).arg( poQueryResult->value(5).toInt() ), "center" );
        addTableCell( poQueryResult->value(4).toString(), "center" );
        addTableCell( unitPrice.currencyFullStringShort(), "right" );
        addTableCell( totalPrice.currencyFullStringShort(), "right" );
    }

    cCurrency  unitTotalPrice( nTotalPrice );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell();
    addTableCell( QString::number( nTotalUnits ), "center bold" );
    addTableCell(  );
    addTableCell( unitTotalPrice.currencyFullStringShort(), "right bold" );

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
QString cReportPatientcardDebts::_patientCardType(unsigned int p_uiPatientCardTypeId)
//------------------------------------------------------------------------------------
{
    QString qsQuery = QString( "SELECT name FROM patientcardtypes WHERE patientCardTypeId=%1").arg( p_uiPatientCardTypeId );
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    poQueryResult->first();

    return poQueryResult->value(0).toString();
}
//------------------------------------------------------------------------------------
int cReportPatientcardDebts::_unitPrice(unsigned int p_uiPatientCardId, unsigned int p_uiLedgerId)
//------------------------------------------------------------------------------------
{
    QString qsQuery = QString( "SELECT totalPrice, COUNT(totalPrice) "
                               "FROM patientcardunits, ledger WHERE "
                               "patientcardunits.ledgerId=ledger.ledgerId AND "
                               "patientcardunits.patientCardId=%1 AND "
                               "patientcardunits.ledgerId=%2").arg( p_uiPatientCardId ).arg( p_uiLedgerId );
    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    poQueryResult->first();

    return poQueryResult->value(0).toInt()/poQueryResult->value(1).toInt();
}
//------------------------------------------------------------------------------------
