#include <QMessageBox>

#include "belenus.h"
#include "dlgpaneltypes.h"
#include "../edit/dlgpaneltypeedit.h"

cDlgPanelTypes::cDlgPanelTypes( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel Types List" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    setupTableView();
}

cDlgPanelTypes::~cDlgPanelTypes()
{
}

void cDlgPanelTypes::setupTableView()
{
    cTracer obTracer( "cDlgPanelTypes::setupTableView" );

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

void cDlgPanelTypes::refreshTable()
{
    cTracer obTracer( "cDlgPanelTypes::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT panelTypeId, name, active, archive FROM panelTypes";
    }
    else
    {
        m_qsQuery = "SELECT panelTypeId AS id, name FROM panelTypes";
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelTypes::enableButtons()
{
    cTracer obTracer( "cDlgPanelTypes::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPanelTypes::newClicked( bool )
{
    cDBPanelTypes *poPanelTypes = new cDBPanelTypes;
    poPanelTypes->createNew();

    cDlgPanelTypeEdit  obDlgEdit( this );
    obDlgEdit.setWindowTitle( tr( "New Panel Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanelTypes->id();
        refreshTable();
    }

    delete poPanelTypes;
}

void cDlgPanelTypes::editClicked( bool )
{
    cDBPanelTypes  *poPanelTypes = NULL;

    try
    {
        poPanelTypes = new cDBPanelTypes;
        poPanelTypes->load( m_uiSelectedId );

        cDlgPanelTypeEdit  obDlgEdit( this, poPanelTypes );
        obDlgEdit.setWindowTitle( tr("Edit paneltype") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanelTypes ) delete poPanelTypes;
    }
    catch( cSevException &e )
    {
        if( poPanelTypes ) delete poPanelTypes;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPanelTypes::deleteClicked( bool )
{
    cDBPanelTypes *poPanelTypes = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Panel Type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanelTypes = new cDBPanelTypes;
            poPanelTypes->load( m_uiSelectedId );
            if( poPanelTypes->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPanelTypes->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelTypes ) delete poPanelTypes;
        }
        catch( cSevException &e )
        {
            if( poPanelTypes ) delete poPanelTypes;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

