#include <QMessageBox>

#include "dlgusers.h"
#include "../edit/dlguseredit.h"
#include "belenus.h"

cDlgUsers::cDlgUsers( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "User List" ) );

    m_uiSelectedId = g_obUser.id();

    setupTableView();
}

cDlgUsers::~cDlgUsers()
{
}

void cDlgUsers::setupTableView()
{
    cTracer obTracer( "cDlgUsers::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Real Name" ) );
    }
    else
    {
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Real Name" ) );
    }
    tbvCrud->resizeColumnToContents( 2 );
    tbvCrud->resizeColumnToContents( 3 );
    tbvCrud->setColumnHidden( 1, true );
}

void cDlgUsers::refreshTable()
{
    cTracer obTracer( "cDlgUsers::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT userId AS id, accgroup, active, name, realName FROM users";
    }
    else
    {
        m_qsQuery = "SELECT userId AS id, accgroup, name, realName FROM users WHERE active = 1";
    }

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
        obDlgEdit.setWindowTitle( QString::fromStdString( poUser->realName() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( m_uiSelectedId != g_obUser.id() ) delete poUser;
    }
    catch( cSevException &e )
    {
        if( poUser ) delete poUser;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgUsers::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this User?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QString stQuery = QString( "UPDATE users SET active=0, name=\"%1\" WHERE userId=%1" ).arg( m_uiSelectedId );
        g_poDB->executeQuery( stQuery.toStdString(), true );

        m_uiSelectedId = 0;
        refreshTable();
    }
}
