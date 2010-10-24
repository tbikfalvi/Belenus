#include <QPushButton>
#include <QMessageBox>

#include "dlgcompanyedit.h"
#include "../db/dbzipregioncity.h"

cDlgCompanyEdit::cDlgCompanyEdit( QWidget *p_poParent, cDBCompany *p_poCompany )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Company" ) );
    setWindowIcon( QIcon("./resources/40x40_company.png") );

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

    m_poCompany = p_poCompany;
    if( m_poCompany )
    {
        ledName->setText( m_poCompany->name() );
        if( m_poCompany->licenceId() == 0 && m_poCompany->id() > 0 )
            checkIndependent->setChecked( true );

        ledContactPerson->setText( m_poCompany->contactPerson() );
        if( g_poPrefs->getDefaultCountry().length() > 0 && m_poCompany->id() == 0 )
        {
            ledCountry->setText( g_poPrefs->getDefaultCountry() );
        }
        else
        {
            ledCountry->setText( m_poCompany->country() );
        }
        cmbRegion->setEditText( m_poCompany->region() );
        ledZip->setText( m_poCompany->zip() );
        ledCity->setText( m_poCompany->city() );
        ledAddress->setText( m_poCompany->address() );
        ledContractId->setText( m_poCompany->contractId() );
        deValidDateFrom->setDate( QDate::fromString(m_poCompany->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poCompany->validDateTo(),"yyyy-MM-dd") );
        ( m_poCompany->discountType()==1?rbDiscountValue->setChecked(true):rbDiscountPercent->setChecked(true) );
        ledDiscount->setText( QString::number(m_poCompany->discount()) );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poCompany->licenceId() == 0 && m_poCompany->id() > 0 )
            {
                ledName->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }
}

cDlgCompanyEdit::~cDlgCompanyEdit()
{
}

void cDlgCompanyEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Company name cannot be empty." ) );
    }

    checkRegionZipCity();

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poCompany->setLicenceId( 0 );
            }
            else
            {
                m_poCompany->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poCompany->setName( ledName->text() );
            m_poCompany->setContactPerson( ledContactPerson->text() );
            m_poCompany->setCountry( ledCountry->text() );
            m_poCompany->setRegion( cmbRegion->currentText() );
            m_poCompany->setZip( ledZip->text() );
            m_poCompany->setCity( ledCity->text() );
            m_poCompany->setAddress( ledAddress->text() );
            m_poCompany->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
            m_poCompany->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
            if( rbDiscountValue->isChecked() ) m_poCompany->setDiscountType( 1 );
            else if ( rbDiscountPercent->isChecked() ) m_poCompany->setDiscountType( 2 );
            m_poCompany->setDiscount( ledDiscount->text().toInt() );

            m_poCompany->setActive( true );
            m_poCompany->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgCompanyEdit::on_ledZip_textEdited(QString )
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

void cDlgCompanyEdit::on_pbCitySearch_clicked()
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

void cDlgCompanyEdit::checkRegionZipCity()
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
