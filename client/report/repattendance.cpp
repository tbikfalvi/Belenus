//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : repattendance.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================

#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

//====================================================================================

#include "../framework/qtframework.h"
#include "../dlg/dlgprogress.h"
#include "repattendance.h"

//====================================================================================
cDlgReportAttendance::cDlgReportAttendance( QWidget *parent ) : cDlgPreview( parent )
//====================================================================================
{
    cTracer obTrace( "cDlgReportAttendance::cDlgReportAttendance" );

    setReportTitle( tr( "Attendances" ) );
    setReportFileName( tr( "attendances" ) );

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

    lblPatient = new QLabel( tr("Patient :"), grpFilters );
    lblPatient->setObjectName( QString::fromUtf8( "lblPatient" ) );

    cmbPatient = new QComboBox();
    cmbPatient->setObjectName( QString::fromUtf8( "cmbPatient" ) );

    horizontalLayout->insertWidget( 0, cmbPatient );
    horizontalLayout->insertWidget( 0, lblPatient );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );

    QSqlQuery *poQuery = NULL;

    cmbPatient->addItem( tr("<All patients>") );
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND patientId>0" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
    if( poQuery ) delete poQuery;
}
//====================================================================================
cDlgReportAttendance::~cDlgReportAttendance()
//====================================================================================
{
    cTracer obTrace( "cDlgReportAttendance::~cDlgReportAttendance" );
}
//====================================================================================
QString cDlgReportAttendance::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
//====================================================================================
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
//====================================================================================
void cDlgReportAttendance::refreshReport()
//====================================================================================
{
    cTracer obTrace( "cDlgReportAttendance::refreshReport()" );

    cDlgProgress    dlgProgress( this );
    dlgProgress.showProgress();

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
    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
        tcReport.insertText( tr(" | Patient: %1").arg(cmbPatient->itemText(cmbPatient->currentIndex())) );

    tcReport.insertHtml( "<hr>" );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    qsQuery = "";
    qsQuery += QString( "SELECT a.date" );
    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
    {
        qsQuery += QString( ", p.name" );
    }
    qsQuery += QString( ", a.bloodPressureStart, a.bloodPressureStop, a.pulseStart, a.pulseStop, a.length FROM attendance a" );
    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
    {
        qsQuery += QString( " WHERE active=1 " );
        qsQuery += QString( " AND date>=\"%1\" AND date<=\"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    }
    else
    {
        qsQuery += QString( ", patients p WHERE a.active=1 " );
        qsQuery += QString( " AND a.date>=\"%1\" AND a.date<=\"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    }
    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
    {
        qsQuery += QString( " AND patientId=%1 " ).arg(cmbPatient->itemData(cmbPatient->currentIndex()).toInt());
    }
    else
    {
        qsQuery += QString( " AND a.patientId=p.patientId " );
    }

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
        uiColumnCount = 4;
    else
        uiColumnCount = 5;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------
    // Headers

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date" ), obBoldFormat );

    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
    {
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( tr( "Patient" ), obBoldFormat );
    }

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Blood pressure\n(start->stop)" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Pulse\n(start->stop)" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Length" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        QString     qsCell = "";
        int         inColumn = 0;

        // Date
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
        {
            // Patient
            tcReport.movePosition( QTextCursor::NextCell );
            tcReport.setBlockFormat( obLeftCellFormat );
            tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
            inColumn++;
        }

        // Bloodpressure start / stop
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        qsCell = QString( poReportResult->value(inColumn).toString() );
        inColumn++;
        qsCell += QString( " -> " );
        qsCell += QString( poReportResult->value(inColumn).toString() );
        tcReport.insertText( qsCell, obNormalFormat );
        inColumn++;

        // Pulse start / stop
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        qsCell = QString( poReportResult->value(inColumn).toString() );
        inColumn++;
        qsCell += QString( " -> " );
        qsCell += QString( poReportResult->value(inColumn).toString() );
        tcReport.insertText( qsCell, obNormalFormat );
        inColumn++;

        // Length
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;
    }
    delete poReportResult;

    tcReport.movePosition( QTextCursor::NextCell );
    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
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

    dlgProgress.hideProgress();
}
//====================================================================================
