#include <QMessageBox>

#include "belenus.h"
#include "dlgpanels.h"
#include "../edit/dlgpaneledit.h"

cDlgPanels::cDlgPanels( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panels List" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPanels", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_poBtnEdit->setVisible( false );
    m_poBtnEdit->setEnabled( false );
    m_poBtnDelete->setVisible( false );
    m_poBtnDelete->setEnabled( false );

    setupTableView();
}

cDlgPanels::~cDlgPanels()
{
    g_poPrefs->setDialogSize( "ListPanels", QPoint( width(), height() ) );
}

void cDlgPanels::setupTableView()
{
    cTracer obTracer( "cDlgPanels::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Title" ) );
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
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Title" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPanels::refreshTable()
{
    cTracer obTracer( "cDlgPanels::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT panelId, title, active, archive FROM panels WHERE panelId>0";
    }
    else
    {
        m_qsQuery = "SELECT panelId AS id, title FROM panels WHERE panelId>0";
    }

    cDlgCrud::refreshTable();
}

void cDlgPanels::enableButtons()
{
    cTracer obTracer( "cDlgPanels::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
//    m_poBtnEdit->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
//    m_poBtnDelete->setEnabled( false );
}

void cDlgPanels::newClicked( bool )
{
    cDBPanel *poPanels = new cDBPanel;
    poPanels->createNew();

    cDlgPanelEdit  obDlgEdit( this, poPanels );
    obDlgEdit.setWindowTitle( tr( "New Panel" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanels->id();
        refreshTable();
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "You must restart the application." ) );
    }

    delete poPanels;
}

void cDlgPanels::editClicked( bool )
{
/*
    cDBPanel  *poPanels = NULL;

    try
    {
        poPanels = new cDBPanel;
        poPanels->load( m_uiSelectedId );

        cDlgPanelEdit  obDlgEdit( this, poPanels );
        obDlgEdit.setWindowTitle( tr("Edit panel") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanels ) delete poPanels;
    }
    catch( cSevException &e )
    {
        if( poPanels ) delete poPanels;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
*/
}

void cDlgPanels::deleteClicked( bool )
{
/*
    cDBPanel *poPanels = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Panel Group?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanels = new cDBPanel;
            poPanels->load( m_uiSelectedId );
            if( poPanels->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPanels->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanels ) delete poPanels;
        }
        catch( cSevException &e )
        {
            if( poPanels ) delete poPanels;
            g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
        }
    }
*/
}

