#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientorigin.h"
#include "../edit/dlgpatientoriginedit.h"

cDlgPatientOrigin::cDlgPatientOrigin( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Origin List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientorigin.png") );

    setupTableView();
}

cDlgPatientOrigin::~cDlgPatientOrigin()
{
}

void cDlgPatientOrigin::setupTableView()
{
    cTracer obTracer( "cDlgPatientOrigin::setupTableView" );

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

void cDlgPatientOrigin::refreshTable()
{
    cTracer obTracer( "cDlgPatientOrigin::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientOriginId, licenceId, name, active, archive FROM patientOrigin";
    }
    else
    {
        m_qsQuery = "SELECT patientOriginId AS id, name FROM patientOrigin WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientOrigin::enableButtons()
{
    cTracer obTracer( "cDlgPatientOrigin::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPatientOrigin::newClicked( bool )
{
    cDBPatientOrigin *poPatientOrigin = new cDBPatientOrigin;
    poPatientOrigin->createNew();

    cDlgPatientOriginEdit  obDlgEdit( this, poPatientOrigin );
    obDlgEdit.setWindowTitle( tr( "New Patient Origin" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatientOrigin->id();
        refreshTable();
    }

    delete poPatientOrigin;
}

void cDlgPatientOrigin::editClicked( bool )
{
    cDBPatientOrigin  *poPatientOrigin = NULL;

    try
    {
        poPatientOrigin = new cDBPatientOrigin;
        poPatientOrigin->load( m_uiSelectedId );

        cDlgPatientOriginEdit  obDlgEdit( this, poPatientOrigin );
        obDlgEdit.setWindowTitle( poPatientOrigin->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatientOrigin ) delete poPatientOrigin;
    }
    catch( cSevException &e )
    {
        if( poPatientOrigin ) delete poPatientOrigin;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPatientOrigin::deleteClicked( bool )
{
    cDBPatientOrigin  *poPatientOrigin = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Patient Origin?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPatientOrigin = new cDBPatientOrigin;
            poPatientOrigin->load( m_uiSelectedId );
            poPatientOrigin->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPatientOrigin ) delete poPatientOrigin;
        }
        catch( cSevException &e )
        {
            if( poPatientOrigin ) delete poPatientOrigin;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
