#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientcardtypeedit.h"

cDlgPatientCardTypeEdit::cDlgPatientCardTypeEdit( QWidget *p_poParent, cDBPatientCardType *p_poPatientCardType )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCardType = p_poPatientCardType;
/*
    if( m_poPatientCardType )
    {
        ledName->setText( QString::fromStdString( m_poPatientCardType->name() ) );

        if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
*/
}

cDlgPatientCardTypeEdit::~cDlgPatientCardTypeEdit()
{
}

void cDlgPatientCardTypeEdit::accept ()
{
    bool  boCanBeSaved = true;
    /*if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Patientcard type cannot be empty." ) );
    }*/

    if( boCanBeSaved )
    {
        try
        {
/*
            m_poPatientCardType->setName( ledName->text().toStdString() );

            if( checkIndependent->isChecked() )
            {
                m_poPatientCardType->setLicenceId( 0 );
            }
            else
            {
                m_poPatientCardType->setLicenceId( g_poPrefs->getLicenceId() );
            }
*/
            m_poPatientCardType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
