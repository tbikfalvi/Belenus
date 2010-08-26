#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <iostream>

#include "dlgpatientedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "../db/dbpostponed.h"
#include "../db/dbzipregioncity.h"
#include "../crud/dlgattendance.h"
#include "../db/dbpatientcardtype.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent, cDBPatient *p_poPatient, cDBPostponed *p_poPostponed )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPatientEdit::cDlgPatientEdit" );

    setupUi( this );

    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbAttendances->setIcon( QIcon("./resources/40x40_attendance.png") );

    m_poPostponed = p_poPostponed;
    m_poPatient = p_poPatient;
    m_poAddress = new cDBAddress();
    m_poAddress->createNew();
    m_poPatientCard = new cDBPatientCard();
    m_poPatientCard->createNew();

    if( m_poPatient )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientOriginId, name FROM patientOrigin WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbPatientOrigin->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->patientOriginId() == poQuery->value( 0 ) )
                cmbPatientOrigin->setCurrentIndex( cmbPatientOrigin->count()-1 );
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT reasonToVisitId, name FROM reasonToVisit WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbReasonToVisit->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->reasonToVisitId() == poQuery->value( 0 ) )
                cmbReasonToVisit->setCurrentIndex( cmbReasonToVisit->count()-1 );
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT illnessGroupId, name FROM illnessGroups WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbIllnessGroup->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->illnessGroupId() == poQuery->value( 0 ) )
                cmbIllnessGroup->setCurrentIndex( cmbIllnessGroup->count()-1 );
        }

        if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            {
                tbwPatient->setToolTip( tr("You are not allowed to change this patient data.") );

                ledName->setEnabled( false );
                rbGenderFemale->setEnabled( false );
                rbGenderMale->setEnabled( false );
                ledUniqueId->setEnabled( false );
                deDateBirth->setEnabled( false );
                chkRegularCustomer->setEnabled( false );
                chkEmployee->setEnabled( false );
                chkService->setEnabled( false );
                chkHealthInsurance->setEnabled( false );
                cmbHealthInsurance->setEnabled( false );
                chkCompany->setEnabled( false );
                cmbCompany->setEnabled( false );
                chkDoctorProposed->setEnabled( false );
                cmbDoctor->setEnabled( false );
                ptComment->setEnabled( false );

                ledCountry->setEnabled( false );
                ledRegion->setEnabled( false );
                ledZip->setEnabled( false );
                ledCity->setEnabled( false );
                ledStreet->setEnabled( false );
                cmbStreet->setEnabled( false );
                ledStreetNumber->setEnabled( false );
                ledFloor->setEnabled( false );
                ledDoor->setEnabled( false );
                pbAddressAdd->setEnabled( false );
                ledPhone->setEnabled( false );
                ledEmail1->setEnabled( false );
                ledEmail2->setEnabled( false );

                ledHeight->setEnabled( false );
                ledWeight->setEnabled( false );
                cmbIllnessGroup->setEnabled( false );
                ptIllness->setEnabled( false );
                ptSymptom->setEnabled( false );
                ptMedicineCurrent->setEnabled( false );
                ptMedicineAllergy->setEnabled( false );

                cmbPatientOrigin->setEnabled( false );
                cmbReasonToVisit->setEnabled( false );

                pbSave->setEnabled( false );
                pbFinishLater->setEnabled( false );
                pbAttendances->setEnabled( false );
            }
        }
        if( m_poPatient->id() == 0 )
        {
            pbAttendances->setEnabled( false );
        }
        else
        {
            pbFinishLater->setEnabled( false );
        }

        try
        {
            m_poAddress->loadPatientSelected( m_poPatient->id() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Primary address not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
            }
        }

        ledName->setText( m_poPatient->name() );
        if( m_poPatient->gender() == 1 ) rbGenderMale->setChecked(true);
        else if( m_poPatient->gender() == 2 ) rbGenderFemale->setChecked(true);
        deDateBirth->setDate( QDate::fromString(m_poPatient->dateBirth(),"yyyy-MM-dd") );
        ledUniqueId->setText( m_poPatient->uniqueId() );
        if( deDateBirth->date().year() != 1900 )
        {
            QDate   qdDays = deDateBirth->date();
            QDate   qdAge  = QDate( 1900, 1, 1 );

            qdAge = qdAge.addDays( qdDays.daysTo(QDate::currentDate()) );

            ledAge->setText( QString::number(qdAge.year()-1900) );
        }
        try
        {
            m_poPatientCard->loadPatient( m_poPatient->id() );
            ledBarcode->setText( m_poPatientCard->barcode() );

            cDBPatientCardType  obDBPatientCardType;

            obDBPatientCardType.load( m_poPatientCard->patientCardTypeId() );
            ledPatientcardType->setText( obDBPatientCardType.name() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Patient id not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
            }
        }
        chkRegularCustomer->setChecked( m_poPatient->regularCustomer() );
        chkEmployee->setChecked( m_poPatient->employee() );
        chkService->setChecked( m_poPatient->service() );
        chkHealthInsurance->setChecked( m_poPatient->healthInsurance() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT healthInsuranceId, name FROM healthInsurances WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbHealthInsurance->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->healthInsuranceId() == poQuery->value( 0 ) )
                cmbHealthInsurance->setCurrentIndex( cmbHealthInsurance->count()-1 );
        }
        chkCompany->setChecked( m_poPatient->company() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT companyId, name FROM companies WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbCompany->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->companyId() == poQuery->value( 0 ) )
                cmbCompany->setCurrentIndex( cmbCompany->count()-1 );
        }
        chkDoctorProposed->setChecked( m_poPatient->doctorProposed() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT doctorId, name FROM doctors WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbDoctor->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->doctorId() == poQuery->value( 0 ) )
                cmbDoctor->setCurrentIndex( cmbDoctor->count()-1 );
        }

        ledHeight->setText( QString::number(m_poPatient->height()) );
        ledWeight->setText( QString::number(m_poPatient->weight()) );
        ptIllness->setPlainText( m_poPatient->illnesses() );
        ptSymptom->setPlainText( m_poPatient->symptoms() );
        ptMedicineCurrent->setPlainText( m_poPatient->medicineCurrent() );
        ptMedicineAllergy->setPlainText( m_poPatient->medicineAllergy() );
        ledCountry->setText( m_poAddress->country() );
        ledRegion->setText( m_poAddress->region() );
        ledCity->setText( m_poAddress->city() );
        ledZip->setText( m_poAddress->zip() );
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
        ledPhone->setText( m_poPatient->phone() );
        ledEmail1->setText( m_poPatient->email().left(m_poPatient->email().indexOf("@")) );
        ledEmail2->setText( m_poPatient->email().mid(m_poPatient->email().indexOf("@")+1) );
        ptComment->setPlainText( m_poPatient->comment() );
    }

    if( g_poPrefs->getDefaultCountry().length() > 0 && m_poPatient->id() == 0 )
    {
        ledCountry->setText( g_poPrefs->getDefaultCountry() );
    }
}

