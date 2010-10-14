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
#include "../crud/dlgaddress.h"
#include "../crud/dlgcompany.h"
#include "../crud/dlgdoctor.h"
#include "../crud/dlghealthinsurance.h"
#include "../crud/dlgzipregioncityselect.h"
#include "../db/dbdoctor.h"
#include "../crud/dlgillnessgroup.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent, cDBPatient *p_poPatient, cDBPostponed *p_poPostponed )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPatientEdit::cDlgPatientEdit" );

    setupUi( this );

    m_bInit = true;

    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbAttendances->setIcon( QIcon("./resources/40x40_attendance.png") );
    pbAddressAdd->setIcon( QIcon("./resources/40x40_address.png") );
    pbCitySearch->setIcon( QIcon("./resources/40x40_search.png") );
    pbVerifyAddress->setIcon( QIcon("./resources/40x40_question.png") );

    tbwPatient->setCurrentIndex( 0 );

    m_poPostponed = p_poPostponed;
    m_poPatient = p_poPatient;
    m_poAddress = new cDBAddress();
    m_poAddress->createNew();
    m_poPatientCard = new cDBPatientCard();
    m_poPatientCard->createNew();

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
    while( poQuery->next() )
    {
        cmbRegion->addItem( poQuery->value( 0 ).toString() );
    }
    cmbRegion->setEditText( "" );

    if( m_poPatient )
    {
        cmbPatientOrigin->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientOriginId, name FROM patientOrigin WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbPatientOrigin->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->patientOriginId() == poQuery->value( 0 ) )
                cmbPatientOrigin->setCurrentIndex( cmbPatientOrigin->count()-1 );
        }
        cmbReasonToVisit->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT reasonToVisitId, name FROM reasonToVisit WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbReasonToVisit->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->reasonToVisitId() == poQuery->value( 0 ) )
                cmbReasonToVisit->setCurrentIndex( cmbReasonToVisit->count()-1 );
        }
        FillIllnessGroupCombo();
