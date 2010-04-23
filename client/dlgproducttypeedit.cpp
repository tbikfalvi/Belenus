#include <QPushButton>
#include <QMessageBox>

#include "dlgproducttypeedit.h"

cDlgProductTypeEdit::cDlgProductTypeEdit( QWidget *p_poParent, cDBProductType *p_poProductType ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poProductType = p_poProductType;
    if( m_poProductType )
    {
        ledName->setText( QString::fromStdString( m_poProductType->name() ) );
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgProductTypeEdit::~cDlgProductTypeEdit()
{
}

void cDlgProductTypeEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Producttype name cannot be empty." ) );
        ledName->setFocus();
        return;
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poProductType->setName( ledName->text().toStdString() );

            m_poProductType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
