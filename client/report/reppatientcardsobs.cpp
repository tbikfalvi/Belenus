#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "reppatientcardsobs.h"


cDlgReportPatientCardObs::cDlgReportPatientCardObs( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgReportPatientCardObs::cDlgReportPatientCardObs" );

    setReportTitle( tr( "Patientcards will be obsolete" ) );

    lblDate = new QLabel( tr("Validity will be expire between dates :"), grpFilters );
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
    dteEndDate->setDate( QDate::currentDate().addMonths(1) );
    dteEndDate->setDisplayFormat( "yyyy-MM-dd" );

    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );
}

cDlgReportPatientCardObs::~cDlgReportPatientCardObs()
{
    cTracer obTrace( "cDlgReportPatientCardObs::~cDlgReportPatientCardObs" );
}

QString cDlgReportPatientCardObs::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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

void cDlgReportPatientCardObs::refreshReport()
{
    cTracer obTrace( "cDlgReportPatientCardObs::refreshReport()" );

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

    QTextBlockFormat obCenterCellFormat;
    obCenterCellFormat.setLeftMargin( 10 );
    obCenterCellFormat.setRightMargin( 10 );
    obCenterCellFormat.setAlignment( Qt::AlignCenter );

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
    qsQuery += QString( "SELECT pc.barcode, p.name, pct.name, pc.validDateFrom, pc.validDateTo, pc.units, pc.timeLeft FROM patientCards pc, patientCardTypes pct, patients p WHERE pc.active=1 AND pc.patientCardTypeId=pct.patientCardTypeId AND pc.patientId=p.patientId" );
    qsQuery += QString( " AND (pc.validDateTo>=\"%1\" AND pc.validDateTo<=\"%2\") " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "ORDER BY pc.barcode" );

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 6;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------
    // Headers
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Barcode" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Owner" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Type" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Valid" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Units left" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Time left" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------
    // Summary variables

//    int     inSumSample  = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        QString     qsTemp;
        QTime       qtTemp;
        int         inColumn = 0;

        // Barcode
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Owner
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Type
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Valid
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        qsTemp = poReportResult->value(inColumn).toString();
        qsTemp += " => ";
        inColumn++;
        qsTemp += poReportResult->value(inColumn).toString();
        tcReport.insertText( qsTemp, obNormalFormat );
        inColumn++;

        // Units left
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Time left
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        qtTemp = QTime( poReportResult->value(inColumn).toInt()/3600,
                        (poReportResult->value(inColumn).toInt()%3600)/60,
                        (poReportResult->value(inColumn).toInt()%3600)%60);
        tcReport.insertText( qtTemp.toString("hh:mm:ss"), obNormalFormat );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------
    // Summary fields
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.movePosition( QTextCursor::NextCell );
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

