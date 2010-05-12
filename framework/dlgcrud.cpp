#include "../belenus.h"
#include "dlgcrud.h"

cDlgCrud::cDlgCrud( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTracer( "cDlgCrud::cDlgCrud" );

    setupUi( this );

    m_qsQuery       = "";
    m_poModel       = new cQTMySQLQueryModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_uiSelectedId  = 0;
    m_inSelectedRow = -1;

    m_poSortedModel->setSourceModel( m_poModel );
    tbvCrud->setModel( m_poSortedModel );

    m_poBtnNew = new QPushButton( tr( "&New" ), this );
    m_poBtnNew->setIcon( QIcon("./resources/40x40_new.gif") );
    btbButtons->addButton( m_poBtnNew, QDialogButtonBox::ActionRole );

    m_poBtnDelete = new QPushButton( tr( "&Delete" ), this );
    m_poBtnDelete->setIcon( QIcon("./resources/40x40_delete.gif") );
    btbButtons->addButton( m_poBtnDelete, QDialogButtonBox::ActionRole );

    m_poBtnEdit = new QPushButton( tr( "&Edit" ), this );
    m_poBtnEdit->setIcon( QIcon("./resources/40x40_edit.gif") );
    btbButtons->addButton( m_poBtnEdit, QDialogButtonBox::ActionRole );

    m_poBtnClose = new QPushButton( tr( "&Close" ), this );
    m_poBtnClose->setIcon( QIcon("./resources/40x40_exit.gif") );
    btbButtons->addButton( m_poBtnClose, QDialogButtonBox::AcceptRole );

    connect( m_poBtnNew, SIGNAL( clicked( bool ) ), this, SLOT( newClicked( bool ) ) );
    connect( m_poBtnDelete, SIGNAL( clicked( bool ) ), this, SLOT( deleteClicked( bool ) ) );
    connect( m_poBtnEdit, SIGNAL( clicked( bool ) ), this, SLOT( editClicked( bool ) ) );
}

cDlgCrud::~cDlgCrud()
{
    if( m_poSortedModel ) delete m_poSortedModel;
    if( m_poModel ) delete m_poModel;
}

void cDlgCrud::setupTableView()
{
    cTracer obTracer( "cDlgCrud::setupTableView" );

    if( m_poModel->headerData( 0, Qt::Horizontal ) == "id" )
        tbvCrud->setColumnHidden( 0, true );

    connect( tbvCrud->selectionModel(), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this, SLOT( itemSelectionChanged( const QItemSelection &, const QItemSelection & ) ) );
}

void cDlgCrud::refreshTable()
{
    cTracer obTracer( "cDlgCrud::refreshTable" );

    tbvCrud->selectionModel()->blockSignals( true );

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
            tbvCrud->selectRow( m_inSelectedRow );
        }
    }
    else
    {
        m_inSelectedRow = -1;
    }
    enableButtons();

    tbvCrud->selectionModel()->blockSignals( false );
}

void cDlgCrud::itemSelectionChanged( const QItemSelection &p_obSelected,
                                     const QItemSelection &)
{
    m_inSelectedRow = -1;
    m_uiSelectedId  = 0;
    if( tbvCrud->selectionModel()->hasSelection() )
    {
        m_inSelectedRow = p_obSelected.indexes().at( 0 ).row();
        m_uiSelectedId = m_poSortedModel->index( m_inSelectedRow, 0 ).data().toUInt();
    }

    enableButtons();
}
