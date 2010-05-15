#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardedit.h"
#include "../db/dbpatientcardtype.h"

cDlgPatientCardEdit::cDlgPatientCardEdit( QWidget *p_poParent, cDBPatientCard *p_poPatientCard )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_bDlgLoaded = false;

    pbSave->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
    cbActive->setChecked( true );
    ledUnits->setEnabled( g_obUser.isInGroup( "admin" ) && !m_poPatientCard->id() );
    teTimeLeft->setEnabled( g_obUser.isInGroup( "admin" ) && !m_poPatientCard->id() );

    if( m_poPatientCard->id() == 0 || ( m_poPatientCard->id() > 0 && !m_poPatientCard->active() ) )
    {
        ledUnits->setEnabled( false );
        teTimeLeft->setEnabled( false );
        deValidDate->setEnabled( false );
    }

    m_poPatientCard = p_poPatientCard;
    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( QString::fromStdString( m_poPatientCard->barcode() ) );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
                cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
        cbActive->setChecked( m_poPatientCard->active() );
        ledUnits->setText( QString::number(m_poPatientCard->units()) );
        teTimeLeft->setTime( QTime::fromString(QString::fromStdString(m_poPatientCard->timeLeft()),"hh:mm:ss") );
        deValidDate->setDate( QDate::fromString(QString::fromStdString(m_poPatientCard->validDate()),"yyyy-MM-dd") );
        pteComment->setPlainText( QString::fromStdString( m_poPatientCard->comment() ) );

        if( m_poPatientCard->licenceId() == 0 && m_poPatientCard->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCard->licenceId() == 0 && m_poPatientCard->id() > 0 )
            {
                pbSave->setEnabled( false );
            }
        }
    }
    m_bDlgLoaded = true;
}

cDlgPatientCardEdit::~cDlgPatientCardEdit()
{
}

void cDlgPatientCardEdit::on_pbSave_clicked()
{
    time_t     ttTime;
    struct tm *poTm;
    time( &ttTime );
    poTm = localtime( &ttTime );

    bool  boCanBeSaved = true;

    if( ledBarcode->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Barcode cannot be empty." ) );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. Barcode should be %1 character length." ).arg(g_poPrefs->getBarcodeLength()) );
    }
    else
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
        if( poQuery->numRowsAffected() > 0 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. This barcode already saved into database." ) );
        }
    }
    if( cbActive->isChecked() )
    {
        if( cmbCardType->currentIndex() == 0 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Invalid Patientcard type.\nFor active patientcard other type should be selected.\n\nIf you want to connect the card to this type,\ndeactivate the card with unchecking the Active checkbox." ) );
        }
        if( cmbPatient->currentIndex() == 0 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Please select valid patient for the card.\nFor active patientcard an owner must be selected.\n\nIf you don't want to add this card to any patient,\ndeactivate the card with unchecking the Active checkbox." ) );
        }
        if( ledUnits->text() == "" )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Available units cannot be empty." ) );
        }
        else if( ledUnits->text().toInt() < 1 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Number of available units can not be less then 1.\n\nIf you want to reset the number of available units\ndeactivate the card with unchecking the Active checkbox." ) );
        }
        if( deValidDate->date() <= QDate(poTm->tm_year+1900,poTm->tm_mon+1,poTm->tm_mday) )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Incorrect validation date.\n\nIf you want to reset the date of validation\ndeactivate the card with unchecking the Active checkbox." ) );
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            bool bIsCardActivated = false;

            if( !m_poPatientCard->active() && cbActive->isChecked() )
                bIsCardActivated = true;

            m_poPatientCard->setBarcode( ledBarcode->text().toStdString() );
            m_poPatientCard->setActive( cbActive->isChecked() );
            m_poPatientCard->setPatientCardTypeId( cmbCardType->itemData( cmbCardType->currentIndex() ).toInt() );
            m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toInt() );
            m_poPatientCard->setUnits( ledUnits->text().toInt() );
            m_poPatientCard->setTimeLeft( teTimeLeft->time().toString("hh:mm:ss").toStdString() );
            m_poPatientCard->setValidDate( deValidDate->date().toString("yyyy-MM-dd").toStdString() );
            m_poPatientCard->setComment( pteComment->toPlainText().toStdString() );

            if( checkIndependent->isChecked() )
            {
                m_poPatientCard->setLicenceId( 0 );
            }
            else
            {
                m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poPatientCard->save();

            QDialog::accept();

            if( bIsCardActivated )
                QMessageBox::information( this, "Info", "kartya eladas ..." );
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}

void cDlgPatientCardEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardEdit::on_cbActive_toggled(bool checked)
{
    cmbCardType->setEnabled( checked );
    cmbPatient->setEnabled( checked );
    gbInformation->setEnabled( checked );
}

void cDlgPatientCardEdit::on_cmbCardType_currentIndexChanged(int index)
{
    if( !m_bDlgLoaded )
        return;

    if( m_poPatientCard->id() == 0 || ( m_poPatientCard->id() > 0 && !m_poPatientCard->active() ) )
    {
        m_poPatientCardType = new cDBPatientCardType;
        m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

        g_obLogger << "Unittime: " << m_poPatientCardType->units()*m_poPatientCardType->unitTime() << cQTLogger::EOM;
        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        teTimeLeft->setTime( QTime(m_poPatientCardType->units()*m_poPatientCardType->unitTime()/60,m_poPatientCardType->units()*m_poPatientCardType->unitTime()%60,0,0) );
        if( m_poPatientCardType->validDays() > 0 )
        {
            deValidDate->setDate( QDate::currentDate().addDays(m_poPatientCardType->validDays()) );
        }
        else
        {
            deValidDate->setDate( QDate::fromString(QString::fromStdString(m_poPatientCardType->validDateTo()),"yyyy-MM-dd") );
        }
    }
}
