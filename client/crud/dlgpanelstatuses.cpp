#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelstatuses.h"
#include "../edit/dlgpanelstatusesedit.h"

cDlgPanelStatuses::cDlgPanelStatuses( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panelstatus List" ) );
    setWindowIcon( QIcon("./resources/40x40_panelstatuses.gif") );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT panelStatusId, licenceId, name, seqNumber, archive FROM panelStatuses";
    }
    else
    {
        m_qsQuery = "SELECT panelStatusId AS id, name, seqNumber FROM panelStatuses WHERE archive<>\"DEL\"";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgPanelStatuses::~cDlgPanelStatuses()
{
}

void cDlgPanelStatuses::setupTableView()
{
    cTracer obTracer( "cDlgPanelStatuses::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Sequence order" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Sequence order" ) );
    }
}

void cDlgPanelStatuses::enableButtons()
{
    cTracer obTracer( "cDlgPanelStatuses::enableButtons" );

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

void cDlgPanelStatuses::newClicked( bool )
{
    cDBPanelStatuses *poPanelStatuses = new cDBPanelStatuses;
    poPanelStatuses->createNew();

    cDlgPanelStatusesEdit  obDlgEdit( this, poPanelStatuses );
    obDlgEdit.setWindowTitle( tr( "New Panelstatus" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanelStatuses->id();
        refreshTable();
    }

    delete poPanelStatuses;
}

void cDlgPanelStatuses::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Panelstatus?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "UPDATE panelStatuses SET archive=\"DEL\" WHERE panelStatusId=%1" ).arg( m_uiSelectedId );
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

void cDlgPanelStatuses::editClicked( bool )
{
    cDBPanelStatuses  *poPanelStatuses = NULL;

    try
    {
        poPanelStatuses = new cDBPanelStatuses;
        poPanelStatuses->load( m_uiSelectedId );

        cDlgPanelStatusesEdit  obDlgEdit( this, poPanelStatuses );
        obDlgEdit.setWindowTitle( QString::fromStdString( poPanelStatuses->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanelStatuses ) delete poPanelStatuses;
    }
    catch( cSevException &e )
    {
        if( poPanelStatuses ) delete poPanelStatuses;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
