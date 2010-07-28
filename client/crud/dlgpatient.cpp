#include <QMessageBox>

#include "belenus.h"
#include "dlgpatient.h"
#include "../edit/dlgpatientedit.h"
#include "../db/dbpostponed.h"

cDlgPatient::cDlgPatient( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    setupTableView();
}

cDlgPatient::~cDlgPatient()
{
}

void cDlgPatient::setupTableView()
{
    cTracer obTracer( "cDlgPatient::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "UniqueId" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "UniqueId" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatient::refreshTable()
{
    cTracer obTracer( "cDlgPatient::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, licenceId, name, uniqueId, active, archive FROM patients WHERE patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, uniqueId FROM patients WHERE patientId>0 AND active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgPatient::enableButtons()
{
    cTracer obTracer( "cDlgPatient::enableButtons" );

    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 );
}

void cDlgPatient::newClicked( bool )
{
    cDBPatient *poPatient = new cDBPatient;
    poPatient->createNew();

    cDlgPatientEdit  obDlgEdit( this, poPatient );
    obDlgEdit.setWindowTitle( tr( "New Patient" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatient->id();
    }
    refreshTable();

    delete poPatient;
}

void cDlgPatient::editClicked( bool )
{
    cDBPatient  *poPatient = NULL;

    try
    {
        poPatient = new cDBPatient;
        poPatient->load( m_uiSelectedId );

        cDlgPatientEdit  obDlgEdit( this, poPatient );
        obDlgEdit.setWindowTitle( poPatient->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            cDBPostponed    obDBPostponed;
            obDBPostponed.removePatient( m_uiSelectedId );
        }
        refreshTable();

        if( poPatient ) delete poPatient;
    }
    catch( cSevException &e )
    {
        if( poPatient ) delete poPatient;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}

void cDlgPatient::deleteClicked( bool )
{
    cDBPatient  *poPatient = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Patient?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            cDBPostponed    obDBPostponed;
            obDBPostponed.removePatient( m_uiSelectedId );

            poPatient = new cDBPatient;
            poPatient->load( m_uiSelectedId );
            poPatient->remove();

            m_uiSelectedId = 0;
            refreshTable();
            if( poPatient ) delete poPatient;
        }
        catch( cSevException &e )
        {
            if( poPatient ) delete poPatient;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }
    }
}