cDlgPatientEdit::~cDlgPatientEdit()
{
    cTracer obTrace( "cDlgPatientEdit::~cDlgPatientEdit" );

    if( m_poAddress )
        delete m_poAddress;
}


void cDlgPatientEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    bool  boSkipErrorMessages = false;

    if( ledName->text() == "" )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Patient name cannot be empty." ) );
        ledName->setFocus();
        return;
    }
    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Patient gender must be selected.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( deDateBirth->date().year() == 1900 &&
        deDateBirth->date().month() == 1 &&
        deDateBirth->date().day() == 1 &&
        !boSkipErrorMessages )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Patient date of birth must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( ledUniqueId->text() == "" && !boSkipErrorMessages )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Patient unique identification value must be given.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }

    if( boCanBeSaved )
    {
        if( SavePatientData() )
            QDialog::accept();
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to add new patient to database "
                                      "\nand fill mandatory data later?"),
                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
        {
            on_pbFinishLater_clicked();
        }
    }
}

void cDlgPatientEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientEdit::on_pbFinishLater_clicked()
{
    if( ledName->text() == "" )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Patient name cannot be empty." ) );
        ledName->setFocus();
        return;
    }

    QSqlQuery  *poQuery = NULL;
    try
    {
        if( SavePatientData() )
        {
            if( m_poPostponed == NULL )
            {
                m_poPostponed = new cDBPostponed();

                m_poPostponed->createNew();
                m_poPostponed->setPatientId( m_poPatient->id() );
                m_poPostponed->save();
                delete m_poPostponed;
            }
            QDialog::reject();
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}

bool cDlgPatientEdit::SavePatientData()
{
    bool bRet = false;

    try
    {
        if( checkIndependent->isChecked() )
        {
            m_poPatient->setLicenceId( 0 );
        }
        else
        {
            m_poPatient->setLicenceId( g_poPrefs->getLicenceId() );
        }
        m_poPatient->setName( ledName->text() );
        if( rbGenderMale->isChecked() )
            m_poPatient->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poPatient->setGender( 2 );
        m_poPatient->setUniqueId( ledUniqueId->text() );
        m_poPatient->setDateBirth( deDateBirth->date().toString("yyyy-MM-dd") );
        m_poPatient->setRegularCustomer( chkRegularCustomer->isChecked() );
        m_poPatient->setEmployee( chkEmployee->isChecked() );
        m_poPatient->setService( chkService->isChecked() );
        m_poPatient->setHealthInsurance( chkHealthInsurance->isChecked() );
        if( chkHealthInsurance->isChecked() )
            m_poPatient->setHealthInsuranceId( cmbHealthInsurance->itemData( cmbHealthInsurance->currentIndex() ).toUInt() );
        else
            m_poPatient->setHealthInsuranceId( 0 );
        m_poPatient->setCompany( chkCompany->isChecked() );
        if( chkCompany->isChecked() )
            m_poPatient->setCompanyId( cmbCompany->itemData( cmbCompany->currentIndex() ).toUInt() );
        else
            m_poPatient->setCompanyId( 0 );
        m_poPatient->setDoctorProposed( chkDoctorProposed->isChecked() );
        if( chkDoctorProposed->isChecked() )
            m_poPatient->setDoctorId( cmbDoctor->itemData( cmbDoctor->currentIndex() ).toUInt() );
        else
            m_poPatient->setDoctorId( 0 );
        m_poPatient->setComment( ptComment->toPlainText() );

        m_poAddress->setPatientId( m_poPatient->id() );
        m_poAddress->setCountry( ledCountry->text() );
        m_poAddress->setRegion( ledRegion->text() );
        m_poAddress->setZip( ledZip->text() );
        m_poAddress->setCity( ledCity->text() );
        m_poAddress->setStreet( ledStreet->text() );
        m_poAddress->setPublicPlaceId( cmbStreet->itemData( cmbStreet->currentIndex() ).toUInt() );
        m_poAddress->setStreetNumber( ledStreetNumber->text() );
        m_poAddress->setFloor( ledFloor->text() );
        m_poAddress->setDoor( ledDoor->text() );
        m_poPatient->setPhone( ledPhone->text() );
        m_poPatient->setEmail( ledEmail1->text()+QString("@")+ledEmail2->text() );

        m_poPatient->setHeight( ledHeight->text().toInt() );
        m_poPatient->setWeight( ledWeight->text().toInt() );
        m_poPatient->setIllnesses( ptIllness->toPlainText() );
        m_poPatient->setSymptoms( ptSymptom->toPlainText() );
        m_poPatient->setMedicineCurrent( ptMedicineCurrent->toPlainText() );
        m_poPatient->setMedicineAllergy( ptMedicineAllergy->toPlainText() );

        m_poPatient->setPatientOriginId( cmbPatientOrigin->itemData( cmbPatientOrigin->currentIndex() ).toUInt() );
        m_poPatient->setReasonToVisitId( cmbReasonToVisit->itemData( cmbReasonToVisit->currentIndex() ).toUInt() );

        m_poAddress->save();
        m_poPatient->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }

    return bRet;
}

void cDlgPatientEdit::on_ledZip_textEdited(QString )
{
    if( ledZip->text().left(1) == "1" )
    {
        cDBZipRegionCity    obDBZipRegionCity;

        obDBZipRegionCity.load( "1" );
        ledCity->setText( obDBZipRegionCity.city() );
    }
    else if( ledZip->text().length() == g_poPrefs->getZipLength() )
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
                g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
            }
            else
            {
                ledCity->setText( "" );
            }
        }
    }
}

void cDlgPatientEdit::on_pbCitySearch_clicked()
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
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }
        else
        {
            QMessageBox::information( this, tr("Information"),
                                      tr("There is no city in database like\n\n\"%1\"").arg(ledCity->text()) );
            ledZip->setText( "" );
        }
    }
}

void cDlgPatientEdit::on_pbAttendances_clicked()
{
    cDlgAttendance  obDlgAttendance( this );

    obDlgAttendance.setPatientId( m_poPatient->id() );

    obDlgAttendance.exec();
}

void cDlgPatientEdit::on_pbHealthInsurance_clicked()
{

}

void cDlgPatientEdit::on_pbCompany_clicked()
{

}

void cDlgPatientEdit::on_pbDoctor_clicked()
{

}
