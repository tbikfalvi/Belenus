#include <QMessageBox>

#include "belenus.h"
#include "dlgwaitlistinfo.h"
#include "../db/dbwaitlist.h"
#include "../db/dbpatientcardunits.h"

cDlgWaitlistInfo::cDlgWaitlistInfo( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Waiting guests" ) );
    setWindowIcon( QIcon("./resources/40x40_device_later.png") );

    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);
    m_poBtnDelete->setEnabled(false);

    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);
    m_poBtnDelete->setVisible(false);

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListWaitListSelectInfo", QPoint(620,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgWaitlistInfo::~cDlgWaitlistInfo()
{
    g_poPrefs->setDialogSize( "ListWaitListSelectInfo", QPoint( width(), height() ) );
}

void cDlgWaitlistInfo::setupTableView()
{
    cTracer obTracer( "cDlgWaitlistInfo::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Paneltype name" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Device usage (minutes)" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Registered at" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Paneltype name" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Device usage (minutes)" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Registered at" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 0, Qt::AscendingOrder );
    }
}

void cDlgWaitlistInfo::refreshTable()
{
    cTracer obTracer( "cDlgWaitlistInfo::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT waitlistId, waitlist.licenceId, comment, barcode, panelTypes.name, CAST((lengthCash+lengthCard)/60 AS SIGNED) as length, modified FROM waitlist, panelTypes WHERE waitlist.panelTypeId=panelTypes.panelTypeId";
    }
    else
    {
        m_qsQuery = "SELECT waitlistId AS id, comment, barcode, panelTypes.name, CAST((lengthCash+lengthCard)/60 AS SIGNED) as length, modified FROM waitlist, panelTypes WHERE waitlist.panelTypeId=panelTypes.panelTypeId";
    }

    cDlgCrud::refreshTable();
}

void cDlgWaitlistInfo::enableButtons()
{
    cTracer obTracer( "cDlgWaitlistInfo::enableButtons" );

//    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
//    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
//    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 /*&& g_obUser.isInGroup( cAccessGroup::ADMIN )*/ );
}

void cDlgWaitlistInfo::newClicked( bool )
{
}

void cDlgWaitlistInfo::editClicked( bool )
{
}

void cDlgWaitlistInfo::deleteClicked( bool )
{
}
