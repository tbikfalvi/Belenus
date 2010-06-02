#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcardtype.h"
#include "../edit/dlgpatientcardtypeedit.h"

cDlgPatientCardType::cDlgPatientCardType( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Cardtype List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcardtype.gif") );

    setupTableView();
}

cDlgPatientCardType::~cDlgPatientCardType()
{
}

void cDlgPatientCardType::setupTableView()
{
    cTracer obTracer( "cDlgPatientCardType::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
    }
}

void cDlgPatientCardType::refreshTable()
{
    cTracer obTracer( "cDlgPatientCardType::refreshTable" );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientCardTypeId, licenceId, name, price, units, active, archive FROM patientCardTypes";
    }
    else
    {
        m_qsQuery = "SELECT patientCardTypeId AS id, name, price, units FROM patientCardTypes WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientCardType::enableButtons()
{
    cTracer obTracer( "cDlgPatientCardType::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( "admin" ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( "admin" ) );
}

void cDlgPatientCardType::newClicked( bool )
{
    cDBPatientCardType *poPatientCardType = new cDBPatientCardType;
    poPatientCardType->createNew();

    cDlgPatientCardTypeEdit  obDlgEdit( this, poPatientCardType );
    obDlgEdit.setWindowTitle( tr( "New PatientCard Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatientCardType->id();
        refreshTable();
    }

    delete poPatientCardType;
}

void cDlgPatientCardType::editClicked( bool )
{
    cDBPatientCardType  *poPatientCardType = NULL;

    try
    {
        poPatientCardType = new cDBPatientCardType;
        poPatientCardType->load( m_uiSelectedId );

        cDlgPatientCardTypeEdit  obDlgEdit( this, poPatientCardType );
        obDlgEdit.setWindowTitle( QString::fromStdString( poPatientCardType->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatientCardType ) delete poPatientCardType;
    }
    catch( cSevException &e )
    {
        if( poPatientCardType ) delete poPatientCardType;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgPatientCardType::deleteClicked( bool )
{
    cDBPatientCardType  *poPatientCardType = NULL;

    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this PatientCard Type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPatientCardType = new cDBPatientCardType;
            poPatientCardType->load( m_uiSelectedId );
            poPatientCardType->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPatientCardType ) delete poPatientCardType;
        }
        catch( cSevException &e )
        {
            if( poPatientCardType ) delete poPatientCardType;

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}