/*        cmbIllnessGroup->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT illnessGroupId, name FROM illnessGroups WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbIllnessGroup->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->illnessGroupId() == poQuery->value( 0 ) )
                cmbIllnessGroup->setCurrentIndex( cmbIllnessGroup->count()-1 );
        }*/

        if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            checkIndependent->setChecked( true );

        if( m_poPatient->id() == 0 )
        {
            pbAttendances->setEnabled( false );
        }
        else
        {
            pbFinishLater->setEnabled( false );
        }

        ledName->setText( m_poPatient->name() );
        if( m_poPatient->gender() == 1 ) rbGenderMale->setChecked(true);
        else if( m_poPatient->gender() == 2 ) rbGenderFemale->setChecked(true);
        deDateBirth->setDate( QDate::fromString(m_poPatient->dateBirth(),"yyyy-MM-dd") );
        ledUniqueId->setText( m_poPatient->uniqueId() );
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
                g_obLogger(e.severity()) << e.what() << EOM;
            }
        }
        chkRegularCustomer->setChecked( m_poPatient->regularCustomer() );
        chkEmployee->setChecked( m_poPatient->employee() );
        chkService->setChecked( m_poPatient->service() );
        chkHealthInsurance->setChecked( m_poPatient->healthInsurance() );
        FillHealthInsuranceCombo();
        chkCompany->setChecked( m_poPatient->company() );
        FillCompanyCombo();
        chkDoctorProposed->setChecked( m_poPatient->doctorProposed() );
        FillDoctorTypeCombo();
        FillDoctorCombo();

        ledHeight->setText( QString::number(m_poPatient->height()) );
        ledWeight->setText( QString::number(m_poPatient->weight()) );
        ptIllness->setPlainText( m_poPatient->illnesses() );
        ptSymptom->setPlainText( m_poPatient->symptoms() );
        ptMedicineCurrent->setPlainText( m_poPatient->medicineCurrent() );
        ptMedicineAllergy->setPlainText( m_poPatient->medicineAllergy() );
        FillDefaultAddress();
        FillPhoneNumber();
        ledEmail1->setText( m_poPatient->email().left(m_poPatient->email().indexOf("@")) );
        ledEmail2->setText( m_poPatient->email().mid(m_poPatient->email().indexOf("@")+1) );
        ptComment->setPlainText( m_poPatient->comment() );

        if( ledZip->text().length() )
            pbCitySearch->setEnabled( false );
        else
            pbCitySearch->setEnabled( true );

        ( m_poPatient->discountType() == 1 ? rbDiscountValue->setChecked( true ) : rbDiscountPercent->setChecked( true) );

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
                cmbRegion->setEnabled( false );
                ledZip->setEnabled( false );
                ledCity->setEnabled( false );
                ledStreet->setEnabled( false );
                cmbStreet->setEnabled( false );
                ledStreetNumber->setEnabled( false );
                ledFloor->setEnabled( false );
                ledDoor->setEnabled( false );
                pbAddressAdd->setEnabled( false );
                ledPhoneCountry->setEnabled( false );
                ledPhoneRegion->setEnabled( false );
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
    }

    if( g_poPrefs->getDefaultCountry().length() > 0 && m_poPatient->id() == 0 )
    {
        ledCountry->setText( g_poPrefs->getDefaultCountry() );
    }

    if( poQuery ) delete poQuery;

    m_bInit = false;
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
        g_obLogger(e.severity()) << e.what() << EOM;
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

        QString  qsQuery = QString( "UPDATE address SET primaryAddress = 0 WHERE patientId = %1" ).arg( m_poPatient->id() );
        g_poDB->executeQTQuery( qsQuery );

        checkRegionZipCity();

        m_poAddress->setLicenceId( g_poPrefs->getLicenceId() );
        m_poAddress->setName( m_poPatient->name() );
        m_poAddress->setPrimaryAddress( true );
        m_poAddress->setActive( true );
        m_poAddress->setCountry( ledCountry->text() );
        m_poAddress->setRegion( cmbRegion->currentText() );
        m_poAddress->setZip( ledZip->text() );
        m_poAddress->setCity( ledCity->text() );
        m_poAddress->setStreet( ledStreet->text() );
        m_poAddress->setPublicPlaceId( cmbStreet->itemData( cmbStreet->currentIndex() ).toUInt() );
        m_poAddress->setStreetNumber( ledStreetNumber->text() );
        m_poAddress->setFloor( ledFloor->text() );
        m_poAddress->setDoor( ledDoor->text() );
        m_poPatient->setPhone( QString("%1 %2 %3").arg(ledPhoneCountry->text()).arg(ledPhoneRegion->text()).arg(ledPhone->text()) );
        m_poPatient->setEmail( ledEmail1->text()+QString("@")+ledEmail2->text() );

        m_poPatient->setHeight( ledHeight->text().toInt() );
        m_poPatient->setWeight( ledWeight->text().toInt() );
        m_poPatient->setIllnessGroupId( cmbIllnessGroup->itemData( cmbIllnessGroup->currentIndex() ).toUInt() );
        m_poPatient->setIllnesses( ptIllness->toPlainText() );
        m_poPatient->setSymptoms( ptSymptom->toPlainText() );
        m_poPatient->setMedicineCurrent( ptMedicineCurrent->toPlainText() );
        m_poPatient->setMedicineAllergy( ptMedicineAllergy->toPlainText() );

        m_poPatient->setPatientOriginId( cmbPatientOrigin->itemData( cmbPatientOrigin->currentIndex() ).toUInt() );
        m_poPatient->setReasonToVisitId( cmbReasonToVisit->itemData( cmbReasonToVisit->currentIndex() ).toUInt() );

        m_poPatient->save();

        m_poAddress->setPatientId( m_poPatient->id() );
        m_poAddress->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return bRet;
}

void cDlgPatientEdit::on_ledZip_textEdited(QString )
{
    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        cDBZipRegionCity    obDBZipRegionCity;

        try
        {
            obDBZipRegionCity.load( ledZip->text() );
            cmbRegion->setEditText( obDBZipRegionCity.region() );
            ledCity->setText( obDBZipRegionCity.city() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
//            else
//            {
//                cmbRegion->setEditText( "" );
//                ledCity->setText( "" );
//            }
        }
    }
    if( ledZip->text().length() )
        pbCitySearch->setEnabled( false );
    else
        pbCitySearch->setEnabled( true );
}

