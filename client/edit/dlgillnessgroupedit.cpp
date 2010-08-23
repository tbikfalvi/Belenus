#include <QPushButton>
#include <QMessageBox>

#include "dlgillnessgroupedit.h"

cDlgIllnessGroupEdit::cDlgIllnessGroupEdit( QWidget *p_poParent, cDBIllnessGroup *p_poIllnessGroup )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Illness group" ) );
    setWindowIcon( QIcon("./resources/40x40_illnessgroup.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );

    m_poIllnessGroup = p_poIllnessGroup;
    if( m_poIllnessGroup )
    {
        ledName->setText( m_poIllnessGroup->name() );
        if( m_poIllnessGroup->licenceId() == 0 && m_poIllnessGroup->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poIllnessGroup->licenceId() == 0 && m_poIllnessGroup->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgIllnessGroupEdit::~cDlgIllnessGroupEdit()
{
}

void cDlgIllnessGroupEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Illness group cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poIllnessGroup->setName( ledName->text() );
            if( checkIndependent->isChecked() )
            {
                m_poIllnessGroup->setLicenceId( 0 );
            }
            else
            {
                m_poIllnessGroup->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poIllnessGroup->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
