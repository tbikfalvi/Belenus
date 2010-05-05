#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientorigin.h"
#include "../edit/dlgpatientoriginedit.h"

cDlgPatientOrigin::cDlgPatientOrigin( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Origin List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientOriginId, licenceId, name, archive FROM patientOrigin";
    }
    else
    {
        m_qsQuery = "SELECT patientOriginId AS id, name FROM patientOrigin WHERE archive<>\"DEL\"";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

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

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
    }
}

void cDlgPatientOrigin::enableButtons()
{
    cTracer obTracer( "cDlgPatientOrigin::enableButtons" );

    if( m_uiSelectedId )
    {
        bool boAdmin = g_obUser.isInGroup( "admin" );
        m_poBtnDelete->setEnabled( boAdmin );
        m_poBtnEdit->setEnabled( boAdmin );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
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

void cDlgPatientOrigin::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Patient Origin?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "UPDATE patientOrigin SET archive=\"DEL\" WHERE patientOriginId=%1" ).arg( m_uiSelectedId );
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

void cDlgPatientOrigin::editClicked( bool )
{
    cDBPatientOrigin  *poPatientOrigin = NULL;

    try
    {
        poPatientOrigin = new cDBPatientOrigin;
        poPatientOrigin->load( m_uiSelectedId );

        cDlgPatientOriginEdit  obDlgEdit( this, poPatientOrigin );
        obDlgEdit.setWindowTitle( QString::fromStdString( poPatientOrigin->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatientOrigin ) delete poPatientOrigin;
    }
    catch( cSevException &e )
    {
        if( poPatientOrigin ) delete poPatientOrigin;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
