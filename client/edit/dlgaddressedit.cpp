#include <QPushButton>
#include <QMessageBox>

#include "dlgaddressedit.h"

cDlgAddressEdit::cDlgAddressEdit( QWidget *p_poParent, cDBAddress *p_poAddress )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Address" ) );
    setWindowIcon( QIcon("./resources/40x40_address.png") );

    m_poAddress = p_poAddress;
    if( m_poAddress )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND patientId>0" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poAddress->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
        ledCountry->setText( m_poAddress->country() );
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
            m_poAddress->setCountry( ledCountry->text() );
            m_poAddress->setRegion( ledRegion->text() );
            m_poAddress->setZip( ledZip->text() );
            m_poAddress->setCity( ledCity->text() );
            m_poAddress->setStreet( ledStreet->text() );
            m_poAddress->setPublicPlaceId( cmbStreet->itemData( cmbStreet->currentIndex() ).toUInt() );
            m_poAddress->setStreetNumber( ledStreetNumber->text() );
            m_poAddress->setFloor( ledFloor->text() );
            m_poAddress->setDoor( ledDoor->text() );

            m_poAddress->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
