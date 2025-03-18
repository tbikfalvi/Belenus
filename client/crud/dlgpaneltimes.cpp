
#include <QMessageBox>
#include <QFileDialog>

#include "belenus.h"
#include "dlgpaneltimes.h"
#include "../edit/dlgpaneluseedit.h"
#include "../db/dbpanels.h"

cDlgPanelTimes::cDlgPanelTimes( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel times" ) );
    setWindowIcon( QIcon("./resources/40x40_clock.png") );

    m_poBtnClose->setIcon( QIcon("./resources/40x40_exit.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "DlgPanelTimes", QPoint(800,500) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgPanelTimes::~cDlgPanelTimes()
{
    g_poPrefs->setDialogSize( "DlgPanelTimes", QPoint( width(), height() ) );
}

void cDlgPanelTimes::setupTableView()
{
    cTracer obTracer( "cDlgPanelTimes::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
}

void cDlgPanelTimes::refreshTable()
{
    cTracer obTracer( "cDlgPanelTimes::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString("SELECT panelUseId, licenceId, name, useTime, (usePrice/100) as usePrice, archive FROM panelUses WHERE active=1");
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS id, name, useTime, (usePrice/100) as usePrice FROM panelUses WHERE active=1");
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelTimes::enableButtons()
{
    cTracer obTracer( "cDlgPanelTimes::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPanelTimes::newClicked( bool )
{
    cDBPanelUses *poPanelUse = new cDBPanelUses;
    poPanelUse->createNew();

    cDlgPanelUseEdit  obDlgEdit( this, poPanelUse );
    obDlgEdit.setWindowTitle( tr( "New paneluse" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanelUse->id();
        refreshTable();
    }

    delete poPanelUse;
}

void cDlgPanelTimes::editClicked( bool )
{
    cDBPanelUses  *poPanelUse = NULL;

    try
    {
        poPanelUse = new cDBPanelUses;
        poPanelUse->load( m_uiSelectedId );

        cDlgPanelUseEdit  obDlgEdit( this, poPanelUse );
        obDlgEdit.setWindowTitle( tr("Edit paneluse") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanelUse ) delete poPanelUse;
    }
    catch( cSevException &e )
    {
        if( poPanelUse ) delete poPanelUse;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgPanelTimes::deleteClicked( bool )
{
    cDBPanelUses  *poPanelUse = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this paneluse?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanelUse = new cDBPanelUses;
            poPanelUse->load( m_uiSelectedId );
            poPanelUse->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelUse ) delete poPanelUse;
        }
        catch( cSevException &e )
        {
            if( poPanelUse ) delete poPanelUse;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

