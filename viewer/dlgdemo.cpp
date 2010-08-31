#include <QPushButton>
#include <QDialogButtonBox>
#include <QSqlQuery>

#include "../framework/qtframework.h"
#include "dlgdemo.h"
#include "dlgpreview.h"

cDlgDemo::cDlgDemo( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );

    cmbRegion->addItem( tr( "Any" ) );
    QSqlQuery* poRegions = NULL;
    poRegions = g_poDB->executeQTQuery( "SELECT DISTINCT(region) FROM zipRegionCity ORDER BY region ASC" );
    while( poRegions->next() )
    {
        cmbRegion->addItem( poRegions->value( 0 ).toString() );
    }
    cmbRegion->setCurrentIndex( 0 );
    if( poRegions ) delete poRegions;

    QPushButton *poShowButton = btbButtons->addButton( tr( "&Show" ), QDialogButtonBox::ActionRole );
    btbButtons->addButton( tr( "&Close" ), QDialogButtonBox::RejectRole );

    connect( poShowButton, SIGNAL(clicked()), this, SLOT(showReport()) );
}

cDlgDemo::~cDlgDemo()
{
}

void cDlgDemo::showReport()
{
    cDlgPreview obPreview( this );
    obPreview.exec();
}

/*
void cDlgDemo::refreshTable()
{
    QString qsQuery = "SELECT zip,region,city from zipRegionCity WHERE ";

    if( cmbRegion->currentIndex() )
    {
        qsQuery += QString( "region=\"%1\" AND " ).arg( cmbRegion->currentText() );
    }
    qsQuery += QString( "zip > %1" ).arg( ledMinPC->text() );

    m_poModel->clear();

    m_poModel->setColumnCount( 3 );
    m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Zip" ) );
    m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Region" ) );
    m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "City" ) );

    QSqlQuery  *poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );
    while( poReportResult->next() )
    {
        QList<QStandardItem*>  obReportRow;
        obReportRow.append( new QStandardItem( poReportResult->value( 0 ).toString() ) );
        obReportRow.append( new QStandardItem( poReportResult->value( 1 ).toString() ) );
        obReportRow.append( new QStandardItem( poReportResult->value( 2 ).toString() ) );
        m_poModel->appendRow( obReportRow );
    }

    QList<QStandardItem*>  obTotalRow;
    obTotalRow.append( new QStandardItem( tr( "Total" ) ) );

    int inRegionCount = 1;
    if( cmbRegion->currentIndex() == 0 ) inRegionCount = cmbRegion->count() - 1;
    if( inRegionCount > 1 ) obTotalRow.append( new QStandardItem( QString( tr( "%1 Regions" ) ).arg( inRegionCount ) ) );
    else obTotalRow.append( new QStandardItem( tr( "1 Region" ) ) );

    int inCityCount = m_poModel->rowCount();
    if( inCityCount > 1 ) obTotalRow.append( new QStandardItem( QString( tr( "%1 Cities" ) ).arg( inCityCount ) ) );
    else obTotalRow.append( new QStandardItem( tr( "1 City" ) ) );

    m_poModel->appendRow( obTotalRow );

    tbvReport->resizeColumnToContents( 0 );
    tbvReport->resizeColumnToContents( 1 );
}
*/