void cDlgPatientEdit::on_pbCitySearch_clicked()
{
    cDBZipRegionCity    obDBZipRegionCity;

    try
    {
        cDlgZipRegionCitySelect obDlgZipRegionCitySelect( this, ledCity->text() );

        if( obDlgZipRegionCitySelect.exec() == QDialog::Accepted )
        {
            obDBZipRegionCity.load( obDlgZipRegionCitySelect.selected() );
            ledCity->setText( obDBZipRegionCity.city() );
            cmbRegion->setEditText( obDBZipRegionCity.region() );
            ledZip->setText( obDBZipRegionCity.zip() );
        }
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

void cDlgPatientEdit::on_pbAttendances_clicked()
{
    cDlgAttendance  obDlgAttendance( this );

    obDlgAttendance.setPatientId( m_poPatient->id() );

    obDlgAttendance.exec();
}

void cDlgPatientEdit::on_pbHealthInsurance_clicked()
{
    cDlgHealthInsurance  obDlgHealthInsurance( this );

    obDlgHealthInsurance.exec();
    m_poPatient->setHealthInsuranceId( cmbHealthInsurance->itemData(cmbHealthInsurance->currentIndex()).toUInt() );
    FillHealthInsuranceCombo();
    updateDiscount();
}

void cDlgPatientEdit::on_pbCompany_clicked()
{
    cDlgCompany  obDlgCompany( this );

    obDlgCompany.exec();
    m_poPatient->setCompanyId( cmbCompany->itemData( cmbCompany->currentIndex() ).toUInt() );
    FillCompanyCombo();
    updateDiscount();
}

void cDlgPatientEdit::on_pbDoctor_clicked()
{
    cDlgDoctor  obDlgDoctor( this );

    obDlgDoctor.exec();
    m_poPatient->setDoctorId( cmbDoctor->itemData( cmbDoctor->currentIndex() ).toUInt() );
    FillDoctorCombo();
    updateDiscount();
}

void cDlgPatientEdit::on_pbAddressAdd_clicked()
{
    cDlgAddress     obDlgAddress( this );

    obDlgAddress.setPatientId( m_poPatient->id() );
    obDlgAddress.exec();
    FillDefaultAddress();
}

void cDlgPatientEdit::FillHealthInsuranceCombo()
{
    QSqlQuery *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT healthInsuranceId, name FROM healthInsurances WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    cmbHealthInsurance->clear();
    cmbHealthInsurance->addItem( tr("<Not selected>"), 0 );
    while( poQuery->next() )
    {
        cmbHealthInsurance->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poPatient->healthInsuranceId() == poQuery->value( 0 ) )
            cmbHealthInsurance->setCurrentIndex( cmbHealthInsurance->count()-1 );
    }
}

void cDlgPatientEdit::FillIllnessGroupCombo()
{
    QSqlQuery *poQuery;

    cmbIllnessGroup->clear();
    cmbIllnessGroup->addItem( tr("<Not selected>"), 0 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT illnessGroupId, name FROM illnessGroups WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    while( poQuery->next() )
    {
        cmbIllnessGroup->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poPatient->illnessGroupId() == poQuery->value( 0 ) )
            cmbIllnessGroup->setCurrentIndex( cmbIllnessGroup->count()-1 );
    }
}

void cDlgPatientEdit::FillCompanyCombo()
{
    QSqlQuery *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT companyId, name FROM companies WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    cmbCompany->clear();
    cmbCompany->addItem( tr("<Not selected>"), 0 );
    while( poQuery->next() )
    {
        cmbCompany->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poPatient->companyId() == poQuery->value( 0 ) )
            cmbCompany->setCurrentIndex( cmbCompany->count()-1 );
    }
}

void cDlgPatientEdit::on_cmbDoctorType_currentIndexChanged(int /*index*/)
{
    cTracer obTrace( "cDlgPatientEdit::on_cmbDoctorType_currentIndexChanged" );

    if( m_bInit )
        return;

    FillDoctorCombo();

    if( cmbDoctor->currentIndex() > 0 )
        chkDoctorProposed->setChecked( true );
    else
        chkDoctorProposed->setChecked( false );
    updateDiscount();
}

