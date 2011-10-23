#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgproductedit.h"

cDlgProductEdit::cDlgProductEdit( QWidget *p_poParent, cDBProduct *p_poProduct )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Product" ) );
    setWindowIcon( QIcon("./resources/40x40_product.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    m_poProduct = p_poProduct;

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
}

cDlgProductEdit::~cDlgProductEdit()
{
}

void cDlgProductEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of product must be set." ) );
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
            m_poProduct->setName( ledName->text() );
            m_poProduct->setNetPrice( ledPrice->text().toUInt() );
            m_poProduct->setVatPercent( ledVatpercent->text().toInt() );
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
