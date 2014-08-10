#include <QMessageBox>

#include "belenus.h"
#include "dlgadvertisements.h"
#include "../edit/dlgadvertisementedit.h"

cDlgAdvertisements::cDlgAdvertisements( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Advertisement List" ) );
    setWindowIcon( QIcon("./resources/40x40_advertisement.png") );

    setupTableView();
}

cDlgAdvertisements::~cDlgAdvertisements()
{
}

void cDlgAdvertisements::setupTableView()
{
    cTracer obTracer( "cDlgAdvertisements::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Caption" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Timer" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );

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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Caption" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Timer" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Active" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgAdvertisements::refreshTable()
{
    cTracer obTracer( "cDlgAdvertisements::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT advertisementId, licenceId, name, caption, timer active, archive FROM advertisements";
    }
    else
    {
        m_qsQuery = "SELECT advertisementId AS id, name, caption, timer, active FROM advertisements";
    }

    cDlgCrud::refreshTable();
}

void cDlgAdvertisements::enableButtons()
{
    cTracer obTracer( "cDlgAdvertisements::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgAdvertisements::newClicked( bool )
{
    cDBAdvertisements *poAdvertisements = new cDBAdvertisements;
    poAdvertisements->createNew();

    cDlgAdvertisementEdit  obDlgEdit( this, poAdvertisements );
    obDlgEdit.setWindowTitle( tr( "New Advertisement" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poAdvertisements->id();
        refreshTable();
    }

    delete poAdvertisements;
}

void cDlgAdvertisements::editClicked( bool )
{
    cDBAdvertisements  *poAdvertisements = NULL;

    try
    {
        poAdvertisements = new cDBAdvertisements;
        poAdvertisements->load( m_uiSelectedId );

        cDlgAdvertisementEdit  obDlgEdit( this, poAdvertisements );
        obDlgEdit.setWindowTitle( poAdvertisements->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poAdvertisements ) delete poAdvertisements;
    }
    catch( cSevException &e )
    {
        if( poAdvertisements ) delete poAdvertisements;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgAdvertisements::deleteClicked( bool )
{
    cDBAdvertisements  *poAdvertisements = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Advertisement?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poAdvertisements = new cDBAdvertisements;
            poAdvertisements->load( m_uiSelectedId );
            if( poAdvertisements->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poAdvertisements->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poAdvertisements ) delete poAdvertisements;
        }
        catch( cSevException &e )
        {
            if( poAdvertisements ) delete poAdvertisements;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
