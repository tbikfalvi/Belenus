#include <QMessageBox>
#include <QStringList>

#include "belenus.h"
#include "dlgshoppingcart.h"
#include "../db/dbshoppingcart.h"
#include "../dlg/dlgcassaaction.h"
#include "../db/dbpanels.h"
#include "../db/dbproduct.h"
#include "../db/dbledger.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"
#include "../db/dbpatientcardunits.h"
#include "../db/dbdiscount.h"

cDlgShoppingCart::cDlgShoppingCart( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Shopping cart" ) );
    setWindowIcon( QIcon("./resources/40x40_shoppingcart.png") );

    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    tbvCrud->setSelectionMode( QAbstractItemView::MultiSelection );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );

    lblPanel = new QLabel( this );
    lblPanel->setObjectName( QString::fromUtf8( "lblPanel" ) );
    lblPanel->setText( tr("Panel :") );
    horizontalLayout->addWidget( lblPanel );

    cmbPanel = new QComboBox( this );
    cmbPanel->setObjectName( QString::fromUtf8( "cmbPanel" ) );
    horizontalLayout->addWidget( cmbPanel );

    lblGuest = new QLabel( this );
    lblGuest->setObjectName( QString::fromUtf8( "lblGuest" ) );
    lblGuest->setText( tr("Guest :") );
    horizontalLayout->addWidget( lblGuest );

    cmbGuest = new QComboBox( this );
    cmbGuest->setObjectName( QString::fromUtf8( "cmbGuest" ) );
    horizontalLayout->addWidget( cmbGuest );

    horizontalSpacer = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer );

    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery   *poQuery;

    cmbPanel->addItem( tr("<All panels>"), -1 );
    cmbPanel->addItem( tr("<Panel independent>"), 0 );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT panelId, title FROM panels WHERE active=1" ) );
    while( poQuery->next() )
    {
        cmbPanel->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    cmbGuest->addItem( tr("<All guests>"), -1 );
    cmbGuest->addItem( tr("<Guest independent>"), 0 );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1" ) );
    while( poQuery->next() )
    {
        cmbGuest->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    connect( cmbPanel, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( cmbGuest, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );

    pbPatientCard = new QPushButton( tr( "Add Patient card to cart" ), this );
    pbPatientCard->setObjectName( QString::fromUtf8( "pbPatientCard" ) );
    pbPatientCard->setIconSize( QSize(20, 20) );
    pbPatientCard->setIcon( QIcon("./resources/40x40_patientcard.png") );
    btbButtons->addButton( pbPatientCard, QDialogButtonBox::ActionRole );

    pbProduct = new QPushButton( tr( "Add Product to cart" ), this );
    pbProduct->setObjectName( QString::fromUtf8( "pbProduct" ) );
    pbProduct->setIconSize( QSize(20, 20) );
    pbProduct->setIcon( QIcon("./resources/40x40_product.png") );
    btbButtons->addButton( pbProduct, QDialogButtonBox::ActionRole );

    pbPayment = new QPushButton( tr( "Payment" ), this );
    pbPayment->setObjectName( QString::fromUtf8( "pbPayment" ) );
    pbPayment->setIconSize( QSize(20, 20) );
    pbPayment->setIcon( QIcon("./resources/40x40_cassa.png") );
    btbButtons->addButton( pbPayment, QDialogButtonBox::ActionRole );

    connect( pbPatientCard, SIGNAL(clicked()), this, SLOT(on_pbPatientCard_clicked()) );
    connect( pbProduct, SIGNAL(clicked()), this, SLOT(on_pbProduct_clicked()) );
    connect( pbPayment, SIGNAL(clicked(bool)), this, SLOT(on_pbPayment_clicked()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ShoppingCart", QPoint(800,400) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    m_poBtnNew->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnSave->setVisible(false);
    m_poBtnDelete->setVisible( g_obUser.isInGroup( cAccessGroup::USER ) );
}

cDlgShoppingCart::~cDlgShoppingCart()
{
    g_poPrefs->setDialogSize( "ShoppingCart", QPoint( width(), height() ) );
}

void cDlgShoppingCart::setPanelFilter( unsigned int p_uiPanelId )
{
    for( int i=0; i<cmbPanel->count(); i++ )
    {
        if( cmbPanel->itemData(i).toUInt() == p_uiPanelId )
        {
            cmbPanel->setCurrentIndex( i );
            break;
        }
    }
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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Panel" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Patient" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "ProductId" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "PatientCardId" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "NetPrice" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "VATPercent" ) );
        m_poModel->setHeaderData( 9, Qt::Horizontal, tr( "SumPrice" ) );
        m_poModel->setHeaderData( 10, Qt::Horizontal, tr( "Count" ) );
        m_poModel->setHeaderData( 11, Qt::Horizontal, tr( "Discount" ) );
        m_poModel->setHeaderData( 12, Qt::Horizontal, tr( "Voucher" ) );
        m_poModel->setHeaderData( 13, Qt::Horizontal, tr( "TotalSumPrice" ) );
        m_poModel->setHeaderData( 14, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 15, Qt::Horizontal, tr( "Archive" ) );

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
        tbvCrud->resizeColumnToContents( 10 );
        tbvCrud->resizeColumnToContents( 11 );
        tbvCrud->resizeColumnToContents( 12 );
        tbvCrud->resizeColumnToContents( 13 );
        tbvCrud->resizeColumnToContents( 14 );
        tbvCrud->resizeColumnToContents( 15 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Panel" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Patient" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "NetPrice" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "VATPercent" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "SumPrice" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Count" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Discount" ) );
        m_poModel->setHeaderData( 9, Qt::Horizontal, tr( "Voucher" ) );
        m_poModel->setHeaderData( 10, Qt::Horizontal, tr( "TotalSumPrice" ) );
        m_poModel->setHeaderData( 11, Qt::Horizontal, tr( "Comment" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );
        tbvCrud->resizeColumnToContents( 9 );
        tbvCrud->resizeColumnToContents( 10 );
        tbvCrud->resizeColumnToContents( 11 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgShoppingCart::refreshTable()
{
    cTracer obTracer( "cDlgShoppingCart::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT shoppingCartItemId, shoppingCartItems.licenceId, title, patients.name, productId, patientCardId, itemName, itemNetPrice/100, itemVAT, itemSumPrice/100, itemCount, discountValue/100, voucher/100, (itemSumPrice*itemCount-discountValue)/100 AS totalSumPrice, shoppingCartItems.comment, shoppingCartItems.archive FROM shoppingCartItems JOIN patients ON shoppingCartItems.patientId = patients.patientId JOIN panels ON shoppingCartItems.panelId = panels.panelId WHERE shoppingCartItemId>0";
    }
    else
    {
        m_qsQuery = "SELECT shoppingCartItemId AS id, title, patients.name, itemName, itemNetPrice/100, itemVAT, itemSumPrice/100, itemCount, discountValue/100, voucher/100, (itemSumPrice*itemCount-discountValue)/100 AS totalSumPrice, shoppingCartItems.comment FROM shoppingCartItems JOIN patients ON shoppingCartItems.patientId = patients.patientId JOIN panels ON shoppingCartItems.panelId = panels.panelId WHERE shoppingCartItemId>0";
    }

    int nPanelId = cmbPanel->itemData( cmbPanel->currentIndex() ).toInt();
    if( nPanelId > -1 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "panels.panelId=%1" ).arg( nPanelId );
    }

    int nGuestId = cmbPanel->itemData( cmbGuest->currentIndex() ).toInt();
    if( nGuestId > -1 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.patientId=%1" ).arg( nGuestId );
    }

    cDlgCrud::refreshTable();
}

void cDlgShoppingCart::enableButtons()
{
    cTracer obTracer( "cDlgShoppingCart::enableButtons" );

    m_poBtnDelete->setEnabled( m_inSelectedCount > 0 && g_obUser.isInGroup( cAccessGroup::USER ) );
    pbPayment->setEnabled( m_inSelectedCount > 0 );
}

void cDlgShoppingCart::newClicked( bool ) {}
void cDlgShoppingCart::editClicked( bool ) {}

void cDlgShoppingCart::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to delete the selected items?"),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
    {
        cDBShoppingCart obDBShoppingCart;
        QStringList qslItemIds;

        for( int i=0; i<tbvCrud->selectionModel()->selectedRows().count(); i++ )
        {
            unsigned int uiShoppingCardId = tbvCrud->selectionModel()->selectedRows().at(i).data().toUInt();

            obDBShoppingCart.load( uiShoppingCardId );
            qslItemIds << QString::number( uiShoppingCardId );

            if( obDBShoppingCart.panelId() != 0 &&
                obDBShoppingCart.productId() == 0 &&
                obDBShoppingCart.patientCardId() == 0 &&
                !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("Deleting panel use is not allowed from shopping cart.\n"
                                         "Please return to the panel and press ESC to reject panel use.") );
                return;
            }
        }

        for( int i=0; i<qslItemIds.count(); i++ )
        {
            obDBShoppingCart.load( qslItemIds.at(i).toInt() );

            if( obDBShoppingCart.ledgerTypeId() == cDBLedger::LT_PC_SELL || obDBShoppingCart.ledgerTypeId() == cDBLedger::LT_PC_REFILL )
            {
                try
                {
                    cDBPatientCard      obDBPatientCard;
                    cDBPatientCardType  obDBPatientCardType;

                    obDBPatientCard.load( obDBShoppingCart.patientCardId() );
                    obDBPatientCardType.load( obDBPatientCard.patientCardTypeId() );

                    obDBPatientCard.save();

                    QStringList qslUnitIds = obDBShoppingCart.comment().split("#");

                    QString     qsQuery = QString( "SELECT * FROM patientcardunits WHERE "
                                                   "((prepared=1 AND active=1) OR active=0) AND "
                                                   "patientCardUnitId IN (%1) " )
                                          .arg( qslUnitIds.join(",") );
                    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

                    if( poQuery->size() > 0 )
                    {
                        QMessageBox::warning( this, tr("Warning"),
                                              tr("Some of the units of this patientcard has been used or is in use.\n"
                                                 "Deleting of this entry from shopping cart is not allowed.") );
                        return;
                    }

                    for( int i=0; i<qslUnitIds.count(); i++ )
                    {
                        cDBPatientcardUnit  obDBPatientcardUnit;

                        obDBPatientcardUnit.load( qslUnitIds.at(i).toInt() );
                        obDBPatientcardUnit.remove();
                    }
                    obDBPatientCard.synchronizeUnits();
                    obDBPatientCard.synchronizeTime();
                }
                catch( cSevException &e )
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
                }
            }
            else if( obDBShoppingCart.ledgerTypeId() == cDBLedger::LT_PROD_SELL )
            {
                // No need to Increase product count. Product count only decreased by sell.
            }

            obDBShoppingCart.remove();
        }
        refreshTable();
    }
}

