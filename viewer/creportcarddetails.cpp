
#include <QMessageBox>

#include "../framework/qtframework.h"
#include "creportcarddetails.h"

cReportCardDetails::cReportCardDetails(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Patientcards details ");
    m_qsReportDescription   = tr( "This report shows the details of the selected patient card. "
                                  "Please enter the barcode of the patient card you interested in." );

    _setDataNameEnabled( true );
    _setDataNameLabelText( tr("Barcode :") );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Patientcard types :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All type") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT patientCardTypeId, name FROM patientcardtypes WHERE patientCardTypeId>1 AND active=1" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( qslDataTypes.join("#") );
}

void cReportCardDetails::refreshReport()
{
    m_dlgProgress.progressInit( tr("Get data from database ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_tcReport->insertHtml( "<html><body>" );
    m_tcReport->insertHtml( "<div>" );

    QString     qsTitle = m_qsReportName;
    QString     qsCondition;

    if( filterType().left(1).toInt() > 0 )
    {
        qsTitle.append( QString( " - %1" ).arg( filterType().mid(2) ) );
        qsCondition.append( QString(" AND patientCardTypeId=%1 ").arg(filterType().left(1).toInt()) );
    }
    if( filterName().length() > 0 )
    {
        qsTitle.append( QString( " - %1" ).arg( filterName() ) );
        qsCondition.append( QString(" AND barcode LIKE \"\%%1\%\" ").arg(filterName()) );
    }
    m_tcReport->insertText( qsTitle, *obTitleFormat );
    m_tcReport->setCharFormat( *obNormalFormat );

    m_tcReport->insertHtml( "</div>");
    m_tcReport->insertHtml( "<hr>" );

    m_tcReport->insertHtml( "<div>" );


    QString      qsQueryCards = "SELECT * FROM patientcards WHERE patientCardId>0 AND active=1";

    if( qsCondition.length() > 0 )
    {
        qsQueryCards.append( qsCondition );
    }

    QSqlQuery   *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
    QStringList  qslQueryResult;

    if( poQueryResultCards->size() < 1 )
    {
        obTableFormat->setAlignment( Qt::AlignLeft );
        m_tcReport->insertTable( 1, 1, *obTableFormat );
        m_tcReport->setBlockFormat( *obCenterCellFormat );
        m_tcReport->insertText( tr("No valid patientcard found for the selected filters"), *obItalicFormat );
    }
    else if( poQueryResultCards->size() == 1 )
    {
    }
    else
    {
        m_dlgProgress.setProgressMax( poQueryResultCards->size()+1 );
        m_dlgProgress.increaseProgressValue();

        while( poQueryResultCards->next() )
        {
            QStringList qslRecord;

            qslRecord << poQueryResultCards->value(5).toString();
            qslRecord << poQueryResultCards->value(7).toString();
            qslRecord << poQueryResultCards->value(9).toString();
            qslRecord << poQueryResultCards->value(10).toString();
            qslRecord << poQueryResultCards->value(11).toString();
            qslRecord << poQueryResultCards->value(6).toString();

            qslQueryResult << qslRecord.join("#");
            m_dlgProgress.increaseProgressValue();
        }

        m_dlgProgress.progressInit( tr("Displaying data ..."), 0, qslQueryResult.size() );
        m_dlgProgress.setProgressValue( 0 );

        obTableFormat->setAlignment( Qt::AlignLeft );
        m_tcReport->insertTable( qslQueryResult.size()+1, 5, *obTableFormat );

        // Add table header row
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Barcode" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obCenterCellFormat );
        m_tcReport->insertText( tr( "No. units" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obCenterCellFormat );
        m_tcReport->insertText( tr( "Time" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obCenterCellFormat );
        m_tcReport->insertText( tr( "Valid" ), *obBoldFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( tr( "Comment" ), *obBoldFormat );

        for( int i=0; i<qslQueryResult.size(); i++ )
        {
            QStringList qslRecord = qslQueryResult.at(i).split('#');

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obLeftCellFormat );
            m_tcReport->insertText( qslRecord.at(0), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( qslRecord.at(1), *obNormalFormat );

            unsigned int    uiTimeLeft = qslRecord.at(2).toInt();
            QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( qtTemp.toString( "hh:mm:ss" ), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( QString("%1 -> %2").arg( qslRecord.at(3) ).arg( qslRecord.at(4) ), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obLeftCellFormat );
            m_tcReport->insertText( qslRecord.at(5), *obNormalFormat );

            m_dlgProgress.increaseProgressValue();
        }
    }

    m_tcReport->insertHtml( "</div>");

    m_tcReport->insertHtml( QString("</body></html>") );

    m_dlgProgress.hide();
}
