
#include "../framework/qtframework.h"
#include "creportcardinactive.h"

cReportCardInactive::cReportCardInactive(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Inactive patientcards ");
    m_qsReportDescription   = tr( "This report shows the inactive patient cards." );
}

void cReportCardInactive::refreshReport()
{
    cReport::refreshReport();

    m_tcReport->insertHtml( "<html><body>" );
    m_tcReport->insertHtml( "<div>" );

    m_tcReport->insertText( m_qsReportName + "   ", *obTitleFormat );

    m_tcReport->insertHtml( "</div>");
    m_tcReport->insertHtml( "<hr>" );

    m_tcReport->insertHtml( "<div>" );

    QString qsQueryCards = QString( "SELECT pc.barcode, pct.name as cardtype, pc.comment, pc.units, p.name as owner "
                                    "FROM patientcards as pc, patientcardtypes as pct, patients as p "
                                    "WHERE pc.patientCardTypeId=pct.patientCardTypeId "
                                    "AND pc.patientId=p.patientId "
                                    "AND patientCardId>0 "
                                    "AND pc.patientCardTypeId<>1 "
                                    "AND pc.active=0 "
                                    );
    //m_tcReport->insertText( qsQueryCards, *obNormalFormat );
    QSqlQuery *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

    obTableFormat->setAlignment( Qt::AlignLeft );
    m_tcReport->insertTable( poQueryResultCards->size()+1, 5, *obTableFormat );

    // Add table header row
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Barcode" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Card type" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Comment" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obCenterCellFormat );
    m_tcReport->insertText( tr( "No. units" ), *obBoldFormat );
    m_tcReport->movePosition( QTextCursor::NextCell );
    m_tcReport->setBlockFormat( *obLeftCellFormat );
    m_tcReport->insertText( tr( "Card owner" ), *obBoldFormat );

    while( poQueryResultCards->next() )
    {
        QString     qsBarcode = poQueryResultCards->value(0).toString();
        QString     qsCardType = poQueryResultCards->value(1).toString();
        QString     qsComment = poQueryResultCards->value(2).toString();
        QString     qsUnits = poQueryResultCards->value(3).toString();
        QString     qsOwner = poQueryResultCards->value(4).toString();

        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsBarcode, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsCardType, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsComment, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsUnits, *obNormalFormat );
        m_tcReport->movePosition( QTextCursor::NextCell );
        m_tcReport->setBlockFormat( *obLeftCellFormat );
        m_tcReport->insertText( qsOwner, *obNormalFormat );
    }

    m_tcReport->insertHtml( "</div>");

    m_tcReport->insertHtml( QString("</body></html>") );
}
