
#include "../framework/qtframework.h"
#include "creportpatientcardtype.h"
#include "currency.h"

cReportPatientCardType::cReportPatientCardType(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    cTracer obTrace( "cReportPatientCardType::cReportPatientCardType" );

    m_qsReportName          = tr(" Active patientcardtypes ");
    m_qsReportDescription   = tr( "This report shows the active patientcard types.  Please select type of expiration from the list box below." );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Expiration ... :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All type") );
    qslDataTypes << QString( "1|%1" ).arg( tr("set by No. of days") );
    qslDataTypes << QString( "2|%1" ).arg( tr("set by date intervall") );

    setFilterDataType( QString( "0|%1" ).arg( tr("All type") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}

void cReportPatientCardType::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QString qsQueryCards;
    int     nFilterType = filterType().left(1).toInt();

    if( nFilterType == 0 )
    {
        qsQueryCards = QString( "SELECT name, price, units, validDateFrom, validDateTo, validDays, unitTime " );
        qsQueryCards += QString( "FROM patientcardtypes WHERE patientCardTypeId>1 AND active=1 " );
    }
    else if( nFilterType == 1 )
    {
        qsQueryCards = QString( "SELECT name, price, units, validDays, unitTime " );
        qsQueryCards += QString( "FROM patientcardtypes WHERE patientCardTypeId>1 AND validDays>0 AND active=1 " );
    }
    else if( nFilterType == 2 )
    {
        qsQueryCards = QString( "SELECT name, price, units, validDateFrom, validDateTo, unitTime " );
        qsQueryCards += QString( "FROM patientcardtypes WHERE patientCardTypeId>1 AND validDays=0 AND active=1 " );
    }

    //m_tcReport->insertText( qsQueryCards, *obNormalFormat );
    QSqlQuery   *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
    QStringList  qslQueryResult;

    m_dlgProgress.setProgressValue( 90 );

    m_dlgProgress.setProgressMax( poQueryResultCards->size()*2+1 );
    m_dlgProgress.setProgressValue( 0 );

    while( poQueryResultCards->next() )
    {
        QStringList qslRecord;

        qslRecord << poQueryResultCards->value(0).toString();
        qslRecord << poQueryResultCards->value(1).toString();
        qslRecord << poQueryResultCards->value(2).toString();

        if( nFilterType == 0 )
        {
            qslRecord << tr( "%1 -> %2 | %3 days" ).arg( poQueryResultCards->value(3).toString() ).arg( poQueryResultCards->value(4).toString() ).arg( poQueryResultCards->value(5).toString() );
            qslRecord << poQueryResultCards->value(6).toString();
        }
        else if( nFilterType == 1 )
        {
            qslRecord << tr( "%1 days" ).arg( poQueryResultCards->value(3).toString() );
            qslRecord << poQueryResultCards->value(4).toString();
        }
        else if( nFilterType == 2 )
        {
            qslRecord << tr( "%1 -> %2" ).arg( poQueryResultCards->value(3).toString() ).arg( poQueryResultCards->value(4).toString() );
            qslRecord << poQueryResultCards->value(5).toString();
        }

        qslQueryResult << qslRecord.join("#");
        m_dlgProgress.increaseProgressValue();
    }

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Name" ), "bold" );
    addTableCell( tr( "Price" ), "center bold" );
    addTableCell( tr( "Units" ), "center bold" );
    addTableCell( tr( "Valid" ), "center bold" );
    addTableCell( tr( "Unit time" ), "center bold" );

    for( int i=0; i<qslQueryResult.size(); i++ )
    {
        QStringList qslRecord = qslQueryResult.at(i).split('#');

        cCurrency   obPrice( qslRecord.at(1).toInt() );

        addTableRow();
        addTableCell( qslRecord.at(0) );
        addTableCell( obPrice.currencyFullStringShort(), "right" );
        addTableCell( qslRecord.at(2), "center" );
        addTableCell( qslRecord.at(3), "center" );
        addTableCell( tr( "%1 minute(s)" ).arg( qslRecord.at(4) ), "center" );

        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
