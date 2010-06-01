#include <QPushButton>
#include <QMessageBox>

#include "dlgpaneltypeedit.h"
#include "belenus.h"

cDlgPanelTypeEdit::cDlgPanelTypeEdit( QWidget *p_poParent, cDBPanelTypes *p_poPanelTypes )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelTypeEdit::cDlgPanelTypeEdit" );

    setupUi( this );

    m_poPanelTypes = p_poPanelTypes;

    if( m_poPanelTypes )
    {
        ledNameVal->setText( QString::fromStdString(m_poPanelTypes->name()) );
    }
}

cDlgPanelTypeEdit::~cDlgPanelTypeEdit()
{
}

void cDlgPanelTypeEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledNameVal->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of Panel Type can not be empty." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanelTypes->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanelTypes->setName( ledNameVal->text().toStdString() );
        m_poPanelTypes->setActive( true );
        m_poPanelTypes->save();
        QDialog::accept();
    }
}

void cDlgPanelTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

