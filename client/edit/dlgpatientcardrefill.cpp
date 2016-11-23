//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgpatientcardrefill.cpp
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

#include "dlgpatientcardrefill.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgcassaaction.h"
#include "../db/dbledger.h"
#include "../db/dbshoppingcart.h"
#include "../db/dbpatientcardunits.h"
#include "../db/dbdiscount.h"
#include "../crud/dlgpatientselect.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardRefill::cDlgPatientCardRefill( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;
    m_bDlgLoaded        = false;

    setWindowTitle( tr( "Refilling Patientcard" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );
    pbSell->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbSelectPatient->setIcon( QIcon("./resources/40x40_search.png") );
    deValidDateFrom->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );
    deValidDateTo->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );

        cmbCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            if( poQuery->value(0) == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
                continue;

            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
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
cDlgPatientCardRefill::~cDlgPatientCardRefill()
{
    g_poPrefs->setDialogSize( "SellPatientCard", QPoint( width(), height() ) );

    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::setPatientCardOwner( const unsigned int p_uiPatientId )
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
void cDlgPatientCardRefill::slotRefreshWarningColors()
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::slotEnableButtons()
{
    if( m_poPatientCard->parentId() > 0 )
    {
        pbSell->setEnabled( false );

        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( m_poPatientCard->parentId() );

        QMessageBox::warning( this, tr("Attention"),
                              tr("This patientcard attached to another card therefore it can not be refilled.\n\n"
                                 "Please refill the main patientcard with barcode: %1").arg(obDBPatientCard.barcode()) );
    }
    else
    {
        if( m_poPatientCardType->id() == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("You are not allowed to create System Service Patientcard.") );
            pbSell->setEnabled( false );
        }
        else
        {
            pbSell->setEnabled( true );
        }
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::on_cmbCardType_currentIndexChanged(int index)
{
    if( !m_bDlgLoaded )
        return;

    m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

    unsigned int    uiUnits = m_poPatientCardType->units();// + m_poPatientCard->units();
    unsigned int    uiUnitTime = m_poPatientCardType->units()*m_poPatientCardType->unitTime()*60;// + m_poPatientCard->timeLeft();

    ledUnits->setText( QString::number(uiUnits) );
    teTimeLeft->setTime( QTime(uiUnitTime/3600,(uiUnitTime%3600)/60,(uiUnitTime%3600)%60,0) );

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

    slotRefreshWarningColors();
    slotEnableButtons();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::on_cmbPatient_currentIndexChanged(int /*index*/)
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
void cDlgPatientCardRefill::on_pbSell_clicked()
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
        //ledBarcode->setEnabled( true );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Invalid barcode. Barcode should be %1 character length." ).arg(g_poPrefs->getBarcodeLength()) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        //ledBarcode->setEnabled( true );
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
            //ledBarcode->setEnabled( true );
        }
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
        qsErrorMessage.append( tr( "Incorrect validation date.\nThe patientcard with the selected units would not be used." ) );
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCard->setActive( true );
            m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );

            if( deValidDateTo->date() > QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") )
            {
                m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
            }
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
                obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PC_REFILL );
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
                QString         qsComment       = tr("Refill patientcard [%1]").arg(m_poPatientCard->barcode());
                unsigned int    uiCouponId = 0;
                cDBDiscount     obDBDiscount;

                obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

                if( inCassaAction == QDialog::Accepted && !bShoppingCart )
                {
                    /*if( uiCouponId > 0 )
                    {
                        obDBDiscount.load( uiCouponId );

                        obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
                    }*/
                    uiLedgerId = g_obCassa.cassaProcessPatientCardRefill( *m_poPatientCard, obDBShoppingCart, qsComment, inPayType );
                }
                else if( inCassaAction != QDialog::Accepted )
                {
                    // Nem tortent meg az eladas
                    return;
                }
            }

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
                obDBPatientcardUnit.setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
                obDBPatientcardUnit.setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
                obDBPatientcardUnit.setDateTime( "" );
                obDBPatientcardUnit.setActive( true );
                obDBPatientcardUnit.save();
                qslUnitIds << QString::number( obDBPatientcardUnit.id() );
            }

            g_obLogger(cSeverity::INFO) << "Patientcard ["
                                        << m_poPatientCard->barcode()
                                        << "] refilled with Type ["
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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardRefill::on_pbSelectPatient_clicked()
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
