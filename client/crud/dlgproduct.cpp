#include <QMessageBox>

#include "belenus.h"
#include "dlgproduct.h"
#include "../edit/dlgproductedit.h"

cDlgProduct::cDlgProduct( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product List" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListProducts", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgProduct::~cDlgProduct()
{
    g_poPrefs->setDialogSize( "ListProducts", QPoint( width(), height() ) );
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
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Net price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "VAT percent" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Sum price" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Product count" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Net price" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "VAT percent" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Sum price" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Product count" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgProduct::refreshTable()
{
    cTracer obTracer( "cDlgProduct::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT productId, licenceId, name, netPriceSell, vatpercentSell, netPriceSell+((netPriceSell/100)*vatpercentSell) AS sumPriceSell, productCount, active, archive FROM products WHERE productId>0";
    }
    else
    {
        m_qsQuery = "SELECT productId AS id, name, netPriceSell, vatpercentSell, netPriceSell+((netPriceSell/100)*vatpercentSell) AS sumPriceSell, productCount FROM products WHERE active=1 AND productId>0";
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
    cDBProduct  *poProduct = new cDBProduct;
    poProduct->load( m_uiSelectedId );

    QString     qsQuestion = tr( "Are you sure you want to delete this Product?" );
    if( poProduct->isProductLinkedToProductType() )
    {
        qsQuestion = tr("This product is linked to one or more product type.\n"
                        "Are you sure you want to delete this product?\n\n"
                        "Deleting this product will not delete the product types assigned.");
    }

    if( QMessageBox::question( this, tr( "Question" ),
                               qsQuestion,
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
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
