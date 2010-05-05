#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientoriginedit.h"

cDlgPatientOriginEdit::cDlgPatientOriginEdit( QWidget *p_poParent, cDBPatientOrigin *p_poPatientOrigin )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientOrigin = p_poPatientOrigin;
    if( m_poPatientOrigin )
    {
        ledName->setText( QString::fromStdString( m_poPatientOrigin->name() ) );
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
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
                m_poPatientOrigin->setLicenceId( 1 );
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
