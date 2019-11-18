//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgpatientcardsell.cpp
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

#include "dlgpatientcardsell.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgcassaaction.h"
#include "../db/dbledger.h"
#include "../db/dbshoppingcart.h"
#include "../db/dbpatientcardunits.h"
#include "../db/dbdiscount.h"
#include "../crud/dlgpatientselect.h"
#include "../edit/dlgguestedit.h"
#include "../communication_rfid.h"

extern cCommRFID       *g_poCommRFID;

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardSell::cDlgPatientCardSell( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;
    m_bDlgLoaded        = false;

    setWindowTitle( tr( "Selling Patientcard" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );
    pbSell->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbSelectPatient->setIcon( QIcon("./resources/40x40_search.png") );
    pbCreatePatient->setIcon( QIcon("./resources/40x40_patient_new.png") );
    pbRFID->setIcon( QIcon("./resources/40x40_rfid.png") );

    pbRFID->setEnabled( false );

    deValidDateFrom->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );
    deValidDateTo->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    m_dlgProgress = new cDlgProgress( this );

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );
        ledRFID->setText( m_poPatientCard->RFID() );
        if( m_poPatientCard->RFID().length() > 0 )
        {
            chkRFID->setChecked( true );
            pbRFID->setEnabled( true );
        }

        cmbCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            if( poQuery->value(0) == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
                continue;

            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
                cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
        }

        cmbPatient->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }

        ledUnits->setText( QString::number(m_poPatientCard->units()) );
        teTimeLeft->setTime( QTime( m_poPatientCard->timeLeft()/3600, (m_poPatientCard->timeLeft()%3600)/60, (m_poPatientCard->timeLeft()%3600)%60, 0 ) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCard->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );

        if( m_poPatientCard->active() )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("This patientcard is in use. Active patientcard can not be sold.\n"
                                     "Please refill the card if you want to add additional units.") );
            pbSell->setEnabled( false );
        }
    }

    slotRefreshWarningColors();
    slotEnableButtons();

    if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledBarcode->setEchoMode( QLineEdit::Password );
    }
    else
    {
        ledBarcode->setEchoMode( QLineEdit::Normal );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "SellPatientCard", QPoint(440,380) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bDlgLoaded = true;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardSell::~cDlgPatientCardSell()
{
    g_poPrefs->setDialogSize( "SellPatientCard", QPoint( width(), height() ) );

    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//===========================================================================================================
//
//----------------------------------------------------------------------------------------------
void cDlgPatientCardSell::timerEvent(QTimerEvent *)
{
    m_nTimerCounter++;

    if( m_nTimerCounter > 40 )
    {
        killTimer( m_nTimer );
        m_dlgProgress->hideProgress();
    }

    if( g_poCommRFID != NULL && g_poCommRFID->isRFIDConnected() )
    {
        QString qsRFID = g_poCommRFID->readRFID();

        if( qsRFID.length() > 0 )
        {
            try
            {
                // remove \n\r from the end
                qsRFID = qsRFID.left( qsRFID.length()-2 );

                ledRFID->setText( qsRFID );

                killTimer( m_nTimer );
                m_dlgProgress->hideProgress();
                cmbCardType->setFocus();
            }
            catch( cSevException &e )
            {
                g_obLogger(cSeverity::INFO) << "RFID [" << qsRFID << "] not found in database" << EOM;
                g_obGen.showTrayWarning( tr( "Reading card data failed or this card is not registered in database." ) );
            }

            g_obLogger(cSeverity::INFO) << "RFID read [" << qsRFID << "] " << EOM;
            slotRefreshWarningColors();
        }
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::setPatientCardOwner( const unsigned int p_uiPatientId )
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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::slotRefreshWarningColors()
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );
    chkRFID->setStyleSheet( "QCheckBox {font: normal;}" );

    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    if( cmbPatient->currentIndex() == 0 )
    {
        lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }
    if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( chkRFID->isChecked() && ledRFID->text().length() < 1 )
    {
        chkRFID->setStyleSheet( "QCheckBox {font: bold; color: red;}" );
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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::slotEnableButtons()
{
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::on_cmbCardType_currentIndexChanged(int index)
{
    if( !m_bDlgLoaded )
        return;

    m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

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

    cCurrency   cPrice( QString::number(m_poPatientCardType->price()/100), cCurrency::CURR_GROSS, m_poPatientCardType->vatpercent() );

    int priceTotal = cPrice.currencyValue().toInt()/100;
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

    cCurrency cDiscount( QString::number(discount) );

    if( discount != priceTotal )
        ledPrice->setText( QString("%1 (%2)").arg(cDiscount.currencyFullStringShort()).arg(cPrice.currencyFullStringShort()) );
    else
        ledPrice->setText( cPrice.currencyFullStringShort() );

    if( m_poPatientCardType->id() == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("You are not allowed to create System Service Patientcard.") );
        pbSell->setEnabled( false );
    }
    else if( !m_poPatientCard->active() )
    {
        pbSell->setEnabled( true );
    }

    slotRefreshWarningColors();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::on_cmbPatient_currentIndexChanged(int /*index*/)
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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::on_pbSell_clicked()
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

    if( chkRFID->isChecked() && ledRFID->text().length() < 1 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "RFID must be set." ) );
        chkRFID->setStyleSheet( "QCheckBox {font: bold; color: red;}" );
    }

    if( cmbCardType->currentIndex() == 0 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Invalid Patientcard type.\nFor active patientcard other type should be selected.\nIf you want to connect the card to this type,\ndeactivate the card." ) );
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
        qsErrorMessage.append( tr( "Number of available units can not be less then 1.\nIf you want to reset the number of available units\ndeactivate the card." ) );
        lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( deValidDateTo->date() < QDate::currentDate() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Incorrect validation date.\nIf you want to reset the date of validation\ndeactivate the card." ) );
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setRFID( ledRFID->text() );
            m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
            m_poPatientCard->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setComment( pteComment->toPlainText() );

            cDBShoppingCart obDBShoppingCart;
            bool            bShoppingCart = false;
            unsigned int    uiLedgerId = 0;

            // Szerviz kartyat nem kell eladni
            if( m_poPatientCardType->id() > 1 )
            {
                if( !g_obCassa.isCassaEnabled() )
                {
                    QMessageBox::warning( this, tr("Attention"),
                                          tr("Cassa is disabled!\n\n"
                                             "Please relogin to enable cassa.") );
                    return;
                }

                cCurrency   cPrice( QString::number(m_poPatientCardType->price()/100), cCurrency::CURR_GROSS, m_poPatientCardType->vatpercent() );

                int     inPriceTotal        = cPrice.currencyValue().toInt();
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

                cCurrency cDiscounted( QString::number(inPriceDiscounted) );

                obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
                obDBShoppingCart.setGuestId( m_poPatientCard->patientId() );
                obDBShoppingCart.setProductId( 0 );
                obDBShoppingCart.setPatientCardId( m_poPatientCard->id() );
                obDBShoppingCart.setPatientCardTypeId( m_poPatientCardType->id() );
                obDBShoppingCart.setPanelId( 0 );
                obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PC_SELL );
                obDBShoppingCart.setItemName( QString("%1 - %2").arg(m_poPatientCardType->name()).arg(m_poPatientCard->barcode()) );
                obDBShoppingCart.setItemCount( 1 );
                obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
                obDBShoppingCart.setItemVAT( m_poPatientCardType->vatpercent() );
                obDBShoppingCart.setItemDiscount( inPriceTotal - inPriceDiscounted );
                obDBShoppingCart.setItemSumPrice( inPriceDiscounted );

                cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

                obDlgCassaAction.setPayWithCash();

                int             inCassaAction   = obDlgCassaAction.exec();
                int             inPayType       = 0;
                QString         qsComment       = tr("Sell patientcard [%1]").arg(m_poPatientCard->barcode());
                unsigned int    uiCouponId = 0;
                //cDBDiscount     obDBDiscount;

                obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

                if( inCassaAction == QDialog::Accepted && !bShoppingCart )
                {
                    /*if( uiCouponId > 0 )
                    {
                        obDBDiscount.load( uiCouponId );

                        obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                    }*/
                    uiLedgerId = g_obCassa.cassaProcessPatientCardSell( *m_poPatientCard, obDBShoppingCart, qsComment, true, inPayType );
                }
                else if( inCassaAction != QDialog::Accepted )
                {
                    // Nem tortent meg az eladas
                    return;
                }
            }

            m_poPatientCard->setActive( true );
            m_poPatientCard->save();

            cDBPatientcardUnit  obDBPatientcardUnit;
            QStringList         qslUnitIds;

            for( int i=0; i<ledUnits->text().toInt(); i++ )
            {
                obDBPatientcardUnit.createNew();
                obDBPatientcardUnit.setLicenceId( m_poPatientCard->licenceId() );
                obDBPatientcardUnit.setPatientCardId( m_poPatientCard->id() );
                obDBPatientcardUnit.setPatientCardTypeId( m_poPatientCardType->id() );
                obDBPatientcardUnit.setLedgerId( uiLedgerId );
                obDBPatientcardUnit.setUnitTime( m_poPatientCardType->unitTime() );
                obDBPatientcardUnit.setUnitPrice( m_poPatientCardType->price()/ledUnits->text().toInt() );
                obDBPatientcardUnit.setValidDateFrom( m_poPatientCard->validDateFrom() );
                obDBPatientcardUnit.setValidDateTo( m_poPatientCard->validDateTo() );
                obDBPatientcardUnit.setDateTime( "" );
                obDBPatientcardUnit.setActive( true );
                obDBPatientcardUnit.save();
                qslUnitIds << QString::number( obDBPatientcardUnit.id() );
            }

            g_obLogger(cSeverity::INFO) << "Patientcard ["
                                        << m_poPatientCard->barcode()
                                        << "] sold with Type ["
                                        << m_poPatientCardType->name()
                                        << "] Units ["
                                        << ledUnits->text().toInt()
                                        << "]"
                                        << EOM;

            m_poPatientCard->synchronizeUnits();
            m_poPatientCard->synchronizeTime();
            m_poPatientCard->save();

            if( bShoppingCart )
            {
                obDBShoppingCart.setComment( qslUnitIds.join("#") );
                obDBShoppingCart.save();
            }

            m_poPatientCard->sendDataToWeb();

            QDate   deValidDateTo = QDate::fromString( m_poPatientCard->validDateTo(), "yyyy-MM-dd" );
            if( g_poPrefs->isAutoMailOnPCSell() )
            {
                g_obLogger(cSeverity::INFO) << "PatientCard sold, send auto mail about sell" << EOM;
                m_poPatientCard->sendAutoMail( AUTO_MAIL_ON_PCSELL, QDate::currentDate().toString("yyyy-MM-dd"), 0, "" );
            }
            if( g_poPrefs->isAutoMailOnPCExpiration() )
            {
                g_obLogger(cSeverity::INFO) << "PatientCard sold, send auto mail about expiration" << EOM;
                m_poPatientCard->sendAutoMail( AUTO_MAIL_ON_EXPIRE, deValidDateTo.addDays( g_poPrefs->getPCExpirationDays()*(-1) ).toString("yyyy-MM-dd"), 0, qslUnitIds.join(",") );
            }

            QDialog::accept();

        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), qsErrorMessage );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardSell::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardSell::on_pbSelectPatient_clicked()
{
    cDlgPatientSelect  obDlgPatientSelect( this );

    if( obDlgPatientSelect.exec() == QDialog::Accepted )
    {
        if( obDlgPatientSelect.selectedPatientId() > 0 )
        {
            cmbPatient->setCurrentIndex( cmbPatient->findData( obDlgPatientSelect.selectedPatientId() ) );
        }
    }
}

void cDlgPatientCardSell::on_pbCreatePatient_clicked()
{
    QSqlQuery *poQuery;

    cDBGuest *poGuest = new cDBGuest;
    poGuest->createNew();

    cDlgGuestEdit  obDlgEdit( this, poGuest );
    obDlgEdit.setWindowTitle( tr( "New Patient" ) );

    unsigned int    uiGuestId = 0;

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        uiGuestId = obDlgEdit.guestId();
    }

    cmbPatient->clear();
    cmbPatient->addItem( tr("<Not selected>"), 0 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
    while( poQuery->next() )
    {
        cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( uiGuestId == poQuery->value( 0 ) )
            cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
    }

    delete poGuest;
}

void cDlgPatientCardSell::on_chkRFID_clicked()
{
    pbRFID->setEnabled( chkRFID->isChecked() );
    if( !chkRFID->isChecked() )
    {
        ledRFID->setText( "" );
    }
    slotRefreshWarningColors();
}

void cDlgPatientCardSell::on_pbRFID_clicked()
{
    m_nTimerCounter = 0;
    m_nTimer = startTimer( 250 );
    m_dlgProgress->showInformation( tr( "Please read your RFID card!" ) );
}
