#include <QMessageBox>

#include "belenus.h"
#include "dlgreasontovisit.h"
#include "../edit/dlgreasontovisitedit.h"

cDlgReasonToVisit::cDlgReasonToVisit( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Reason List" ) );
    setWindowIcon( QIcon("./resources/40x40_reasontovisit.gif") );

    setupTableView();
}

cDlgReasonToVisit::~cDlgReasonToVisit()
{
}

void cDlgReasonToVisit::setupTableView()
{
    cTracer obTracer( "cDlgReasonToVisit::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgReasonToVisit::refreshTable()
{
    cTracer obTracer( "cDlgReasonToVisit::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT reasonToVisitId, licenceId, name, active, archive FROM reasonToVisit";
    }
    else
    {
        m_qsQuery = "SELECT reasonToVisitId AS id, name FROM reasonToVisit WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgReasonToVisit::enableButtons()
{
    cTracer obTracer( "cDlgReasonToVisit::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgReasonToVisit::newClicked( bool )
{
    cDBReasonToVisit *poReasonToVisit = new cDBReasonToVisit;
    poReasonToVisit->createNew();

    cDlgReasonToVisitEdit  obDlgEdit( this, poReasonToVisit );
    obDlgEdit.setWindowTitle( tr( "New Reason" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poReasonToVisit->id();
        refreshTable();
    }

    delete poReasonToVisit;
}

void cDlgReasonToVisit::editClicked( bool )
{
    cDBReasonToVisit  *poReasonToVisit = NULL;

    try
    {
        poReasonToVisit = new cDBReasonToVisit;
        poReasonToVisit->load( m_uiSelectedId );

        cDlgReasonToVisitEdit  obDlgEdit( this, poReasonToVisit );
        obDlgEdit.setWindowTitle( QString::fromStdString( poReasonToVisit->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poReasonToVisit ) delete poReasonToVisit;
    }
    catch( cSevException &e )
    {
        if( poReasonToVisit ) delete poReasonToVisit;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgReasonToVisit::deleteClicked( bool )
{
    cDBReasonToVisit  *poReasonToVisit = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Reason?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poReasonToVisit = new cDBReasonToVisit;
            poReasonToVisit->load( m_uiSelectedId );
            poReasonToVisit->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poReasonToVisit ) delete poReasonToVisit;
        }
        catch( cSevException &e )
        {
            if( poReasonToVisit ) delete poReasonToVisit;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
