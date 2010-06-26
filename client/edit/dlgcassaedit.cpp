#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaedit.h"

cDlgCassaEdit::cDlgCassaEdit( QWidget *p_poParent, cDBCassa *p_poCassa )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_qsQuery       = "";
    m_poModel       = new cQTMySQLQueryModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_uiSelectedId  = 0;
    m_inSelectedRow = -1;

    m_poSortedModel->setSourceModel( m_poModel );
    tbvCassa->setModel( m_poSortedModel );

    m_poCassa = p_poCassa;
    if( m_poCassa )
    {
    }
}

cDlgCassaEdit::~cDlgCassaEdit()
{
    if( m_poSortedModel ) delete m_poSortedModel;
    if( m_poModel ) delete m_poModel;
}

void cDlgCassaEdit::setupTableView()
{
    cTracer obTracer( "cDlgCassaEdit::setupTableView" );

    refreshTable();

    if( m_poModel->headerData( 0, Qt::Horizontal ) == "id" )
        tbvCassa->setColumnHidden( 0, true );

    connect( tbvCassa->selectionModel(), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this, SLOT( itemSelectionChanged( const QItemSelection &, const QItemSelection & ) ) );

    m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
}

void cDlgCassaEdit::refreshTable()
{
    m_qsQuery = QString( "SELECT * FROM cassadenominations, denominations WHERE denominations.denominationId=cassadenominations.denominationId AND cassadenominations.cassaId=%1 GROUP BY denominations.denominationId" ).arg(m_poCassa->id());
}
