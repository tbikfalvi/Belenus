#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "repledgermain.h"
#include "../db/dbpanels.h"

cDlgRepLedgerMain::cDlgRepLedgerMain( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgRepLedgerMain::cDlgRepLedgerMain" );

    setReportTitle( tr( "Main ledger" ) );

    lblDate = new QLabel( tr("Date:"), grpFilters );
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

    chkShowTimes = new QCheckBox( grpFilters );
    chkShowTimes->setObjectName( QString::fromUtf8( "chkShowTimes" ) );
    chkShowTimes->setText( tr("Show time intervall of different usages") );

    horizontalLayout->insertWidget( 0, chkShowTimes );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );
}

cDlgRepLedgerMain::~cDlgRepLedgerMain()
{
    cTracer obTrace( "cDlgRepLedgerMain::~cDlgRepLedgerMain" );
}

QString cDlgRepLedgerMain::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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

void cDlgRepLedgerMain::refreshReport()
{
    cTracer obTrace( "cDlgRepLedgerMain::refreshReport()" );

    g_dlgProgress->showProgress();

    m_tdReport.clear();
    m_tdReport.setMetaInformation( QTextDocument::DocumentTitle, m_qsReportName );

    QTextCharFormat obTitleFormat;
    obTitleFormat.setFontPointSize( 14.0 );
    obTitleFormat.setFontWeight( QFont::Bold );

    QTextCharFormat obSubTitleFormat;
    obSubTitleFormat.setFontPointSize( 12.0 );
    obSubTitleFormat.setFontWeight( QFont::Bold );

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

    //======================================================================================================
    //
    // Title of the report
    //
    //======================================================================================================

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obNormalFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<br>" );

    //======================================================================================================
    //
    // List of incomes
    //
    //======================================================================================================

    tcReport.insertText( tr("List of incomes"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    //------------------------------------------------------------------------------------------------------

    qsQuery = "";
    qsQuery += QString( "SELECT " );
    qsQuery += QString( "lt.name AS LedgerType, " );
    qsQuery += QString( "p.title AS Device, " );
    qsQuery += QString( "SUM(l.netPrice) AS Net, " );
    qsQuery += QString( "SUM(l.totalPrice) AS Total, " );
    qsQuery += QString( "COUNT(case when l.discount = 0 then 1 else null end) AS NCount, " );
    qsQuery += QString( "SUM(l.discount) as Discount, " );
    qsQuery += QString( "COUNT(case when l.discount > 0 then 1 else null end) AS DCount, " );
    qsQuery += QString( "COUNT(case when paymentMethodId = 1 then 1 else null end) AS UseCash, " );
    qsQuery += QString( "COUNT(case when paymentMethodId = 3 then 1 else null end) AS UseCreditCard, " );
    qsQuery += QString( "COUNT(l.panelId) AS Count " );
    qsQuery += QString( "FROM ledger l, ledgerTypes lt, panels p " );
    qsQuery += QString( "WHERE " );
    qsQuery += QString( "l.ledgerTypeId=lt.ledgerTypeId " );
    qsQuery += QString( "AND " );
    qsQuery += QString( "l.panelId=p.panelId " );
    qsQuery += QString( "AND " );
    qsQuery += QString( "l.active=1 " );
    qsQuery += QString( "AND " );
    qsQuery += QString( "DATE(l.ledgerTime) >= \"%1\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "AND " );
    qsQuery += QString( "DATE(l.ledgerTime) <= \"%1\" " ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "GROUP BY LedgerType, Device" );

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 10;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Ledger type" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Device" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Net" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Total" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\nfull price" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Sum of\ndiscounts" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\ndiscounts" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Cash\npayment" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "C.Card\npayment" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\nall type" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //------------------------------------------------------------------------------------------------------

    inSumNet                = 0;
    inSumTotal              = 0;
    inSumCount              = 0;
    inSumDiscount           = 0;
    inSumDCount             = 0;
    inSumSumCount           = 0;
    inSumNoCashUse          = 0;
    inSumNoCreditCardUse    = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int     inColumn = 0;

        // Type
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Device
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Net
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumNet += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Total
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumTotal += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count of normal
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Sum of discount
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumDiscount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count of discount
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumDCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No cash usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCashUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No credit card usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCreditCardUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumSumCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumNet, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumTotal, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumDiscount, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumDCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCashUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCreditCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumSumCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.insertHtml( "<br>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    // Device usage main table
    //
    //======================================================================================================

    tcReport.insertText( tr("Use of devices"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    //------------------------------------------------------------------------------------------------------

    uiColumnCount = 7;
    if( chkShowTimes->isChecked() )
        uiColumnCount += 2;

    tcReport.insertTable( g_poPrefs->getPanelCount() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Device" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    if( chkShowTimes->isChecked() )
    {
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( tr( "Device usage\nwith card / with payment" ), obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( tr( "Device usage\nused time / interrupted time" ), obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Patients" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Card\nusages" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Payed\nusages" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Cash\nusages" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "C.Card\nusages" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Sum of\npayment usage" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //------------------------------------------------------------------------------------------------------

    inSumUsedWithCard       = 0;
    inSumUsedWithPayment    = 0;
    inSumTimeReal           = 0;
    inSumTimeInterrupted    = 0;
    inSumPatientCount       = 0;
    inSumNoCardUse          = 0;
    inSumNoPaymentUse       = 0;
    inSumNoCashUse          = 0;
    inSumNoCreditCardUse    = 0;
    inSumCash               = 0;

    //------------------------------------------------------------------------------------------------------

    for( int i=0; i<(int)g_poPrefs->getPanelCount(); i++ )
    {
        cDBPanel    obDBPanel;
        int         inColumn = 0;

        obDBPanel.load( i+1 );

        qsQuery = "";
        qsQuery += QString( "SELECT " );
        if( chkShowTimes->isChecked() )
        {
            qsQuery += QString( "timeCard AS TimeCard, timeCash AS TimeCash, " );
            qsQuery += QString( "timeReal AS TimeUsed, timeLeft AS TimeUnused, " );
        }
        qsQuery += QString( "COUNT(patientId) AS Patient, " );
        qsQuery += QString( "COUNT(case when units = 1 then 1 else null end) AS UseCard, " );
        qsQuery += QString( "COUNT(case when cash > 0 then 1 else null end) AS UsePayed, " );
        qsQuery += QString( "COUNT(case when paymentMethodId = 1 then 1 else null end) AS UseCash, " );
        qsQuery += QString( "COUNT(case when paymentMethodId = 3 then 1 else null end) AS UseCreditCard, " );
        qsQuery += QString( "SUM(cash) " );
        qsQuery += QString( "FROM ledgerDevice " );
        qsQuery += QString( "WHERE " );
        qsQuery += QString( "DATE(ledgerTime) >= \"%1\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) );
        qsQuery += QString( "AND " );
        qsQuery += QString( "DATE(ledgerTime) <= \"%1\" " ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
        qsQuery += QString( "AND " );
        qsQuery += QString( "panelId=%1 " ).arg(i+1);

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();

        // Device
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( obDBPanel.title(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );

        if( chkShowTimes->isChecked() )
        {
            QString qsCell;

            // Device usage with card / payment
            qsCell = "";
            inSumUsedWithCard += poReportResult->value( inColumn ).toInt();
            qsCell += intTimeToString( poReportResult->value( inColumn++ ).toInt() );
            qsCell += QString( " - " );
            inSumUsedWithPayment += poReportResult->value( inColumn ).toInt();
            qsCell += intTimeToString( poReportResult->value( inColumn++ ).toInt() );
            tcReport.setBlockFormat( obCenterCellFormat );
            tcReport.insertText( qsCell, obNormalFormat );
            tcReport.movePosition( QTextCursor::NextCell );

            // Device usage real / interrupted
            qsCell = "";
            inSumTimeReal += poReportResult->value( inColumn ).toInt();
            qsCell += intTimeToString( poReportResult->value( inColumn++ ).toInt() );
            qsCell += QString( " - " );
            inSumTimeInterrupted += poReportResult->value( inColumn ).toInt();
            qsCell += intTimeToString( poReportResult->value( inColumn++ ).toInt() );
            tcReport.setBlockFormat( obCenterCellFormat );
            tcReport.insertText( qsCell, obNormalFormat );
            tcReport.movePosition( QTextCursor::NextCell );
        }

        // No patients
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumPatientCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No Card usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCardUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No Payment usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoPaymentUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No cash usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCashUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No credit card usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCreditCardUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Sum of cash usage
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( convertCurrency( 0, g_poPrefs->getCurrencyShort() ), obNormalFormat );
        else
            tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumCash += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;
    }

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    if( chkShowTimes->isChecked() )
    {
        QString qsCell;

        tcReport.setBlockFormat( obCenterCellFormat );
        qsCell = intTimeToString( inSumUsedWithCard ) + QString(" - ") + intTimeToString( inSumUsedWithPayment );
        tcReport.insertText( qsCell, obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obCenterCellFormat );
        qsCell = intTimeToString( inSumTimeReal ) + QString(" - ") + intTimeToString( inSumTimeInterrupted );
        tcReport.insertText( qsCell, obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumPatientCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoPaymentUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCashUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCreditCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumCash, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.insertHtml( "<br>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    // List of patientcard types
    //
    //======================================================================================================

    tcReport.insertText( tr("List of sold patientcard types"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    qsQuery = "";
    qsQuery += QString( "SELECT " );
    qsQuery += QString( "pct.name AS PatientCardType, " );
    qsQuery += QString( "SUM(l.netPrice) AS SumNet, " );
    qsQuery += QString( "SUM(l.totalPrice) AS SumTotal, " );
    qsQuery += QString( "COUNT(case when l.discount = 0 then 1 else null end) AS NCount, " );
    qsQuery += QString( "SUM(l.discount) as Discount, " );
    qsQuery += QString( "COUNT(case when l.discount > 0 then 1 else null end) AS DCount, " );
    qsQuery += QString( "COUNT(case when paymentMethodId = 1 then 1 else null end) AS UseCash, " );
    qsQuery += QString( "COUNT(case when paymentMethodId = 3 then 1 else null end) AS UseCreditCard, " );
    qsQuery += QString( "COUNT(l.netPrice) AS Count " );
    qsQuery += QString( "FROM " );
    qsQuery += QString( "ledger l, patientCardTypes pct " );
    qsQuery += QString( "WHERE " );
    qsQuery += QString( "l.patientCardTypeId=pct.patientCardTypeId " );
    qsQuery += QString( "AND " );
    qsQuery += QString( "l.patientCardTypeId>0 ");
    qsQuery += QString( "AND " );
    qsQuery += QString( "DATE(l.ledgerTime) >= \"%1\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "AND " );
    qsQuery += QString( "DATE(l.ledgerTime) <= \"%1\" " ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "GROUP BY PatientCardType " );

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 9;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Patientcard Type" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Sum of\nnet income" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Sum of\ntotal income" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\nfull price" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Sum of\ndiscounts" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\ndiscounts" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Cash\npayment" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "C.Card\npayment" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( tr( "Number of\nall type" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //------------------------------------------------------------------------------------------------------

    inSumNet                = 0;
    inSumTotal              = 0;
    inSumCount              = 0;
    inSumDiscount           = 0;
    inSumDCount             = 0;
    inSumSumCount           = 0;
    inSumNoCashUse          = 0;
    inSumNoCreditCardUse    = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int     inColumn = 0;

        // Type
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Net
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumNet += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Total
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumTotal += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Sum of discount
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumDiscount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count of discount
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumDCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No cash usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCashUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No credit card usage
        tcReport.setBlockFormat( obCenterCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCreditCardUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // Count
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumSumCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumNet, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumTotal, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumDiscount, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumDCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCashUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumNoCreditCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( QString::number( inSumSumCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
                                                                                                                                                                                                                //======================================================================================================
    g_dlgProgress->hideProgress();
}
