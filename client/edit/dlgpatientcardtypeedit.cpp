#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardtypeedit.h"

cDlgPatientCardTypeEdit::cDlgPatientCardTypeEdit( QWidget *p_poParent, cDBPatientCardType *p_poPatientCardType )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Patient card type" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcardtype.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    deValidDateFrom->setDate( QDate(2000,1,1) );
    deValidDateTo->setDate( QDate(2000,1,1) );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPatientCardType", QPoint(420,245) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_poPatientCardType = p_poPatientCardType;

    if( m_poPatientCardType )
    {
        ledName->setText( m_poPatientCardType->name() );
        ledPrice->setText( QString::number(m_poPatientCardType->price()) );
        ledVatpercent->setText( QString::number(m_poPatientCardType->vatpercent()) );
        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        ledUnitTime->setText( QString::number(m_poPatientCardType->unitTime()) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCardType->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        ledValidDays->setText( QString::number(m_poPatientCardType->validDays()) );

        if( m_poPatientCardType->validDays() < 1 )
        {
            rbInterval->setChecked( true );
            deValidDateFrom->setEnabled( true );
            deValidDateTo->setEnabled( true );
            ledValidDays->setEnabled( false );
        }
        else
        {
            rbDays->setChecked( true );
            deValidDateFrom->setEnabled( false );
            deValidDateTo->setEnabled( false );
            ledValidDays->setEnabled( true );
        }

        if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            {
                ledName->setEnabled( false );
                ledPrice->setEnabled( false );
                ledVatpercent->setEnabled( false );
                ledUnits->setEnabled( false );
                ledUnitTime->setEnabled( false );
                rbInterval->setEnabled( false );
                deValidDateFrom->setEnabled( false );
                deValidDateTo->setEnabled( false );
                rbDays->setEnabled( false );
                ledValidDays->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poPatientCardType->id() > 0 )
            checkIndependent->setEnabled( false );
    }
}

cDlgPatientCardTypeEdit::~cDlgPatientCardTypeEdit()
{
    g_poPrefs->setDialogSize( "EditPatientCardType", QPoint( width(), height() ) );
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
    else if( rbInterval->isChecked() && deValidDateTo->date() <= QDate::currentDate() )
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
            m_poPatientCardType->setName( ledName->text() );
            m_poPatientCardType->setPrice( ledPrice->text().toUInt() );
            m_poPatientCardType->setVatpercent( ledVatpercent->text().toInt() );
            m_poPatientCardType->setUnits( ledUnits->text().toUInt() );
            m_poPatientCardType->setUnitTime( ledUnitTime->text().toUInt() );
            m_poPatientCardType->setValidDateFrom( (rbDays->isChecked()?"2000-01-01":deValidDateFrom->date().toString("yyyy-MM-dd")) );
            m_poPatientCardType->setValidDateTo( (rbDays->isChecked()?"2000-01-01":deValidDateTo->date().toString("yyyy-MM-dd")) );
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
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgPatientCardTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}
