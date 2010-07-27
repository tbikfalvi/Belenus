#include <QPushButton>
#include <QMessageBox>

#include "dlgreasontovisitedit.h"

cDlgReasonToVisitEdit::cDlgReasonToVisitEdit( QWidget *p_poParent, cDBReasonToVisit *p_poReasonToVisit )
    : QDialog( p_poParent )
{
    setupUi( this );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );

    m_poReasonToVisit = p_poReasonToVisit;
    if( m_poReasonToVisit )
    {
        ledName->setText( QString::fromStdString( m_poReasonToVisit->name() ) );
        if( m_poReasonToVisit->licenceId() == 0 && m_poReasonToVisit->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poReasonToVisit->licenceId() == 0 && m_poReasonToVisit->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgReasonToVisitEdit::~cDlgReasonToVisitEdit()
{
}

void cDlgReasonToVisitEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Reason to visit cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poReasonToVisit->setName( ledName->text().toStdString() );
            if( checkIndependent->isChecked() )
            {
                m_poReasonToVisit->setLicenceId( 0 );
            }
            else
            {
                m_poReasonToVisit->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poReasonToVisit->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}