void cDlgShoppingCart::on_pbPatientCard_clicked()
{
    emit signalSellPatientCard();
    refreshTable();
}

void cDlgShoppingCart::on_pbProduct_clicked()
{
    emit signalSellProduct();
    refreshTable();
}

void cDlgShoppingCart::on_pbPayment_clicked()
{
    try
    {
        QStringList qslIds;

        for( int i=0; i< tbvCrud->selectionModel()->selectedRows().count(); i++ )
        {
            qslIds << tbvCrud->selectionModel()->selectedRows().at(i).data().toString();
        }

        QString     qsQuery = QString("SELECT SUM(itemSumPrice-discountValue) AS shoppingCartSum, "
                                      "SUM(card) as card, "
                                      "SUM(cash) as cash, "
                                      "SUM(voucher) as voucher, "
                                      "SUM(itemVAT) as Vat, "
                                      "SUM(discountValue) as Discount "
                                      "FROM shoppingcartitems "
                                      "WHERE shoppingCartItemId IN (%1) ").arg(qslIds.join(QString(",")));
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        cDBShoppingCart obDBShoppingCart;

        obDBShoppingCart.createNew();

        if( poQuery->first() )
        {
            obDBShoppingCart.setItemSumPrice( poQuery->value( 0 ).toInt() );
            obDBShoppingCart.setCard( poQuery->value( 1 ).toInt() );
            obDBShoppingCart.setCash( poQuery->value( 2 ).toInt() );
            obDBShoppingCart.setVoucher( poQuery->value( 3 ).toInt() );
            obDBShoppingCart.setItemVAT( poQuery->value( 4 ).toInt() );
            obDBShoppingCart.setItemDiscount( poQuery->value( 5 ).toInt() );
        }

        cDlgCassaAction obDlgCassaAction( this, &obDBShoppingCart );
        obDlgCassaAction.payShoppingCart();

        if( obDlgCassaAction.exec() == QDialog::Accepted )
        {
            int             inPayType = 0;
            bool            bShoppingCart = false;
            unsigned int    uiCouponId = 0;

            obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

            for( int i=0; i<tbvCrud->selectionModel()->selectedRows().count(); i++ )
            {
                QString         qsComment = "";

                obDBShoppingCart.load( tbvCrud->selectionModel()->selectedRows().at(i).data().toUInt() );

                // Restructure the payment category based on selection
                if( inPayType == 1 )
                { // Payed with card
                    obDBShoppingCart.setCard( obDBShoppingCart.card() +
                                              obDBShoppingCart.cash() +
                                              obDBShoppingCart.voucher() );
                }
                else if( inPayType == 2 )
                {// Payed with cash
                    obDBShoppingCart.setCash( obDBShoppingCart.card() +
                                              obDBShoppingCart.cash() +
                                              obDBShoppingCart.voucher() );
                }
                else
                {// Payed with other
                    obDBShoppingCart.setVoucher( obDBShoppingCart.card() +
                                                 obDBShoppingCart.cash() +
                                                 obDBShoppingCart.voucher() );
                }

                if( obDBShoppingCart.panelId() > 0 &&
                    obDBShoppingCart.productId() == 0 &&
                    obDBShoppingCart.patientCardId() == 0)
                {
                    cDBPanel    obDBPanel;

                    obDBPanel.load( obDBShoppingCart.panelId() );
                    qsComment = tr("Using device: %1").arg(obDBPanel.title());

                    unsigned int uiLedgerId = g_obCassa.cassaProcessDeviceUse( obDBShoppingCart, qsComment, inPayType, obDBPanel.title() );

                    emit signalPaymentProcessed( obDBShoppingCart.panelId(), uiLedgerId, inPayType );
                }
                else if( obDBShoppingCart.productId() > 0 )
                {
                    qsComment = tr("Selling product: %1").arg(obDBShoppingCart.itemName());
                    g_obCassa.cassaProcessProductSell( obDBShoppingCart, qsComment, inPayType );
                }
                else if( obDBShoppingCart.patientCardId() > 0 )
                {
                    cDBPatientCard  obDBPatientCard;

                    obDBPatientCard.load( obDBShoppingCart.patientCardId() );
                    bool bNewCard = obDBShoppingCart.ledgerTypeId()==cDBLedger::LT_PC_SELL?true:false;
                    if( bNewCard )
                    {
                        qsComment = tr("Sell patientcard [%1]").arg(obDBPatientCard.barcode());
                    }
                    else
                    {
                        qsComment = tr("Refill patientcard [%1]").arg(obDBPatientCard.barcode());
                    }
                    g_obCassa.cassaProcessPatientCardSell( obDBPatientCard, obDBShoppingCart, qsComment, bNewCard, inPayType );
                }

                obDBShoppingCart.remove();
            }

            QDialog::accept();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
