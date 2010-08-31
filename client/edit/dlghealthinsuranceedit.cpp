#include <QPushButton>
#include <QMessageBox>

#include "dlghealthinsuranceedit.h"

cDlgHealthInsuranceEdit::cDlgHealthInsuranceEdit( QWidget *p_poParent, cDBHealthInsurance *p_poHealthInsurance )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Health Insurance Fund" ) );
    setWindowIcon( QIcon("./resources/40x40_healthinsurance.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poHealthInsurance = p_poHealthInsurance;
    if( m_poHealthInsurance )
    {
        ledName->setText( m_poHealthInsurance->name() );
        if( m_poHealthInsurance->licenceId() == 0 && m_poHealthInsurance->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poHealthInsurance->licenceId() == 0 && m_poHealthInsurance->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgHealthInsuranceEdit::~cDlgHealthInsuranceEdit()
{
}

void cDlgHealthInsuranceEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Health insurance fund name cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poHealthInsurance->setLicenceId( 0 );
            }
            else
            {
                m_poHealthInsurance->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poHealthInsurance->setName( ledName->text() );
            m_poHealthInsurance->setActive( true );
            m_poHealthInsurance->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}
