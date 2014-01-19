#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelgroups.h"
#include "../edit/dlgpanelgroupedit.h"

cDlgPanelGroups::cDlgPanelGroups( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel Groups List" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPanelGroups", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgPanelGroups::~cDlgPanelGroups()
{
    g_poPrefs->setDialogSize( "ListPanelGroups", QPoint( width(), height() ) );
}

void cDlgPanelGroups::setupTableView()
{
    cTracer obTracer( "cDlgPanelGroups::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPanelGroups::refreshTable()
{
    cTracer obTracer( "cDlgPanelGroups::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT panelGroupId, name, active, archive FROM panelGroups WHERE panelGroupId>0";
    }
    else
    {
        m_qsQuery = "SELECT panelGroupId AS id, name FROM panelGroups WHERE panelGroupId>0";
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelGroups::enableButtons()
{
    cTracer obTracer( "cDlgPanelGroups::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPanelGroups::newClicked( bool )
{
    cDBPanelGroups *poPanelGroups = new cDBPanelGroups;
    poPanelGroups->createNew();

    cDlgPanelGroupEdit  obDlgEdit( this, poPanelGroups );
    obDlgEdit.setWindowTitle( tr( "New Panel Group" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanelGroups->id();
        refreshTable();
    }

    delete poPanelGroups;
}

void cDlgPanelGroups::editClicked( bool )
{
    cDBPanelGroups  *poPanelGroups = NULL;

    try
    {
        poPanelGroups = new cDBPanelGroups;
        poPanelGroups->load( m_uiSelectedId );

        cDlgPanelGroupEdit  obDlgEdit( this, poPanelGroups );
        obDlgEdit.setWindowTitle( tr("Edit panelgroup") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanelGroups ) delete poPanelGroups;
    }
    catch( cSevException &e )
    {
        if( poPanelGroups ) delete poPanelGroups;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPanelGroups::deleteClicked( bool )
{
    cDBPanelGroups *poPanelGroups = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Panel Group?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanelGroups = new cDBPanelGroups;
            poPanelGroups->load( m_uiSelectedId );
            if( poPanelGroups->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPanelGroups->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelGroups ) delete poPanelGroups;
        }
        catch( cSevException &e )
        {
            if( poPanelGroups ) delete poPanelGroups;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

