
#include <QMessageBox>

#include "dlgproductstorage.h"
#include "ui_dlgproductstorage.h"
#include "../db/dbproductactiontype.h"
#include "../db/dbcassahistory.h"
#include "../db/dbproducthistory.h"
#include "crud/dlgproductactiontype.h"
#include "../db/dbledger.h"
#include "../db/dbshoppingcart.h"

dlgProductStorage::dlgProductStorage( QWidget *parent, cDBProduct *p_poProduct ) : QDialog(parent)
{
    setupUi( this );

    m_bInit = true;

    setWindowTitle( tr( "Product storage" ) );
    setWindowIcon( QIcon("./resources/40x40_storage.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbActionTypes->setIcon( QIcon("./resources/40x40_productactiontype.png") );

    pbSave->setToolTip( tr("Please check the cassa and action type again\n"
                           "and make sure the correct amount of price and\n"
                           "count of product selected before continue.\n\n"
                           "Please note: the value of SUM price will be\n"
                           "archived in cassa history.") );

    cmbCassa->addItem( tr("<General cassa>") );
    cmbCassa->addItem( tr("Daily, user related cassa") );
    cmbCassa->setToolTip( tr("Select appropriate cassa type for product action.\n\n"
                             "<General cassa> is related to the solarium studio\n"
                             "any action will not modify the balance of the actual cassa\n"
                             "<Daily, user related cassa> is the currently used cassa\n"
                             "any action will increase or decrease the balance of the cassa.") );

    _fillProductActionList();

    m_poProduct = p_poProduct;

    if( m_poProduct )
    {
        ledProductName->setText( m_poProduct->name() );
    }
    ledPrice->setText( "0" );
    ledVatPercent->setText( "0" );
    ledProductCount->setText( "0" );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ProductStorage", QPoint(400,310) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bInit = false;
}

dlgProductStorage::~dlgProductStorage()
{
    g_poPrefs->setDialogSize( "ProductStorage", QPoint( width(), height() ) );
}

void dlgProductStorage::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    unsigned int uiPATypeId = cmbProductAction->itemData( cmbProductAction->currentIndex() ).toUInt();

    cDBProductActionType    obDBProductActionType;
    obDBProductActionType.load( uiPATypeId );

    cCurrency   cPrice( ledPrice->text(), cCurrency::CURR_GROSS, ledVatPercent->text().toInt() );

    if( ledPrice->text().length() == 0 )
        ledPrice->setText( "0" );

    if( ledVatPercent->text().length() == 0 )
        ledVatPercent->setText( "0" );

    if( ledProductCount->text().length() == 0 )
        ledProductCount->setText( "0" );

    if( (obDBProductActionType.increaseProductCount() ||
         obDBProductActionType.decreaseProductCount()) &&
        ledProductCount->text().toInt() == 0 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("You have selected a product action that modifies the\n"
                                 "count of the selected product but the value has not defined.\n\n"
                                 "Please define a valid product count or select another product action.") );
        boCanBeSaved = false;
    }
    if( cPrice.currencyValue().toInt() > 0 && ledProductCount->text().toInt() == 0 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("You have defined price for the selected action\n"
                                 "but the count of product not defined.\n\n"
                                 "Please define a valid product count or reset the price.") );
        boCanBeSaved = false;
    }

    if( boCanBeSaved )
    {
        try
        {
            if( cPrice.currencyValue().toInt() > 0 )
            {
                int nPrice = cPrice.currencyValue().toInt();

                if( obDBProductActionType.cassaActionIndication().compare( tr("Negative") ) == 0 )
                {
                    nPrice *= (-1);
                }
                nPrice *= ledProductCount->text().toInt();

                cDBShoppingCart obDBShoppingCart;

                obDBShoppingCart.createNew();
                obDBShoppingCart.setProductId( m_poProduct->id() );
                obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PROD_SELL );
                obDBShoppingCart.setItemName( obDBProductActionType.name() );
                obDBShoppingCart.setItemCount( ledProductCount->text().toInt() );
                obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
                obDBShoppingCart.setItemVAT( ledVatPercent->text().toInt() );
                obDBShoppingCart.setItemSumPrice( nPrice );

                bool bGlobalCassa = ( cmbCassa->currentIndex()==0 ? true : false );

                g_obCassa.cassaProcessProductStorageChange( obDBShoppingCart, tr("Product name: %1").arg( m_poProduct->name() ), bGlobalCassa );
            }

            int nProductCount = m_poProduct->productCount();
            int nProductCountChange = 0;

            if( obDBProductActionType.increaseProductCount() )
                nProductCountChange += ledProductCount->text().toInt();
            else if( obDBProductActionType.decreaseProductCount() )
                nProductCountChange -= ledProductCount->text().toInt();

            nProductCount += nProductCountChange;

            m_poProduct->setProductCount( nProductCount );
            m_poProduct->save();

            cDBProductHistory   obDBProductHistory;
            obDBProductHistory.createNew();
            obDBProductHistory.setLicenceId( g_poPrefs->getLicenceId() );
            obDBProductHistory.setProductId( m_poProduct->id() );
            obDBProductHistory.setPATypeId( obDBProductActionType.id() );
            obDBProductHistory.setUserId( g_obUser.id() );
            obDBProductHistory.setItemCount( nProductCountChange );
            obDBProductHistory.setNetPrice( cPrice.currencyValue().toInt() );
            obDBProductHistory.setVatPercent( ledVatPercent->text().toInt() );
            obDBProductHistory.save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        QDialog::accept();
    }
}

