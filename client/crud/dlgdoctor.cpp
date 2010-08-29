#include <QMessageBox>

#include "belenus.h"
#include "dlgdoctor.h"
#include "../edit/dlgdoctoredit.h"

cDlgDoctor::cDlgDoctor( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Doctor List" ) );
    setWindowIcon( QIcon("./resources/40x40_doctor.png") );

    setupTableView();
}

cDlgDoctor::~cDlgDoctor()
{
}

void cDlgDoctor::setupTableView()
{
    cTracer obTracer( "cDlgDoctor::setupTableView" );

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

void cDlgDoctor::refreshTable()
{
    cTracer obTracer( "cDlgDoctor::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT doctorId, licenceId, name, active, archive FROM doctors";
    }
    else
    {
        m_qsQuery = "SELECT doctorId AS id, name FROM doctors WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgDoctor::enableButtons()
{
    cTracer obTracer( "cDlgDoctor::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgDoctor::newClicked( bool )
{
    cDBDoctor *poDoctor = new cDBDoctor;
    poDoctor->createNew();

    cDlgDoctorEdit  obDlgEdit( this, poDoctor );
    obDlgEdit.setWindowTitle( tr( "New Doctor" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poDoctor->id();
        refreshTable();
    }

    delete poDoctor;
}

void cDlgDoctor::editClicked( bool )
{
    cDBDoctor  *poDoctor = NULL;

    try
    {
        poDoctor = new cDBDoctor;
        poDoctor->load( m_uiSelectedId );

        cDlgDoctorEdit  obDlgEdit( this, poDoctor );
        obDlgEdit.setWindowTitle( poDoctor->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poDoctor ) delete poDoctor;
    }
    catch( cSevException &e )
    {
        if( poDoctor ) delete poDoctor;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}

void cDlgDoctor::deleteClicked( bool )
{
    cDBDoctor  *poDoctor = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Doctor?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poDoctor = new cDBDoctor;
            poDoctor->load( m_uiSelectedId );
            poDoctor->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poDoctor ) delete poDoctor;
        }
        catch( cSevException &e )
        {
            if( poDoctor ) delete poDoctor;
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }
    }
}
