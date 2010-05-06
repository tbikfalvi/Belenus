#include <QMessageBox>

#include "belenus.h"
#include "dlgreasontovisit.h"
#include "../edit/dlgreasontovisitedit.h"

cDlgReasonToVisit::cDlgReasonToVisit( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Reason List" ) );
    setWindowIcon( QIcon("./resources/40x40_reasontovisit.gif") );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT reasonToVisitId, licenceId, name, archive FROM reasonToVisit";
    }
    else
    {
        m_qsQuery = "SELECT reasonToVisitId AS id, name FROM reasonToVisit WHERE archive<>\"DEL\"";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

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

void cDlgReasonToVisit::enableButtons()
{
    cTracer obTracer( "cDlgReasonToVisit::enableButtons" );

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

void cDlgReasonToVisit::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Reason?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "UPDATE reasonToVisit SET archive=\"DEL\" WHERE reasonToVisitId=%1" ).arg( m_uiSelectedId );
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

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
