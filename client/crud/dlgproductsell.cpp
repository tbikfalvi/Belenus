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
#include "../db/dbledger.h"

cDlgProductSell::cDlgProductSell( QWidget *p_poParent, QString p_qsBarcode ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgProductSell::cDlgProductSell" );

    setupUi( this );

    setWindowTitle( tr( "Selling Products" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbPayment->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    connect( ledFilterName, SIGNAL(returnPressed()), this, SLOT(on_pbRefresh_clicked()) );
    connect( ledBarcode, SIGNAL(returnPressed()), this, SLOT(on_pbRefresh_clicked()) );

    lblCurrencyToPay->setText( g_poPrefs->getCurrencyShort() );

/*
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

    lblFilterName = new QLabel( this );
    lblFilterName->setObjectName( QString::fromUtf8( "lblFilterName" ) );
    lblFilterName->setText( tr("Name: ") );
    layoutFilterGroup->addWidget( lblFilterName );

    ledFilterName = new QLineEdit( this );
    ledFilterName->setObjectName( QString::fromUtf8( "ledFilterName" ) );
    layoutFilterGroup->addWidget( ledFilterName );

    lblBarcode = new QLabel( this );
    lblBarcode->setObjectName( QString::fromUtf8( "lblBarcode" ) );
    lblBarcode->setText( tr("Barcode: ") );
    layoutFilterGroup->addWidget( lblBarcode );

    ledBarcode = new QLineEdit( this );
    ledBarcode->setObjectName( QString::fromUtf8( "ledBarcode" ) );
    layoutFilterGroup->addWidget( ledBarcode );

    spacerFilterGroup = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layoutFilterGroup->addItem( spacerFilterGroup );

    pbRefresh = new QPushButton( tr( "Refresh" ), this );
    pbRefresh->setObjectName( QString::fromUtf8( "pbRefresh" ) );
    pbRefresh->setIconSize( QSize(20, 20) );
    pbRefresh->setIcon( QIcon("./resources/40x40_refresh.png") );
    layoutFilterGroup->addWidget( pbRefresh );

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

    spacerToPay = new QSpacerItem( 10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layoutItemToSellGroup->addItem( spacerToPay );

    lblTotalPrice = new QLabel( this );
    lblTotalPrice->setObjectName( QString::fromUtf8( "lblTotalPrice" ) );
    lblTotalPrice->setText( tr("Price to pay: ") );
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

    connect( pbRefresh, SIGNAL(clicked()), this, SLOT(refreshTable()) );
//    connect( ledBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbItemCountIncrease, SIGNAL(clicked(bool)), this, SLOT(on_pbItemCountIncrease_clicked()) );
    connect( pbItemCountDecrease, SIGNAL(clicked(bool)), this, SLOT(on_pbItemCountDecrease_clicked()) );
    connect( pbPayment, SIGNAL(clicked(bool)), this, SLOT(on_pbPayment_clicked()) );
    connect( pbToCart, SIGNAL(clicked(bool)), this, SLOT(on_pbToCart_clicked()) );
    connect( pbEditProducts, SIGNAL(clicked()), this, SLOT(on_pbEditProducts_clicked()) );
*/
    m_obProduct.createNew();

//    setupTableView();

    if( p_qsBarcode.length() )
    {
        ledBarcode->setText( p_qsBarcode );
//        refreshTable();
    }

    pbPayment->setEnabled( false );
    ledFilterName->setFocus();

    on_pbRefresh_clicked();

    if( ledFilterName->text().length() == 0 && ledBarcode->text().length() == 0 )
        ledFilterName->setFocus();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ProductSell", QPoint(555,200) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgProductSell::~cDlgProductSell()
{
    cTracer obTrace( "cDlgProductSell::~cDlgProductSell" );

    g_poPrefs->setDialogSize( "ProductSell", QPoint( width(), height() ) );
}

void cDlgProductSell::on_pbPayment_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbPayment_clicked" );

    cDBShoppingCart obDBShoppingCart;
    obDBShoppingCart.createNew();

    int     nDiscount = 0;
    int     nCount = cmbProductCount->itemData( cmbProductCount->currentIndex() ).toInt();

    if( nCount == 0 )
        return;

    cCurrency   cPrice( m_obProduct.netPriceSell() * nCount, cCurrency::CURR_NET, m_obProduct.vatPercentSell() );

    int     nTotalPrice = cPrice.currencyValue().toInt();

    if( g_obGuest.id() > 0 )
    {
        nDiscount = nTotalPrice - g_obGuest.getDiscountedPrice( nTotalPrice );
    }
    else
    {
        nDiscount = nTotalPrice - m_obProduct.getDiscountedPrice( nTotalPrice );
    }

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( m_obProduct.id() );
    obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PROD_SELL );
    obDBShoppingCart.setItemName( m_obProduct.name() );
    obDBShoppingCart.setItemCount( nCount );
    obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
    obDBShoppingCart.setItemVAT( m_obProduct.vatPercentSell() );
    obDBShoppingCart.setItemDiscount( nDiscount );
    obDBShoppingCart.setItemSumPrice( nTotalPrice - nDiscount );

    emit signalPaymentProcessed( obDBShoppingCart );

    QDialog::accept();
}

/*void cDlgProductSell::on_pbToCart_clicked()
{
    cTracer obTrace( "cDlgProductSell::on_pbToCart_clicked" );

    cDBShoppingCart obDBShoppingCart;
    obDBShoppingCart.createNew();

    int     nDiscount = 0;
    int     nCount = ledItemCount->text().toInt();

    cCurrency   cPrice( m_obProduct.netPriceSell()*nCount, cCurrency::CURR_GROSS, m_obProduct.vatPercentSell() );

    int     nTotalPrice = cPrice.currencyValue().toInt();

    if( g_obGuest.id() > 0 )
    {
        nDiscount = nTotalPrice - g_obGuest.getDiscountedPrice( nTotalPrice );
    }

    obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
    obDBShoppingCart.setGuestId( g_obGuest.id() );
    obDBShoppingCart.setProductId( m_obProduct.id() );
    obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PROD_SELL );
    obDBShoppingCart.setItemName( m_obProduct.name() );
    obDBShoppingCart.setItemCount( nCount );
    obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
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
*/
void cDlgProductSell::_calculateTotalPrice()
{
    if( cmbProduct->currentIndex() < 0 || cmbProduct->currentIndex() >= qvProducts.count() )
        return;

    int     nCount = cmbProductCount->itemData( cmbProductCount->currentIndex() ).toInt();

    pbPayment->setEnabled( nCount > 0 );

    m_obProduct.load( qvProducts.at(cmbProduct->currentIndex())->uiId );

    cCurrency   cPrice( m_obProduct.netPriceSell() * nCount, cCurrency::CURR_NET, m_obProduct.vatPercentSell() );

    int     nTotalPrice = cPrice.currencyValue().toInt();
    int     nDiscountedPrice = nTotalPrice;

    if( g_obGuest.id() > 0 )
    {
        nDiscountedPrice = g_obGuest.getDiscountedPrice( nTotalPrice );
    }
    else
    {
        nDiscountedPrice = m_obProduct.getDiscountedPrice( nTotalPrice );
    }

    g_obLogger(cSeverity::DEBUG) << "**********"
                                 << " netprice: " << qvProducts.at(cmbProduct->currentIndex())->inNetPrice
                                 << " count: " << nCount
                                 << " total " << nTotalPrice
                                 << " discount " << nDiscountedPrice
                                 << EOM;

    cCurrency cDiscount( nDiscountedPrice );

    if( nDiscountedPrice != nTotalPrice )
        ledAmountToPay->setText( QString("%1 (%2)").arg(cDiscount.currencyStringSeparator()).arg(cPrice.currencyStringSeparator()) );
    else
        ledAmountToPay->setText( cPrice.currencyStringSeparator() );
}
//===========================================================================================================
void cDlgProductSell::on_pbRefresh_clicked()
//===========================================================================================================
{
    QString m_qsQuery = "SELECT productId AS id, name, barcode, netPriceSell, vatpercentSell, productCount FROM products WHERE active=1 AND productId>0";

    if( ledBarcode->text().length() > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '%" + ledBarcode->text() + "%'" );
    }

    if( ledFilterName->text().length() > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '%" + ledFilterName->text() + "%'" );
    }

    m_qsQuery += " ORDER BY name ";

    QSqlQuery   *poQuery = g_poDB->executeQTQuery( m_qsQuery );
    tsProducts  *tsTemp;

    qvProducts.clear();
    cmbProduct->clear();
    cmbProduct->setEnabled( true );

    if( poQuery->size() > 1 )
    {
        tsTemp = new tsProducts;

        tsTemp->uiId            = 0;
        tsTemp->qsName          = "";
        tsTemp->qsBarcode       = "";
        tsTemp->inNetPrice      = 0;
        tsTemp->inVatPercent    = 0;
        tsTemp->inProductCount  = 0;

        qvProducts.append( tsTemp );

        cmbProduct->addItem( tr("<Select product to sell>"), 0 );
    }
    else if( poQuery->size() < 1 )
    {
        tsTemp = new tsProducts;

        tsTemp->uiId            = 0;
        tsTemp->qsName          = "";
        tsTemp->qsBarcode       = "";
        tsTemp->inNetPrice      = 0;
        tsTemp->inVatPercent    = 0;
        tsTemp->inProductCount  = 0;

        qvProducts.append( tsTemp );

        cmbProduct->addItem( tr("<No product found for selected filter conditions>"), 0 );
        cmbProduct->setEnabled( false );
    }

    while( poQuery->next() )
    {
        tsTemp = new tsProducts;

        tsTemp->uiId            = poQuery->value(0).toUInt();
        tsTemp->qsName          = poQuery->value(1).toString();
        tsTemp->qsBarcode       = poQuery->value(2).toString();
        tsTemp->inNetPrice      = poQuery->value(3).toInt();
        tsTemp->inVatPercent    = poQuery->value(4).toInt();
        tsTemp->inProductCount  = poQuery->value(5).toInt();

        qvProducts.append( tsTemp );

        cmbProduct->addItem( poQuery->value(1).toString(), poQuery->value(0).toUInt() );
    }

    cmbProduct->setCurrentIndex( 0 );
    cmbProductCount->setFocus();
}
//===========================================================================================================
void cDlgProductSell::on_cmbProduct_currentIndexChanged(int index)
//===========================================================================================================
{
    if(  index < 0 || index >= qvProducts.count() )
        return;

    cmbProductCount->clear();
    cmbProductCount->setEnabled( false );
    ledPrice->setText( "" );
    ledCurrentCount->setText( "" );

    if( cmbProduct->itemData(index).toInt() > 0 )
    {
        cCurrency   cPrice( qvProducts.at(index)->inNetPrice, cCurrency::CURR_NET, qvProducts.at(index)->inVatPercent );

        ledPrice->setText( cPrice.currencyFullStringShort() );
        ledCurrentCount->setText( QString::number(qvProducts.at(index)->inProductCount) );

        cmbProductCount->addItem( tr("<Choose ...>"), 0 );
        for( int i=0; i<qvProducts.at(index)->inProductCount; i++ )
        {
            cmbProductCount->addItem( QString::number(i+1), i+1 );
        }
        cmbProductCount->setEnabled( qvProducts.at(index)->inProductCount > 0 );
        _calculateTotalPrice();
    }
}
//===========================================================================================================
void cDlgProductSell::on_cmbProductCount_currentIndexChanged(int /*index*/)
//===========================================================================================================
{
    _calculateTotalPrice();
}
//===========================================================================================================
void cDlgProductSell::on_pbCancel_clicked()
{
    QDialog::reject();
}
