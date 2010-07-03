#include <QMessageBox>

#include "belenus.h"
#include "dlgpatient.h"
#include "../edit/dlgpatientedit.h"
#include "../db/dbpostponed.h"

cDlgPatient::cDlgPatient( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

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
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "UniqueId" ) );
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
        refreshTable();
    }

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
        obDlgEdit.setWindowTitle( QString::fromStdString( poPatient->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatient ) delete poPatient;
    }
    catch( cSevException &e )
    {
        if( poPatient ) delete poPatient;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgPatient::deleteClicked( bool )
{
    cDBPatient  *poPatient = NULL;

    if( QMessageBox::question( this, tr( "Confirmation" ),
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

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}
