#include <QMessageBox>

#include "belenus.h"
#include "dlgillnessgroup.h"
#include "../edit/dlgillnessgroupedit.h"

cDlgIllnessGroup::cDlgIllnessGroup( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Illness Group List" ) );
    setWindowIcon( QIcon("./resources/40x40_illnessgroup.png") );

    setupTableView();
}

cDlgIllnessGroup::~cDlgIllnessGroup()
{
}

void cDlgIllnessGroup::setupTableView()
{
    cTracer obTracer( "cDlgIllnessGroup::setupTableView" );

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

void cDlgIllnessGroup::refreshTable()
{
    cTracer obTracer( "cDlgIllnessGroup::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT illnessGroupId, licenceId, name, active, archive FROM illnessGroups";
    }
    else
    {
        m_qsQuery = "SELECT illnessGroupId AS id, name FROM illnessGroups WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgIllnessGroup::enableButtons()
{
    cTracer obTracer( "cDlgIllnessGroup::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgIllnessGroup::newClicked( bool )
{
    cDBIllnessGroup *poIllnessGroup = new cDBIllnessGroup;
    poIllnessGroup->createNew();

    cDlgIllnessGroupEdit  obDlgEdit( this, poIllnessGroup );
    obDlgEdit.setWindowTitle( tr( "New Illness Group" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poIllnessGroup->id();
        refreshTable();
    }

    delete poIllnessGroup;
}

void cDlgIllnessGroup::editClicked( bool )
{
    cDBIllnessGroup  *poIllnessGroup = NULL;

    try
    {
        poIllnessGroup = new cDBIllnessGroup;
        poIllnessGroup->load( m_uiSelectedId );

        cDlgIllnessGroupEdit  obDlgEdit( this, poIllnessGroup );
        obDlgEdit.setWindowTitle( poIllnessGroup->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poIllnessGroup ) delete poIllnessGroup;
    }
    catch( cSevException &e )
    {
        if( poIllnessGroup ) delete poIllnessGroup;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgIllnessGroup::deleteClicked( bool )
{
    cDBIllnessGroup  *poIllnessGroup = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Illness Group?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poIllnessGroup = new cDBIllnessGroup;
            poIllnessGroup->load( m_uiSelectedId );
            poIllnessGroup->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poIllnessGroup ) delete poIllnessGroup;
        }
        catch( cSevException &e )
        {
            if( poIllnessGroup ) delete poIllnessGroup;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