void cDlgPatientEdit::on_cmbDoctor_currentIndexChanged( int )
{
    cTracer obTrace( "cDlgPatientEdit::on_cmbDoctor_currentIndexChanged" );

    if( !m_bInit && cmbDoctor->count() > 0 )
        m_poPatient->setDoctorId( cmbDoctor->itemData( cmbDoctor->currentIndex() ).toUInt() );

    g_obLogger(cSeverity::INFO) << "on_cmbDoctor_currentIndexChanged> DoctorId: " << QString::number(m_poPatient->doctorId()) << EOM;

    cDBDoctor   obDBDoctor;

    obDBDoctor.createNew();
    try
    {
        if( m_poPatient->doctorId() > 0 )
        {
            obDBDoctor.load( m_poPatient->doctorId() );

            for( int i=0; i<cmbDoctorType->count(); i++ )
            {
                if( cmbDoctorType->itemData( i ).toUInt() == obDBDoctor.doctorTypeId() )
                {
                    cmbDoctorType->setCurrentIndex( i );
                    break;
                }
            }
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Doctor id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    if( cmbDoctor->currentIndex() > 0 )
        chkDoctorProposed->setChecked( true );
    else
        chkDoctorProposed->setChecked( false );
    updateDiscount();
}

void cDlgPatientEdit::FillDoctorTypeCombo()
{
    cTracer obTrace( "cDlgPatientEdit::FillDoctorTypeCombo" );

    QSqlQuery *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT doctorTypeId, name FROM doctorTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    cmbDoctorType->clear();
    cmbDoctorType->addItem( tr("<Not selected>"), 0 );
    while( poQuery->next() )
    {
        cmbDoctorType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }
}

void cDlgPatientEdit::FillDoctorCombo()
{
    cTracer obTrace( "cDlgPatientEdit::FillDoctorCombo" );

    QSqlQuery   *poQuery;
    QString      qsQuery = QString( "SELECT doctorId, doctorLicence FROM doctors WHERE active=1 AND archive<>\"DEL\"" );

    if( cmbDoctorType->currentIndex() > 0 )
    {
        qsQuery += QString( " AND doctorTypeId=%1" ).arg(cmbDoctorType->itemData( cmbDoctorType->currentIndex() ).toUInt());
    }
    else if( m_poPatient->doctorId() > 0 )
    {
        qsQuery += QString( " AND doctorTypeId=(SELECT doctorTypeId FROM doctors WHERE doctorId=%1)" ).arg(m_poPatient->doctorId());
    }
    qsQuery += QString( " ORDER BY doctorLicence" );

    poQuery = g_poDB->executeQTQuery( qsQuery );
    m_bInit = true;
    cmbDoctor->clear();
    cmbDoctor->addItem( tr("<Not selected>"), 0 );
    m_bInit = false;
    while( poQuery->next() )
    {
        cmbDoctor->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poPatient->doctorId() == poQuery->value( 0 ) )
            cmbDoctor->setCurrentIndex( cmbDoctor->count()-1 );
    }
}

void cDlgPatientEdit::on_cmbHealthInsurance_currentIndexChanged(int)
{
    if( cmbHealthInsurance->currentIndex() > 0 )
        chkHealthInsurance->setChecked( true );
    else
        chkHealthInsurance->setChecked( false );
    updateDiscount();
}

void cDlgPatientEdit::on_cmbCompany_currentIndexChanged(int)
{
    if( cmbCompany->currentIndex() > 0 )
        chkCompany->setChecked( true );
    else
        chkCompany->setChecked( false );
    updateDiscount();
}

void cDlgPatientEdit::on_deDateBirth_dateChanged(QDate)
{
    if( deDateBirth->date().year() != 1900 )
    {
        QDate   qdDays = deDateBirth->date();
        QDate   qdAge  = QDate( 1900, 1, 1 );

        qdAge = qdAge.addDays( qdDays.daysTo(QDate::currentDate()) );

        ledAge->setText( QString::number(qdAge.year()-1900) );
    }
}

void cDlgPatientEdit::FillDefaultAddress()
{
    if( m_poPatient->id() == 0 )
        return;

    QSqlQuery *poQuery;

    try
    {
        m_poAddress->loadPatientSelected( m_poPatient->id() );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Primary address not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    ledCountry->setText( m_poAddress->country() );
    cmbRegion->setEditText( m_poAddress->region() );
    ledCity->setText( m_poAddress->city() );
    ledZip->setText( m_poAddress->zip() );
    ledStreet->setText( m_poAddress->street() );
    cmbStreet->clear();
    poQuery = g_poDB->executeQTQuery( QString( "SELECT publicPlaceId, name FROM publicPlaces WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    while( poQuery->next() )
    {
        cmbStreet->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poAddress->publicPlaceId() == poQuery->value( 0 ) )
            cmbStreet->setCurrentIndex( cmbStreet->count()-1 );
    }
    ledStreetNumber->setText( m_poAddress->streetNumber() );
    ledFloor->setText( m_poAddress->floor() );
    ledDoor->setText( m_poAddress->door() );
    if( poQuery ) delete poQuery;
}

void cDlgPatientEdit::FillPhoneNumber()
{
    int     inRegionStart   = m_poPatient->phone().indexOf(" ",0);
    int     inPhoneStart    = m_poPatient->phone().indexOf(" ",inRegionStart+1);

    ledPhoneCountry->setText( m_poPatient->phone().left(inRegionStart) );
    ledPhoneRegion->setText( m_poPatient->phone().mid(inRegionStart+1,inPhoneStart-inRegionStart-1) );
    ledPhone->setText( m_poPatient->phone().mid(inPhoneStart+1) );
}

void cDlgPatientEdit::on_pbVerifyAddress_clicked()
{
    checkRegionZipCity();
}

void cDlgPatientEdit::checkRegionZipCity()
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
                    if( QString(e.what()).compare("ZipRegionCity city not found") != 0 )
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

void cDlgPatientEdit::on_pbIllnessGroup_clicked()
{
    cDlgIllnessGroup  obDlgIllnessGroup( this );

    obDlgIllnessGroup.exec();
    FillIllnessGroupCombo();
}

void cDlgPatientEdit::on_rbDiscountValue_clicked()
{
    calculateDiscount(1);
}

void cDlgPatientEdit::on_rbDiscountPercent_clicked()
{
    calculateDiscount(2);
}

void cDlgPatientEdit::calculateDiscount(int p_inDiscountType)
{
    if( !chkRegularCustomer->isChecked() && !chkEmployee->isChecked() && !chkService->isChecked() &&
        !chkCompany->isChecked() && !chkHealthInsurance->isChecked() && !chkDoctorProposed->isChecked() )
    {
        ledDiscountValue->setText( "0" );
        return;
    }

    QSqlQuery   *poQuery;
    QString      m_qsQuery = "";
    int          m_inMax = 0;
    int          m_inValueIdx = 0;

    m_qsQuery += QString( "SELECT * FROM discounts WHERE " );

    if( p_inDiscountType == 1 )
    {
        m_qsQuery += QString( "discountValue>0 " );
        m_inValueIdx = 8;
    }
    else if( p_inDiscountType == 2 )
    {
        m_qsQuery += QString( "discountPercent>0 " );
        m_inValueIdx = 9;
    }
    if( chkRegularCustomer->isChecked() || chkEmployee->isChecked() || chkService->isChecked() ||
        chkCompany->isChecked() || chkHealthInsurance->isChecked() )
    {
        m_qsQuery += QString( "AND ( " );
        if( chkRegularCustomer->isChecked() )
            m_qsQuery += QString( "regularCustomer>0 OR " );
        if( chkEmployee->isChecked() )
            m_qsQuery += QString( "employee>0 OR " );
        if( chkService->isChecked() )
            m_qsQuery += QString( "service>0 OR " );
        if( chkCompany->isChecked() )
            m_qsQuery += QString( "companyId=%1 OR " ).arg(cmbCompany->itemData(cmbCompany->currentIndex()).toUInt());
        if( chkHealthInsurance->isChecked() )
            m_qsQuery += QString( "healthInsuranceId=%1 OR " ).arg(cmbHealthInsurance->itemData(cmbHealthInsurance->currentIndex()).toUInt());
        m_qsQuery.chop(3);
        m_qsQuery += QString( ") " );
    }
    m_qsQuery += QString( "AND active=1" );

    poQuery = g_poDB->executeQTQuery( m_qsQuery );
    while( poQuery->next() )
    {
        if( m_inMax < poQuery->value( m_inValueIdx ).toInt() )
        {
            m_inMax = poQuery->value( m_inValueIdx ).toInt();
        }
    }
    if( poQuery ) delete poQuery;

    ledDiscountValue->setText( QString::number(m_inMax) );
}

void cDlgPatientEdit::updateDiscount()
{
    if( rbDiscountValue->isChecked() )
        calculateDiscount(1);
    else if( rbDiscountPercent->isChecked() )
        calculateDiscount(2);
}

void cDlgPatientEdit::on_chkRegularCustomer_stateChanged(int )
{
    updateDiscount();
}

void cDlgPatientEdit::on_chkEmployee_stateChanged(int )
{
    updateDiscount();
}

void cDlgPatientEdit::on_chkService_stateChanged(int )
{
    updateDiscount();
}
