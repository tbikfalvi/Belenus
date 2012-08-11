//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgproductsell.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Termek eladas.
//====================================================================================

#include <QMessageBox>

#include "belenus.h"
#include "dlgproductsell.h"
#include "dlgproduct.h"

cDlgProductSell::cDlgProductSell( QWidget *p_poParent, QString p_qsBarcode ) : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgProductSell::cDlgProductSell" );

    setWindowTitle( tr( "Selling Products" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);

    verticalLayoutTop = new QVBoxLayout();
    verticalLayoutTop->setObjectName( QString::fromUtf8( "verticalLayoutTop" ) );

    gbFilter = new QGroupBox( tr(" Search filters "), this );
    gbFilter->setObjectName( QString::fromUtf8( "gbFilter" ) );
    verticalLayoutTop->addWidget( gbFilter );

    layoutFilterGroup = new QHBoxLayout();
    layoutFilterGroup->setObjectName( QString::fromUtf8( "layoutFilterGroup" ) );

    lblBarcode = new QLabel( this );
    lblBarcode->setObjectName( QString::fromUtf8( "lblBarcode" ) );
    lblBarcode->setText( tr("Barcode: ") );
    layoutFilterGroup->addWidget( lblBarcode );

    ledBarcode = new QLineEdit( this );
    ledBarcode->setObjectName( QString::fromUtf8( "ledBarcode" ) );
    layoutFilterGroup->addWidget( ledBarcode );

    spacerFilterGroup = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layoutFilterGroup->addItem( spacerFilterGroup );

    gbFilter->setLayout( layoutFilterGroup );

    //---------------------------------------------------------------------------------------------
    gbItemToSell = new QGroupBox( tr(" Selected product to sell "), this );
    gbItemToSell->setObjectName( QString::fromUtf8( "gbItemToSell" ) );
    verticalLayoutTop->addWidget( gbItemToSell );

    layoutItemToSellGroup = new QHBoxLayout();
    layoutItemToSellGroup->setObjectName( QString::fromUtf8( "layoutItemToSellGroup" ) );

    lblName = new QLabel( this );
    lblName->setObjectName( QString::fromUtf8( "lblName" ) );
    lblName->setText( tr("Name: ") );
    layoutItemToSellGroup->addWidget( lblName );

    ledName = new QLineEdit( this );
    ledName->setObjectName( QString::fromUtf8( "ledName" ) );
    ledName->setEnabled( false );
    layoutItemToSellGroup->addWidget( ledName );

    lblItemCount = new QLabel( this );
    lblItemCount->setObjectName( QString::fromUtf8( "lblItemCount" ) );
    lblItemCount->setText( tr("Count: ") );
    layoutItemToSellGroup->addWidget( lblItemCount );

    ledItemCount = new QLineEdit( this );
    ledItemCount->setObjectName( QString::fromUtf8( "ledItemCount" ) );
    ledItemCount->setEnabled( false );
    ledItemCount->setMinimumWidth( 40 );
    ledItemCount->setMaximumWidth( 40 );
    layoutItemToSellGroup->addWidget( ledItemCount );

    pbItemCountIncrease = new QPushButton( this );
    pbItemCountIncrease->setObjectName( QString::fromUtf8( "pbItemCountIncrease" ) );
    pbItemCountIncrease->setIconSize( QSize(16, 16) );
    pbItemCountIncrease->setIcon( QIcon("./resources/40x40_plus.png") );
    layoutItemToSellGroup->addWidget( pbItemCountIncrease );

    pbItemCountDecrease = new QPushButton( this );
    pbItemCountDecrease->setObjectName( QString::fromUtf8( "pbItemCountDecrease" ) );
    pbItemCountDecrease->setIconSize( QSize(16, 16) );
    pbItemCountDecrease->setIcon( QIcon("./resources/40x40_minus.png") );
    layoutItemToSellGroup->addWidget( pbItemCountDecrease );

    lblTotalPrice = new QLabel( this );
    lblTotalPrice->setObjectName( QString::fromUtf8( "lblTotalPrice" ) );
    lblTotalPrice->setText( tr("Total price: ") );
    layoutItemToSellGroup->addWidget( lblTotalPrice );

    ledTotalPrice = new QLineEdit( this );
    ledTotalPrice->setObjectName( QString::fromUtf8( "ledTotalPrice" ) );
    ledTotalPrice->setEnabled( false );
    ledTotalPrice->setMinimumWidth( 150 );
    ledTotalPrice->setMaximumWidth( 150 );
    layoutItemToSellGroup->addWidget( ledTotalPrice );

    gbItemToSell->setLayout( layoutItemToSellGroup );

    verticalLayout->insertLayout( 0, verticalLayoutTop );

    pbCancel = new QPushButton( tr( "Exit" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_exit.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    pbPayment = new QPushButton( tr( "Sell" ), this );
    pbPayment->setObjectName( QString::fromUtf8( "pbPayment" ) );
    pbPayment->setIconSize( QSize(20, 20) );
    pbPayment->setIcon( QIcon("./resources/40x40_cassa.png") );
    btbButtonsSide->addButton( pbPayment, QDialogButtonBox::ActionRole );

    pbToCart = new QPushButton( tr( "To shopping cart" ), this );
    pbToCart->setObjectName( QString::fromUtf8( "pbToCart" ) );
    pbToCart->setIconSize( QSize(20, 20) );
    pbToCart->setIcon( QIcon("./resources/40x40_shoppingcart.png") );
    btbButtonsSide->addButton( pbToCart, QDialogButtonBox::ActionRole );

    pbEditProducts = new QPushButton( tr( "Product list" ), this );
    pbEditProducts->setObjectName( QString::fromUtf8( "pbEditProducts" ) );
    pbEditProducts->setIconSize( QSize(20, 20) );
    pbEditProducts->setIcon( QIcon("./resources/40x40_product.png") );
    btbButtonsSide->addButton( pbEditProducts, QDialogButtonBox::ActionRole );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ProductSell", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    connect( ledBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbItemCountIncrease, SIGNAL(clicked(bool)), this, SLOT(on_pbItemCountIncrease_clicked()) );
    connect( pbItemCountDecrease, SIGNAL(clicked(bool)), this, SLOT(on_pbItemCountDecrease_clicked()) );
    connect( pbPayment, SIGNAL(clicked(bool)), this, SLOT(on_pbPayment_clicked()) );
    connect( pbToCart, SIGNAL(clicked(bool)), this, SLOT(on_pbToCart_clicked()) );
    connect( pbEditProducts, SIGNAL(clicked()), this, SLOT(on_pbEditProducts_clicked()) );

    m_obProduct.createNew();

    setupTableView();

    if( p_qsBarcode.length() )
        ledBarcode->setText( p_qsBarcode );
}

cDlgProductSell::~cDlgProductSell()
{
    cTracer obTrace( "cDlgProductSell::~cDlgProductSell" );

    g_poPrefs->setDialogSize( "ProductSell", QPoint( width(), height() ) );
}

void cDlgProductSell::setupTableView()
{
    cTracer obTracer( "cDlgProductSell::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Net price" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "VAT percent" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Sum price" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Product count" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 9, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );
        tbvCrud->resizeColumnToContents( 9 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Net price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "VAT percent" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Sum price" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Product count" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgProductSell::refreshTable()
{
    cTracer obTracer( "cDlgProductSell::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT productId, licenceId, name, barcode, netPriceSell, vatpercentSell, netPriceSell+((netPriceSell/100)*vatpercentSell) AS sumPriceSell, productCount, active, archive FROM products WHERE productId>0";
    }
    else
    {
        m_qsQuery = "SELECT productId AS id, name, barcode, netPriceSell, vatpercentSell, netPriceSell+((netPriceSell/100)*vatpercentSell) AS sumPriceSell, productCount FROM products WHERE active=1 AND productId>0";
    }

    QString qsTemp = ledBarcode->text();
    if( qsTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '\%%1\%'" ).arg( qsTemp );
    }

    cDlgCrud::refreshTable();
}

void cDlgProductSell::enableButtons()
{
    cTracer obTracer( "cDlgProductSell::enableButtons" );

    if( m_uiSelectedId )
    {
        m_obProduct.load( m_uiSelectedId );
    }
    else
    {
        m_obProduct.createNew();
    }

    ledName->setText( m_obProduct.name() );
    ledItemCount->setText( QString::number( (m_obProduct.productCount()>0?1:0) ) );
    _calculateTotalPrice();

    enablePayment();
}

void cDlgProductSell::enablePayment()
{
    cTracer obTrace( "cDlgProductSell::enablePayment" );

    if( m_uiSelectedId && ledItemCount->text().toInt() > 0 )
    {
        pbPayment->setEnabled( true );
        pbToCart->setEnabled( true );
    }
    else
    {
        pbPayment->setEnabled( false );
        pbToCart->setEnabled( false );
    }
}

void cDlgProductSell::newClicked( bool ) {}
void cDlgProductSell::editClicked( bool ) {}
void cDlgProductSell::deleteClicked( bool ) {}

void cDlgProductSell::on_pbItemCountIncrease_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbItemCountIncrease_clicked" );

    int nCount = ledItemCount->text().toInt();

    if( nCount < m_obProduct.productCount() )
    {
        nCount++;
        ledItemCount->setText( QString::number( nCount ) );
        _calculateTotalPrice();
    }
}

void cDlgProductSell::on_pbItemCountDecrease_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbItemCountDecrease_clicked" );

    int nCount = ledItemCount->text().toInt();

    if( nCount > 1 )
    {
        nCount--;
        ledItemCount->setText( QString::number( nCount ) );
        _calculateTotalPrice();
    }
}

void cDlgProductSell::on_pbPayment_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbPayment_clicked" );

    cDBShoppingCart obDBShoppingCart;
    obDBShoppingCart.createNew();

    int     nDiscount = 0;
    int     nCount = ledItemCount->text().toInt();
    int     nTotalPrice = ( m_obProduct.netPriceSell() + (m_obProduct.netPriceSell()/100)*m_obProduct.vatPercentSell() ) * nCount;

    if( g_obGuest.id() > 0 )
    {
        nDiscount = nTotalPrice - g_obGuest.getDiscountedPrice( nTotalPrice );
    }

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( m_obProduct.id() );
    obDBShoppingCart.setItemName( m_obProduct.name() );
    obDBShoppingCart.setItemCount( nCount );
    obDBShoppingCart.setItemNetPrice( m_obProduct.netPriceSell() );
    obDBShoppingCart.setItemVAT( m_obProduct.vatPercentSell() );
    obDBShoppingCart.setItemDiscount( nDiscount );
    obDBShoppingCart.setItemSumPrice( nTotalPrice - nDiscount );

    g_obLogger(cSeverity::DEBUG) << QString("cDlgProductSell::on_pbPayment_clicked() >> Name: %1 | Net: %2 | Count: %3 | Disc.: %4 | Sum: %5 ").arg(obDBShoppingCart.itemName()).arg(obDBShoppingCart.itemNetPrice()).arg(obDBShoppingCart.itemCount()).arg(obDBShoppingCart.itemDiscount()).arg(obDBShoppingCart.itemSumPrice()) << EOM;

    emit signalPaymentProcessed( obDBShoppingCart );

    QDialog::accept();
}

void cDlgProductSell::on_pbToCart_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbToCart_clicked" );

    cDBShoppingCart obDBShoppingCart;
    obDBShoppingCart.createNew();

    int     nDiscount = 0;
    int     nCount = ledItemCount->text().toInt();
    int     nTotalPrice = ( m_obProduct.netPriceSell() + (m_obProduct.netPriceSell()/100)*m_obProduct.vatPercentSell() ) * nCount;

    if( g_obGuest.id() > 0 )
    {
        nDiscount = nTotalPrice - g_obGuest.getDiscountedPrice( nTotalPrice );
    }

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( m_obProduct.id() );
    obDBShoppingCart.setItemName( m_obProduct.name() );
    obDBShoppingCart.setItemCount( nCount );
    obDBShoppingCart.setItemNetPrice( m_obProduct.netPriceSell() );
    obDBShoppingCart.setItemVAT( m_obProduct.vatPercentSell() );
    obDBShoppingCart.setItemDiscount( nDiscount );
    obDBShoppingCart.setItemSumPrice( nTotalPrice - nDiscount );

    obDBShoppingCart.save();

    QDialog::accept();
}

void cDlgProductSell::on_pbEditProducts_clicked()
{
    cDlgProduct obDlgProduct( this );

    obDlgProduct.exec();
    refreshTable();
}

void cDlgProductSell::_calculateTotalPrice()
{
    int     nCount = ledItemCount->text().toInt();
    int     nTotalPrice = ( m_obProduct.netPriceSell() + (m_obProduct.netPriceSell()/100)*m_obProduct.vatPercentSell() ) * nCount;
    int     nDiscountedPrice = nTotalPrice;

    if( g_obGuest.id() > 0 )
    {
        nDiscountedPrice = g_obGuest.getDiscountedPrice( nTotalPrice );
    }
    else
    {
        nDiscountedPrice = m_obProduct.getDiscountedPrice( nTotalPrice );
    }

    if( nDiscountedPrice != nTotalPrice )
        ledTotalPrice->setText( QString("%1 (%2)").arg(convertCurrency(nDiscountedPrice,g_poPrefs->getCurrencyShort())).arg(convertCurrency(nTotalPrice,g_poPrefs->getCurrencyShort())) );
    else
        ledTotalPrice->setText( convertCurrency(nTotalPrice,g_poPrefs->getCurrencyShort()) );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
QString cDlgProductSell::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}

