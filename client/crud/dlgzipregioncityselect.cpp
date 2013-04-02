#include <QMessageBox>

#include "belenus.h"
#include "dlgzipregioncityselect.h"
#include "dlg/dlgprogress.h"

cDlgZipRegionCitySelect::cDlgZipRegionCitySelect( QWidget *p_poParent, QString p_qsCity )
    : cDlgZipRegionCity( p_poParent )
{
    setWindowTitle( tr( "ZipRegionCity List" ) );
    setWindowIcon( QIcon("./resources/40x40_zipregioncity.png") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);

    if( p_qsCity.length() > 0)
    {
        m_qsCity = p_qsCity.replace( QString("*"),QString("\%"));
    }

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

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListZipRegionCitySelect", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgZipRegionCitySelect::~cDlgZipRegionCitySelect()
{
    g_poPrefs->setDialogSize( "ListZipRegionCitySelect", QPoint( width(), height() ) );
}

void cDlgZipRegionCitySelect::setupTableView()
{
    cTracer obTracer( "cDlgZipRegionCitySelect::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Region" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Zip" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "City" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 3, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Region" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Zip" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "City" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
}

void cDlgZipRegionCitySelect::refreshTable()
{
    cTracer obTracer( "cDlgZipRegionCitySelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT zipRegionCityId, licenceId, region, zip, city, active, archive FROM zipRegionCity WHERE zipRegionCityId>0";
    }
    else
    {
        m_qsQuery = "SELECT zipRegionCityId AS id, region, zip, city FROM zipRegionCity WHERE zipRegionCityId>0 AND active=1";
    }

    if( m_qsCity.length() > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "city LIKE \'%1\'" ).arg( m_qsCity );
    }

    cDlgCrud::refreshTable();
}

void cDlgZipRegionCitySelect::enableButtons()
{
    cTracer obTracer( "cDlgZipRegionCitySelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgZipRegionCitySelect::newClicked( bool ) {}
void cDlgZipRegionCitySelect::editClicked( bool ) {}
void cDlgZipRegionCitySelect::deleteClicked( bool ) {}

void cDlgZipRegionCitySelect::on_pbSelect_clicked()
{
    cDlgZipRegionCity::accept();
}

unsigned int cDlgZipRegionCitySelect::selected()
{
    return m_uiSelectedId;
}
