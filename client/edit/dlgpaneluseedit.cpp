#include <QPushButton>
#include <QMessageBox>

#include "dlgpaneluseedit.h"
#include "belenus.h"

cDlgPanelUseEdit::cDlgPanelUseEdit( QWidget *p_poParent, cDBPanelUses *p_poPanelUses, unsigned int p_inPanelId )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelUseEdit::cDlgPanelUseEdit" );

    setupUi( this );

    setWindowTitle( tr( "Device uses" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    lblCurrency->setText( g_poPrefs->getCurrencyShort() );

    m_poPanelUses   = p_poPanelUses;
    m_inPanelId     = p_inPanelId;

    if( m_poPanelUses )
    {
        ledUseName->setText( m_poPanelUses->name() );
        ledUseTime->setText( QString::number(m_poPanelUses->useTime()) );

        cCurrency   cPrice( m_poPanelUses->usePrice() );
        ledUsePrice->setText( cPrice.currencyString() );
    }
    on_ledUsePrice_textEdited("");
}

cDlgPanelUseEdit::~cDlgPanelUseEdit()
{
}

void cDlgPanelUseEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    bool  bIsNumber = false;

    if( ledUseName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name can not be empty." ), QMessageBox::Ok );
    }

    ledUseTime->text().toInt( &bIsNumber );
    if( ledUseTime->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use time can not be empty." ), QMessageBox::Ok );
    }
    else if( !bIsNumber )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use time value is invalid." ), QMessageBox::Ok );
    }
    else if( ledUseTime->text().toInt() < 1 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use time must be greater than zero." ), QMessageBox::Ok );
    }

    cCurrency   cPrice( ledUsePrice->text() );

    cPrice.currencyValue().toInt( &bIsNumber );
    if( ledUsePrice->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use price can not be empty." ), QMessageBox::Ok );
    }
    else if( !bIsNumber )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use price value is invalid." ), QMessageBox::Ok );
    }
    else if( cPrice.currencyValue().toInt() <= 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Price of usage must be greater than zero." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanelUses->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanelUses->setPanelId( m_inPanelId );
        m_poPanelUses->setName( ledUseName->text() );
        m_poPanelUses->setUseTime( ledUseTime->text().toInt() );
        m_poPanelUses->setUsePrice( cPrice.currencyValue().toInt() );
        m_poPanelUses->save();
        QDialog::accept();
    }
}

void cDlgPanelUseEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}


void cDlgPanelUseEdit::on_ledUsePrice_textEdited(const QString &arg1)
{
    cCurrency currPrice( ledUsePrice->text(), cCurrency::CURR_GROSS, g_poPrefs->getDeviceUseVAT() );

    lblPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(g_poPrefs->getDeviceUseVAT()) );
}
