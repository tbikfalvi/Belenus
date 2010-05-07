#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include "dlgpatientedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent, cDBPatient *p_poPatient )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPatientEdit::cDlgPatientEdit" );

    setupUi( this );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.gif") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.gif") );
    pbAttendances->setIcon( QIcon("./resources/40x40_attendance.gif") );

    m_poPatient = p_poPatient;
    if( m_poPatient )
    {
        QSqlQuery *poQuery;

        cmbPatientOrigin->addItem( tr("<Not set>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientoriginid, name FROM patientorigin WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbPatientOrigin->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->patientOriginId() == poQuery->value( 0 ) )
                cmbPatientOrigin->setCurrentIndex( cmbPatientOrigin->count()-1 );
        }
        cmbReasonToVisit->addItem( tr("<Not set>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT reasontovisitid, name FROM reasontovisit WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbReasonToVisit->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatient->reasonToVisitId() == poQuery->value( 0 ) )
                cmbReasonToVisit->setCurrentIndex( cmbReasonToVisit->count()-1 );
        }

        if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
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
        if( QMessageBox::critical( this, tr( "Error" ), tr( "atient unique identification field must be given.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }

    if( boCanBeSaved )
    {
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
    QMessageBox::information( this, tr( "Info" ), tr( "Finish later." ) );
}
