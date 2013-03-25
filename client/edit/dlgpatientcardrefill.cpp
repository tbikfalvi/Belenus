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

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );

        cmbCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            if( poQuery->value(0) == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
                continue;

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

        ledUnits->setText( QString::number(m_poPatientCard->units()) );
        teTimeLeft->setTime( QTime( m_poPatientCard->timeLeft()/3600, (m_poPatientCard->timeLeft()%3600)/60, (m_poPatientCard->timeLeft()%3600)%60, 0 ) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCard->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );
    }

    slotRefreshWarningColors();
    slotEnableButtons();

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
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::on_cmbCardType_currentIndexChanged(int index)
{
    if( !m_bDlgLoaded )
        return;

    m_poPatientCardType->load( cmbCardType->itemData( index ).toInt() );

    unsigned int    uiUnits = m_poPatientCardType->units() + m_poPatientCard->units();
    unsigned int    uiUnitTime = m_poPatientCardType->units()*m_poPatientCardType->unitTime()*60 + m_poPatientCard->timeLeft();

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

    int priceTotal = cPrice.currencyValue().toInt();
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
    else
    {
        pbSell->setEnabled( true );
    }

    slotRefreshWarningColors();
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
        ledBarcode->setEnabled( true );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Invalid barcode. Barcode should be %1 character length." ).arg(g_poPrefs->getBarcodeLength()) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        ledBarcode->setEnabled( true );
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
            ledBarcode->setEnabled( true );
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
        qsErrorMessage.append( tr( "Incorrect validation date.\nIf you want to reset the date of validation\ndeactivate the card." ) );
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setActive( true );
            m_poPatientCard->setPatientCardTypeId( cmbCardType->itemData( cmbCardType->currentIndex() ).toUInt() );
            m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
            m_poPatientCard->setUnits( ledUnits->text().toInt() );
            m_poPatientCard->setTimeLeftStr( teTimeLeft->time().toString("hh:mm:ss") );
            m_poPatientCard->setValidDateFrom( deValidDateFrom->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
            m_poPatientCard->setComment( pteComment->toPlainText() );

            // Szerviz kartyat nem kell eladni
            if( m_poPatientCard->patientCardTypeId() > 1 )
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
                obDBShoppingCart.setLedgerTypeId( cDBShoppingCart::LT_PC_REFILL );
                obDBShoppingCart.setItemName( QString("%1 - %2").arg(m_poPatientCardType->name()).arg(m_poPatientCard->barcode()) );
                obDBShoppingCart.setItemCount( 1 );
                obDBShoppingCart.setItemNetPrice( m_poPatientCardType->price() );
                obDBShoppingCart.setItemVAT( m_poPatientCardType->vatpercent() );
                obDBShoppingCart.setItemDiscount( inPriceTotal - inPriceDiscounted );
                obDBShoppingCart.setItemSumPrice( inPriceDiscounted );

                cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

                obDlgCassaAction.setPayWithCash();

                int     inCassaAction   = obDlgCassaAction.exec();
                int     inPayType       = 0;
                QString qsComment       = tr("Refill patientcard [%1]").arg(m_poPatientCard->barcode());
                bool    bShoppingCart   = false;

                obDlgCassaAction.cassaResult( &inPayType, &qsComment, &bShoppingCart );

                if( inCassaAction == QDialog::Accepted && !bShoppingCart )
                {
                    g_obCassa.cassaProcessPatientCardRefill( *m_poPatientCard, obDBShoppingCart, qsComment, inPayType );
                }
                else if( inCassaAction != QDialog::Accepted )
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
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardRefill::on_pbCancel_clicked()
{
    QDialog::reject();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
QString cDlgPatientCardRefill::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
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
