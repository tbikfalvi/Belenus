#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <iostream>

#include "dlgpatientedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "../db/dbpostponed.h"
#include "../db/dbzipregioncity.h"

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
                deDateBirth->setEnabled( false );
                ledUniqueId->setEnabled( false );

                ledHeight->setEnabled( false );
                ledWeight->setEnabled( false );
                cmbIllnessGroup->setEnabled( false );
                ptMedicineCurrent->setEnabled( false );
                ptMedicineAllergy->setEnabled( false );

                cmbPatientOrigin->setEnabled( false );
                cmbReasonToVisit->setEnabled( false );

                ledCountry->setEnabled( false );
                ledRegion->setEnabled( false );
                ledZip->setEnabled( false );
                ledCity->setEnabled( false );
                ledAddress->setEnabled( false );
                ledPhone->setEnabled( false );
                ledEmail->setEnabled( false );
                ptComment->setEnabled( false );

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

        ledHeight->setText( QString::number(m_poPatient->height()) );
        ledWeight->setText( QString::number(m_poPatient->weight()) );
        ptMedicineCurrent->setPlainText( m_poPatient->medicineCurrent() );
        ptMedicineAllergy->setPlainText( m_poPatient->medicineAllergy() );

        ledCountry->setText( m_poPatient->country() );
        ledRegion->setText( m_poPatient->region() );
        ledCity->setText( m_poPatient->city() );
        ledZip->setText( m_poPatient->zip() );
        ledAddress->setText( m_poPatient->address() );
        ledPhone->setText( m_poPatient->phone() );
        ledEmail->setText( m_poPatient->email() );
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
        m_poPatient->setPatientOriginId( cmbPatientOrigin->itemData( cmbPatientOrigin->currentIndex() ).toInt() );
        m_poPatient->setReasonToVisitId( cmbReasonToVisit->itemData( cmbReasonToVisit->currentIndex() ).toInt() );
        m_poPatient->setName( ledName->text() );
        if( rbGenderMale->isChecked() )
            m_poPatient->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poPatient->setGender( 2 );
        m_poPatient->setDateBirth( deDateBirth->date().toString("yyyy-MM-dd") );
        m_poPatient->setUniqueId( ledUniqueId->text() );
        m_poPatient->setCountry( ledCountry->text() );
        m_poPatient->setRegion( ledRegion->text() );
        m_poPatient->setCity( ledCity->text() );
        m_poPatient->setZip( ledZip->text() );
        m_poPatient->setAddress( ledAddress->text() );
        m_poPatient->setEmail( ledEmail ->text() );
        m_poPatient->setPhone( ledPhone->text() );
        m_poPatient->setComment( ptComment->toPlainText() );

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

}
