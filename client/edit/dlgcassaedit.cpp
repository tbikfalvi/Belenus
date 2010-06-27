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

    setupTableView();
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

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Denomination" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Value" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Denomination" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Value" ) );
    }
}

void cDlgCassaEdit::refreshTable()
{
    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = QString( "SELECT cassadenominations.denominationId, cassadenominations.licenceId, denominations.denomination, cassadenominations.value, cassadenominations.archive FROM cassadenominations, denominations WHERE denominations.denominationId=cassadenominations.denominationId AND cassadenominations.cassaId=%1 GROUP BY denominations.denominationId" ).arg(m_poCassa->id());
    }
    else
    {
        m_qsQuery = QString( "SELECT cassadenominations.denominationId as id, denominations.denomination, cassadenominations.value FROM cassadenominations, denominations WHERE denominations.denominationId=cassadenominations.denominationId AND cassadenominations.cassaId=%1 GROUP BY denominations.denominationId" ).arg(m_poCassa->id());
    }

    tbvCassa->selectionModel()->blockSignals( true );

    m_poModel->setQuery( m_qsQuery );

    if( m_uiSelectedId )
    {
        QModelIndex      obFirstIdx = m_poSortedModel->index( 0, 0 );
        QModelIndexList  obResults  = m_poSortedModel->match( obFirstIdx, Qt::DisplayRole, m_uiSelectedId, 1, Qt::MatchExactly );
        if( obResults.empty() )
        {
            m_uiSelectedId = 0;
            m_inSelectedRow = -1;
        }
        else
        {
            m_inSelectedRow = obResults.at( 0 ).row();
            tbvCassa->selectRow( m_inSelectedRow );
        }
    }
    else
    {
        m_inSelectedRow = -1;
    }
    enableButtons();

    tbvCassa->selectionModel()->blockSignals( false );
}

void cDlgCassaEdit::itemSelectionChanged( const QItemSelection &p_obSelected,
                                     const QItemSelection &)
{
    m_inSelectedRow = -1;
    m_uiSelectedId  = 0;
    if( tbvCassa->selectionModel()->hasSelection() )
    {
        m_inSelectedRow = p_obSelected.indexes().at( 0 ).row();
        m_uiSelectedId = m_poSortedModel->index( m_inSelectedRow, 0 ).data().toUInt();
    }

    enableButtons();
}

void cDlgCassaEdit::enableButtons()
{

}
