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

    pbPayment = new QPushButton( tr( "Payment" ), this );
    pbPayment->setObjectName( QString::fromUtf8( "pbPayment" ) );
    pbPayment->setIconSize( QSize(20, 20) );
    pbPayment->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtons->addButton( pbPayment, QDialogButtonBox::ActionRole );

    connect( pbPayment, SIGNAL(clicked(bool)), this, SLOT(on_pbPayment_clicked()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ShoppingCart", QPoint(800,400) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    m_poBtnNew->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnSave->setVisible(false);
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
        m_poModel->setHeaderData( 12, Qt::Horizontal, tr( "TotalSumPrice" ) );
        m_poModel->setHeaderData( 13, Qt::Horizontal, tr( "Archive" ) );

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
        m_poModel->setHeaderData( 9, Qt::Horizontal, tr( "TotalSumPrice" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );
        tbvCrud->resizeColumnToContents( 9 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgShoppingCart::refreshTable()
{
    cTracer obTracer( "cDlgShoppingCart::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT shoppingCartItemId, shoppingCartItems.licenceId, title, patients.name, productId, patientCardId, itemName, itemNetPrice, itemVAT, itemSumPrice, itemCount, discountValue, (itemSumPrice*itemCount-discountValue) AS totalSumPrice, shoppingCartItems.archive FROM shoppingCartItems JOIN patients ON shoppingCartItems.patientId = patients.patientId JOIN panels ON shoppingCartItems.panelId = panels.panelId";
    }
    else
    {
        m_qsQuery = "SELECT shoppingCartItemId AS id, title, patients.name, itemName, itemNetPrice, itemVAT, itemSumPrice, itemCount, discountValue, (itemSumPrice*itemCount-discountValue) AS totalSumPrice FROM shoppingCartItems JOIN patients ON shoppingCartItems.patientId = patients.patientId JOIN panels ON shoppingCartItems.panelId = panels.panelId";
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

    m_poBtnDelete->setEnabled( m_inSelectedCount > 0 );
    pbPayment->setEnabled( m_inSelectedCount > 0 );
}

void cDlgShoppingCart::newClicked( bool ) {}
void cDlgShoppingCart::editClicked( bool ) {}

void cDlgShoppingCart::deleteClicked( bool )
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
        else if( obDBShoppingCart.patientCardId() != 0 )
        {
            QString     qsQuery = QString("SELECT * FROM ledger WHERE patientCardId=%1 order by ledgerId DESC").arg(obDBShoppingCart.patientCardId());
            QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

            if( poQuery->first() )
            {
                try
                {
                    cDBPatientCardType  obDBPatientCardType;
                    cDBPatientCard      obDBPatientCard;

                    obDBPatientCardType.load( poQuery->value( 7 ).toUInt() );
                    obDBPatientCard.load( poQuery->value( 8 ).toUInt() );

                    obDBPatientCard.setUnits( obDBPatientCard.units() - obDBPatientCardType.units() );
                    obDBPatientCard.setTimeLeft( obDBPatientCard.timeLeft() - obDBPatientCardType.units() * obDBPatientCardType.unitTime() * 60 );
                    obDBPatientCard.save();

                    cDBLedger obDBLedger;

                    obDBLedger.load( poQuery->value( 0 ).toUInt() );
                    obDBLedger.revoke();
                }
                catch( cSevException &e )
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
                }
            }
        }
    }

    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to delete the selected items?"),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
    {
        for( int i=0; i<qslItemIds.count(); i++ )
        {
            obDBShoppingCart.load( qslItemIds.at(i).toInt() );
            obDBShoppingCart.remove();
        }
        refreshTable();
    }
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

        QString     qsQuery = QString("SELECT SUM(itemSumPrice) AS shoppingCartSum FROM shoppingcartitems WHERE shoppingCartItemId IN (%1) ").arg(qslIds.join(QString(",")));
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        cDBShoppingCart obDBShoppingCart;

        obDBShoppingCart.createNew();

        if( poQuery->first() )
        {
            obDBShoppingCart.setItemSumPrice( poQuery->value( 0 ).toInt() );
        }

        cDlgCassaAction obDlgCassaAction( this, &obDBShoppingCart );
        obDlgCassaAction.actionPayment();
        if( obDlgCassaAction.exec() == QDialog::Accepted )
        {
            for( int i=0; i< tbvCrud->selectionModel()->selectedRows().count(); i++ )
            {
                obDBShoppingCart.load( tbvCrud->selectionModel()->selectedRows().at(i).data().toUInt() );

                int     inPayType = 0;
                QString qsComment = "";
                bool    bShoppingCart = false;

                if( obDBShoppingCart.panelId() > 0 &&
                    obDBShoppingCart.productId() == 0 &&
                    obDBShoppingCart.patientCardId() == 0)
                {
                    cDBPanel    obDBPanel;

                    obDBPanel.load( obDBShoppingCart.panelId() );
                    qsComment = tr("Using device: %1").arg(obDBPanel.title());
                }
                else if( obDBShoppingCart.productId() > 0 )
                {
                    qsComment = tr("Selling product: %1").arg(obDBShoppingCart.itemName());
                }

                obDlgCassaAction.cassaResult( &inPayType, &qsComment, &bShoppingCart );

                if( inPayType == cDlgCassaAction::PAY_CASH )
                {
                    g_obCassa.cassaAddMoneyAction( obDBShoppingCart.itemSumPrice(), qsComment );
                }

                if( obDBShoppingCart.panelId() > 0 &&
                    obDBShoppingCart.productId() == 0 &&
                    obDBShoppingCart.patientCardId() == 0)
                {
                    emit signalPaymentProcessed( obDBShoppingCart, inPayType, qsComment );
                }
                else if( obDBShoppingCart.productId() > 0 )
                {
                    cDBProduct  obDBProduct;

                    obDBProduct.load( obDBShoppingCart.productId() );
                    obDBProduct.decreaseProductCount( obDBShoppingCart.itemCount() );
                    obDBProduct.save();
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
