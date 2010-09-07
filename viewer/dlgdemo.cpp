#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>

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

    lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );

    dteEndDate = new QDateTimeEdit();
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );

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

    QTextCharFormat obTextFormat;
    obTextFormat.setFontPointSize( 10.0 );
    obTextFormat.setFontWeight( QFont::Normal );

    QTextCharFormat obHeaderFormat;
    obHeaderFormat.setFontPointSize( 10.0 );
    obHeaderFormat.setFontWeight( QFont::Bold );

    QTextCursor  tcReport( &m_tdReport );

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obTextFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    tcReport.insertText( QString( " | %1" ).arg( cmbEventType->currentText() ) );
    tcReport.insertHtml( "<hr>" );

    QTextTableFormat obTableFormat;
    obTableFormat.setHeaderRowCount( 1 );
    obTableFormat.setBorderStyle( QTextFrameFormat::BorderStyle_None );

    QSqlQuery *poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( "SELECT DATE(l.ledgerTime), u.name, lt.name, p.title, pct.name, l.netPrice FROM users u, ledger l, ledgerTypes lt, panels p, patientCardTypes pct WHERE l.userId=u.userId AND l.ledgerTypeId=lt.ledgerTypeId AND l.panelId=p.panelId AND l.patientCardTypeId=pct.patientCardTypeId" );

    unsigned int uiColumnCount = 6;
    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormat );
    tcReport.insertText( tr( "Date" ), obHeaderFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( tr( "Operator" ), obHeaderFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( tr( "Event" ), obHeaderFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( tr( "Device" ), obHeaderFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( tr( "Pass Type" ), obHeaderFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( tr( "Amount" ), obHeaderFormat );

    long liTotal = 0;
    while( poReportResult->next() )
    {
        for( unsigned int i = 0; i < uiColumnCount; i++ )
        {
            tcReport.movePosition( QTextCursor::NextCell );
            tcReport.insertText( poReportResult->value( i ).toString(), obTextFormat );
        }
        liTotal += poReportResult->value( 5 ).toInt();
    }
    delete poReportResult;

    for( int i = 0; i < uiColumnCount; i++ ) tcReport.movePosition( QTextCursor::NextCell );
    tcReport.insertText( QString::number( liTotal ), obHeaderFormat );
}
