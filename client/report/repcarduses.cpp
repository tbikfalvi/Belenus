//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : repcarduses.cpp
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
#include "repcarduses.h"

//====================================================================================
cDlgReportCardUses::cDlgReportCardUses( QWidget *parent ) : cDlgPreview( parent )
//====================================================================================
{
    cTracer obTrace( "cDlgReportCardUses::cDlgReportCardUses" );

    setReportTitle( tr( "Patientcard uses" ) );

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

    lblPatientCard = new QLabel( tr("Patientcard :"), grpFilters );
    lblPatientCard->setObjectName( QString::fromUtf8( "lblPatientCard" ) );

    cmbPatientCard = new QComboBox();
    cmbPatientCard->setObjectName( QString::fromUtf8( "cmbPatientCard" ) );

/*    lblPatient = new QLabel( tr("Patient :"), grpFilters );
    lblPatient->setObjectName( QString::fromUtf8( "lblPatient" ) );

    cmbPatient = new QComboBox();
    cmbPatient->setObjectName( QString::fromUtf8( "cmbPatient" ) );*/

//    horizontalLayout->insertWidget( 0, cmbPatient );
//    horizontalLayout->insertWidget( 0, lblPatient );
    horizontalLayout->insertWidget( 0, cmbPatientCard );
    horizontalLayout->insertWidget( 0, lblPatientCard );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );

    QSqlQuery *poQuery = NULL;

    /*cmbPatient->addItem( tr("<All patients>") );
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
    }*/

    cmbPatientCard->addItem( tr("<All patientcards>") );
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardId, barcode FROM patientCards WHERE active=1 AND patientCardId>0" ) );
        while( poQuery->next() )
        {
            cmbPatientCard->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
    if( poQuery ) delete poQuery;
}
//====================================================================================
cDlgReportCardUses::~cDlgReportCardUses()
//====================================================================================
{
    cTracer obTrace( "cDlgReportCardUses::~cDlgReportCardUses" );
}
//====================================================================================
QString cDlgReportCardUses::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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
void cDlgReportCardUses::refreshReport()
//====================================================================================
{
    cTracer obTrace( "cDlgReportCardUses::refreshReport()" );

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
    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() > 0 )
        tcReport.insertText( tr(" | Patientcard barcode: %1").arg(cmbPatientCard->itemText(cmbPatientCard->currentIndex())) );
/*    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
        tcReport.insertText( tr(" | Patient: %1").arg(cmbPatient->itemText(cmbPatient->currentIndex())) );*/

    tcReport.insertHtml( "<hr>" );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    qsQuery = "";
    qsQuery += QString( "SELECT DISTINCT pch.dateTimeUsed, pch.units, pch.time" );
    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() <= 0 )
    {
        qsQuery += QString( ", pc.barcode" );
    }
    /*else if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
    {
        qsQuery += QString( ", (SELECT p.name FROM patientCards pc, patients p WHERE pc.patientCardId=1 AND p.patientId=pc.patientId ) AS name" );
    }*/
    qsQuery += QString( " FROM patientCardHistories pch" );
    if( /*cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 && */
        cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() > 0 )
    {
        qsQuery += QString( " WHERE active=1 " );
        qsQuery += QString( " AND dateTimeUsed>=\"%1\" AND dateTimeUsed<=\"%2 23:59:59\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    }
    else
    {
        qsQuery += QString( ", patientCards pc WHERE pch.active=1 " ); // , patients p
        qsQuery += QString( " AND pch.dateTimeUsed>=\"%1\" AND pch.dateTimeUsed<=\"%2 23:59:59\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    }
    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() > 0 )
    {
        qsQuery += QString( " AND pch.patientCardId=%1 " ).arg(cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt());
    }
    else
    {
        qsQuery += QString( " AND pch.patientCardId=pc.patientCardId " );
    }
/*    if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() > 0 )
    {
        qsQuery += QString( " AND patientId=%1 " ).arg(cmbPatient->itemData(cmbPatient->currentIndex()).toInt());
    }
    else
    {
        qsQuery += QString( " AND a.patientId=p.patientId " );
    }*/

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    uiColumnCount = 3;

    /*if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
        uiColumnCount++;*/
    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() <= 0 )
        uiColumnCount++;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------
    // Headers

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date time" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "No units" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Length" ), obBoldFormat );

    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() <= 0 )
    {
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( tr( "Patientcard" ), obBoldFormat );
    }

    /*if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
    {
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( tr( "Patient" ), obBoldFormat );
    }*/

    //------------------------------------------------------------------------------------------------------
    // Summary variables

    QTime   qtTemp;
    int     inSumUnits  = 0;
    int     inSumLength = 0;

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int         inColumn = 0;

        // Date
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        QString qsTemp = poReportResult->value(inColumn).toString();
        tcReport.insertText( qsTemp.replace(10,1,' '), obNormalFormat );
        inColumn++;

        // Units
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inSumUnits += poReportResult->value(inColumn).toInt();
        inColumn++;

        // Length
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        qtTemp = QTime::fromString(poReportResult->value(inColumn).toString(),"hh:mm:ss");
        inSumLength += qtTemp.hour()*3600 + qtTemp.minute()*60 + qtTemp.second();
        inColumn++;

        if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() <= 0 )
        {
            // PatientCard
            tcReport.movePosition( QTextCursor::NextCell );
            tcReport.setBlockFormat( obLeftCellFormat );
            tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
            inColumn++;
        }

        /*if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
        {
            // Patient
            tcReport.movePosition( QTextCursor::NextCell );
            tcReport.setBlockFormat( obLeftCellFormat );
            tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
            inColumn++;
        }*/
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------
    // Summary fields

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number(inSumUnits), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obCenterCellFormat );
    qtTemp = QTime( inSumLength/3600, (inSumLength%3600)/60, (inSumLength%3600)%60 );
    tcReport.insertText( qtTemp.toString("hh:mm:ss"), obBoldFormat );

    if( cmbPatientCard->itemData(cmbPatientCard->currentIndex()).toInt() <= 0 )
        tcReport.movePosition( QTextCursor::NextCell );

    /*if( cmbPatient->itemData(cmbPatient->currentIndex()).toInt() <= 0 )
        tcReport.movePosition( QTextCursor::NextCell );*/

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
