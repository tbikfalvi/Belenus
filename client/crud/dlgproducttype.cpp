#include <QMessageBox>

#include "belenus.h"
#include "dlgproduct.h"
#include "dlgproducttype.h"
#include "dlgproductactiontype.h"
#include "../edit/dlgproducttypeedit.h"
#include "../db/dbproduct.h"

cDlgProductType::cDlgProductType( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product Type List" ) );
    setWindowIcon( QIcon("./resources/40x40_producttype.png") );

    m_poParent = p_poParent;

    pbProduct = new QPushButton( tr( "Products" ), this );
    pbProduct->setObjectName( QString::fromUtf8( "pbProduct" ) );
    pbProduct->setIconSize( QSize(20, 20) );
    pbProduct->setIcon( QIcon("./resources/40x40_product.png") );
    btbButtonsSide->addButton( pbProduct, QDialogButtonBox::ActionRole );
    connect( pbProduct, SIGNAL(clicked()), this, SLOT(_slotProducts()) );
    pbProductActionType = new QPushButton( tr( "Product action types" ), this );
    pbProductActionType->setObjectName( QString::fromUtf8( "pbProductActionType" ) );
    pbProductActionType->setIconSize( QSize(20, 20) );
    pbProductActionType->setIcon( QIcon("./resources/40x40_productactiontype.png") );
    btbButtonsSide->addButton( pbProductActionType, QDialogButtonBox::ActionRole );
    connect( pbProductActionType, SIGNAL(clicked()), this, SLOT(_slotProductActionTypes()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListProductTypes", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgProductType::~cDlgProductType()
{
    g_poPrefs->setDialogSize( "ListProductTypes", QPoint( width(), height() ) );
}

void cDlgProductType::setupTableView()
{
    cTracer obTracer( "cDlgProductType::setupTableView" );

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

void cDlgProductType::refreshTable()
{
    cTracer obTracer( "cDlgProductType::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT productTypeId, licenceId, name, active, archive FROM productTypes";
    }
    else
    {
        m_qsQuery = "SELECT productTypeId AS id, name FROM productTypes WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgProductType::enableButtons()
{
    cTracer obTracer( "cDlgProductType::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgProductType::newClicked( bool )
{
    cDBProductType *poProductType = new cDBProductType;
    poProductType->createNew();

    cDlgProductTypeEdit  obDlgEdit( this, poProductType );
    obDlgEdit.setWindowTitle( tr( "New Product Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poProductType->id();
        refreshTable();
    }

    delete poProductType;
}

void cDlgProductType::editClicked( bool )
{
    cDBProductType  *poProductType = NULL;

    try
    {
        poProductType = new cDBProductType;
        poProductType->load( m_uiSelectedId );

        cDlgProductTypeEdit  obDlgEdit( this, poProductType );
        obDlgEdit.setWindowTitle( poProductType->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poProductType ) delete poProductType;
    }
    catch( cSevException &e )
    {
        if( poProductType ) delete poProductType;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgProductType::deleteClicked( bool )
{
    cDBProductType  *poProductType = new cDBProductType;
    poProductType->load( m_uiSelectedId );

    QString     qsQuestion = tr( "Are you sure you want to delete this Product Type?" );
    if( poProductType->isProductTypeLinkedToProduct() )
    {
        qsQuestion = tr("There are products attached to this product type.\n"
                        "Are you sure you want to delete this product type?\n\n"
                        "Deleting this product type will not delete the products assigned.");
    }

    if( QMessageBox::question( this, tr( "Question" ),
                               qsQuestion,
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            if( poProductType->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poProductType->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poProductType ) delete poProductType;
        }
        catch( cSevException &e )
        {
            if( poProductType ) delete poProductType;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgProductType::_slotProducts()
{
    cDlgProduct obDlgProduct( m_poParent );

    QDialog::accept();
    obDlgProduct.exec();
}

void cDlgProductType::_slotProductActionTypes()
{
    cDlgProductActionType   obDlgProductActionType( m_poParent );

    QDialog::accept();
    obDlgProductActionType.exec();
}


