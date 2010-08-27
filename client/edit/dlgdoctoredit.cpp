#include <QPushButton>
#include <QMessageBox>

#include "dlgdoctoredit.h"

cDlgDoctorEdit::cDlgDoctorEdit( QWidget *p_poParent, cDBDoctor *p_poDoctor )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Doctor" ) );
    setWindowIcon( QIcon("./resources/40x40_doctor.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );

    m_poDoctor = p_poDoctor;
    if( m_poDoctor )
    {
        ledName->setText( m_poDoctor->name() );
        ledLicence->setText( m_poDoctor->licence() );
        ptData->setPlainText( m_poDoctor->data() );
        if( m_poDoctor->licenceId() == 0 && m_poDoctor->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poDoctor->licenceId() == 0 && m_poDoctor->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgDoctorEdit::~cDlgDoctorEdit()
{
}

void cDlgDoctorEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Doctor name cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poDoctor->setLicenceId( 0 );
            }
            else
            {
                m_poDoctor->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poDoctor->setName( ledName->text() );
            m_poDoctor->setLicence( ledLicence->text() );
            m_poDoctor->setData( ptData->toPlainText() );
            m_poDoctor->setActive( true );
            m_poDoctor->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
