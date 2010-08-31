#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardedit.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgcassaaction.h"
#include "../db/dbledger.h"

cDlgPatientCardEdit::cDlgPatientCardEdit( QWidget *p_poParent, cDBPatientCard *p_poPatientCard )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Patient card" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    m_bDlgLoaded        = false;
    m_bNewCard          = true;
    m_bRefillCard       = false;
    m_bIsCardActivated  = false;

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;

    if( m_poPatientCard->patientId() > 0 )
    {
        m_bNewCard = false;
    }

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    cbActive->setChecked( true );
    ledUnits->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) && !m_poPatientCard->id() );
    teTimeLeft->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) && !m_poPatientCard->id() );
    ledPrice->setEnabled( false );

    if( m_poPatientCard->id() == 0 || ( m_poPatientCard->id() > 0 && !m_poPatientCard->active() ) )
    {
        ledUnits->setEnabled( false );
        teTimeLeft->setEnabled( false );
        deValidDate->setEnabled( false );
    }

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );
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
        teTimeLeft->setTime( QTime( m_poPatientCard->timeLeft()/3600, (m_poPatientCard->timeLeft()%3600)/60, (m_poPatientCard->timeLeft()%3600)%60, 0 ) );
        deValidDate->setDate( QDate::fromString(m_poPatientCard->validDate(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );

        if( m_poPatientCard->licenceId() == 0 && m_poPatientCard->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
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
    if( m_poPatientCardType ) delete m_poPatientCardType;
}

void cDlgPatientCardEdit::activatePatientCard()
{
    cbActive->setChecked( true );
    cmbCardType->setFocus();
}

void cDlgPatientCardEdit::refillPatientCard()
{
    m_bRefillCard = true;

    ledBarcode->setEnabled( false );
    cmbCardType->setCurrentIndex( 0 );
    ledPrice->setText( convertCurrency(0,g_poPrefs->getCurrencyShort()) );
    cmbPatient->setEnabled( false );
    cmbCardType->setFocus();
}

void cDlgPatientCardEdit::setPatientCardOwner( const unsigned int p_uiPatientId )
{
    for( int i=0; i<cmbPatient->count(); i++ )
    {
        if( cmbPatient->itemData(i).toUInt() == p_uiPatientId )
        {
            cmbPatient->setCurrentIndex(i);
            break;
        }
    }
}

void cDlgPatientCardEdit::on_pbSave_clicked()
{
    bool  boSkipErrorMessages = false;
    time_t     ttTime;
    struct tm *poTm;
    time( &ttTime );
    poTm = localtime( &ttTime );

    bool  boCanBeSaved = true;

    if( ledBarcode->text() == "" )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Barcode cannot be empty.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. Barcode should be %1 character length.\n\nPress Ignore to skip other error messages." ).arg(g_poPrefs->getBarcodeLength()), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    else
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
        if( poQuery->numRowsAffected() > 0 )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. This barcode already saved into database.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
            {
                boSkipErrorMessages = true;
            }
        }
    }
    if( cbActive->isChecked() )
    {
        if( cmbCardType->currentIndex() == 0 && !boSkipErrorMessages )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Invalid Patientcard type.\nFor active patientcard other type should be selected.\n\nIf you want to connect the card to this type,\ndeactivate the card with unchecking the Active checkbox.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
            {
                boSkipErrorMessages = true;
            }
        }
        if( cmbPatient->currentIndex() == 0 && !boSkipErrorMessages )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Please select valid patient for the card.\nFor active patientcard an owner should be selected.\n\nIf you don't want to add this card to any patient,\npress the Save button." ), QMessageBox::Ok, QMessageBox::Save ) == QMessageBox::Save )
            {
                boCanBeSaved = true;
            }
        }
        if( ledUnits->text() == "" && !boSkipErrorMessages )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Available units cannot be empty.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
            {
                boSkipErrorMessages = true;
            }
        }
        else if( ledUnits->text().toInt() < 1 && !boSkipErrorMessages )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Number of available units can not be less then 1.\n\nIf you want to reset the number of available units\ndeactivate the card with unchecking the Active checkbox.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
            {
                boSkipErrorMessages = true;
            }
        }
        if( deValidDate->date() <= QDate(poTm->tm_year+1900,poTm->tm_mon+1,poTm->tm_mday) && !boSkipErrorMessages )
        {
            boCanBeSaved = false;
            if( QMessageBox::critical( this, tr( "Error" ), tr( "Incorrect validation date.\n\nIf you want to reset the date of validation\ndeactivate the card with unchecking the Active checkbox.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
            {
                boSkipErrorMessages = true;
            }
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            if( !m_poPatientCard->active() && cbActive->isChecked() )
                m_bIsCardActivated = true;

            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setActive( cbActive->isChecked() );
            m_poPatientCard->setPatientCardTypeId( cmbCardType->itemData( cmbCardType->currentIndex() ).toUInt() );
            m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
            m_poPatientCard->setUnits( ledUnits->text().toInt() );
            m_poPatientCard->setTimeLeftStr( teTimeLeft->time().toString("hh:mm:ss") );
            m_poPatientCard->setValidDate( deValidDate->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setComment( pteComment->toPlainText() );

            if( checkIndependent->isChecked() )
            {
                m_poPatientCard->setLicenceId( 0 );
            }
            else
            {
                m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            }

            // Kartya aktivalva lett, tehat el kell adni
            // Szerviz kartyat nem kell eladni
            if( m_bIsCardActivated && m_poPatientCard->patientCardTypeId() > 1 )
            {
                cDlgCassaAction     obDlgCassaAction(this);
                int                 inPriceTotal;

                inPriceTotal = m_poPatientCardType->price() + (m_poPatientCardType->price()/100)*m_poPatientCardType->vatpercent();
                obDlgCassaAction.setInitialMoney( inPriceTotal );
                obDlgCassaAction.setPayWithCash();
                if( obDlgCassaAction.exec() == QDialog::Accepted )
                {
                    int     inPayType = 0;
                    QString qsComment = QString("[%1] - %2 - ").arg(m_poPatientCard->barcode()).arg(m_poPatientCard->comment());

                    obDlgCassaAction.cassaResult( &inPayType, &qsComment );
                    if( inPayType == 1 )
                    {
                        g_obCassa.cassaAddMoneyAction( inPriceTotal, qsComment );
                    }
                    cDBLedger   obDBLedger;

                    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
                    if( m_bNewCard )
                        obDBLedger.setLedgerTypeId( 2 );
                    else
                        obDBLedger.setLedgerTypeId( 3 );
                    obDBLedger.setUserId( g_obUser.id() );
                    obDBLedger.setProductId( 0 );
                    obDBLedger.setPatientCardTypeId( m_poPatientCard->patientCardTypeId() );
                    obDBLedger.setPanelId( 0 );
                    obDBLedger.setName( m_poPatientCard->barcode() );
                    obDBLedger.setNetPrice( m_poPatientCardType->price() );
                    obDBLedger.setVatpercent( m_poPatientCardType->vatpercent() );
                    obDBLedger.setComment( qsComment );
                    obDBLedger.setActive( true );
                    obDBLedger.save();
                }
                else
                {
                    // Nem tortent meg az eladas
                    return;
                }
            }

            m_poPatientCard->save();

            QDialog::accept();

        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
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

    if( m_bRefillCard )
    {
        m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

        if( m_poPatientCardType->unitTime() > 0 )
        {
            ledUnits->setText( QString::number(m_poPatientCard->timeLeft()/(m_poPatientCardType->unitTime()*60)) );
        }
        else
        {
            ledUnits->setText( "0" );
        }
        if( m_poPatientCardType->validDays() > 0 )
        {
            deValidDate->setDate( QDate::currentDate().addDays(m_poPatientCardType->validDays()) );
        }
        else
        {
            deValidDate->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        }
    }
    else if( m_poPatientCard->id() == 0 || ( m_poPatientCard->id() > 0 && !m_poPatientCard->active() ) )
    {
        m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

//        g_obLogger(cSeverity::DEBUG) << "Unittime: " << m_poPatientCardType->units()*m_poPatientCardType->unitTime() << EOM;

        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        teTimeLeft->setTime( QTime(m_poPatientCardType->units()*m_poPatientCardType->unitTime()/60,m_poPatientCardType->units()*m_poPatientCardType->unitTime()%60,0,0) );
        if( m_poPatientCardType->validDays() > 0 )
        {
            deValidDate->setDate( QDate::currentDate().addDays(m_poPatientCardType->validDays()) );
        }
        else
        {
            deValidDate->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        }
        int priceTotal = m_poPatientCardType->price() + (m_poPatientCardType->price()/100)*m_poPatientCardType->vatpercent();
        ledPrice->setText( convertCurrency(priceTotal,g_poPrefs->getCurrencyShort()) );

        if( m_poPatientCardType->id() == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("You are not allowed to create System Service Patientcard.") );
            pbSave->setEnabled( false );
        }
        else
        {
            pbSave->setEnabled( true );
        }
    }
}

QString cDlgPatientCardEdit::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}
