
#include <QMessageBox>

#include "../framework/qtframework.h"
#include "creportpatientcardtype.h"
#include "currency.h"

cReportPatientCardType::cReportPatientCardType(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Active patientcardtypes ");
    m_qsReportDescription   = tr( "This report shows the active patientcard types.  Please select type of expiration from the list box below." );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Expiration ... :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All type") );
    qslDataTypes << QString( "1|%1" ).arg( tr("set by No. of days") );
    qslDataTypes << QString( "2|%1" ).arg( tr("set by date intervall") );

    setFilterDataType( qslDataTypes.join("#") );
}

void cReportPatientCardType::refreshReport()
{
    m_dlgProgress.progressInit( tr("Get data from database ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_qsReportHtml.append( "<html><body>" );
    m_qsReportHtml.append( "<div>" );

    m_tcReport->insertText( m_qsReportName, *obTitleFormat );
    m_tcReport->setCharFormat( *obNormalFormat );

    m_qsReportHtml.append( "</div>");
    m_qsReportHtml.append( "<hr>" );

    m_qsReportHtml.append( "<div>" );

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

    m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
    m_dlgProgress.increaseProgressValue();

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

    m_dlgProgress.progressInit( tr("Displaying data ..."), 0, qslQueryResult.size() );
    m_dlgProgress.setProgressValue( 0 );

/*    obTableFormat->setAlignment( Qt::AlignLeft );
    m_tcReport->insertTable( poQueryResultCards->size()+1, 5, *obTableFormat );

    // Add table header row
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Name" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Price" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Units" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obCenterCellFormat );
    m_tcReport->insertText( tr( "Valid" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Unit time" ), *obBoldFormat );*/

    m_qsReportHtml.append( "<table>" );
    m_qsReportHtml.append( "<tr>" );
    m_qsReportHtml.append( "<td>" );
    m_qsReportHtml.append( tr( "Name" ) );
    m_qsReportHtml.append( "</td>" );
    m_qsReportHtml.append( "<td>" );
    m_qsReportHtml.append( tr( "Price" ) );
    m_qsReportHtml.append( "</td>" );
    m_qsReportHtml.append( "<td>" );
    m_qsReportHtml.append( tr( "Units" ) );
    m_qsReportHtml.append( "</td>" );
    m_qsReportHtml.append( "<td>" );
    m_qsReportHtml.append( tr( "Valid" ) );
    m_qsReportHtml.append( "</td>" );
    m_qsReportHtml.append( "<td>" );
    m_qsReportHtml.append( tr( "Unit time" ) );
    m_qsReportHtml.append( "</td>" );
    m_qsReportHtml.append( "</tr>" );

    for( int i=0; i<qslQueryResult.size(); i++ )
    {
        QStringList qslRecord = qslQueryResult.at(i).split('#');

        cCurrency   obPrice( qslRecord.at(1).toInt() );

/*        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qslRecord.at(0), *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( obPrice.currencyFullStringShort(), *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qslRecord.at(2), *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qslRecord.at(3), *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qslRecord.at(4), *obNormalFormat );*/

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( qslRecord.at(0) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( obPrice.currencyFullStringShort() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( qslRecord.at(2) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( qslRecord.at(3) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( qslRecord.at(4) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );

        m_dlgProgress.increaseProgressValue();
    }

    m_qsReportHtml.append( "</div>");

    m_qsReportHtml.append( QString("</body></html>") );

    m_tcReport->insertHtml( m_qsReportHtml );

    m_dlgProgress.hide();
}
