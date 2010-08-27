#include <QMessageBox>

#include "belenus.h"
#include "dlgaddress.h"
#include "../edit/dlgaddressedit.h"

cDlgAddress::cDlgAddress( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Address List" ) );
    setWindowIcon( QIcon("./resources/40x40_address.png") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblPatient = new QLabel( this );
    lblPatient->setObjectName( QString::fromUtf8( "lblPatient" ) );
    lblPatient->setText( tr("Patient: ") );
    horizontalLayout->addWidget( lblPatient );
    cmbPatient = new QComboBox( this );
    cmbPatient->setObjectName( QString::fromUtf8( "cmbPatient" ) );
    cmbPatient->resize( 200, 20 );
    horizontalLayout->addWidget( cmbPatient );
    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery *poQuery = NULL;

    cmbPatient->addItem( tr("<All patient>"), 0 );
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND patientId>0" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( g_obPatient.id() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
    if( poQuery ) delete poQuery;

    setupTableView();
}

cDlgAddress::~cDlgAddress()
{
}

void cDlgAddress::setPatientId( const unsigned int p_uiPatientId )
{
    for( int i=0; i<cmbPatient->count(); i++ )
    {
        if( cmbPatient->itemData( i ).toUInt() == p_uiPatientId )
        {
            cmbPatient->setCurrentIndex( i );
            break;
        }
    }
}

void cDlgAddress::setupTableView()
{
    cTracer obTracer( "cDlgAddress::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "City" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Zip" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Address" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "City" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Zip" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Address" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
    }
}

void cDlgAddress::refreshTable()
{
    cTracer obTracer( "cDlgAddress::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT addressId, licenceId, city, zip, street, active, archive FROM address";
    }
    else
    {
        m_qsQuery = "SELECT addressId AS id, city, zip, street FROM address WHERE active=1";
    }
    unsigned int uiPatientId = cmbPatient->itemData( cmbPatient->currentIndex() ).toInt();
    if( uiPatientId > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patientId=%1" ).arg( uiPatientId );
    }

    cDlgCrud::refreshTable();
}

void cDlgAddress::enableButtons()
{
    cTracer obTracer( "cDlgAddress::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgAddress::newClicked( bool )
{
    cDBAddress *poAddress = new cDBAddress;
    poAddress->createNew();

    cDlgAddressEdit  obDlgEdit( this, poAddress );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poAddress->id();
        refreshTable();
    }

    delete poAddress;
}

void cDlgAddress::editClicked( bool )
{
    cDBAddress  *poAddress = NULL;

    try
    {
        poAddress = new cDBAddress;
        poAddress->load( m_uiSelectedId );

        cDlgAddressEdit  obDlgEdit( this, poAddress );
        obDlgEdit.setWindowTitle( poAddress->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poAddress ) delete poAddress;
    }
    catch( cSevException &e )
    {
        if( poAddress ) delete poAddress;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}

void cDlgAddress::deleteClicked( bool )
{
    cDBAddress  *poAddress = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this address?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poAddress = new cDBAddress;
            poAddress->load( m_uiSelectedId );
            poAddress->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poAddress ) delete poAddress;
        }
        catch( cSevException &e )
        {
            if( poAddress ) delete poAddress;
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }
    }
}
