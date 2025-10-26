
#include "../framework/qtframework.h"
#include "creportpatientcardunitchange.h"
#include "currency.h"

//------------------------------------------------------------------------------------
cReportPatientcardUnitChange::cReportPatientcardUnitChange(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
//------------------------------------------------------------------------------------
{
    m_qsReportName          = tr(" Patientcard unit changes ");
    m_qsReportDescription   = tr( "This report shows the change of the patientcard units on the selected date intervall. "
                                  "Please select the date interval you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate().addMonths(-1);

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

}
//------------------------------------------------------------------------------------
void cReportPatientcardUnitChange::refreshReport()
//------------------------------------------------------------------------------------
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    QSqlQuery   *poQueryResultCards = NULL;
    QString      qsQueryCards = "";
    QString      qsDateCondition = "";

    g_poDB->executeQTQuery( "DELETE FROM `report_cardhistory` " );

    qsDateCondition = QString(" AND ledgerTime>\"%1 00:00:00\" AND ledgerTime<\"%2 23:59:59\" ")
                                .arg( filterDateStart().toString( "yyyy-MM-dd" ) )
                                .arg( filterDateStop().toString( "yyyy-MM-dd" ) );

    //----------------------------------------------------------------------------------------------
    // Unit increase
    qsQueryCards = QString( "SELECT ledger.ledgertime, "
                            "ledgertypes.name, "
                            "patientcardtypes.name, "
                            "COUNT( patientcardunits.unittime ), "
                            "patientcardunits.unittime, "
                            "ledger.totalprice, "
                            "users.realname, "
                            "barcode "
                            "FROM `ledger`, `ledgertypes`, `patientcards`, `patientcardtypes`, `users`, `patientcardunits` "
                            "WHERE "
                            "ledger.ledgerid = patientcardunits.ledgerid AND "
                            "ledger.userid = users.userid AND "
                            "ledger.patientcardid = patientcards.patientcardid AND "
                            "ledger.patientcardtypeid = patientcardtypes.patientcardtypeid AND "
                            "ledger.ledgertypeid = ledgertypes.ledgertypeid AND "
                            "ledger.ledgertypeid = 11 %1 "
                            "GROUP BY ledger.ledgerid" ).arg( qsDateCondition );

    poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
    m_dlgProgress.increaseProgressMax( poQueryResultCards->size() );

    while( poQueryResultCards->next() )
    {
        g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName`, `barcode` ) "
                                         " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8' ) " )
                                        .arg( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) )
                                        .arg( poQueryResultCards->value(1).toString() )
                                        .arg( poQueryResultCards->value(3).toString() )
                                        .arg( poQueryResultCards->value(4).toString() )
                                        .arg( poQueryResultCards->value(2).toString().replace( QString("\'"), QString("\\\'") ) )
                                        .arg( poQueryResultCards->value(5).toString() )
                                        .arg( poQueryResultCards->value(6).toString() )
                                        .arg( poQueryResultCards->value(7).toString() ) );
        m_dlgProgress.increaseProgressValue();
    }

    //----------------------------------------------------------------------------------------------
    // Unit decrease
    qsQueryCards = QString( "SELECT ledger.ledgertime, "
                            "ledgertypes.name, "
                            "patientcardtypes.name, "
                            "itemCount, "
                            "users.realname, "
                            "barcode "
                            "FROM `ledger`, `ledgertypes`, `patientcards`, `patientcardtypes`, `users` "
                            "WHERE "
                            "ledger.userid = users.userid AND "
                            "ledger.patientcardid = patientcards.patientcardid AND "
                            "ledger.patientcardtypeid = patientcardtypes.patientcardtypeid AND "
                            "ledger.ledgertypeid = ledgertypes.ledgertypeid AND "
                            "ledger.ledgertypeid = 12 %1 " ).arg( qsDateCondition );

    poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

    while( poQueryResultCards->next() )
    {
        QString     qsLedgertime        = poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
        QString     qsLedgerTypeName    = poQueryResultCards->value(1).toString();
        QString     qsPatientCardType   = poQueryResultCards->value(2).toString().replace( QString("\'"), QString("\\\'") );
        int         nItemCount          = poQueryResultCards->value(3).toInt() * (-1);
        QString     qsUser              = poQueryResultCards->value(4).toString();
        QString     qsBarcode           = poQueryResultCards->value(5).toString();

        g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName`, `barcode` ) "
                                         " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8' ) " )
                                        .arg( qsLedgertime )
                                        .arg( qsLedgerTypeName )
                                        .arg( QString::number( nItemCount ) )
                                        .arg( "" )
                                        .arg( qsPatientCardType )
                                        .arg( "" )
                                        .arg( qsUser )
                                        .arg( qsBarcode ) );
        m_dlgProgress.increaseProgressValue();
    }


    startReport();

    // Berlet egyseg feltoltesek es levonasok
    addTitle( m_qsReportName );
    addSubTitle( tr( "Patientcard unit modifications between dates %1 -> %2" ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addDescription( tr( "The items in the list show the interventions through which the "
                        "number of units of the respective card was increased or decreased.") );
    addHorizontalLine();

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Patientcard barcode" ), "center bold" );
    addTableCell( tr( "Date of action" ), "center bold" );
    addTableCell( tr( "Action name" ), "center bold" );
    addTableCell( tr( "Number of units" ), "center bold" );
    addTableCell( tr( "Unit time" ), "center bold" );
    addTableCell( tr( "Unit type" ), "center bold" );
    addTableCell( tr( "Action performed by" ), "center bold" );

    poQueryResultCards = g_poDB->executeQTQuery( "SELECT dateCardAction, cardAction, countUnits, unitTime, unitType, priceAction, userName, barcode FROM `report_cardhistory` ORDER BY dateCardAction " );

    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );

    while( poQueryResultCards->next() )
    {
        QString     queryDateCardAction = poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm" );
        QString     queryCardAction     = poQueryResultCards->value(1).toString();
        int         queryUnitCount      = poQueryResultCards->value(2).toInt();
        int         queryUnitTime       = poQueryResultCards->value(3).toInt();
        QString     queryUnitType       = poQueryResultCards->value(4).toString();
        //int         queryPriceAction    = poQueryResultCards->value(5).toInt();
        QString     queryUserName       = poQueryResultCards->value(6).toString();
        QString     queryBarcode        = poQueryResultCards->value(7).toString();

        addTableRow();
        addTableCell( queryBarcode );
        addTableCell( queryDateCardAction );
        addTableCell( queryCardAction );
        addTableCell( QString::number( queryUnitCount ) );
        addTableCell( QString::number( queryUnitTime ) );
        addTableCell( queryUnitType );
        addTableCell( queryUserName );

        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();
    finishReport();

    m_dlgProgress.hide();
}
