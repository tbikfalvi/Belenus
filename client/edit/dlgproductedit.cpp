#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgproductedit.h"
#include "../db/dbproducttype.h"

cDlgProductEdit::cDlgProductEdit( QWidget *p_poParent, cDBProduct *p_poProduct )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Product" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    m_poProduct = p_poProduct;

    QStringList qslProductTypes;

    if( m_poProduct->id() > 0 )
    {
        qslProductTypes = m_poProduct->productTypes();
    }

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT productTypeId, name FROM productTypes WHERE active=1 AND archive<>\"DEL\"" ) );
    while( poQuery->next() )
    {
        if( qslProductTypes.contains( poQuery->value( 0 ).toString() ) )
        {
            listProductsAssigned->addItem( poQuery->value( 1 ).toString() );
        }
        else
        {
            listProductsIndependent->addItem( poQuery->value( 1 ).toString() );
        }
    }
    if( poQuery ) delete poQuery;

    if( m_poProduct )
    {
        ledName->setText( m_poProduct->name() );
        ledPrice->setText( QString::number(m_poProduct->netPrice()) );
        ledVatpercent->setText( QString::number(m_poProduct->vatPercent()) );

        if( m_poProduct->licenceId() == 0 && m_poProduct->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poProduct->licenceId() == 0 && m_poProduct->id() > 0 )
            {
                ledName->setEnabled( false );
                ledPrice->setEnabled( false );
                ledVatpercent->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poProduct->id() > 0 )
            checkIndependent->setEnabled( false );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditProduct", QPoint(600,360) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgProductEdit::~cDlgProductEdit()
{
    g_poPrefs->setDialogSize( "EditProduct", QPoint( width(), height() ) );
}

void cDlgProductEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of product must be set." ) );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM products WHERE productId<>%1 AND name=\"%2\" AND active=1 AND archive<>\"DEL\"" ).arg(m_poProduct->id()).arg(ledName->text()) );
        if( poQuery->first() )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Product with this name already exists.\nPlease set another one." ) );
        }
        if( poQuery ) delete poQuery;
    }
    if( ledPrice->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Price of product must be set." ) );
    }
    if( ledVatpercent->text() == "" )
        ledVatpercent->setText( "0" );

    if( boCanBeSaved )
    {
        try
        {
            QStringList     qslProductTypes;
            for( int i=0; i<listProductsAssigned->count(); i++ )
            {
                cDBProductType  poProductType;

                poProductType.load( listProductsAssigned->item(i)->text() );
                qslProductTypes.append( QString("%1").arg(poProductType.id()) );
            }

            m_poProduct->setName( ledName->text() );
            m_poProduct->setNetPrice( ledPrice->text().toUInt() );
            m_poProduct->setVatPercent( ledVatpercent->text().toInt() );
            m_poProduct->setProductTypes( qslProductTypes );
            m_poProduct->setActive( true );

            if( checkIndependent->isChecked() )
            {
                m_poProduct->setLicenceId( 0 );
            }
            else
            {
                m_poProduct->setLicenceId( g_poPrefs->getLicenceId() );
            }

            m_poProduct->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgProductEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgProductEdit::on_pbProductAdd_clicked()
{
    for( int i=listProductsIndependent->count()-1; i>-1; i-- )
    {
        if( listProductsIndependent->item(i)->isSelected() )
        {
            listProductsAssigned->addItem( listProductsIndependent->takeItem(i) );
        }
    }
}

void cDlgProductEdit::on_pbProductAddAll_clicked()
{
    for( int i=listProductsIndependent->count()-1; i>-1; i-- )
    {
        listProductsAssigned->addItem( listProductsIndependent->takeItem(i) );
    }
}

void cDlgProductEdit::on_pbProductRemove_clicked()
{
    for( int i=listProductsAssigned->count()-1; i>-1; i-- )
    {
        if( listProductsAssigned->item(i)->isSelected() )
        {
            listProductsIndependent->addItem( listProductsAssigned->takeItem(i) );
        }
    }
}

void cDlgProductEdit::on_pbProductRemoveAll_clicked()
{
    for( int i=listProductsAssigned->count()-1; i>-1; i-- )
    {
        listProductsIndependent->addItem( listProductsAssigned->takeItem(i) );
    }
}
