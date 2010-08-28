#include <QPushButton>
#include <QMessageBox>

#include "dlgcompanyedit.h"

cDlgCompanyEdit::cDlgCompanyEdit( QWidget *p_poParent, cDBCompany *p_poCompany )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Company" ) );
    setWindowIcon( QIcon("./resources/40x40_company.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poCompany = p_poCompany;
    if( m_poCompany )
    {
        ledName->setText( m_poCompany->name() );
        if( m_poCompany->licenceId() == 0 && m_poCompany->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poCompany->licenceId() == 0 && m_poCompany->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgCompanyEdit::~cDlgCompanyEdit()
{
}

void cDlgCompanyEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Company name cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poCompany->setLicenceId( 0 );
            }
            else
            {
                m_poCompany->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poCompany->setName( ledName->text() );
            m_poCompany->setActive( true );
            m_poCompany->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
