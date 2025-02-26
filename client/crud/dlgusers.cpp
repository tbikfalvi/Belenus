#include <QMessageBox>

#include "dlgusers.h"
#include "../edit/dlguseredit.h"
#include "belenus.h"

cDlgUsers::cDlgUsers( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "User List" ) );
    setWindowIcon( QIcon("./resources/40x40_user.png") );

    m_uiSelectedId = g_obUser.id();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListUsers", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgUsers::~cDlgUsers()
{
    g_poPrefs->setDialogSize( "ListUsers", QPoint( width(), height() ) );
}

void cDlgUsers::setupTableView()
{
    cTracer obTracer( "cDlgUsers::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Active" ) );
    m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Name" ) );
    m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Real Name" ) );
    m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Privilege" ) );
    m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Comment" ) );

    tbvCrud->sortByColumn( 4, Qt::AscendingOrder );

    tbvCrud->resizeColumnToContents( 2 );
    tbvCrud->resizeColumnToContents( 3 );
    tbvCrud->resizeColumnToContents( 4 );
    tbvCrud->resizeColumnToContents( 5 );
    tbvCrud->setColumnHidden( 1, true );
}

void cDlgUsers::refreshTable()
{
    cTracer obTracer( "cDlgUsers::refreshTable" );

    m_qsQuery = "SELECT userId AS id, accgroup, active, name, realName, accGroup, comment FROM users";

    cDlgCrud::refreshTable();
}

void cDlgUsers::enableButtons()
{
    cTracer obTracer( "cDlgUsers::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    if( m_uiSelectedId )
    {
        m_poBtnDelete->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) &&
                                   g_obUser.isInGroup( (cAccessGroup::teAccessGroup)(m_poSortedModel->index( m_inSelectedRow, 1 ).data().toUInt()) ) &&
                                   g_obUser.id() != m_uiSelectedId );
        m_poBtnEdit->setEnabled( (g_obUser.isInGroup( cAccessGroup::ADMIN ) &&
                                  g_obUser.isInGroup( (cAccessGroup::teAccessGroup)(m_poSortedModel->index( m_inSelectedRow, 1 ).data().toUInt()) )) ||
                                  g_obUser.id() == m_uiSelectedId );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
}

void cDlgUsers::newClicked( bool )
{
    cDBUser *poUser = new cDBUser;
    poUser->createNew();

    cDlgUserEdit  obDlgEdit( this, poUser );
    obDlgEdit.setWindowTitle( tr( "New User" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poUser->id();
        refreshTable();
    }

    delete poUser;
}

void cDlgUsers::editClicked( bool )
{
    cDBUser  *poUser = NULL;

    try
    {
        if( m_uiSelectedId == g_obUser.id() )
        {
            poUser = &g_obUser;
        }
        else
        {
            poUser = new cDBUser;
            poUser->load( m_uiSelectedId );
        }

        cDlgUserEdit  obDlgEdit( this, poUser );
        obDlgEdit.setWindowTitle( poUser->realName() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( m_uiSelectedId != g_obUser.id() ) delete poUser;
    }
    catch( cSevException &e )
    {
        if( poUser ) delete poUser;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgUsers::deleteClicked( bool )
{
    cDBUser  *poUser = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this User?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poUser = new cDBUser;
            poUser->load( m_uiSelectedId );
            if( poUser->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poUser->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poUser ) delete poUser;
        }
        catch( cSevException &e )
        {
            if( poUser ) delete poUser;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}
