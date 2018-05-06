#include <QMessageBox>

#include "belenus.h"
#include "dlgskintypes.h"
#include "../edit/dlgskintypeedit.h"

cDlgSkinTypes::cDlgSkinTypes( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Skin Types List" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListSkinTypes", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgSkinTypes::~cDlgSkinTypes()
{
    g_poPrefs->setDialogSize( "ListSkinTypes", QPoint( width(), height() ) );
}

void cDlgSkinTypes::setupTableView()
{
    cTracer obTracer( "cDlgSkinTypes::setupTableView" );

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

void cDlgSkinTypes::refreshTable()
{
    cTracer obTracer( "cDlgSkinTypes::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT skinTypeId, skinTypeName, active, archive FROM skinTypes WHERE skinTypeId>0";
    }
    else
    {
        m_qsQuery = "SELECT skinTypeId AS id, skinTypeName FROM skinTypes WHERE skinTypeId>0";
    }

    cDlgCrud::refreshTable();
}

void cDlgSkinTypes::enableButtons()
{
    cTracer obTracer( "cDlgSkinTypes::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId>0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgSkinTypes::newClicked( bool )
{
    cDBSkinTypes *poSkinTypes = new cDBSkinTypes;
    poSkinTypes->createNew();

    cDlgSkinTypeEdit  obDlgEdit( this, poSkinTypes );
    obDlgEdit.setWindowTitle( tr( "New Skin Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poSkinTypes->id();
        refreshTable();
    }

    delete poSkinTypes;
}

void cDlgSkinTypes::editClicked( bool )
{
    cDBSkinTypes  *poSkinTypes = NULL;

    try
    {
        poSkinTypes = new cDBSkinTypes;
        poSkinTypes->load( m_uiSelectedId );

        cDlgSkinTypeEdit  obDlgEdit( this, poSkinTypes );
        obDlgEdit.setWindowTitle( tr("Edit skintype") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poSkinTypes ) delete poSkinTypes;
    }
    catch( cSevException &e )
    {
        if( poSkinTypes ) delete poSkinTypes;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgSkinTypes::deleteClicked( bool )
{
    cDBSkinTypes *poSkinTypes = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Skin Type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patients WHERE skinTypeId=%1" ).arg( m_uiSelectedId ) );
            if( poQuery->first() )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete skin type\n"
                                         "linked to one or more patients."));
                return;
            }

            poSkinTypes = new cDBSkinTypes;
            poSkinTypes->load( m_uiSelectedId );
            poSkinTypes->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poSkinTypes ) delete poSkinTypes;
        }
        catch( cSevException &e )
        {
            if( poSkinTypes ) delete poSkinTypes;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

