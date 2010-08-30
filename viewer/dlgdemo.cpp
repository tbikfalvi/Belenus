#include <QString>

#include "../framework/qtframework.h"
#include "dlgdemo.h"

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

    m_poModel       = new QStandardItemModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_poSortedModel->setSourceModel( m_poModel );
    tbvReport->setModel( m_poSortedModel );

    refreshTable();

    connect( cmbRegion, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( ledMinPC, SIGNAL(editingFinished()), this, SLOT(refreshTable()) );
}

cDlgDemo::~cDlgDemo()
{
    if( m_poSortedModel ) delete m_poSortedModel;
    if( m_poModel ) delete m_poModel;
}

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
    tbvReport->resizeColumnToContents( 0 );
    tbvReport->resizeColumnToContents( 1 );

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
}
