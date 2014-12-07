#include <QMessageBox>

#include "belenus.h"
#include "dlgproduct.h"
#include "dlgproducttype.h"
#include "dlgproductactiontype.h"
#include "../edit/dlgproductedit.h"
#include "../dlg/dlgproductstorage.h"
#include "../dlg/dlgstockprocess.h"

cDlgProduct::cDlgProduct( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product List" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    m_poParent = p_poParent;

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblFilterProductType = new QLabel( this );
    lblFilterProductType->setObjectName( QString::fromUtf8( "lblFilterProductType" ) );
    lblFilterProductType->setText( tr("Product type: ") );
    horizontalLayout->addWidget( lblFilterProductType );
    cmbFilterProductType = new QComboBox( this );
    cmbFilterProductType->setObjectName( QString::fromUtf8( "cmbFilterProductType" ) );
    horizontalLayout->addWidget( cmbFilterProductType );
    lblFilterName = new QLabel( this );
    lblFilterName->setObjectName( QString::fromUtf8( "lblFilterName" ) );
    lblFilterName->setText( tr("Name: ") );
    horizontalLayout->addWidget( lblFilterName );
    ledFilterName = new QLineEdit( this );
    ledFilterName->setObjectName( QString::fromUtf8( "ledFilterName" ) );
    ledFilterName->setMaximumWidth( 150 );
    horizontalLayout->addWidget( ledFilterName );
    lblFilterBarcode = new QLabel( this );
    lblFilterBarcode->setObjectName( QString::fromUtf8( "lblFilterBarcode" ) );
    lblFilterBarcode->setText( tr("Barcode: ") );
    horizontalLayout->addWidget( lblFilterBarcode );
    ledFilterBarcode = new QLineEdit( this );
    ledFilterBarcode->setObjectName( QString::fromUtf8( "ledFilterBarcode" ) );
    ledFilterBarcode->setMaximumWidth( 150 );
    horizontalLayout->addWidget( ledFilterBarcode );
    lblFilterMinCount = new QLabel( this );
    lblFilterMinCount->setObjectName( QString::fromUtf8( "lblFilterMinCount" ) );
    lblFilterMinCount->setText( tr("Product count minimum: ") );
    horizontalLayout->addWidget( lblFilterMinCount );
    ledFilterMinCount = new QLineEdit( this );
    ledFilterMinCount->setObjectName( QString::fromUtf8( "ledFilterMinCount" ) );
    ledFilterMinCount->setMaximumWidth( 25 );
    horizontalLayout->addWidget( ledFilterMinCount );
    lblFilterMaxCount = new QLabel( this );
    lblFilterMaxCount->setObjectName( QString::fromUtf8( "lblFilterMaxCount" ) );
    lblFilterMaxCount->setText( tr("maximum: ") );
    horizontalLayout->addWidget( lblFilterMaxCount );
    ledFilterMaxCount = new QLineEdit( this );
    ledFilterMaxCount->setObjectName( QString::fromUtf8( "ledFilterMaxCount" ) );
    ledFilterMaxCount->setMaximumWidth( 35 );
    horizontalLayout->addWidget( ledFilterMaxCount );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );

    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery   *poQuery;
    QString      qsQuery = "SELECT * FROM producttypes WHERE productTypeId>0 AND active=1";

    cmbFilterProductType->addItem( tr("<All product types>"), -1 );
    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        cmbFilterProductType->addItem( poQuery->value( 2 ).toString(), poQuery->value( 0 ) );
    }

    pbProductType = new QPushButton( tr( "Product types" ), this );
    pbProductType->setObjectName( QString::fromUtf8( "pbProductType" ) );
    pbProductType->setIconSize( QSize(20, 20) );
    pbProductType->setIcon( QIcon("./resources/40x40_producttype.png") );
    pbProductType->setAutoDefault( false );
    btbButtonsSide->addButton( pbProductType, QDialogButtonBox::ActionRole );
    connect( pbProductType, SIGNAL(clicked()), this, SLOT(_slotProductTypes()) );
    pbProductActionType = new QPushButton( tr( "Product action types" ), this );
    pbProductActionType->setObjectName( QString::fromUtf8( "pbProductActionType" ) );
    pbProductActionType->setIconSize( QSize(20, 20) );
    pbProductActionType->setIcon( QIcon("./resources/40x40_productactiontype.png") );
    pbProductActionType->setAutoDefault( false );
    btbButtonsSide->addButton( pbProductActionType, QDialogButtonBox::ActionRole );
    connect( pbProductActionType, SIGNAL(clicked()), this, SLOT(_slotProductActionTypes()) );

    pbStockProcess = new QPushButton( tr( "Stock process" ), this );
    pbStockProcess->setObjectName( QString::fromUtf8( "pbStockProcess" ) );
    pbStockProcess->setIconSize( QSize(20, 20) );
    pbStockProcess->setIcon( QIcon("./resources/40x40_storage.png") );
    pbStockProcess->setAutoDefault( false );
    btbButtonsSide->addButton( pbStockProcess, QDialogButtonBox::ActionRole );
    connect( pbStockProcess, SIGNAL(clicked()), this, SLOT(_slotStockProcess()) );
    pbStockProcess->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    pbStockIncrease = new QPushButton( tr( "Increase stock" ), this );
    pbStockIncrease->setObjectName( QString::fromUtf8( "pbStockIncrease" ) );
    pbStockIncrease->setIconSize( QSize(20, 20) );
    pbStockIncrease->setIcon( QIcon("./resources/40x40_stock_add.png") );
    pbStockIncrease->setAutoDefault( false );
    btbButtons->addButton( pbStockIncrease, QDialogButtonBox::ActionRole );
    connect( pbStockIncrease, SIGNAL(clicked()), this, SLOT(_slotStockIncrease()) );
    pbStockDecrease = new QPushButton( tr( "Decrease stock" ), this );
    pbStockDecrease->setObjectName( QString::fromUtf8( "pbStockDecrease" ) );
    pbStockDecrease->setIconSize( QSize(20, 20) );
    pbStockDecrease->setIcon( QIcon("./resources/40x40_stock_remove.png") );
    pbStockDecrease->setAutoDefault( false );
    btbButtons->addButton( pbStockDecrease, QDialogButtonBox::ActionRole );
    connect( pbStockDecrease, SIGNAL(clicked()), this, SLOT(_slotStockDecrease()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListProducts", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    connect( cmbFilterProductType, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( ledFilterName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledFilterBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledFilterMinCount, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledFilterMaxCount, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );

    m_poBtnSave->setEnabled( false );
    m_poBtnSave->setVisible( false );
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
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "VAT percent" ) );
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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "VAT percent" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Product count" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 1 );
    }
}

