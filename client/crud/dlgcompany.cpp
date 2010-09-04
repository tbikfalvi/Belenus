#include <QMessageBox>

#include "belenus.h"
#include "dlgcompany.h"
#include "../edit/dlgcompanyedit.h"

cDlgCompany::cDlgCompany( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Company List" ) );
    setWindowIcon( QIcon("./resources/40x40_company.png") );

    setupTableView();
}

cDlgCompany::~cDlgCompany()
{
}

void cDlgCompany::setupTableView()
{
    cTracer obTracer( "cDlgCompany::setupTableView" );

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

void cDlgCompany::refreshTable()
{
    cTracer obTracer( "cDlgCompany::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT companyId, licenceId, name, active, archive FROM companies";
    }
    else
    {
        m_qsQuery = "SELECT companyId AS id, name FROM companies WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgCompany::enableButtons()
{
    cTracer obTracer( "cDlgCompany::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgCompany::newClicked( bool )
{
    cDBCompany *poCompany = new cDBCompany;
    poCompany->createNew();

    cDlgCompanyEdit  obDlgEdit( this, poCompany );
    obDlgEdit.setWindowTitle( tr( "New Company" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poCompany->id();
        refreshTable();
    }

    delete poCompany;
}

void cDlgCompany::editClicked( bool )
{
    cDBCompany  *poCompany = NULL;

    try
    {
        poCompany = new cDBCompany;
        poCompany->load( m_uiSelectedId );

        cDlgCompanyEdit  obDlgEdit( this, poCompany );
        obDlgEdit.setWindowTitle( poCompany->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poCompany ) delete poCompany;
    }
    catch( cSevException &e )
    {
        if( poCompany ) delete poCompany;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgCompany::deleteClicked( bool )
{
    cDBCompany  *poCompany = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Company?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poCompany = new cDBCompany;
            poCompany->load( m_uiSelectedId );
            poCompany->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poCompany ) delete poCompany;
        }
        catch( cSevException &e )
        {
            if( poCompany ) delete poCompany;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
