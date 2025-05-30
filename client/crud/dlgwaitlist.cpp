#include <QMessageBox>

#include "belenus.h"
#include "dlgwaitlist.h"
#include "../db/dbwaitlist.h"
#include "../db/dbpatientcardunits.h"

cDlgWaitlist::cDlgWaitlist( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Waiting guests" ) );
    setWindowIcon( QIcon("./resources/40x40_device_later.png") );

    m_poBtnClose->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);
    m_poBtnDelete->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);
    m_poBtnDelete->setVisible(false);

    pbSelect = new QPushButton( tr( "Select" ), this );
    pbSelect->setObjectName( QString::fromUtf8( "pbSelect" ) );
    pbSelect->setIconSize( QSize(20, 20) );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListWaitListSelect", QPoint(620,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgWaitlist::~cDlgWaitlist()
{
    g_poPrefs->setDialogSize( "ListWaitListSelect", QPoint( width(), height() ) );
}

void cDlgWaitlist::setupTableView()
{
    cTracer obTracer( "cDlgWaitlist::setupTableView" );

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
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Paneltype name" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Device usage (minutes)" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 0, Qt::AscendingOrder );
    }
}

void cDlgWaitlist::refreshTable()
{
    cTracer obTracer( "cDlgWaitlist::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT waitlistId, waitlist.licenceId, comment, barcode, panelTypes.name, CAST((lengthCash+lengthCard)/60 AS SIGNED) as length FROM waitlist, panelTypes WHERE waitlist.panelTypeId=panelTypes.panelTypeId";
    }
    else
    {
        m_qsQuery = "SELECT waitlistId AS id, comment, barcode, panelTypes.name, CAST((lengthCash+lengthCard)/60 AS SIGNED) as length FROM waitlist, panelTypes WHERE waitlist.panelTypeId=panelTypes.panelTypeId";
    }

    cDlgCrud::refreshTable();
}

void cDlgWaitlist::enableButtons()
{
    cTracer obTracer( "cDlgWaitlist::enableButtons" );

//    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
//    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
//    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 /*&& g_obUser.isInGroup( cAccessGroup::ADMIN )*/ );
}

void cDlgWaitlist::newClicked( bool )
{
}

void cDlgWaitlist::editClicked( bool )
{
}

void cDlgWaitlist::deleteClicked( bool )
{
}

void cDlgWaitlist::on_pbSelect_clicked()
{
    QDialog::accept();
}
