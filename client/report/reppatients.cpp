#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "reppatients.h"


cDlgReportPatients::cDlgReportPatients( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgReportPatients::cDlgReportPatients" );

    setReportTitle( tr( "Patients" ) );

    lblDate = new QLabel( tr("Entered into database :"), grpFilters );
    lblDate->setObjectName( QString::fromUtf8( "lblDate" ) );

    dteStartDate = new QDateTimeEdit();
    dteStartDate->setObjectName( QString::fromUtf8( "dteStartDate" ) );
    dteStartDate->setCalendarPopup( true );
    dteStartDate->setDate( QDate::currentDate().addMonths(-1) );
    dteStartDate->setDisplayFormat( "yyyy-MM-dd" );

    lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );

    dteEndDate = new QDateTimeEdit();
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );
    dteEndDate->setDate( QDate::currentDate() );
    dteEndDate->setDisplayFormat( "yyyy-MM-dd" );

    lblAge = new QLabel( tr("Age :"), grpFilters );
    lblAge->setObjectName( QString::fromUtf8( "lblAge" ) );

    cmbAge = new QComboBox();
    cmbAge->setObjectName( QString::fromUtf8( "cmbAge" ) );
    cmbAge->addItem( tr("<All patients>") );
    cmbAge->addItem( tr("0 year to 10 years") );
    cmbAge->addItem( tr("10 year to 20 years") );
    cmbAge->addItem( tr("20 year to 30 years") );
    cmbAge->addItem( tr("30 year to 40 years") );
    cmbAge->addItem( tr("40 year to 50 years") );
    cmbAge->addItem( tr("50 year to 60 years") );
    cmbAge->addItem( tr("60 year to 70 years") );
    cmbAge->addItem( tr("70 year to 80 years") );
    cmbAge->addItem( tr("80 year to 90 years") );
    cmbAge->addItem( tr("90 year to 100 years") );

    horizontalLayout->insertWidget( 0, cmbAge );
    horizontalLayout->insertWidget( 0, lblAge );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );
}

cDlgReportPatients::~cDlgReportPatients()
{
    cTracer obTrace( "cDlgReportPatients::~cDlgReportPatients" );
}

void cDlgReportPatients::refreshReport()
{
    cTracer obTrace( "cDlgReportPatients::refreshReport()" );

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
    qsQuery += QString( "SELECT name, gender, dateBirth FROM patients WHERE active=1 " );
    qsQuery += QString( "AND created>=\"%1\" AND created<=\"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 3;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------
    // Headers
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Name" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Gender" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date of birth" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------
    // Summary variables

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int         inColumn = 0;

        // Name
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        // Gender
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        if( poReportResult->value(inColumn).toInt() == 1 )
            tcReport.insertText( tr("Male"), obNormalFormat );
        else if( poReportResult->value(inColumn).toInt() == 2 )
            tcReport.insertText( tr("Female"), obNormalFormat );
        inColumn++;

        // Date of birth
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------
    // Summary fields
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
