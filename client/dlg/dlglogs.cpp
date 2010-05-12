#include "dlglogs.h"
#include "../framework/sevexception.h"

cLogsSqlModel::cLogsSqlModel( QObject *p_poParent )
    : cQTMySQLQueryModel( p_poParent )
{
}

cLogsSqlModel::~cLogsSqlModel()
{
}

QVariant cLogsSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data( index, role );
    if( value.isValid() && role == Qt::DisplayRole &&
        index.column() == 2 )
        return cSeverity::toStr( (cSeverity::teSeverity)value.toInt() );
    return value;
}

cDlgLogs::cDlgLogs( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    daeStartDate->setDate( QDate::currentDate() );
    daeEndDate->setDate( QDate::currentDate() );

    m_poModel = new cLogsSqlModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_poSortedModel->setSourceModel( m_poModel );

    refreshTable();

    m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Date/Time" ) );
    m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "User" ) );
    m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Severity" ) );
    m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Message" ) );

    tbvLogs->horizontalHeader()->setStretchLastSection( true );
    tbvLogs->setModel( m_poSortedModel );

    connect( daeStartDate, SIGNAL( dateChanged( const QDate & ) ),
             this, SLOT( dateChanged( const QDate & ) ) );
    connect( daeEndDate, SIGNAL( dateChanged( const QDate & ) ),
             this, SLOT( dateChanged( const QDate & ) ) );

    tbvLogs->resizeColumnToContents( 0 );
    tbvLogs->resizeColumnToContents( 1 );
}

cDlgLogs::~cDlgLogs()
{
    delete m_poSortedModel;
    delete m_poModel;
}

void cDlgLogs::refreshTable( void )
{
    QString  qsQuery = QString( "SELECT logs.date, users.name, logs.severity, logs.message FROM logs LEFT JOIN users ON logs.userId = users.userId WHERE logs.severity <= %1 AND logs.date >= \"" ).arg( sliFilter->value() );
    qsQuery += daeStartDate->date().toString( "yyyy-MM-dd" );
    qsQuery += " 00:00\" AND logs.date <= \"";
    qsQuery += daeEndDate->date().toString( "yyyy-MM-dd" );
    qsQuery += " 23:59\"";

    m_poModel->setQuery( qsQuery );
}

void cDlgLogs::on_sliFilter_valueChanged( int p_inValue )
{
    lblFilterValue->setText( cSeverity::toStr( (cSeverity::teSeverity)p_inValue ) );
    refreshTable();
}

void cDlgLogs::dateChanged( const QDate & )
{
    refreshTable();
}
