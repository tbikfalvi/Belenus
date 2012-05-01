//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgpatientcardedit.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//===========================================================================================================
// Berletek adatait kezelo ablak.
//===========================================================================================================

#include <QPushButton>
#include <QMessageBox>
#include <ctime>

//===========================================================================================================

#include "dlgpatientcardedit.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgcassaaction.h"
#include "../db/dbledger.h"
#include "../db/dbshoppingcart.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardEdit::cDlgPatientCardEdit( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;
    m_bDlgLoaded        = false;
    m_bNewCard          = true;
    m_bRefillCard       = false;
    m_bIsCardActivated  = false;

    setWindowTitle( tr( "Patient card" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    cbActive->setChecked( true );
    ledUnits->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) && !m_poPatientCard->id() );
    teTimeLeft->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) && !m_poPatientCard->id() );
    ledPrice->setEnabled( false );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    if( m_poPatientCard->patientId() > 0 )
    {
        m_bNewCard = false;
    }

    if( m_poPatientCard->id() == 0 || ( m_poPatientCard->id() > 0 && !m_poPatientCard->active() ) )
    {
        ledUnits->setEnabled( false );
        teTimeLeft->setEnabled( false );
        deValidDateFrom->setEnabled( false );
        deValidDateTo->setEnabled( false );
    }

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );
        cmbCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
                cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
        }
        cmbPatient->addItem( tr("<Not selected>"), 0 );

        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
        cbActive->setChecked( m_poPatientCard->active() );
        ledUnits->setText( QString::number(m_poPatientCard->units()) );
        teTimeLeft->setTime( QTime( m_poPatientCard->timeLeft()/3600, (m_poPatientCard->timeLeft()%3600)/60, (m_poPatientCard->timeLeft()%3600)%60, 0 ) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCard->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );

        if( m_poPatientCard->licenceId() == 0 && m_poPatientCard->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCard->licenceId() == 0 && m_poPatientCard->id() > 0 )
            {
                ledBarcode->setEnabled( false );
                cbActive->setEnabled( false );
                cmbCardType->setEnabled( false );
                ledPrice->setEnabled( false );
                cmbPatient->setEnabled( false );
                ledUnits->setEnabled( false );
                teTimeLeft->setEnabled( false );
                deValidDateFrom->setEnabled( false );
                deValidDateTo->setEnabled( false );
                pteComment->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poPatientCard->id() > 0 )
            checkIndependent->setEnabled( false );
    }
    if( m_poPatientCard->patientId() > 0 )
    {
        cmbPatient->setEnabled( false );
    }

    slotRefreshWarningColors();
    slotEnableButtons();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPatientCard", QPoint(440,380) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bDlgLoaded = true;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardEdit::~cDlgPatientCardEdit()
{
    g_poPrefs->setDialogSize( "EditPatientCard", QPoint( width(), height() ) );

    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::slotRefreshWarningColors()
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( cbActive->isChecked() )
    {
        lblPatient->setStyleSheet( "QLabel {font: normal;}" );
        if( cmbPatient->currentIndex() == 0 )
        {
            lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        }
        if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
        {
            lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( cmbCardType->currentIndex() == 0 )
        {
            lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( ledUnits->text() == "" )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        else if( ledUnits->text().toInt() < 1 )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( cmbPatient->currentIndex() == 0 )
        {
            lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        }
        if( deValidDateTo->date() < QDate::currentDate() )
        {
            lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::slotEnableButtons()
{
    if( !m_poPatientCard->active() && cbActive->isChecked() )
    {
        pbSave->setIcon( QIcon("./resources/40x40_cassa.png") );
    }
    else
    {
        pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    }
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::activatePatientCard()
{
    cbActive->setChecked( true );
    cmbCardType->setFocus();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::refillPatientCard()
{
    m_bRefillCard = true;

    ledBarcode->setEnabled( false );
    cmbCardType->setCurrentIndex( 0 );
    ledPrice->setText( convertCurrency(0,g_poPrefs->getCurrencyShort()) );
    cmbPatient->setEnabled( false );
    cmbCardType->setFocus();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
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
    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( ledBarcode->text() == "" )
    {
        boCanBeSaved = false;
        qsErrorMessage.append( tr( "Barcode cannot be empty." ) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Invalid barcode. Barcode should be %1 character length." ).arg(g_poPrefs->getBarcodeLength()) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
        if( poQuery->numRowsAffected() > 0 )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Invalid barcode. This barcode already saved into database."  ) );
            lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }
    if( cbActive->isChecked() )
    {
        if( cmbCardType->currentIndex() == 0 )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Invalid Patientcard type.\nFor active patientcard other type should be selected.\nIf you want to connect the card to this type,\ndeactivate the card with unchecking the Active checkbox." ) );
            lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( cmbPatient->currentIndex() == 0 )
        {
            lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        }
        if( ledUnits->text() == "" )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Available units cannot be empty." ) );
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        else if( ledUnits->text().toInt() < 1 )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Number of available units can not be less then 1.\nIf you want to reset the number of available units\ndeactivate the card with unchecking the Active checkbox." ) );
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( deValidDateTo->date() < QDate::currentDate() )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Incorrect validation date.\nIf you want to reset the date of validation\ndeactivate the card with unchecking the Active checkbox." ) );
            lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
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
            m_poPatientCard->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
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
                if( !g_obCassa.isCassaEnabled() )
                {
                    QMessageBox::warning( this, tr("Attention"),
                                          tr("Cassa is disabled!\n\n"
                                             "Please relogin to enable cassa.") );
                    return;
                }

                int     inPriceTotal        = m_poPatientCardType->price()+(m_poPatientCardType->price()/100)*m_poPatientCardType->vatpercent();
                int     inPriceDiscounted   = 0;

                if( cmbPatient->currentIndex() > 0 )
                {
                    cDBGuest  obDBPatientTemp;

                    obDBPatientTemp.load( cmbPatient->itemData(cmbPatient->currentIndex()).toUInt() );
                    inPriceDiscounted = obDBPatientTemp.getDiscountedPrice( inPriceTotal );
                }
                else
                {
                    inPriceDiscounted = inPriceTotal;
                }

                cDBShoppingCart obDBShoppingCart;

                obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
                obDBShoppingCart.setGuestId( m_poPatientCard->patientId() );
                obDBShoppingCart.setProductId( 0 );
                obDBShoppingCart.setPatientCardId( m_poPatientCard->id() );
                obDBShoppingCart.setPanelId( 0 );
                obDBShoppingCart.setItemName( QString("%1 - %2").arg(m_poPatientCardType->name()).arg(m_poPatientCard->barcode()) );
                obDBShoppingCart.setItemCount( 1 );
                obDBShoppingCart.setItemNetPrice( m_poPatientCardType->price() );
                obDBShoppingCart.setItemVAT( m_poPatientCardType->vatpercent() );
                obDBShoppingCart.setItemDiscount( inPriceTotal - inPriceDiscounted );
                obDBShoppingCart.setItemSumPrice( inPriceDiscounted );

                cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

                obDlgCassaAction.setPayWithCash();
                if( obDlgCassaAction.exec() == QDialog::Accepted )
                {
                    int     inPayType = 0;
                    QString qsComment = QString("[%1] - %2 - ").arg(m_poPatientCard->barcode()).arg(m_poPatientCard->comment());

                    obDlgCassaAction.cassaResult( &inPayType, &qsComment );
                    if( inPayType == cDlgCassaAction::PAY_CASH )
                    {
                        g_obCassa.cassaAddMoneyAction( inPriceDiscounted, qsComment );
                    }
                    cDBLedger   obDBLedger;

                    obDBLedger.setLicenceId( g_poPrefs->getLicenceId() );
                    if( m_bNewCard )
                        obDBLedger.setLedgerTypeId( 2 );
                    else
                        obDBLedger.setLedgerTypeId( 3 );
                    obDBLedger.setLedgerDeviceId( 0 );
                    obDBLedger.setPaymentMethod( inPayType );
                    obDBLedger.setUserId( g_obUser.id() );
                    obDBLedger.setProductId( 0 );
                    obDBLedger.setPatientCardTypeId( m_poPatientCard->patientCardTypeId() );
                    obDBLedger.setPatientCardId( m_poPatientCard->id() );
                    obDBLedger.setPanelId( 0 );
                    obDBLedger.setName( m_poPatientCard->barcode() );
                    obDBLedger.setNetPrice( m_poPatientCardType->price() );
                    obDBLedger.setDiscount( inPriceTotal - inPriceDiscounted );
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
    else
    {
        QMessageBox::critical( this, tr( "Error" ), qsErrorMessage );
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

    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );
    if( cbActive->isChecked() )
    {
        if( cmbCardType->currentIndex() == 0 )
        {
            lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( ledUnits->text() == "" )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        else if( ledUnits->text().toInt() < 1 )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( cmbPatient->currentIndex() == 0 )
        {
            lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        }
        if( deValidDateTo->date() < QDate::currentDate() )
        {
            lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }

    slotEnableButtons();
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
            deValidDateFrom->setDate( QDate::currentDate() );
            deValidDateTo->setDate( QDate::currentDate().addDays(m_poPatientCardType->validDays()) );
        }
        else
        {
            deValidDateFrom->setDate( QDate::fromString(m_poPatientCardType->validDateFrom(),"yyyy-MM-dd") );
            deValidDateTo->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
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
            deValidDateFrom->setDate( QDate::currentDate() );
            deValidDateTo->setDate( QDate::currentDate().addDays(m_poPatientCardType->validDays()) );
        }
        else
        {
            deValidDateFrom->setDate( QDate::fromString(m_poPatientCardType->validDateFrom(),"yyyy-MM-dd") );
            deValidDateTo->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        }
        int priceTotal = m_poPatientCardType->price() + (m_poPatientCardType->price()/100)*m_poPatientCardType->vatpercent();

        int discount = 0;

        if( cmbPatient->currentIndex() > 0 )
        {
            cDBGuest  obDBPatientTemp;

            obDBPatientTemp.load( cmbPatient->itemData(cmbPatient->currentIndex()).toUInt() );
            discount = obDBPatientTemp.getDiscountedPrice( priceTotal );
        }
        else
        {
            discount = priceTotal;
        }

        if( discount != priceTotal )
            ledPrice->setText( QString("%1 (%2)").arg(convertCurrency(discount,g_poPrefs->getCurrencyShort())).arg(convertCurrency(priceTotal,g_poPrefs->getCurrencyShort())) );
        else
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

    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( cbActive->isChecked() )
    {
        if( cmbCardType->currentIndex() == 0 )
        {
            lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( ledUnits->text() == "" )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        else if( ledUnits->text().toInt() < 1 )
        {
            lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
        if( deValidDateTo->date() < QDate::currentDate() )
        {
            lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
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

void cDlgPatientCardEdit::on_cmbPatient_currentIndexChanged(int /*index*/)
{
    if( !m_bDlgLoaded )
        return;

    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    if( cmbPatient->currentIndex() == 0 )
    {
        lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }

    on_cmbCardType_currentIndexChanged( cmbCardType->currentIndex() );
}

void cDlgPatientCardEdit::on_ledBarcode_lostFocus()
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );

    if( ledBarcode->text() == "" )
    {
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
        if( poQuery->numRowsAffected() > 0 )
        {
            lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }
}
