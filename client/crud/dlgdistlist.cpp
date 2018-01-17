#include <QMessageBox>

#include "belenus.h"
#include "dlgdistlist.h"
#include "../edit/dlgdistlistedit.h"

cDlgDistList::cDlgDistList( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Distribution lists" ) );
    setWindowIcon( QIcon("./resources/40x40_distlist.png") );

    m_poParent = p_poParent;

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListDistLists", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgDistList::~cDlgDistList()
{
    g_poPrefs->setDialogSize( "ListDistLists", QPoint( width(), height() ) );
}

void cDlgDistList::setupTableView()
{
    cTracer obTracer( "cDlgDistList::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Description" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Active" ) );
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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Description" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgDistList::refreshTable()
{
    cTracer obTracer( "cDlgDistList::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT distlistId, licenceId, name, description, active, archive FROM distlist";
    }
    else
    {
        m_qsQuery = "SELECT distlistId AS id, name, description FROM distlist WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgDistList::enableButtons()
{
    cTracer obTracer( "cDlgDistList::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgDistList::newClicked( bool )
{
    cDBDistList *poDistList = new cDBDistList;
    poDistList->createNew();

    cDlgDistListEdit  obDlgEdit( this, poDistList );
    obDlgEdit.setWindowTitle( tr( "New Distribution list" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poDistList->id();
        refreshTable();
    }

    delete poDistList;
}

void cDlgDistList::editClicked( bool )
{
    cDBDistList  *poDistList = NULL;

    try
    {
        poDistList = new cDBDistList;
        poDistList->load( m_uiSelectedId );

        cDlgDistListEdit  obDlgEdit( this, poDistList );
        obDlgEdit.setWindowTitle( poDistList->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poDistList ) delete poDistList;
    }
    catch( cSevException &e )
    {
        if( poDistList ) delete poDistList;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgDistList::deleteClicked( bool )
{
    cDBDistList  *poDistList = new cDBDistList;
    poDistList->load( m_uiSelectedId );

    QString     qsQuestion = tr( "Are you sure you want to delete this Distribution list?" );

    if( QMessageBox::question( this, tr( "Question" ),
                               qsQuestion,
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poDistList->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poDistList ) delete poDistList;
        }
        catch( cSevException &e )
        {
            if( poDistList ) delete poDistList;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

