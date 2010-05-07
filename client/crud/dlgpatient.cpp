#include <QMessageBox>

#include "belenus.h"
#include "dlgpatient.h"
#include "../edit/dlgpatientedit.h"

cDlgPatient::cDlgPatient( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientId, licenceId, name, uniqueId, archive FROM patients";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, uniqueId FROM patients WHERE archive<>\"DEL\"";
    }

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

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "UniqueId" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "UniqueId" ) );
    }
}

void cDlgPatient::enableButtons()
{
    cTracer obTracer( "cDlgPatient::enableButtons" );

    if( m_uiSelectedId )
    {
        m_poBtnDelete->setEnabled( true );
        m_poBtnEdit->setEnabled( true );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
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

void cDlgPatient::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Patient?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "UPDATE patients SET archive=\"DEL\" WHERE patientId=%1" ).arg( m_uiSelectedId );
            g_poDB->executeQuery( stQuery.toStdString(), true );

            m_uiSelectedId = 0;
            refreshTable();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
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
