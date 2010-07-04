#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelstatuses.h"
#include "../edit/dlgpanelstatusesedit.h"

cDlgPanelStatuses::cDlgPanelStatuses( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panelstatus List" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.gif") );

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

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "PanelType" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Sequence order" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "PanelType" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Sequence order" ) );
    }
}

void cDlgPanelStatuses::refreshTable()
{
    cTracer obTracer( "cDlgPanelStatuses::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT panelStatuses.panelStatusId, panelStatuses.licenceId, panelStatuses.name, paneltypes.name AS panelType, panelStatuses.seqNumber, panelStatuses.active, panelStatuses.archive FROM panelStatuses, paneltypes WHERE panelStatuses.panelTypeId=panelTypes.panelTypeId";
    }
    else
    {
        m_qsQuery = "SELECT panelStatuses.panelStatusId AS id, panelStatuses.name, paneltypes.name AS panelType, panelStatuses.seqNumber FROM panelStatuses, paneltypes WHERE panelStatuses.active=1 AND panelStatuses.panelTypeId=panelTypes.panelTypeId";
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelStatuses::enableButtons()
{
    cTracer obTracer( "cDlgPanelStatuses::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
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

void cDlgPanelStatuses::deleteClicked( bool )
{
    cDBPanelStatuses  *poPanelStatuses = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Panelstatus?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanelStatuses = new cDBPanelStatuses;
            poPanelStatuses->load( m_uiSelectedId );
            poPanelStatuses->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelStatuses ) delete poPanelStatuses;
        }
        catch( cSevException &e )
        {
            if( poPanelStatuses ) delete poPanelStatuses;

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}
