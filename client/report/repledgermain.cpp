#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "repledgermain.h"
#include "../db/dbpanels.h"

cDlgLedgerMain::cDlgLedgerMain( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgLedgerMain::cDlgLedgerMain" );

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

cDlgLedgerMain::~cDlgLedgerMain()
{
    cTracer obTrace( "cDlgLedgerMain::~cDlgLedgerMain" );
}

QString cDlgLedgerMain::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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

void cDlgLedgerMain::refreshReport()
{
    cTracer obTrace( "cDlgLedgerMain::refreshReport()" );

    setCursor( Qt::WaitCursor);

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

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obNormalFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    tcReport.insertHtml( "<hr>" );
    tcReport.insertHtml( "<br>" );
    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertText( tr("Use of devices"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

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

    int inSumUsedWithCard       = 0;
    int inSumUsedWithPayment    = 0;
    int inSumTimeReal           = 0;
    int inSumTimeInterrupted    = 0;
    int inSumPatientCount       = 0;
    int inSumNoCardUse          = 0;
    int inSumNoPaymentUse       = 0;
    int inSumNoCashUse          = 0;
    int inSumNoCreditCardUse    = 0;
    int inSumCash               = 0;

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
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumPatientCount += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No Card usage
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCardUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No Payment usage
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoPaymentUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No cash usage
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCashUse += poReportResult->value( inColumn ).toInt();
        tcReport.movePosition( QTextCursor::NextCell );
        inColumn++;

        // No credit card usage
        tcReport.setBlockFormat( obRightCellFormat );
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

        tcReport.setBlockFormat( obRightCellFormat );
        qsCell = intTimeToString( inSumUsedWithCard ) + QString(" - ") + intTimeToString( inSumUsedWithPayment );
        tcReport.insertText( qsCell, obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obRightCellFormat );
        qsCell = intTimeToString( inSumTimeReal ) + QString(" - ") + intTimeToString( inSumTimeInterrupted );
        tcReport.insertText( qsCell, obBoldFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumPatientCount ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoPaymentUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoCashUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoCreditCardUse ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumCash, g_poPrefs->getCurrencyShort() ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //------------------------------------------------------------------------------------------------------

/*
    qsQuery = "";
    qsQuery += QString( "SELECT p.title AS Panel, ld.timeCard AS TimeCard, ld.timeCash AS TimeCash, ld.timeLeft AS TimeUnused, ld.timeReal AS TimeUsed, count(case when ld.units = 1 then 1 else null end) AS UseCard, count(case when ld.cash > 0 then 1 else null end) AS UseCash, COUNT(ld.patientId) AS Patient, SUM(ld.cash) AS SumCash, ld.ledgerTime AS LedgerDate FROM panels p LEFT JOIN ledgerDevice ld ON p.panelId=ld.panelId WHERE p.active=1 " );
    qsQuery += QString( "AND (ISNULL(ld.ledgerTime) OR (DATE(ld.ledgerTime) >= \"%1\" AND DATE(ld.ledgerTime) <= \"%2\")) " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "GROUP BY Panel" );

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 9;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatCenter );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Device" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Card usage" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Cash usage" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Unused time" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Used time" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "No card usage" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "No cash usage" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "No patients" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Sum of cash usage" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------

    int     inSumCardUsage  = 0;
    int     inSumCashUsage  = 0;
    int     inSumUnusedTime = 0;
    int     inSumUsedTime   = 0;
    int     inSumNoCard     = 0;
    int     inSumNoCash     = 0;
    int     inSumPatient    = 0;
    int     inSumCash       = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int     inColumn = 0;
        QTime   qtTemp;

        // Device
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inColumn++;

        // Card usage
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        qtTemp = QTime( poReportResult->value( inColumn ).toInt()/3600,
                        (poReportResult->value( inColumn ).toInt()%3600)/60,
                        (poReportResult->value( inColumn ).toInt()%3600)%60, 0 );
        tcReport.insertText( qtTemp.toString("hh:mm:ss"), obNormalFormat );
        inSumCardUsage += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Cash usage
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        qtTemp = QTime( poReportResult->value( inColumn ).toInt()/3600,
                        (poReportResult->value( inColumn ).toInt()%3600)/60,
                        (poReportResult->value( inColumn ).toInt()%3600)%60, 0 );
        tcReport.insertText( qtTemp.toString("hh:mm:ss"), obNormalFormat );
        inSumCashUsage += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Unused time
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        qtTemp = QTime( poReportResult->value( inColumn ).toInt()/3600,
                        (poReportResult->value( inColumn ).toInt()%3600)/60,
                        (poReportResult->value( inColumn ).toInt()%3600)%60, 0 );
        tcReport.insertText( qtTemp.toString("hh:mm:ss"), obNormalFormat );
        inSumUnusedTime += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Used time
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        qtTemp = QTime( poReportResult->value( inColumn ).toInt()/3600,
                        (poReportResult->value( inColumn ).toInt()%3600)/60,
                        (poReportResult->value( inColumn ).toInt()%3600)%60, 0 );
        tcReport.insertText( qtTemp.toString("hh:mm:ss"), obNormalFormat );
        inSumUsedTime += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // No card usage
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCard += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // No cash usage
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumNoCash += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // No patients
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( "0", obNormalFormat );
        else
            tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumPatient += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Sum of cash usage
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        if( poReportResult->value( inColumn ).toString().compare("NULL") == 0 )
            tcReport.insertText( convertCurrency( 0, g_poPrefs->getCurrencyShort() ), obNormalFormat );
        else
            tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumCash += poReportResult->value( inColumn ).toInt();
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );
    //tcReport.setBlockFormat( obLeftCellFormat );
    //tcReport.insertText( "", obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QTime( inSumCardUsage/3600, (inSumCardUsage%3600)/60, (inSumCardUsage%3600)%60, 0 ).toString("hh:mm:ss"), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QTime( inSumCashUsage/3600, (inSumCashUsage%3600)/60, (inSumCashUsage%3600)%60, 0 ).toString("hh:mm:ss"), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QTime( inSumUnusedTime/3600, (inSumUnusedTime%3600)/60, (inSumUnusedTime%3600)%60, 0 ).toString("hh:mm:ss"), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QTime( inSumUsedTime/3600, (inSumUsedTime%3600)/60, (inSumUsedTime%3600)%60, 0 ).toString("hh:mm:ss"), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoCard ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumNoCash ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumPatient ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumCash, g_poPrefs->getCurrencyShort() ), obBoldFormat );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.insertHtml( "<br>" );
    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertText( tr("List of incomes"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================
    qsQuery = "";
    qsQuery += QString( "SELECT lt.name AS LedgerType, p.title AS Device, COUNT(l.panelId) AS Count, SUM(l.netPrice) AS Net, SUM(l.totalPrice) AS Total FROM ledger l, ledgerTypes lt, panels p WHERE l.ledgerTypeId=lt.ledgerTypeId AND l.panelId=p.panelId AND l.active=1 " );
    qsQuery += QString( "AND DATE(l.ledgerTime) >= \"%1\" AND DATE(l.ledgerTime) <= \"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "GROUP BY LedgerType, Device" );

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 5;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Ledger type" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Device" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Count" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Net" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Total" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------

    int     inSumCount  = 0;
    int     inSumNet    = 0;
    int     inSumTotal  = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int     inColumn = 0;

        // Type
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inColumn++;

        // Device
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inColumn++;

        // Count
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumCount += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Net
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumNet += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Total
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumTotal += poReportResult->value( inColumn ).toInt();
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumCount ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumNet, g_poPrefs->getCurrencyShort() ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumTotal, g_poPrefs->getCurrencyShort() ), obBoldFormat );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.insertHtml( "<br>" );
    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertText( tr("List of sold patientcard types"), obSubTitleFormat );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================
    qsQuery = "";
    qsQuery += QString( "SELECT pct.name AS PatientCardType, COUNT(l.netPrice) AS Count, SUM(l.netPrice) AS SumNet, SUM(l.totalPrice) AS SumTotal FROM ledger l, patientCardTypes pct WHERE l.patientCardTypeId=pct.patientCardTypeId AND l.patientCardTypeId>0 ");
    qsQuery += QString( "AND DATE(l.ledgerTime) >= \"%1\" AND DATE(l.ledgerTime) <= \"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    qsQuery += QString( "GROUP BY PatientCardType " );

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 4;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Patientcard Type" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Count" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Sum of net income" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Sum of total income" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------

    inSumCount  = 0;
    inSumNet    = 0;
    inSumTotal  = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int     inColumn = 0;

        // Type
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inColumn++;

        // Count
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value( inColumn ).toString(), obNormalFormat );
        inSumCount += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Net
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumNet += poReportResult->value( inColumn ).toInt();
        inColumn++;

        // Total
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( convertCurrency( poReportResult->value( inColumn ).toInt(), g_poPrefs->getCurrencyShort() ), obNormalFormat );
        inSumTotal += poReportResult->value( inColumn ).toInt();
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( inSumCount ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumNet, g_poPrefs->getCurrencyShort() ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( convertCurrency( inSumTotal, g_poPrefs->getCurrencyShort() ), obBoldFormat );
*/
    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
                                                                                                                                                                                                                //======================================================================================================
    setCursor( Qt::ArrowCursor);
}
