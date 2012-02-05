#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <iostream>

#include "dlgguestedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "../db/dbpatientcardtype.h"
#include "../crud/dlgaddress.h"

cDlgGuestEdit::cDlgGuestEdit( QWidget *p_poParent, cDBGuest *p_poGuest, cDBPostponed* )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgGuestEdit::cDlgGuestEdit" );

    setupUi( this );

    m_bInit = true;

    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbHistory->setIcon( QIcon("./resources/40x40_question.png") );

    m_poGuest = p_poGuest;

    if( m_poGuest )
    {
        ledName->setText( m_poGuest->name() );
        if( m_poGuest->gender() == 1 ) rbGenderMale->setChecked(true);
        else if( m_poGuest->gender() == 2 ) rbGenderFemale->setChecked(true);
        ledEmail->setText( m_poGuest->email() );
        switch( m_poGuest->ageType() )
        {
            case 1:
                rbAge1->setChecked( true );
                break;
            case 2:
                rbAge2->setChecked( true );
                break;
            case 3:
                rbAge3->setChecked( true );
                break;
            case 4:
                rbAge4->setChecked( true );
                break;
            case 5:
                rbAge5->setChecked( true );
                break;
            case 6:
                rbAge6->setChecked( true );
                break;
            case 7:
                rbAge7->setChecked( true );
                break;
            default:
                rbAge0->setChecked( true );
        }
        chkReturning->setChecked( m_poGuest->isReturning() );

        try
        {
            m_poPatientCard = new cDBPatientCard();
            m_poPatientCard->loadPatient( m_poGuest->id() );
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

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            if( m_poGuest->licenceId() == 0 && m_poGuest->id() > 0 )
            {
                ledName->setEnabled( false );
                rbGenderFemale->setEnabled( false );
                rbGenderMale->setEnabled( false );
                ledEmail->setEnabled( false );
                rbAge0->setEnabled( false );
                rbAge1->setEnabled( false );
                rbAge2->setEnabled( false );
                rbAge3->setEnabled( false );
                rbAge4->setEnabled( false );
                rbAge5->setEnabled( false );
                rbAge6->setEnabled( false );
                rbAge7->setEnabled( false );
                chkReturning->setEnabled( false );

                pbSave->setEnabled( false );
            }
        }
    }

    m_bInit = false;
}

cDlgGuestEdit::~cDlgGuestEdit()
{
    cTracer obTrace( "cDlgGuestEdit::~cDlgGuestEdit" );
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
        m_poGuest->setEmail( ledEmail->text() );
        if( rbAge0->isChecked() ) m_poGuest->setAgeType( 0 );
        else if( rbAge1->isChecked() ) m_poGuest->setAgeType( 1 );
        else if( rbAge2->isChecked() ) m_poGuest->setAgeType( 2 );
        else if( rbAge3->isChecked() ) m_poGuest->setAgeType( 3 );
        else if( rbAge4->isChecked() ) m_poGuest->setAgeType( 4 );
        else if( rbAge5->isChecked() ) m_poGuest->setAgeType( 5 );
        else if( rbAge6->isChecked() ) m_poGuest->setAgeType( 6 );
        else if( rbAge7->isChecked() ) m_poGuest->setAgeType( 7 );
        m_poGuest->setIsReturning( chkReturning->isChecked() );

        m_poGuest->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return bRet;
}

void cDlgGuestEdit::on_pbHistory_clicked()
{
    QMessageBox::information( this, tr("Information"), tr("Not implemented yet.") );
}
