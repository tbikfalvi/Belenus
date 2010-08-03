#include <QPushButton>
#include <QMessageBox>

#include "dlgpaneluseedit.h"
#include "belenus.h"

cDlgPanelUseEdit::cDlgPanelUseEdit( QWidget *p_poParent, cDBPanelUses *p_poPanelUses, unsigned int p_inPanelId )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelUseEdit::cDlgPanelUseEdit" );

    setupUi( this );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPanelUses   = p_poPanelUses;
    m_inPanelId     = p_inPanelId;

    if( m_poPanelUses )
    {
        ledUseTime->setText( QString::number(m_poPanelUses->useTime()) );
        ledUsePrice->setText( QString::number(m_poPanelUses->usePrice()) );
    }
}

cDlgPanelUseEdit::~cDlgPanelUseEdit()
{
}

void cDlgPanelUseEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    bool  bIsNumber = false;

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

    ledUsePrice->text().toInt( &bIsNumber );
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
    else if( ledUsePrice->text().toInt() < 1 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Use price must be greater than zero." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanelUses->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanelUses->setPanelId( m_inPanelId );
        m_poPanelUses->setUseTime( ledUseTime->text().toInt() );
        m_poPanelUses->setUsePrice( ledUsePrice->text().toInt() );
        m_poPanelUses->save();
        QDialog::accept();
    }
}

void cDlgPanelUseEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

