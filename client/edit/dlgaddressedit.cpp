#include <QPushButton>
#include <QMessageBox>

#include "dlgaddressedit.h"
#include "../db/dbzipregioncity.h"

cDlgAddressEdit::cDlgAddressEdit( QWidget *p_poParent, cDBAddress *p_poAddress )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Address" ) );
    setWindowIcon( QIcon("./resources/40x40_address.png") );
    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poAddress = p_poAddress;
    if( m_poAddress )
    {
        QSqlQuery *poQuery;

        cmbPatient->addItem( tr("<Independent address>"), 0 );
        if( m_poAddress->patientId() == 0 )
            cmbPatient->setCurrentIndex( 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND patientId>0" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poAddress->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
        ledName->setText( m_poAddress->name() );
        if( g_poPrefs->getDefaultCountry().length() > 0 && m_poAddress->id() == 0 )
        {
            ledCountry->setText( g_poPrefs->getDefaultCountry() );
        }
        else
        {
            ledCountry->setText( m_poAddress->country() );
        }
        ledRegion->setText( m_poAddress->region() );
        ledZip->setText( m_poAddress->zip() );
        ledCity->setText( m_poAddress->city() );
        ledStreet->setText( m_poAddress->street() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT publicPlaceId, name FROM publicPlaces WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbStreet->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poAddress->publicPlaceId() == poQuery->value( 0 ) )
                cmbStreet->setCurrentIndex( cmbStreet->count()-1 );
        }
        ledStreetNumber->setText( m_poAddress->streetNumber() );
        ledFloor->setText( m_poAddress->floor() );
        ledDoor->setText( m_poAddress->door() );
    }
}

cDlgAddressEdit::~cDlgAddressEdit()
{
}

void cDlgAddressEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledCountry->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Address cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poAddress->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
            m_poAddress->setLicenceId( g_poPrefs->getLicenceId() );
            m_poAddress->setName( ledName->text() );
            m_poAddress->setCountry( ledCountry->text() );
            m_poAddress->setRegion( ledRegion->text() );
            m_poAddress->setZip( ledZip->text() );
            m_poAddress->setCity( ledCity->text() );
            m_poAddress->setStreet( ledStreet->text() );
            m_poAddress->setPublicPlaceId( cmbStreet->itemData( cmbStreet->currentIndex() ).toUInt() );
            m_poAddress->setStreetNumber( ledStreetNumber->text() );
            m_poAddress->setFloor( ledFloor->text() );
            m_poAddress->setDoor( ledDoor->text() );
            m_poAddress->setActive( true );

            m_poAddress->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgAddressEdit::on_pbCitySearch_clicked()
{
    cDBZipRegionCity    obDBZipRegionCity;

    try
    {
        obDBZipRegionCity.loadCity( ledCity->text() );
        ledZip->setText( obDBZipRegionCity.zip() );
        ledCity->setText( obDBZipRegionCity.city() );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("ZipRegionCity city not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            QMessageBox::information( this, tr("Information"),
                                      tr("There is no city in database like\n\n\"%1\"").arg(ledCity->text()) );
            ledZip->setText( "" );
        }
    }
}

void cDlgAddressEdit::on_ledZip_textEdited(QString )
{
    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        cDBZipRegionCity    obDBZipRegionCity;

        try
        {
            obDBZipRegionCity.load( ledZip->text() );
            ledCity->setText( obDBZipRegionCity.city() );
            ledRegion->setText( obDBZipRegionCity.region() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
            else
            {
                ledCity->setText( "" );
            }
        }
    }
}
