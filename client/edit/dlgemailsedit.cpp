#include <QPushButton>
#include <QMessageBox>

#include "dlgemailsedit.h"

cDlgEmailsEdit::cDlgEmailsEdit( QWidget *p_poParent, cDBSendMail *p_poEmails ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Email" ) );
    setWindowIcon( QIcon("./resources/40x40_draftmail.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poEmails = p_poEmails;
    if( m_poEmails )
    {
        ledRecipients->setText( m_poEmails->recipients() );
    }
}

cDlgEmailsEdit::~cDlgEmailsEdit()
{
}

void cDlgEmailsEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( ledRecipients->text().length() == 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Email recipient field cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
//            m_poEmails->setRecipients( ledName->text() );
//            m_poEmails->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

