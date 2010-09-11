#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "dlgdemo.h"

cDlgDemo::cDlgDemo( QWidget *parent )
    : cDlgPreview( parent )
{
    setReportTitle( tr( "Report Demo" ) );

    lblDate = new QLabel( tr("Date:"), grpFilters );
    lblDate->setObjectName( QString::fromUtf8( "lblDate" ) );

    dteStartDate = new QDateTimeEdit();
    dteStartDate->setObjectName( QString::fromUtf8( "dteStartDate" ) );
    dteStartDate->setCalendarPopup( true );
    dteStartDate->setDate( QDate::currentDate() );

    lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );

    dteEndDate = new QDateTimeEdit();
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );
    dteEndDate->setDate( QDate::currentDate() );

    lblEventType = new QLabel( tr("Event type:"), grpFilters );
    lblEventType->setObjectName( QString::fromUtf8( "lblEventType" ) );

    cmbEventType = new QComboBox( grpFilters );
    cmbEventType->setObjectName( QString::fromUtf8( "cmbEventType" ) );

    horizontalLayout->insertWidget( 0, cmbEventType );
    horizontalLayout->insertWidget( 0, lblEventType );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );

    cmbEventType->addItem( tr( "<All Events>" ) );
    QSqlQuery* poEventTypes = NULL;
    poEventTypes = g_poDB->executeQTQuery( "SELECT name FROM ledgerTypes ORDER BY name ASC" );
    while( poEventTypes->next() )
    {
        cmbEventType->addItem( poEventTypes->value( 0 ).toString() );
    }
    cmbEventType->setCurrentIndex( 0 );
    if( poEventTypes ) delete poEventTypes;
}

cDlgDemo::~cDlgDemo()
{
}

void cDlgDemo::refreshReport()
{
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

    QTextTableFormat obTableFormat;
    obTableFormat.setHeaderRowCount( 1 );
    obTableFormat.setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormat.setAlignment( Qt::AlignHCenter );

    QTextBlockFormat obLeftCellFormat;
    obLeftCellFormat.setLeftMargin( 10 );
    obLeftCellFormat.setRightMargin( 10 );
    obLeftCellFormat.setAlignment( Qt::AlignLeft );

    QTextBlockFormat obRightCellFormat;
    obRightCellFormat.setLeftMargin( 10 );
    obRightCellFormat.setRightMargin( 10 );
    obRightCellFormat.setAlignment( Qt::AlignRight );

    QTextCursor  tcReport( &m_tdReport );

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obNormalFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    tcReport.insertText( QString( " | %1" ).arg( cmbEventType->currentText() ) );
    tcReport.insertHtml( "<hr>" );

    QString qsQuery = "SELECT DATE(l.ledgerTime), u.name, lt.name, p.title, pct.name, l.netPrice FROM users u, ledger l, ledgerTypes lt, panels p, patientCardTypes pct WHERE l.userId=u.userId AND l.ledgerTypeId=lt.ledgerTypeId AND l.panelId=p.panelId AND l.patientCardTypeId=pct.patientCardTypeId";
    qsQuery += QString( " AND DATE(l.ledgerTime) >= \"%1\" AND DATE(l.ledgerTime) <= \"%2\"" ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    if( cmbEventType->currentIndex() ) qsQuery += QString( " AND lt.name=\"%1\"" ).arg( cmbEventType->currentText() );
    QSqlQuery *poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    unsigned int uiColumnCount = 6;
    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormat );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Operator" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Event" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Device" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Pass Type" ), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( tr( "Amount" ), obBoldFormat );

    long liTotal = 0;
    while( poReportResult->next() )
    {
        for( unsigned int i = 0; i < uiColumnCount; i++ )
        {
            tcReport.movePosition( QTextCursor::NextCell );
            if( i < uiColumnCount - 1 ) tcReport.setBlockFormat( obLeftCellFormat );
            else tcReport.setBlockFormat( obRightCellFormat );
            tcReport.insertText( poReportResult->value( i ).toString(), obNormalFormat );
        }
        liTotal += poReportResult->value( 5 ).toInt();
    }
    delete poReportResult;

    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Total:" ), obBoldFormat );
    for( unsigned int i = 0; i < uiColumnCount - 1; i++ ) tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( QString::number( liTotal ), obBoldFormat );
}
