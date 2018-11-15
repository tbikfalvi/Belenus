#include <QMessageBox>

#include "belenus.h"
#include "dlgemails.h"
#include "../edit/dlgemailsedit.h"

cDlgEmails::cDlgEmails( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Unsent Emails List" ) );
    setWindowIcon( QIcon("./resources/40x40_draftmail.png") );

    setupTableView();
}

cDlgEmails::~cDlgEmails()
{
}

void cDlgEmails::setupTableView()
{
    cTracer obTracer( "cDlgEmails::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Recipient" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Patientcard" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Recipient" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Patientcard" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "State" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgEmails::refreshTable()
{
    cTracer obTracer( "cDlgEmails::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT httpSendMailId, licenceId, dateOfSending, recipients, var_barcode, active, archive FROM httpsendmail";
    }
    else
    {
        m_qsQuery = "SELECT httpSendMailId AS id, dateOfSending, recipients, var_barcode, archive FROM httpsendmail";
    }

    cDlgCrud::refreshTable();
}

void cDlgEmails::enableButtons()
{
    cTracer obTracer( "cDlgEmails::enableButtons" );

    m_poBtnNew->setEnabled( false );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgEmails::newClicked( bool )
{
}

void cDlgEmails::editClicked( bool )
{
    cDBSendMail  *poEmails = NULL;

    try
    {
        poEmails = new cDBSendMail;
        poEmails->load( m_uiSelectedId );

        cDlgEmailsEdit  obDlgEdit( this, poEmails );
        obDlgEdit.setWindowTitle( tr( "Edit email settings" ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poEmails ) delete poEmails;
    }
    catch( cSevException &e )
    {
        if( poEmails ) delete poEmails;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgEmails::deleteClicked( bool )
{
    cDBSendMail  *poEmails = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this email?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poEmails = new cDBSendMail;
            poEmails->load( m_uiSelectedId );
            poEmails->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poEmails ) delete poEmails;
        }
        catch( cSevException &e )
        {
            if( poEmails ) delete poEmails;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
