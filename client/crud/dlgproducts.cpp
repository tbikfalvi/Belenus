#include <QMessageBox>

#include "belenus.h"
#include "dlgproducts.h"
#include "dlgproductedit.h"

cDlgProducts::cDlgProducts( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product Types List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT productId, name, barcode, count FROM products";
    }
    else
    {
        m_qsQuery = "SELECT productId AS id, name, barcode, count FROM products";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgProducts::~cDlgProducts()
{
}

void cDlgProducts::setupTableView()
{
    cTracer obTracer( "cDlgProducts::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Count" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Count" ) );
    }

    tbvCrud->resizeColumnToContents( 1 );
    tbvCrud->resizeColumnToContents( 2 );
}

void cDlgProducts::enableButtons()
{
    cTracer obTracer( "cDlgProducts::enableButtons" );

    if( m_uiSelectedId )
    {
        bool boAdmin = g_obUser.isInGroup( "admin" );
        m_poBtnDelete->setEnabled( boAdmin );
        m_poBtnEdit->setEnabled( boAdmin );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
}

void cDlgProducts::newClicked( bool )
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

void cDlgProducts::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Product?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QString stQuery = QString( "DELETE FROM products WHERE productid=%1" ).arg( m_uiSelectedId );
        g_poDB->executeQuery( stQuery.toStdString(), true );

        m_uiSelectedId = 0;
        refreshTable();
    }
}

void cDlgProducts::editClicked( bool )
{
    cDBProduct  *poProduct = NULL;

    try
    {
        poProduct = new cDBProduct;
        poProduct->load( m_uiSelectedId );

        cDlgProductEdit  obDlgEdit( this, poProduct );
        obDlgEdit.setWindowTitle( QString::fromStdString( poProduct->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        delete poProduct;
    }
    catch( cSevException &e )
    {
        if( poProduct ) delete poProduct;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
