#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <iostream>

#include "dlgguestedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "../db/dbpatientcardtype.h"
#include "../crud/dlgaddress.h"

cDlgGuestEdit::cDlgGuestEdit( QWidget *p_poParent, cDBGuest *p_poGuest, cDBPostponed *p_poPostponed )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgGuestEdit::cDlgGuestEdit" );

    setupUi( this );

    m_bInit = true;

    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poGuest = p_poGuest;

    if( m_poGuest )
    {
        ledName->setText( m_poGuest->name() );
        if( m_poGuest->gender() == 1 ) rbGenderMale->setChecked(true);
        else if( m_poGuest->gender() == 2 ) rbGenderFemale->setChecked(true);
        deDateBirth->setDate( QDate::fromString(m_poGuest->dateBirth(),"yyyy-MM-dd") );
        ledUniqueId->setText( m_poGuest->uniqueId() );
        try
        {
            m_poPatientCard->loadPatient( m_poGuest->id() );
            ledBarcode->setText( m_poPatientCard->barcode() );

            cDBPatientCardType  obDBPatientCardType;

            obDBPatientCardType.load( m_poPatientCard->patientCardTypeId() );
            ledPatientcardType->setText( obDBPatientCardType.name() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Guest id not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
        }

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            if( m_poGuest->licenceId() == 0 && m_poGuest->id() > 0 )
            {
                ledName->setEnabled( false );
                rbGenderFemale->setEnabled( false );
                rbGenderMale->setEnabled( false );
                ledUniqueId->setEnabled( false );
                deDateBirth->setEnabled( false );

                pbSave->setEnabled( false );
            }
        }
    }

    m_bInit = false;
}

cDlgGuestEdit::~cDlgGuestEdit()
{
    cTracer obTrace( "cDlgGuestEdit::~cDlgGuestEdit" );

    if( m_poAddress )
        delete m_poAddress;
}


void cDlgGuestEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
//    bool  boSkipErrorMessages = false;

    if( ledName->text() == "" )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Guest name cannot be empty." ) );
        ledName->setFocus();
        return;
    }
/*
    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Guest gender must be selected.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
*/
/*
    if( deDateBirth->date().year() == 1900 &&
        deDateBirth->date().month() == 1 &&
        deDateBirth->date().day() == 1 &&
        !boSkipErrorMessages )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Guest date of birth must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
*/
/*
    if( ledUniqueId->text() == "" && !boSkipErrorMessages )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Guest unique identification value must be given.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
*/
    if( boCanBeSaved )
    {
        if( SaveGuestData() )
            QDialog::accept();
    }
}

void cDlgGuestEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

bool cDlgGuestEdit::SaveGuestData()
{
    bool bRet = false;

    try
    {
        m_poGuest->setLicenceId( g_poPrefs->getLicenceId() );
        m_poGuest->setName( ledName->text() );
        if( rbGenderMale->isChecked() )
            m_poGuest->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poGuest->setGender( 2 );
        m_poGuest->setUniqueId( ledUniqueId->text() );
        m_poGuest->setDateBirth( deDateBirth->date().toString("yyyy-MM-dd") );

        m_poGuest->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return bRet;
}

void cDlgGuestEdit::on_deDateBirth_dateChanged(QDate)
{
    if( deDateBirth->date().year() != 1900 )
    {
        QDate   qdDays = deDateBirth->date();
        QDate   qdAge  = QDate( 1900, 1, 1 );

        qdAge = qdAge.addDays( qdDays.daysTo(QDate::currentDate()) );

        ledAge->setText( QString::number(qdAge.year()-1900) );
    }
}