void dlgProductStorage::on_pbCancel_clicked()
{
    QDialog::reject();
}

void dlgProductStorage::on_cmbProductAction_currentIndexChanged(int index)
{
    if( !m_bInit )
        cmbProductAction->setToolTip( m_qslActionTooltip.at(index) );
}

void dlgProductStorage::on_pbActionTypes_clicked()
{
    cDlgProductActionType obDlgProductActionType( this );

    obDlgProductActionType.exec();
    _fillProductActionList();
}

void dlgProductStorage::_fillProductActionList()
{
    m_bInit = true;

    cmbProductAction->clear();
    m_qslActionTooltip.clear();

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productActionType WHERE active=1" ) );
    while( poQuery->next() )
    {
        cmbProductAction->addItem( poQuery->value(2).toString(), poQuery->value(0).toInt() );

        QString qsChangeCount = tr("not modify");
        if( poQuery->value(3).toInt() )
        {
            qsChangeCount = tr("increase");
        }
        else if( poQuery->value(4).toInt() )
        {
            qsChangeCount = tr("decrease");
        }
        QString qsToolTip = tr("The action <%1> will %2\n"
                               "the count of the selected product.\n"
                               "Based on the selected cassa type, the given price will be\n"
                               "added to actual cassa or to the general ledger.").arg(poQuery->value(2).toString()).arg(qsChangeCount);

        m_qslActionTooltip << qsToolTip;
    }

    if( cmbProductAction->count() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no product action type in the database.\n"
                                 "Without any action type modification of product count is not possible.\n"
                                 "Please create at least one product action type.") );
        pbSave->setEnabled( false );
        cmbProductAction->addItem( tr("<Product action type not found>"), 0 );
    }
    else
    {
        pbSave->setEnabled( true );
        on_cmbProductAction_currentIndexChanged( 0 );
    }

    m_bInit = false;
}

void dlgProductStorage::on_ledPrice_textEdited(const QString &arg1)
{
    slot_refreshPrice();
}

void dlgProductStorage::on_ledVatPercent_textEdited(const QString &arg1)
{
    slot_refreshPrice();
}

void dlgProductStorage::slot_refreshPrice()
{
    cCurrency currPrice( ledPrice->text(), cCurrency::CURR_GROSS, ledVatPercent->text().toInt() );

    lblPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledVatPercent->text()) );
}

