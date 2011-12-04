#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgproducttypeedit.h"
#include "../db/dbproduct.h"


cDlgProductTypeEdit::cDlgProductTypeEdit( QWidget *p_poParent, cDBProductType *p_poProductType )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Product type" ) );
    setWindowIcon( QIcon("./resources/40x40_producttype.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    m_poProductType = p_poProductType;

    QStringList qslProducts;

    if( m_poProductType->id() > 0 )
    {
        qslProducts = m_poProductType->products();
    }

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT productId, name FROM products WHERE active=1 AND archive<>\"DEL\"" ) );
    while( poQuery->next() )
    {
        if( qslProducts.contains( poQuery->value( 0 ).toString() ) )
        {
            listProductsAssigned->addItem( poQuery->value( 1 ).toString() );
        }
        else
        {
            listProductsIndependent->addItem( poQuery->value( 1 ).toString() );
        }
    }
    if( poQuery ) delete poQuery;

    if( m_poProductType )
    {
        ledName->setText( m_poProductType->name() );

        if( m_poProductType->licenceId() == 0 && m_poProductType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poProductType->licenceId() == 0 && m_poProductType->id() > 0 )
            {
                ledName->setEnabled( false );
                listProductsAssigned->setEnabled( false );
                listProductsIndependent->setEnabled( false );
                pbProductAdd->setEnabled( false );
                pbProductAddAll->setEnabled( false );
                pbProductRemove->setEnabled( false );
                pbProductRemoveAll->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poProductType->id() > 0 )
            checkIndependent->setEnabled( false );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditProductType", QPoint(600,360) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgProductTypeEdit::~cDlgProductTypeEdit()
{
    g_poPrefs->setDialogSize( "EditProductType", QPoint( width(), height() ) );
}

void cDlgProductTypeEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of product type must be set." ) );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productTypes WHERE productTypeId<>%1 AND name=\"%2\" AND active=1 AND archive<>\"DEL\"" ).arg(m_poProductType->id()).arg(ledName->text()) );
        if( poQuery->first() )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Product type with this name already exists.\nPlease set another one." ) );
        }
        if( poQuery ) delete poQuery;
    }

    if( boCanBeSaved )
    {
        try
        {
            QStringList     qslProducts;
            for( int i=0; i<listProductsAssigned->count(); i++ )
            {
                cDBProduct  poProduct;

                poProduct.load( listProductsAssigned->item(i)->text() );
                qslProducts.append( QString("%1").arg(poProduct.id()) );
            }

            m_poProductType->setName( ledName->text() );
            m_poProductType->setProducts( qslProducts );
            m_poProductType->setActive( true );

            if( checkIndependent->isChecked() )
            {
                m_poProductType->setLicenceId( 0 );
            }
            else
            {
                m_poProductType->setLicenceId( g_poPrefs->getLicenceId() );
            }

            m_poProductType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgProductTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgProductTypeEdit::on_pbProductAdd_clicked()
{
    for( int i=listProductsIndependent->count()-1; i>-1; i-- )
    {
        if( listProductsIndependent->item(i)->isSelected() )
        {
            listProductsAssigned->addItem( listProductsIndependent->takeItem(i) );
        }
    }
}

void cDlgProductTypeEdit::on_pbProductAddAll_clicked()
{
    for( int i=listProductsIndependent->count()-1; i>-1; i-- )
    {
        listProductsAssigned->addItem( listProductsIndependent->takeItem(i) );
    }
}

void cDlgProductTypeEdit::on_pbProductRemove_clicked()
{
    for( int i=listProductsAssigned->count()-1; i>-1; i-- )
    {
        if( listProductsAssigned->item(i)->isSelected() )
        {
            listProductsIndependent->addItem( listProductsAssigned->takeItem(i) );
        }
    }
}

void cDlgProductTypeEdit::on_pbProductRemoveAll_clicked()
{
    for( int i=listProductsAssigned->count()-1; i>-1; i-- )
    {
        listProductsIndependent->addItem( listProductsAssigned->takeItem(i) );
    }
}

