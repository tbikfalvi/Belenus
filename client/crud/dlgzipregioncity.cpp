#include <QMessageBox>

#include "belenus.h"
#include "dlgzipregioncity.h"
#include "../edit/dlgzipregioncityedit.h"
#include "db/dbpostponed.h"

cDlgZipRegionCity::cDlgZipRegionCity( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "ZipRegionCity List" ) );
    setWindowIcon( QIcon("./resources/40x40_zipregioncity.png") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblRegion = new QLabel( this );
    lblRegion->setObjectName( QString::fromUtf8( "lblRegion" ) );
    lblRegion->setText( tr("Region: ") );
    horizontalLayout->addWidget( lblRegion );
    cmbRegion = new QComboBox( this );
    cmbRegion->setObjectName( QString::fromUtf8( "cmbRegion" ) );
    cmbRegion->resize( 200, 20 );
    horizontalLayout->addWidget( cmbRegion );
    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery *poQuery = NULL;

    cmbRegion->addItem( tr("<All regions>"), 0 );
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
        while( poQuery->next() )
        {
            cmbRegion->addItem( poQuery->value( 0 ).toString() );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
    if( poQuery ) delete poQuery;

    setupTableView();

    connect( cmbRegion, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
}

cDlgZipRegionCity::~cDlgZipRegionCity()
{
}

void cDlgZipRegionCity::setupTableView()
{
    cTracer obTracer( "cDlgZipRegionCity::setupTableView" );

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

void cDlgZipRegionCity::refreshTable()
{
    cTracer obTracer( "cDlgZipRegionCity::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT zipRegionCityId, licenceId, region, zip, city, active, archive FROM zipRegionCity WHERE zipRegionCityId>0";
    }
    else
    {
        m_qsQuery = "SELECT zipRegionCityId AS id, region, zip, city FROM zipRegionCity WHERE zipRegionCityId>0 AND active=1";
    }

    QString qsRegionFilter = cmbRegion->itemText( cmbRegion->currentIndex() );
    if( cmbRegion->currentIndex() > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "region=\'%1\'" ).arg( qsRegionFilter );
    }

    cDlgCrud::refreshTable();
}

void cDlgZipRegionCity::enableButtons()
{
    cTracer obTracer( "cDlgZipRegionCity::enableButtons" );

    m_poBtnNew->setEnabled( true );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 );
}

void cDlgZipRegionCity::newClicked( bool )
{
    cDBZipRegionCity *poZipRegionCity = new cDBZipRegionCity;
    poZipRegionCity->createNew();

    cDlgZipRegionCityEdit  obDlgEdit( this, poZipRegionCity );
    obDlgEdit.setWindowTitle( tr( "New ZipRegionCity" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poZipRegionCity->id();
        refreshTable();
    }

    delete poZipRegionCity;
}

void cDlgZipRegionCity::editClicked( bool )
{
    cDBZipRegionCity  *poZipRegionCity = NULL;

    try
    {
        poZipRegionCity = new cDBZipRegionCity;
        poZipRegionCity->load( m_uiSelectedId );

        cDlgZipRegionCityEdit  obDlgEdit( this, poZipRegionCity );
        obDlgEdit.setWindowTitle( tr("Edit zipregioncity") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poZipRegionCity ) delete poZipRegionCity;
    }
    catch( cSevException &e )
    {
        if( poZipRegionCity ) delete poZipRegionCity;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgZipRegionCity::deleteClicked( bool )
{
    cDBZipRegionCity  *poZipRegionCity = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this ZipRegionCity?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poZipRegionCity = new cDBZipRegionCity;
            poZipRegionCity->load( m_uiSelectedId );
            poZipRegionCity->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poZipRegionCity ) delete poZipRegionCity;
        }
        catch( cSevException &e )
        {
            if( poZipRegionCity ) delete poZipRegionCity;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
