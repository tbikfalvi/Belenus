
#include "../framework/qtframework.h"
#include "creportcardinactive.h"

cReportCardInactive::cReportCardInactive(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Not used patientcards ");
    m_qsReportDescription   = tr( "This report shows the currently inactive patient cards that registered into database but currently not used." );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Patientcard group types :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All cards") );
    qslDataTypes << QString( "1|%1" ).arg( tr("Only pre-registered cards") );
    qslDataTypes << QString( "2|%1" ).arg( tr("Only inactivated cards") );
    qslDataTypes << QString( "3|%1" ).arg( tr("Only lost cards") );

    setFilterDataType( QString( "0|%1" ).arg( tr("All cards") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}

void cReportCardInactive::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QStringList qslFilterType = filterType().split("|");

    QString qsQueryCards    = QString( "SELECT barcode FROM patientcards " );
    QString qsCondition     = "";
    QString qsFilter        = "WHERE patientcards.patientCardId>1 "
                              "AND patientcards.patientCardTypeId<>1 "
                              "AND patientcards.active=0 ";

    switch( qslFilterType.at(0).toInt() )
    {
        case 1:
            qsCondition = "LEFT JOIN ledger ON patientcards.patientCardId=ledger.patientCardId ";
            qsFilter.append( "AND ledger.ledgerId IS NULL " );
            break;
        case 2:
            qsCondition = "LEFT JOIN ledger ON patientcards.patientCardId=ledger.patientCardId ";
            qsFilter.append( "AND ledger.ledgerId IS NOT NULL AND pincode!=\"LOST\" " );
            break;
        case 3:
            qsFilter.append( " AND pincode=\"LOST\" " );
            break;
        default:
            ;
    }
    qsQueryCards.append( qsCondition );
    qsQueryCards.append( qsFilter );
    qsQueryCards.append( "GROUP BY barcode" );

    m_dlgProgress.increaseProgressValue();

    QSqlQuery *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Number of cards" ), "bold" );
    addTableCell( " : ", "bold" );
    addTableCell( QString::number( poQueryResultCards->size() ) );
    m_dlgProgress.increaseProgressValue();

    finishTable();
    finishSection();

    addSeparator();

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Barcode" ), "bold" );
    addTableRow();

    QString qsBarcodes = "";

    if( poQueryResultCards->first() )
    {
        qsBarcodes.append( poQueryResultCards->value(0).toString() );
        while( poQueryResultCards->next() )
        {
            qsBarcodes.append( QString( ", %1" ).arg( poQueryResultCards->value(0).toString() ) );
        }
    }
    addTableCell( qsBarcodes );
    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
