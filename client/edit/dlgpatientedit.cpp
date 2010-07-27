#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include "dlgpatientedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "db/dbpostponed.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent, cDBPatient *p_poPatient, cDBPostponed *p_poPostponed )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPatientEdit::cDlgPatientEdit" );

    setupUi( this );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.gif") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.gif") );
    pbAttendances->setIcon( QIcon("./resources/40x40_attendance.gif") );

    m_poPostponed = p_poPostponed;
    m_poPatient = p_poPatient;

    if( m_poPatient )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientoriginid, name FROM patientorigin WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbPatientOrigin->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->patientOriginId() == poQuery->value( 0 ) )
                cmbPatientOrigin->setCurrentIndex( cmbPatientOrigin->count()-1 );
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT reasontovisitid, name FROM reasontovisit WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbReasonToVisit->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->reasonToVisitId() == poQuery->value( 0 ) )
                cmbReasonToVisit->setCurrentIndex( cmbReasonToVisit->count()-1 );
        }

        if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            {
                gbIdentification->setEnabled( false );
                gbAddress->setEnabled( false );
                gbVisit->setEnabled( false );
                gbAdditionalData->setEnabled( false );

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

        ledName->setText( QString::fromStdString(m_poPatient->name()) );
        if( m_poPatient->gender() == 1 ) rbGenderMale->setChecked(true);
        else if( m_poPatient->gender() == 2 ) rbGenderFemale->setChecked(true);
        deDateBirth->setDate( QDate::fromString(QString::fromStdString(m_poPatient->dateBirth()),"yyyy-MM-dd") );
        ledUniqueId->setText( QString::fromStdString(m_poPatient->uniqueId()) );
        ledCountry->setText( QString::fromStdString(m_poPatient->country()) );
        ledRegion->setText( QString::fromStdString(m_poPatient->region()) );
        ledCity->setText( QString::fromStdString(m_poPatient->city()) );
        ledZip->setText( QString::fromStdString(m_poPatient->zip()) );
        ledAddress->setText( QString::fromStdString(m_poPatient->address()) );
        ledPhone->setText( QString::fromStdString(m_poPatient->phone()) );
        ledEmail->setText( QString::fromStdString(m_poPatient->email()) );
        ptComment->setPlainText( QString::fromStdString(m_poPatient->comment()) );
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
        m_poPatient->setPatientOriginId( cmbPatientOrigin->itemData( cmbPatientOrigin->currentIndex() ).toInt() );
        m_poPatient->setReasonToVisitId( cmbReasonToVisit->itemData( cmbReasonToVisit->currentIndex() ).toInt() );
        m_poPatient->setName( ledName->text().toStdString() );
        if( rbGenderMale->isChecked() )
            m_poPatient->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poPatient->setGender( 2 );
        m_poPatient->setDateBirth( deDateBirth->date().toString("yyyy-MM-dd").toStdString() );
        m_poPatient->setUniqueId( ledUniqueId->text().toStdString() );
        m_poPatient->setCountry( ledCountry->text().toStdString() );
        m_poPatient->setRegion( ledRegion->text().toStdString() );
        m_poPatient->setCity( ledCity->text().toStdString() );
        m_poPatient->setZip( ledZip->text().toStdString() );
        m_poPatient->setAddress( ledAddress->text().toStdString() );
        m_poPatient->setEmail( ledEmail ->text().toStdString() );
        m_poPatient->setPhone( ledPhone->text().toStdString() );
        m_poPatient->setComment( ptComment->toPlainText().toStdString() );

        m_poPatient->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return bRet;
}
