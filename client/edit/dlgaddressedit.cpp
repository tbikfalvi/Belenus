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

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
    while( poQuery->next() )
    {
        cmbRegion->addItem( poQuery->value( 0 ).toString() );
    }
    cmbRegion->setEditText( "" );

    m_poAddress = p_poAddress;
    if( m_poAddress )
    {
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
        cmbRegion->setEditText( m_poAddress->region() );
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

    checkRegionZipCity();

    if( boCanBeSaved )
    {
        try
        {
            m_poAddress->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
            m_poAddress->setLicenceId( g_poPrefs->getLicenceId() );
            m_poAddress->setName( ledName->text() );
            m_poAddress->setCountry( ledCountry->text() );
            m_poAddress->setRegion( cmbRegion->currentText() );
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
            g_obGen.showTrayError( e.what() );
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
            g_obGen.showTrayError( e.what() );
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
            cmbRegion->setEditText( obDBZipRegionCity.region() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
            else
            {
                ledCity->setText( "" );
            }
        }
    }
}

void cDlgAddressEdit::checkRegionZipCity()
{
    cDBZipRegionCity    obDBCheck;

    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        try
        {
            obDBCheck.load( ledZip->text() );

            if( cmbRegion->currentText() != obDBCheck.region() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The defined zip code already saved into the database with\n\n"
                                              "Region : %1\n\n"
                                              "Do you want to overwrite the region entered with this one?").arg(obDBCheck.region()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    cmbRegion->setEditText( obDBCheck.region() );
                }
            }
            if( ledCity->text() != obDBCheck.city() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The defined zip code already saved into the database with\n\n"
                                              "City : %1\n\n"
                                              "Do you want to overwrite the city entered with this one?").arg(obDBCheck.city()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    ledCity->setText( obDBCheck.city() );
                }
            }
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
            else
            {
                try
                {
                    obDBCheck.loadCity( ledCity->text() );

                    if( cmbRegion->currentText() != obDBCheck.region() )
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined city already assigned to a different region\n\n"
                                                      "Region : %1\n\n"
                                                      "Do you want to correct the defined region?").arg(obDBCheck.region()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            cmbRegion->setEditText( obDBCheck.region() );
                        }
                    }

                    if( ledZip->text() != obDBCheck.zip() )
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined city already assigned to a different zip code\n\n"
                                                      "Zip : %1\n\n"
                                                      "Do you save the following data\n\n"
                                                      "Region : %2\n"
                                                      "Zip : %3\n"
                                                      "City : %4\n\n"
                                                      "as new into the database?").arg(obDBCheck.zip()).arg(cmbRegion->currentText()).arg(ledZip->text()).arg(ledCity->text()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            obDBCheck.createNew();
                            obDBCheck.setLicenceId( g_poPrefs->getLicenceId() );
                            obDBCheck.setRegion( cmbRegion->currentText() );
                            obDBCheck.setZip( ledZip->text() );
                            obDBCheck.setCity( ledCity->text() );
                            obDBCheck.setActive( true );
                            obDBCheck.save();
                        }
                    }
                }
                catch( cSevException &e )
                {
                    if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
                    {
                        g_obLogger(e.severity()) << e.what() << EOM;
                        g_obGen.showTrayError( e.what() );
                    }
                    else
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined data did not found in the database\n\n"
                                                      "Region : %1\n"
                                                      "Zip : %2\n"
                                                      "City : %3\n\n"
                                                      "Do you want to save them as new into the database?").arg(cmbRegion->currentText()).arg(ledZip->text()).arg(ledCity->text()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            obDBCheck.createNew();
                            obDBCheck.setLicenceId( g_poPrefs->getLicenceId() );
                            obDBCheck.setRegion( cmbRegion->currentText() );
                            obDBCheck.setZip( ledZip->text() );
                            obDBCheck.setCity( ledCity->text() );
                            obDBCheck.setActive( true );
                            obDBCheck.save();
                        }
                    }
                }
            }
        }
    }
}
