#include <QPushButton>
#include <QMessageBox>

#include "dlgproductedit.h"

cDlgProductEdit::cDlgProductEdit( QWidget *p_poParent, cDBProduct *p_poProduct ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poProduct = p_poProduct;
    if( m_poProduct )
    {
        ledName->setText( QString::fromStdString( m_poProduct->name() ) );
        ledBarcode->setText( QString::fromStdString( m_poProduct->barcode() ) );
        ledPriceIn->setText( QString::number( m_poProduct->priceIn() ) );
        ledPriceOut->setText( QString::number( m_poProduct->priceOut() ) );
        ledCount->setText( QString::number( m_poProduct->count() ) );
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgProductEdit::~cDlgProductEdit()
{
}

void cDlgProductEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Product name cannot be empty." ) );
        ledName->setFocus();
        return;
    }
    if( ledBarcode->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Barcode cannot be empty." ) );
        ledBarcode->setFocus();
        return;
    }
    if( ledPriceIn->text() == "" )
        ledPriceIn->text() = "0";
    if( ledPriceOut->text() == "" )
        ledPriceOut->text() = "0";
    if( ledCount->text() == "" )
        ledCount->text() = "0";

    if( boCanBeSaved )
    {
        try
        {
            m_poProduct->setName( ledName->text().toStdString() );
            m_poProduct->setBarcode( ledBarcode->text().toStdString() );
            m_poProduct->setPriceIn( ledPriceIn->text().toInt() );
            m_poProduct->setPriceOut( ledPriceOut->text().toInt() );
            m_poProduct->setCount( ledCount->text().toInt() );

            m_poProduct ->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}

