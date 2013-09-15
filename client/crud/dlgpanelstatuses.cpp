#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelstatuses.h"
#include "../edit/dlgpanelstatusesedit.h"
#include "../db/dbpanelstatussettings.h"

cDlgPanelStatuses::cDlgPanelStatuses( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panelstatus List" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    m_bStatusChanged = false;

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPanelStatuses", QPoint(450,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgPanelStatuses::~cDlgPanelStatuses()
{
    g_poPrefs->setDialogSize( "ListPanelStatuses", QPoint( width(), height() ) );
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

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 4, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "PanelType" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Sequence order" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );

        tbvCrud->sortByColumn( 3, Qt::AscendingOrder );
    }
}

void cDlgPanelStatuses::refreshTable()
{
    cTracer obTracer( "cDlgPanelStatuses::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT panelStatuses.panelStatusId, panelStatuses.licenceId, panelStatuses.name, panelTypes.name AS panelType, panelStatuses.seqNumber, panelStatuses.active, panelStatuses.archive FROM panelStatuses, panelTypes WHERE panelStatuses.panelTypeId=panelTypes.panelTypeId";
    }
    else
    {
        m_qsQuery = "SELECT panelStatuses.panelStatusId AS id, panelStatuses.name, panelTypes.name AS panelType, panelStatuses.seqNumber FROM panelStatuses, panelTypes WHERE panelStatuses.active=1 AND panelStatuses.panelTypeId=panelTypes.panelTypeId";
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

        cDBPanelStatusSettings  obDBPanelStatusSettings;

        obDBPanelStatusSettings.createNew();
        obDBPanelStatusSettings.setLicenceId( g_poPrefs->getLicenceId() );
        obDBPanelStatusSettings.setPanelStatusId( m_uiSelectedId );
        switch( poPanelStatuses->activateCommand() )
        {
            case 0:
                obDBPanelStatusSettings.setBackgroundColor( "#00ff00" );
                break;
            case 1:
            case 4:
            case 9:
                obDBPanelStatusSettings.setBackgroundColor( "#ffff00" );
                break;
            case 2:
            case 3:
                obDBPanelStatusSettings.setBackgroundColor( "#ff0000" );
                break;
        }
        obDBPanelStatusSettings.setStatusFontName( "Arial" );
        obDBPanelStatusSettings.setStatusFontSize( 18 );
        obDBPanelStatusSettings.setStatusFontColor( "#000000" );
        obDBPanelStatusSettings.setTimerFontName( "Book Antiqua" );
        obDBPanelStatusSettings.setTimerFontSize( 30 );
        obDBPanelStatusSettings.setTimerFontColor( "#000000" );
        obDBPanelStatusSettings.setNextFontName( "Arial" );
        obDBPanelStatusSettings.setNextFontSize( 18 );
        obDBPanelStatusSettings.setNextFontColor( "#000000" );
        obDBPanelStatusSettings.setInfoFontName( "Arial" );
        obDBPanelStatusSettings.setInfoFontSize( 10 );
        obDBPanelStatusSettings.setInfoFontColor( "#000000" );
        obDBPanelStatusSettings.setActive( true );
        obDBPanelStatusSettings.save();

        refreshTable();
        m_bStatusChanged = true;
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
        obDlgEdit.setWindowTitle( poPanelStatuses->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
            m_bStatusChanged = true;
        }

        if( poPanelStatuses ) delete poPanelStatuses;
    }
    catch( cSevException &e )
    {
        if( poPanelStatuses ) delete poPanelStatuses;
        g_obLogger(e.severity()) << e.what() << EOM;
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
            if( poPanelStatuses->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }

            cDBPanelStatusSettings  obDBPanelStatusSettings;

            obDBPanelStatusSettings.loadStatus( poPanelStatuses->id() );
            obDBPanelStatusSettings.remove();

            poPanelStatuses->remove();            
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelStatuses ) delete poPanelStatuses;
            m_bStatusChanged = true;
        }
        catch( cSevException &e )
        {
            if( poPanelStatuses ) delete poPanelStatuses;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

bool cDlgPanelStatuses::isStatusChanged()
{
    return m_bStatusChanged;
}
