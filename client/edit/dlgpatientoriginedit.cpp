#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientoriginedit.h"

cDlgPatientOriginEdit::cDlgPatientOriginEdit( QWidget *p_poParent, cDBPatientOrigin *p_poPatientOrigin )
    : QDialog( p_poParent )
{
    setupUi( this );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );

    m_poPatientOrigin = p_poPatientOrigin;
    if( m_poPatientOrigin )
    {
        ledName->setText( QString::fromStdString( m_poPatientOrigin->name() ) );
        if( m_poPatientOrigin->licenceId() == 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientOrigin->licenceId() == 0 )
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
            m_poPatientOrigin->setName( ledName->text().toStdString() );
            if( checkIndependent->isChecked() )
            {
                m_poPatientOrigin->setLicenceId( 0 );
            }
            else
            {
                m_poPatientOrigin->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poPatientOrigin->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}