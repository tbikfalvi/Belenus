#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientoriginedit.h"

cDlgPatientOriginEdit::cDlgPatientOriginEdit( QWidget *p_poParent, cDBPatientOrigin *p_poPatientOrigin )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Patient origin" ) );
    setWindowIcon( QIcon("./resources/40x40_patientorigin.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPatientOrigin = p_poPatientOrigin;
    if( m_poPatientOrigin )
    {
        ledName->setText( m_poPatientOrigin->name() );
        if( m_poPatientOrigin->licenceId() == 0 && m_poPatientOrigin->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientOrigin->licenceId() == 0 && m_poPatientOrigin->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgPatientOriginEdit::~cDlgPatientOriginEdit()
{
}

void cDlgPatientOriginEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Patient origin cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poPatientOrigin->setLicenceId( 0 );
            }
            else
            {
                m_poPatientOrigin->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poPatientOrigin->setName( ledName->text() );
            m_poPatientOrigin->setActive( true );
            m_poPatientOrigin->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
