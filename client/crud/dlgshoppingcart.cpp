#include <QMessageBox>

#include "belenus.h"
#include "dlgshoppingcart.h"

cDlgShoppingCart::cDlgShoppingCart( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Shopping cart" ) );
    setWindowIcon( QIcon("./resources/40x40_shoppingcart.png") );

    setupTableView();
}

cDlgShoppingCart::~cDlgShoppingCart()
{
}

void cDlgShoppingCart::setupTableView()
{
    cTracer obTracer( "cDlgShoppingCart::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgShoppingCart::refreshTable()
{
    cTracer obTracer( "cDlgShoppingCart::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientOriginId, licenceId, name, active, archive FROM patientOrigin";
    }
    else
    {
        m_qsQuery = "SELECT patientOriginId AS id, name FROM patientOrigin WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgShoppingCart::enableButtons()
{
    cTracer obTracer( "cDlgShoppingCart::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgShoppingCart::newClicked( bool )
{
}

void cDlgShoppingCart::editClicked( bool )
{
}

void cDlgShoppingCart::deleteClicked( bool )
{
}
