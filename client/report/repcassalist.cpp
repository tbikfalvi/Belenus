#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "repcassalist.h"


cDlgReportCassaList::cDlgReportCassaList( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgReportCassaList::cDlgReportCassaList" );

    setReportTitle( tr( "Cassa list" ) );

    lblDate = new QLabel( tr("Date :"), grpFilters );
    lblDate->setObjectName( QString::fromUtf8( "lblDate" ) );

    dteStartDate = new QDateTimeEdit();
    dteStartDate->setObjectName( QString::fromUtf8( "dteStartDate" ) );
    dteStartDate->setCalendarPopup( true );
    dteStartDate->setDate( QDate::currentDate() );
    dteStartDate->setDisplayFormat( "yyyy-MM-dd" );

    lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );

    dteEndDate = new QDateTimeEdit();
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );
    dteEndDate->setDate( QDate::currentDate() );
    dteEndDate->setDisplayFormat( "yyyy-MM-dd" );

    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );
}

cDlgReportCassaList::~cDlgReportCassaList()
{
    cTracer obTrace( "cDlgReportCassaList::~cDlgReportCassaList" );
}

QString cDlgReportCassaList::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}

void cDlgReportCassaList::refreshReport()
{
    cTracer obTrace( "cDlgReportCassaList::refreshReport()" );

    setCursor( Qt::WaitCursor);

    m_tdReport.clear();
    m_tdReport.setMetaInformation( QTextDocument::DocumentTitle, m_qsReportName );

    QTextCharFormat obTitleFormat;
    obTitleFormat.setFontPointSize( 14.0 );
    obTitleFormat.setFontWeight( QFont::Bold );

    QTextCharFormat obNormalFormat;
    obNormalFormat.setFontPointSize( 10.0 );
    obNormalFormat.setFontWeight( QFont::Normal );

    QTextCharFormat obBoldFormat;
    obBoldFormat.setFontPointSize( 10.0 );
    obBoldFormat.setFontWeight( QFont::Bold );

    QTextTableFormat obTableFormatCenter;
    obTableFormatCenter.setHeaderRowCount( 1 );
    obTableFormatCenter.setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormatCenter.setAlignment( Qt::AlignHCenter );

    QTextTableFormat obTableFormatLeft;
    obTableFormatLeft.setHeaderRowCount( 1 );
    obTableFormatLeft.setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormatLeft.setAlignment( Qt::AlignLeft );

    QTextBlockFormat obLeftCellFormat;
    obLeftCellFormat.setLeftMargin( 10 );
    obLeftCellFormat.setRightMargin( 10 );
    obLeftCellFormat.setAlignment( Qt::AlignLeft );

    QTextBlockFormat obRightCellFormat;
    obRightCellFormat.setLeftMargin( 10 );
    obRightCellFormat.setRightMargin( 10 );
    obRightCellFormat.setAlignment( Qt::AlignRight );

    QTextCursor tcReport( &m_tdReport );

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obNormalFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );

    tcReport.insertHtml( "<hr>" );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    qsQuery = "";
    qsQuery += QString( "SELECT ch.actionTime, ch.actionValue, ch.comment, u.name FROM cassaHistory ch, users u WHERE ch.userId=u.userId AND ch.actionValue<>0 " );
    qsQuery += QString( " AND ch.actionTime>=\"%1\" AND ch.actionTime<=\"%2 23:59:59\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( " ORDER BY ch.actionTime " );

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 4;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Amount" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Comment" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Cassa owner" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------

    int     inSumAmount  = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int         inColumn = 0;

        // Date time
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
//        tcReport.insertText( poReportResult->value(inColumn).toString().right(8), obNormalFormat );
        QString qsTemp = poReportResult->value(inColumn).toString();
        tcReport.insertText( qsTemp.replace(10,1,' '), obNormalFormat );
        inColumn++;

        // Amount
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumAmount += poReportResult->value(inColumn).toInt();
        inColumn++;

        // Comment
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Cassa owner
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumAmount, g_poPrefs->getCurrencyShort() ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================

    setCursor( Qt::ArrowCursor);
}

