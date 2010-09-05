#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QSqlQuery>
#include <QTextCursor>

#include "../framework/qtframework.h"
#include "dlgdemo.h"

cDlgDemo::cDlgDemo( QWidget *parent )
    : cDlgPreview( parent )
{
    setReportTitle( tr( "Report Demo" ) );

    QLabel *lblDate = new QLabel( tr("Date:"), grpFilters );
    lblDate->setObjectName( QString::fromUtf8( "lblDate" ) );

    QDateTimeEdit *dteStartDate = new QDateTimeEdit();
    dteStartDate->setObjectName( QString::fromUtf8( "dteStartDate" ) );
    dteStartDate->setCalendarPopup( true );

    QLabel *lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );

    QDateTimeEdit *dteEndDate = new QDateTimeEdit();
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );

    QLabel *lblEventType = new QLabel( tr("Event type:"), grpFilters );
    lblEventType->setObjectName( QString::fromUtf8( "lblEventType" ) );

    QComboBox *cmbEventType = new QComboBox( grpFilters );
    cmbEventType->setObjectName( QString::fromUtf8( "cmbEventType" ) );

    horizontalLayout->insertWidget( 0, cmbEventType );
    horizontalLayout->insertWidget( 0, lblEventType );
    horizontalLayout->insertWidget( 0, dteEndDate );
    horizontalLayout->insertWidget( 0, lblTo );
    horizontalLayout->insertWidget( 0, dteStartDate );
    horizontalLayout->insertWidget( 0, lblDate );

    cmbEventType->addItem( tr( "All Events" ) );
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

    QTextCursor  tcReport( &m_tdReport );

    tcReport.insertText( "Hi Mom!" );
}
