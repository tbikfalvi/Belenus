#include <QPushButton>
#include <QMessageBox>

#include "dlghealthinsuranceedit.h"
#include "../db/dbzipregioncity.h"

cDlgHealthInsuranceEdit::cDlgHealthInsuranceEdit( QWidget *p_poParent, cDBHealthInsurance *p_poHealthInsurance )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Health Insurance Fund" ) );
    setWindowIcon( QIcon("./resources/40x40_healthinsurance.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
    while( poQuery->next() )
    {
        cmbRegion->addItem( poQuery->value( 0 ).toString() );
    }
    cmbRegion->setEditText( "" );

    deValidDateFrom->setDate( QDate::currentDate() );
    deValidDateTo->setDate( QDate::currentDate() );

    m_poHealthInsurance = p_poHealthInsurance;
    if( m_poHealthInsurance )
    {
        ledName->setText( m_poHealthInsurance->name() );

        ledContactPerson->setText( m_poHealthInsurance->contactPerson() );
        if( g_poPrefs->getDefaultCountry().length() > 0 && m_poHealthInsurance->id() == 0 )
        {
            ledCountry->setText( g_poPrefs->getDefaultCountry() );
        }
        else
        {
            ledCountry->setText( m_poHealthInsurance->country() );
        }
        cmbRegion->setEditText( m_poHealthInsurance->region() );
        ledZip->setText( m_poHealthInsurance->zip() );
        ledCity->setText( m_poHealthInsurance->city() );
        ledAddress->setText( m_poHealthInsurance->address() );
        ledContractId->setText( m_poHealthInsurance->contractId() );
        deValidDateFrom->setDate( QDate::fromString(m_poHealthInsurance->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poHealthInsurance->validDateTo(),"yyyy-MM-dd") );
        ( m_poHealthInsurance->discountType()==1?rbDiscountValue->setChecked(true):rbDiscountPercent->setChecked(true) );
        ledDiscount->setText( QString::number(m_poHealthInsurance->discount()) );

        if( m_poHealthInsurance->licenceId() == 0 && m_poHealthInsurance->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poHealthInsurance->licenceId() == 0 && m_poHealthInsurance->id() > 0 )
            {
                ledName->setEnabled( false );
                ledContactPerson->setEnabled( false );
                ledCountry->setEnabled( false );
                cmbRegion->setEnabled( false );
                ledZip->setEnabled( false );
                ledCity->setEnabled( false );
                pbCitySearch->setEnabled( false );
                ledAddress->setEnabled( false );
                ledContractId->setEnabled( false );
                deValidDateFrom->setEnabled( false );
                deValidDateTo->setEnabled( false );
                rbDiscountValue->setEnabled( false );
                rbDiscountPercent->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgHealthInsuranceEdit::~cDlgHealthInsuranceEdit()
{
}

void cDlgHealthInsuranceEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Health insurance fund name cannot be empty." ) );
    }

    checkRegionZipCity();

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poHealthInsurance->setLicenceId( 0 );
            }
            else
            {
                m_poHealthInsurance->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poHealthInsurance->setName( ledName->text() );
            m_poHealthInsurance->setContactPerson( ledContactPerson->text() );
            m_poHealthInsurance->setCountry( ledCountry->text() );
            m_poHealthInsurance->setRegion( cmbRegion->currentText() );
            m_poHealthInsurance->setZip( ledZip->text() );
            m_poHealthInsurance->setCity( ledCity->text() );
            m_poHealthInsurance->setAddress( ledAddress->text() );
            m_poHealthInsurance->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
            m_poHealthInsurance->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
            if( rbDiscountValue->isChecked() ) m_poHealthInsurance->setDiscountType( 1 );
            else if ( rbDiscountPercent->isChecked() ) m_poHealthInsurance->setDiscountType( 2 );
            m_poHealthInsurance->setDiscount( ledDiscount->text().toInt() );

            m_poHealthInsurance->setActive( true );
            m_poHealthInsurance->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgHealthInsuranceEdit::on_ledZip_textEdited(QString )
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
            }
            else
            {
                ledCity->setText( "" );
            }
        }
    }
}

void cDlgHealthInsuranceEdit::on_pbCitySearch_clicked()
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

void cDlgHealthInsuranceEdit::checkRegionZipCity()
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
