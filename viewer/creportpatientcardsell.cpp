
#include "../framework/qtframework.h"
#include "creportpatientcardsell.h"
#include "currency.h"

//------------------------------------------------------------------------------------
cReportPatientcardSell::cReportPatientcardSell(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
//------------------------------------------------------------------------------------
{
    m_qsReportName          = tr(" Patientcard sells ");
    m_qsReportDescription   = tr( "This report shows the patientcard related actions on the selected date interval." );

    _setDateStartEnabled( p_bIsAdmin );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( p_bIsAdmin );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
}
//------------------------------------------------------------------------------------
void cReportPatientcardSell::refreshReport()
//------------------------------------------------------------------------------------
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    //
    addSeparator();
    addSubTitle( tr( "Patientcard sells" ) );

        startSection();
        addTable();

        cCurrency obPCSell( _reportPartPC( LT_PC_SELL ) );

        finishTable();
        finishSection();

    //
    addSeparator();
    addSubTitle( tr( "Patientcard refills" ) );

        startSection();
        addTable();

        cCurrency obPCRefill( _reportPartPC( LT_PC_REFILL ) );

        finishTable();
        finishSection();

    //
    addSeparator();
    addSubTitle( tr( "Lost patientcard replaces" ) );

        startSection();
        addTable();

        cCurrency obPCReplace( _reportPartPC( LT_PC_LOST_REPLACE ) );

        finishTable();
        finishSection();

    //
    addSeparator();
    addSubTitle( tr( "Patientcard shares" ) );

        startSection();
        addTable();

        cCurrency obPCShare( _reportPartPC( LT_PC_ASSIGN_PARTNER ) );

        finishTable();
        finishSection();

    //
    addSeparator();
    addSubTitle( tr( "Patientcard online sells" ) );

        startSection();
        addTable();

        cCurrency obPCSellOnline( _reportPartPC( LT_PC_ONLINE_SELL ) );

        finishTable();
        finishSection();

    //
    addSeparator();
    addSubTitle( tr( "Patientcard online refills" ) );

        startSection();
        addTable();

        cCurrency obPCRefillOnline( _reportPartPC( LT_PC_ONLINE_REFILL ) );

        finishTable();
        finishSection();

    // Income summary
    addSeparator();
    addSubTitle( tr( "Income summary" ) );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr("Patientcard sells") );
        addTableCell( ":", "bold");
        addTableCell( obPCSell.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Patientcard refills") );
        addTableCell( ":", "bold");
        addTableCell( obPCRefill.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Lost patientcard replaces") );
        addTableCell( ":", "bold");
        addTableCell( obPCReplace.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Patientcard shares") );
        addTableCell( ":", "bold");
        addTableCell( obPCShare.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Patientcard online sells") );
        addTableCell( ":", "bold");
        addTableCell( obPCSellOnline.currencyFullStringShort(), "right" );

        addTableRow();
        addTableCell( tr("Patientcard online refills") );
        addTableCell( ":", "bold");
        addTableCell( obPCRefillOnline.currencyFullStringShort(), "right" );

        cCurrency   obTotal( obPCSell.currencyValue().toInt()+
                             obPCRefill.currencyValue().toInt()+
                             obPCReplace.currencyValue().toInt()+
                             obPCShare.currencyValue().toInt() +
                             obPCSellOnline.currencyValue().toInt() +
                             obPCRefillOnline.currencyValue().toInt() );
        addTableRow();
        addTableCell( tr("Sum total"), "bold" );
        addTableCell( ":", "bold");
        addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
int cReportPatientcardSell::_reportPartPC( int p_nPCLedgerType )
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;
    int         nTotal = 0;

    m_dlgProgress.setProgressMax( 100 );
    m_dlgProgress.setProgressValue( 0 );

    qsQuery = QString( "SELECT ledgerTime, patientcards.barcode, patientcardtypes.name, ledger.totalPrice "
                       "FROM ledger, patientcards, patientcardtypes WHERE "
                       "ledger.patientCardId=patientcards.patientCardId AND "
                       "ledger.patientCardTypeId=patientcardtypes.patientCardTypeId AND "
                       "ledgerTime>\"%1 00:00:00\" AND "
                       "ledgerTime<\"%2 24:00:00\" AND "
                       "ledgerTypeId=%3 AND "
                       "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg( p_nPCLedgerType );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    addTableRow();
    addTableCell( tr("Date"), "center bold" );
    addTableCell( tr("Patientcard barcode"), "bold" );
    addTableCell( tr("Patientcard type"), "bold" );
    addTableCell( tr("Amount"), "center bold" );

    while( poQueryResult->next() )
    {
        cCurrency   obPrice( poQueryResult->value(3).toInt() );

        nTotal += poQueryResult->value(3).toInt();

        addTableRow();
        addTableCell( poQueryResult->value(0).toDate().toString("yyyy-MM-dd"), "center" );
        addTableCell( poQueryResult->value(1).toString() );
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
    }

    cCurrency obTotal( nTotal );

    addTableRow();
    addTableCell( tr( "Sum total" ), "bold" );
    addTableCell();
    addTableCell();
    addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    return nTotal;
}
//------------------------------------------------------------------------------------
