
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

    m_qsReportHtml.append( "<html><body>" );
    m_qsReportHtml.append( "<div>" );

    QString     qsTitle = m_qsReportName;
    QString     qsCondition;

    if( filterType().left(1).toInt() > 0 )
    {
        qsTitle.append( QString( " - %1" ).arg( filterType().mid(2) ) );
        qsCondition.append( QString(" AND patientcards.patientCardTypeId=%1 ").arg(filterType().left(1).toInt()) );
    }
    if( filterName().length() > 0 )
    {
        qsTitle.append( QString( " - %1" ).arg( filterName() ) );
        qsCondition.append( QString(" AND patientcards.barcode LIKE \"\%%1\%\" ").arg(filterName()) );
    }
    m_tcReport->insertText( qsTitle, *obTitleFormat );
    m_tcReport->setCharFormat( *obNormalFormat );

    m_qsReportHtml.append( "</div>");
    m_qsReportHtml.append( "<hr>" );

    m_qsReportHtml.append( "<div>" );


    QString      qsQueryCards = "SELECT * FROM patientcards, patientcardtypes, patients WHERE "
                                "patientCardId>0 AND "
                                "patientcards.active=1 AND "
                                "patientcards.patientCardTypeId=patientcardtypes.patientCardTypeId AND "
                                "patientcards.patientId=patients.patientId";

    if( qsCondition.length() > 0 )
    {
        qsQueryCards.append( qsCondition );
    }

    QSqlQuery   *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
    QStringList  qslQueryResult;

    if( poQueryResultCards->size() < 1 )
    {
        m_qsReportHtml.append( "<center><i>" );
        m_qsReportHtml.append( tr("No valid patientcard found for the selected filters") );
        m_qsReportHtml.append( "</i></center>" );
/*        obTableFormat->setAlignment( Qt::AlignLeft );
        m_tcReport->insertTable( 1, 1, *obTableFormat );
        m_tcReport->setBlockFormat( *obCenterCellFormat );
        m_tcReport->insertText( tr("No valid patientcard found for the selected filters"), *obItalicFormat );*/
    }
    else if( poQueryResultCards->size() == 1 )
    {
        poQueryResultCards->first();

        unsigned int    uiPatientCardId = poQueryResultCards->value(0).toUInt();

        m_dlgProgress.progressInit( tr("Displaying data ..."), 0, 7 );
        m_dlgProgress.setProgressValue( 0 );

        obTableFormat->setAlignment( Qt::AlignLeft );
        m_qsReportHtml.append( "<table>" );

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Barcode" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( poQueryResultCards->value(5).toString() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Patientcard type" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( poQueryResultCards->value(18).toString() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Owner" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( poQueryResultCards->value(33).toString() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "No. units" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( poQueryResultCards->value(7).toString() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        unsigned int    uiTimeLeft = poQueryResultCards->value(9).toInt();
        QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Time left" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( qtTemp.toString( "hh:mm:ss" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Valid" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( QString("%1 -> %2").arg( poQueryResultCards->value(10).toString() ).arg( poQueryResultCards->value(11).toString() ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Comment" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( poQueryResultCards->value(6).toString() );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );
        m_dlgProgress.increaseProgressValue();

        m_qsReportHtml.append( "</table>" );

        m_qsReportHtml.append( "</div><p><div>" );
        m_qsReportHtml.append( tr( "Valid and active units" ) );
        m_qsReportHtml.append( "</div><p><div>" );

        qsQueryCards = QString( "SELECT patientCardUnitId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                "FROM patientcardunits "
                                "WHERE patientCardId=%1 "
                                "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                "AND active=1 "
                                "GROUP BY unitTime, validDateTo ORDER BY validDateTo" ).arg( uiPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_qsReportHtml.append( "<table>" );
        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Unit time" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Valid" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Valid till ..." ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "No. units" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );

        while( poQueryResultCards->next() )
        {
            m_qsReportHtml.append( "<tr>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( poQueryResultCards->value(1).toString() );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( QString("%1 -> %2").arg( poQueryResultCards->value(2).toString() ).arg( poQueryResultCards->value(3).toString() ) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( tr("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(poQueryResultCards->value(3).toString(), "yyyy-MM-dd") ) ) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( poQueryResultCards->value(4).toString() );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "</tr>" );
        }
        m_qsReportHtml.append( "</table>" );

        m_qsReportHtml.append( "</div><p><div>" );
        m_qsReportHtml.append( tr( "Patientcard unit usages" ) );
        m_qsReportHtml.append( "</div><p><div>" );

        qsQueryCards = QString( "SELECT dateTimeUsed "
                                "FROM patientcardunits "
                                "WHERE patientCardId=%1 "
                                "AND active=0 " ).arg( uiPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_qsReportHtml.append( "<table>" );
        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Date of usage" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );

        while( poQueryResultCards->next() )
        {
            m_qsReportHtml.append( "<tr>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm" ) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "</tr>" );
        }
        m_qsReportHtml.append( "</table>" );
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

        m_qsReportHtml.append( "<table>" );
//        obTableFormat->setAlignment( Qt::AlignLeft );
//        m_tcReport->insertTable( qslQueryResult.size()+1, 5, *obTableFormat );

        // Add table header row
/*        m_tcReport->setBlockFormat( *obLeftCellFormat );
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
        m_tcReport->insertText( tr( "Comment" ), *obBoldFormat );*/
        m_qsReportHtml.append( "<tr>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Barcode" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "No. units" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Time" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Valid" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "<td>" );
        m_qsReportHtml.append( tr( "Comment" ) );
        m_qsReportHtml.append( "</td>" );
        m_qsReportHtml.append( "</tr>" );

        for( int i=0; i<qslQueryResult.size(); i++ )
        {
            QStringList qslRecord = qslQueryResult.at(i).split('#');

/*            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obLeftCellFormat );
            m_tcReport->insertText( qslRecord.at(0), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( qslRecord.at(1), *obNormalFormat );*/

            unsigned int    uiTimeLeft = qslRecord.at(2).toInt();
            QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );

/*            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( qtTemp.toString( "hh:mm:ss" ), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obCenterCellFormat );
            m_tcReport->insertText( QString("%1 -> %2").arg( qslRecord.at(3) ).arg( qslRecord.at(4) ), *obNormalFormat );

            m_tcReport->movePosition( QTextCursor::NextCell );
            m_tcReport->setBlockFormat( *obLeftCellFormat );
            m_tcReport->insertText( qslRecord.at(5), *obNormalFormat );*/

            m_qsReportHtml.append( "<tr>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( qslRecord.at(0) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( qslRecord.at(1) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( qtTemp.toString( "hh:mm:ss" ) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( QString("%1 -> %2").arg( qslRecord.at(3) ).arg( qslRecord.at(4) ) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "<td>" );
            m_qsReportHtml.append( qslRecord.at(5) );
            m_qsReportHtml.append( "</td>" );
            m_qsReportHtml.append( "</tr>" );

            m_dlgProgress.increaseProgressValue();
        }
    }

    m_qsReportHtml.append( "</div>");
    m_qsReportHtml.append( "</body></html>" );

    m_tcReport->insertHtml( m_qsReportHtml );

    m_dlgProgress.hide();
}
