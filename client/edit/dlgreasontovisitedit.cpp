#include <QPushButton>
#include <QMessageBox>

#include "dlgreasontovisitedit.h"

cDlgReasonToVisitEdit::cDlgReasonToVisitEdit( QWidget *p_poParent, cDBReasonToVisit *p_poReasonToVisit )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Reason to visit" ) );
    setWindowIcon( QIcon("./resources/40x40_reasontovisit.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poReasonToVisit = p_poReasonToVisit;
    if( m_poReasonToVisit )
    {
        ledName->setText( m_poReasonToVisit->name() );
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
        if( m_poReasonToVisit->id() > 0 )
            checkIndependent->setEnabled( false );
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
            m_poReasonToVisit->setName( ledName->text() );
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