void cDlgProduct::refreshTable()
{
    cTracer obTracer( "cDlgProduct::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT products.productId, products.licenceId, name, barcode, (netPriceSell/100) as netPriceSell, vatpercentSell, productCount, active, archive FROM products LEFT JOIN connectproductwithtype ON products.productId = connectproductwithtype.productId WHERE products.productId>0 ";
    }
    else
    {
        m_qsQuery = "SELECT products.productId AS id, name, barcode, (netPriceSell/100) as netPriceSell, vatpercentSell, productCount FROM products LEFT JOIN connectproductwithtype ON products.productId = connectproductwithtype.productId WHERE active=1 AND products.productId>0 ";
    }

    int uiProductTypeId = cmbFilterProductType->itemData( cmbFilterProductType->currentIndex() ).toInt();
    if( uiProductTypeId > -1 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "connectproductwithtype.productTypeId=%1" ).arg( uiProductTypeId );
    }
    QString stTemp;

    stTemp = ledFilterName->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '\%%1\%'" ).arg( stTemp );
    }

    stTemp = ledFilterBarcode->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '\%%1\%'" ).arg( stTemp );
    }

    stTemp = ledFilterMinCount->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "productCount>=%1" ).arg( stTemp );
    }

    stTemp = ledFilterMaxCount->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "productCount<=%1" ).arg( stTemp );
    }

    m_qsQuery += " GROUP BY products.productId ";

    cDlgCrud::refreshTable();
}

void cDlgProduct::enableButtons()
{
    cTracer obTracer( "cDlgProduct::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    pbStockIncrease->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    pbStockDecrease->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
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

void cDlgProduct::_slotProductTypes()
{
    cDlgProductType obDlgProductType( m_poParent );

    QDialog::accept();
    obDlgProductType.exec();
}

void cDlgProduct::_slotProductActionTypes()
{
    cDlgProductActionType   obDlgProductActionType( m_poParent );

    QDialog::accept();
    obDlgProductActionType.exec();
}

void cDlgProduct::_slotStockIncrease()
{
    cDBProduct m_poProduct;

    m_poProduct.load( m_uiSelectedId );

    dlgProductStorage   *obDlgProductStorage = new dlgProductStorage( this, &m_poProduct );

    if( obDlgProductStorage->exec() == QDialog::Accepted )
    {
        refreshTable();
    }
    delete obDlgProductStorage;
}

void cDlgProduct::_slotStockDecrease()
{
    cDBProduct m_poProduct;

    m_poProduct.load( m_uiSelectedId );

    if( m_poProduct.productCount() < 1 )
        return;

    dlgProductStorage   *obDlgProductStorage = new dlgProductStorage( this, &m_poProduct, false );

    if( obDlgProductStorage->exec() == QDialog::Accepted )
    {
        refreshTable();
    }
    delete obDlgProductStorage;
}

void cDlgProduct::_slotStockProcess()
{
    dlgStockProcess *obDlgStockProcess = new dlgStockProcess( this, tr("Stock process") );

    obDlgStockProcess->exec();
    refreshTable();
    delete obDlgStockProcess;
}
