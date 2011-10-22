#include <QMessageBox>

#include "belenus.h"
#include "dlgproduct.h"
#include "../edit/dlgproductedit.h"

cDlgProduct::cDlgProduct( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product List" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    setupTableView();
}

cDlgProduct::~cDlgProduct()
{
}

void cDlgProduct::setupTableView()
{
    cTracer obTracer( "cDlgProduct::setupTableView" );

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

void cDlgProduct::refreshTable()
{
    cTracer obTracer( "cDlgProduct::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT productId, licenceId, name, active, archive FROM products";
    }
    else
    {
        m_qsQuery = "SELECT productId AS id, name FROM products WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgProduct::enableButtons()
{
    cTracer obTracer( "cDlgProduct::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgProduct::newClicked( bool )
{
    cDBProduct *poProduct = new cDBProduct;
    poProduct->createNew();

    cDlgProductEdit  obDlgEdit( this, poProduct );
    obDlgEdit.setWindowTitle( tr( "New Product" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poProduct->id();
        refreshTable();
    }

    delete poProduct;
}

void cDlgProduct::editClicked( bool )
{
    cDBProduct  *poProduct = NULL;

    try
    {
        poProduct = new cDBProduct;
        poProduct->load( m_uiSelectedId );

        cDlgProductEdit  obDlgEdit( this, poProduct );
        obDlgEdit.setWindowTitle( poProduct->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poProduct ) delete poProduct;
    }
    catch( cSevException &e )
    {
        if( poProduct ) delete poProduct;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgProduct::deleteClicked( bool )
{
    cDBProduct  *poProduct = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Product?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poProduct = new cDBProduct;
            poProduct->load( m_uiSelectedId );
            if( poProduct->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poProduct->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poProduct ) delete poProduct;
        }
        catch( cSevException &e )
        {
            if( poProduct ) delete poProduct;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
