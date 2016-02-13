
#include <QPushButton>
#include <QMessageBox>

#include "dlgstockprocess.h"
#include "dlgproductstorage.h"
#include "../db/dbproduct.h"

dlgStockProcess::dlgStockProcess( QWidget *p_poParent, QString p_qsTitle ) : QDialog( p_poParent )
{
    m_bComboFillInProgress = false;

    setupUi( this );

    setWindowTitle( p_qsTitle );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    pbStockIncrease->setIcon( QIcon("./resources/40x40_stock_add.png") );
    pbStockDecrease->setIcon( QIcon("./resources/40x40_stock_remove.png") );

    on_rbBarcode_clicked();

    cmbProduct->clear();
    cmbProduct->setEnabled( false );
    pbStockIncrease->setEnabled( false );
    pbStockDecrease->setEnabled( false );
}

dlgStockProcess::~dlgStockProcess()
{
}

void dlgStockProcess::on_pbExit_clicked()
{
    QDialog::accept();
}

void dlgStockProcess::on_rbBarcode_clicked()
{
    cTracer obTracer( "dlgStockProcess::on_rbBarcode_clicked" );

    ledBarcode->setEnabled( true );
    ledName->setEnabled( false );
    ledBarcode->setFocus();
    ledBarcode->selectAll();
}

void dlgStockProcess::on_rbName_clicked()
{
    cTracer obTracer( "dlgStockProcess::on_rbName_clicked" );

    ledBarcode->setEnabled( false );
    ledName->setEnabled( true );
    ledName->setFocus();
    ledName->selectAll();
}

void dlgStockProcess::on_ledBarcode_editingFinished()
{
    cTracer obTracer( "dlgStockProcess::on_ledBarcode_editingFinished" );

    if( ledBarcode->text().length() == 0 )
        return;

    m_qsFilter = QString( "WHERE barcode LIKE '\%%1\%'" ).arg( ledBarcode->text() );
    _fillProductCombo();
}

void dlgStockProcess::on_ledName_editingFinished()
{
    cTracer obTracer( "dlgStockProcess::on_ledName_editingFinished" );

    if( ledName->text().length() == 0 )
        return;

    m_qsFilter = QString( "WHERE name LIKE '\%%1\%'" ).arg( ledName->text() );
    _fillProductCombo();
}

void dlgStockProcess::_fillProductCombo()
{
    if( m_bComboFillInProgress )
        return;

    cTracer obTracer( "dlgStockProcess::_fillProductCombo" );

    m_bComboFillInProgress = true;

    QString      qsQuery = QString( "SELECT * FROM products %1 AND active=1" ).arg( m_qsFilter );
    QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

    cmbProduct->clear();
    cmbProduct->setEnabled( true );

    if( poQuery->size() < 1 )
    {
        cmbProduct->addItem( tr("<Product not found>"), 0 );
        cmbProduct->setEnabled( false );
        pbStockIncrease->setEnabled( false );
        pbStockDecrease->setEnabled( false );
    }
    else
    {
        pbStockIncrease->setEnabled( true );
        pbStockDecrease->setEnabled( true );
        if( poQuery->size() > 1 )
        {
            cmbProduct->setFocus();
            cmbProduct->addItem( tr("<More than one product found>"), 0 );
        }
        else
        {
            pbStockIncrease->setFocus();
        }
        while( poQuery->next() )
        {
            cmbProduct->addItem( poQuery->value( 2 ).toString(), poQuery->value( 0 ) );
        }
    }
    m_bComboFillInProgress = false;
}

void dlgStockProcess::on_pbStockIncrease_clicked()
{
    cTracer obTracer( "dlgStockProcess::on_pbStockIncrease_clicked" );

    int uiProductId = cmbProduct->itemData( cmbProduct->currentIndex() ).toInt();

    if( uiProductId < 1 )
    {
        cmbProduct->setFocus();
        return;
    }

    try
    {
        cDBProduct m_poProduct;

        m_poProduct.load( uiProductId );

        dlgProductStorage   *obDlgProductStorage = new dlgProductStorage( this, &m_poProduct );

        obDlgProductStorage->exec();
        _restartProcess();
        delete obDlgProductStorage;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void dlgStockProcess::on_pbStockDecrease_clicked()
{
    cTracer obTracer( "dlgStockProcess::on_pbStockDecrease_clicked" );

    int uiProductId = cmbProduct->itemData( cmbProduct->currentIndex() ).toInt();

    if( uiProductId < 1 )
    {
        cmbProduct->setFocus();
        return;
    }

    try
    {
        cDBProduct m_poProduct;

        m_poProduct.load( uiProductId );

        if( m_poProduct.productCount() < 1 )
            return;

        dlgProductStorage   *obDlgProductStorage = new dlgProductStorage( this, &m_poProduct, false );

        obDlgProductStorage->exec();
        _restartProcess();
        delete obDlgProductStorage;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void dlgStockProcess::_restartProcess()
{
    cmbProduct->clear();
    cmbProduct->setEnabled( false );

    if( rbBarcode->isChecked() )
    {
        on_rbBarcode_clicked();
    }
    else
    {
        on_rbName_clicked();
    }
}
