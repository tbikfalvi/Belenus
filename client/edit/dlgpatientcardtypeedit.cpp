#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardtypeedit.h"

cDlgPatientCardTypeEdit::cDlgPatientCardTypeEdit( QWidget *p_poParent, cDBPatientCardType *p_poPatientCardType )
    : QDialog( p_poParent )
{
    time_t     ttTime;
    struct tm *poTm;
    time( &ttTime );
    poTm = localtime( &ttTime );

    setupUi( this );

    setWindowTitle( tr( "Patient card type" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcardtype.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    deValidDateFrom->setDate( QDate(poTm->tm_year+1900,poTm->tm_mon+1,poTm->tm_mday) );
    deValidDateTo->setDate( QDate(poTm->tm_year+1900,poTm->tm_mon+1,poTm->tm_mday) );

    m_poPatientCardType = p_poPatientCardType;

    if( m_poPatientCardType )
    {
        ledName->setText( QString::fromStdString( m_poPatientCardType->name() ) );
        ledPrice->setText( QString::number(m_poPatientCardType->price()) );
        ledVatpercent->setText( QString::number(m_poPatientCardType->vatpercent()) );
        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        ledUnitTime->setText( QString::number(m_poPatientCardType->unitTime()) );
        deValidDateFrom->setDate( QDate::fromString(QString::fromStdString(m_poPatientCardType->validDateFrom()),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(QString::fromStdString(m_poPatientCardType->validDateTo()),"yyyy-MM-dd") );
        ledValidDays->setText( QString::number(m_poPatientCardType->validDays()) );

        if( m_poPatientCardType->validDays() == 0 )
            rbInterval->setChecked( true );
        else
            rbDays->setChecked( true );

        if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            {
                ledName->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
    }
}

cDlgPatientCardTypeEdit::~cDlgPatientCardTypeEdit()
{
}

void cDlgPatientCardTypeEdit::on_rbInterval_toggled(bool checked)
{
    if( checked )
    {
        deValidDateFrom->setEnabled( true );
        deValidDateTo->setEnabled( true );
        ledValidDays->setEnabled( false );
    }
    else
    {
        deValidDateFrom->setEnabled( false );
        deValidDateTo->setEnabled( false );
        ledValidDays->setEnabled( true );
    }
}

void cDlgPatientCardTypeEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    time_t     ttTime;
    struct tm *poTm;
    time( &ttTime );
    poTm = localtime( &ttTime );

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of patientcard type must be set." ) );
    }
    if( ledPrice->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Price of patientcard type must be set." ) );
    }
    if( ledVatpercent->text() == "" )
        ledVatpercent->setText( "0" );
    if( ledUnits->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Number of units of patientcard type must be set." ) );
    }
    if( ledUnitTime->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Unittime of patientcard type must be set." ) );
    }
    if( rbDays->isChecked() && (ledValidDays->text() == "" || ledValidDays->text().toInt() < 1) )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Number of validation days of patientcard type must be set." ) );
    }
    else if( rbInterval->isChecked() && deValidDateTo->date() <= QDate(poTm->tm_year+1900,poTm->tm_mon+1,poTm->tm_mday) )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Invalid end date. End date must be in the future." ) );
    }
    else if( rbInterval->isChecked() && deValidDateFrom->date() > deValidDateTo->date() )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "FROM date must be before TO date." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCardType->setName( ledName->text().toStdString() );
            m_poPatientCardType->setPrice( ledPrice->text().toUInt() );
            m_poPatientCardType->setVatpercent( ledVatpercent->text().toInt() );
            m_poPatientCardType->setUnits( ledUnits->text().toUInt() );
            m_poPatientCardType->setUnitTime( ledUnitTime->text().toUInt() );
            m_poPatientCardType->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd").toStdString() );
            m_poPatientCardType->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd").toStdString() );
            m_poPatientCardType->setValidDays( ledValidDays->text().toUInt() );
            m_poPatientCardType->setActive( true );

            if( checkIndependent->isChecked() )
            {
                m_poPatientCardType->setLicenceId( 0 );
            }
            else
            {
                m_poPatientCardType->setLicenceId( g_poPrefs->getLicenceId() );
            }

            m_poPatientCardType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}

void cDlgPatientCardTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}
