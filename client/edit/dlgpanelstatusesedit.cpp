#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelstatusesedit.h"

cDlgPanelStatusesEdit::cDlgPanelStatusesEdit( QWidget *p_poParent, cDBPanelStatuses *p_poPanelStatuses )
    : QDialog( p_poParent )
{
    setupUi( this );
/*
    m_poPanelStatuses = p_poPanelStatuses;
    if( m_poPanelStatuses )
    {
        ledName->setText( QString::fromStdString( m_poPanelStatuses->name() ) );
        if( m_poPanelStatuses->licenceId() == 0 && m_poPanelStatuses->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPanelStatuses->licenceId() == 0 && m_poPanelStatuses->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
*/
}

cDlgPanelStatusesEdit::~cDlgPanelStatusesEdit()
{
}

void cDlgPanelStatusesEdit::accept ()
{
/*
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
            m_poPanelStatuses->setName( ledName->text().toStdString() );
            if( checkIndependent->isChecked() )
            {
                m_poPanelStatuses->setLicenceId( 0 );
            }
            else
            {
                m_poPanelStatuses->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poPanelStatuses->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
*/
}
