#include <QPushButton>
#include <QMessageBox>

#include "dlgpaymentmethodedit.h"

cDlgPaymentMethodEdit::cDlgPaymentMethodEdit( QWidget *p_poParent, cDBPaymentMethod *p_poPaymentMethod )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Payment methods" ) );
    setWindowIcon( QIcon("./resources/40x40_paymentmethod.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPaymentMethod = p_poPaymentMethod;
    if( m_poPaymentMethod )
    {
        ledName->setText( m_poPaymentMethod->name() );
//        if( m_poPaymentMethod->licenceId() == 0 && m_poPaymentMethod->id() > 0 )
//            checkIndependent->setChecked( true );

//        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
//        {
//            checkIndependent->setEnabled( false );
//            if( m_poPaymentMethod->licenceId() == 0 && m_poPaymentMethod->id() > 0 )
//            {
//                ledName->setEnabled( false );
//                poBtnSave->setEnabled( false );
//            }
//        }
//        if( m_poPaymentMethod->id() > 0 )
//            checkIndependent->setEnabled( false );
    }
}

cDlgPaymentMethodEdit::~cDlgPaymentMethodEdit()
{
}

void cDlgPaymentMethodEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Payment method cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPaymentMethod->setName( ledName->text() );
//            if( checkIndependent->isChecked() )
//            {
//                m_poPaymentMethod->setLicenceId( 0 );
//            }
//            else
//            {
//                m_poPaymentMethod->setLicenceId( g_poPrefs->getLicenceId() );
//            }
            m_poPaymentMethod->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}
