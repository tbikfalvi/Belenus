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

#include <QMessageBox>
//#include <ctime>

//===========================================================================================================

#include "dlgpatientcardedit.h"
#include "dlgpatientcardsell.h"
#include "dlgpatientcardrefill.h"
#include "../db/dbpatientcardunits.h"
#include "../dlg/dlgchangepcvalidity.h"
//#include "../db/dbpatientcardtype.h"
//#include "../dlg/dlgcassaaction.h"
//#include "../db/dbledger.h"
//#include "../db/dbshoppingcart.h"
//#include "../db/dbdiscount.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardEdit::cDlgPatientCardEdit( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
//-----------------------------------------------------------------------------------------------------------
{
    m_bIsCardDeactivated = false;
    m_bIsValidationChanged = false;

    setupUi( this );

    setWindowTitle( tr( "Patient card" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    pbCheckBarcode->setIcon( QIcon("./resources/40x40_question.png") );
    pbChangeValidity->setIcon( QIcon("./resources/40x40_edit.png") );
    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbDeactivate->setIcon( QIcon("./resources/40x40_delete.png") );
    pbSell->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbRefill->setIcon( QIcon("./resources/40x40_cassa.png") );

    m_poPatientCard     = p_poPatientCard;

    // Fill controls with values
    if( m_poPatientCard )
    {
        ledBarcode->setText( m_poPatientCard->barcode() );
        cmbCardType->addItem( tr("<Not selected>"), 0 );
        cmbPatient->addItem( tr("<Not selected>"), 0 );

        // Fill combo boxes only if card is active
        if( m_poPatientCard->active() )
        {
            QSqlQuery *poQuery;

            poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\"" ) );
            while( poQuery->next() )
            {
                // Add service card to list only if user is system administrator
                if( poQuery->value(0) == 1 && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
                    continue;

                cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
                if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
                    cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
            }

            poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
            while( poQuery->next() )
            {
                cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
                if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                    cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
            }
        }

        ledBalance->setText( QString( "%1 unit(s) including %2 minutes" ).arg(m_poPatientCard->units()).arg(m_poPatientCard->timeLeft()/60) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCard->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );
    }

    cmbCardType->setEnabled( m_poPatientCard->active() );
    cmbPatient->setEnabled( m_poPatientCard->active() );
    ledBalance->setEnabled( false );
    deValidDateFrom->setEnabled( false );
    deValidDateTo->setEnabled( false );
    pbChangeValidity->setEnabled( m_poPatientCard->active() );
    pbDeactivate->setEnabled( m_poPatientCard->active() );
    pbSell->setEnabled( !m_poPatientCard->active() );
    pbRefill->setEnabled( m_poPatientCard->active() );

    // If this is a service card, do not deactivate or modify sensitive data
    if( m_poPatientCard->patientCardTypeId() == 1 )
    {
        ledBarcode->setEnabled( false );
        pbCheckBarcode->setEnabled( false );
        cmbCardType->setEnabled( false );
        cmbPatient->setEnabled( false );
        pbChangeValidity->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        pbDeactivate->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        pbSell->setEnabled( false );
        pbRefill->setEnabled( false );
    }

    // If this is a partner card, do not modify it
    if( m_poPatientCard->parentId() > 0 )
    {
        cmbCardType->setEnabled( false );
        pbChangeValidity->setEnabled( false );
        pbSell->setEnabled( false );
        pbRefill->setEnabled( false );
    }

    slotRefreshWarningColors();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPatientCard", QPoint(440,380) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardEdit::~cDlgPatientCardEdit()
//-----------------------------------------------------------------------------------------------------------
{
    g_poPrefs->setDialogSize( "EditPatientCard", QPoint( width(), height() ) );

//    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::slotRefreshWarningColors()
//-----------------------------------------------------------------------------------------------------------
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    if( cmbCardType->currentIndex() == 0 )
    {
        lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    if( cmbPatient->currentIndex() == 0 )
    {
        lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }

    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );
    if( deValidDateTo->date() < QDate::currentDate() )
    {
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_ledBarcode_textChanged(const QString &arg1)
//-----------------------------------------------------------------------------------------------------------
{
    pbCheckBarcode->setIcon( QIcon("./resources/40x40_question.png") );
    pbCheckBarcode->setToolTip( tr("Click this button to verify if the barcode is valid.") );
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbCheckBarcode_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    QString qsToolTip = "";

    if( _checkCardJustForSave(&qsToolTip) )
    {
        pbCheckBarcode->setIcon( QIcon("./resources/40x40_ok.png") );
        qsToolTip = tr( "Barcode is ok." );
    }
    else
    {
        pbCheckBarcode->setIcon( QIcon("./resources/40x40_minus.png") );
    }
    pbCheckBarcode->setToolTip( qsToolTip );
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbChangeValidity_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    cDlgChangePCValidity    obDlgChangePCValidity;

    if( obDlgChangePCValidity.exec() == QDialog::Accepted )
    {
        QDate   qdNewDate;

        cDlgChangePCValidity::selValidity   dateSelection = obDlgChangePCValidity.selectionValidity( &qdNewDate );

        switch( dateSelection )
        {
            case cDlgChangePCValidity::SV_ONEDAY:
                qdNewDate = deValidDateTo->date().addDays(1);
                break;
            case cDlgChangePCValidity::SV_ONEWEEK:
                qdNewDate = deValidDateTo->date().addDays(7);
                break;
            case cDlgChangePCValidity::SV_TWOWEEKS:
                qdNewDate = deValidDateTo->date().addDays(14);
                break;
            case cDlgChangePCValidity::SV_ONEMONTH:
                qdNewDate = deValidDateTo->date().addMonths(1);
                break;
            case cDlgChangePCValidity::SV_THREEMONTHS:
                qdNewDate = deValidDateTo->date().addMonths(3);
                break;
            case cDlgChangePCValidity::SV_HALFYEAR:
                qdNewDate = deValidDateTo->date().addMonths(6);
                break;
            case cDlgChangePCValidity::SV_CUSTOM:
                break;
            default:
                dateSelection = cDlgChangePCValidity::SV_NONE;
        }
        if( dateSelection != cDlgChangePCValidity::SV_NONE )
        {
            if( QMessageBox::question( this, tr("Attention"),
                                       tr("Are you sure you want to change the validity date\n"
                                          "from:  %1  to:  %2  ?").arg( deValidDateTo->date().toString("yyyy-MM-dd") ).arg( qdNewDate.toString("yyyy-MM-dd") ),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
            {
                deValidDateTo->setDate( qdNewDate );
                m_bIsValidationChanged = true;
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbSave_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    if( m_bIsCardDeactivated )
    {
        QDialog::accept();
    }

    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    boCanBeSaved = _checkCardJustForSave( &qsErrorMessage );

    if( m_poPatientCard->active() )
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
    }

    if( boCanBeSaved )
    {
        m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPatientCard->setBarcode( ledBarcode->text() );
        m_poPatientCard->setPatientCardTypeId( cmbCardType->itemData( cmbCardType->currentIndex() ).toUInt() );
        m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
        m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
        m_poPatientCard->setComment( pteComment->toPlainText() );
        m_poPatientCard->save();
        if( m_bIsValidationChanged )
        {
            m_poPatientCard->updateActiveUnits( deValidDateTo->date() );
        }
        m_poPatientCard->synchronizeUnits();

        QDialog::accept();
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbDeactivate_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::warning( this,
                              tr("Attention"),
                              tr("Are you sure you want to deactivate this patientcard?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
    {
        m_bIsCardDeactivated = true;

        m_poPatientCard->setPatientCardTypeId( 0 );
        m_poPatientCard->setParentId( 0 );
        m_poPatientCard->setPatientId( 0 );
        m_poPatientCard->setUnits( 0 );
        m_poPatientCard->setAmount( 0 );
        m_poPatientCard->setTimeLeft( 0 );
        m_poPatientCard->setValidDateFrom( "0000-00-00" );
        m_poPatientCard->setValidDateTo( "0000-00-00" );
        m_poPatientCard->setActive( false );
        m_poPatientCard->save();

        cmbCardType->setCurrentIndex( 0 );
        cmbPatient->setCurrentIndex( 0 );
        ledBalance->setText( "" );
        deValidDateFrom->setDate( QDate(0,0,0) );
        deValidDateTo->setDate( QDate(0,0,0) );

        cDBPatientcardUnit  obDBPatientcardUnit;

        obDBPatientcardUnit.deactivateUnits( m_poPatientCard->id() );

        cmbCardType->setEnabled( m_poPatientCard->active() );
        cmbPatient->setEnabled( m_poPatientCard->active() );
        ledBalance->setEnabled( false );
        deValidDateFrom->setEnabled( false );
        deValidDateTo->setEnabled( false );
        pbChangeValidity->setEnabled( m_poPatientCard->active() );
        pbDeactivate->setEnabled( m_poPatientCard->active() );
        pbSell->setEnabled( !m_poPatientCard->active() );
        pbRefill->setEnabled( m_poPatientCard->active() );

        pbCancel->setEnabled( false );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbSell_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    QString qsErrorMessage = "";

    if( _checkCardJustForSave( &qsErrorMessage ) )
    {
        m_poPatientCard->setBarcode( ledBarcode->text() );
        m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPatientCard->save();

        cDlgPatientCardSell obDlgPatientCardSell( this, m_poPatientCard );
        obDlgPatientCardSell.setPatientCardOwner( g_obGuest.id() );
        if( obDlgPatientCardSell.exec() == QDialog::Accepted )
        {
            QDialog::accept();
        }
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbRefill_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  obDBPatientCard;

    obDBPatientCard.load( m_poPatientCard->barcode() );

    if( obDBPatientCard.timeLeft() < 1 )
    {
        obDBPatientCard.setPatientCardTypeId( 0 );
    }
    cDlgPatientCardRefill obDlgPatientCardRefill( this, &obDBPatientCard );

    if( obDlgPatientCardRefill.exec() == QDialog::Accepted )
    {
        QDialog::accept();
    }
}
//-----------------------------------------------------------------------------------------------------------
bool cDlgPatientCardEdit::_checkCardJustForSave( QString *p_qsErrorMessage )
//-----------------------------------------------------------------------------------------------------------
{
    bool boCanBeSaved = true;

    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );

    if( ledBarcode->text() == "" )
    {
        boCanBeSaved = false;
        p_qsErrorMessage->append( tr( "Barcode cannot be empty." ) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        boCanBeSaved = false;
        if( p_qsErrorMessage->length() ) p_qsErrorMessage->append( "\n\n" );
        p_qsErrorMessage->append( tr( "Invalid barcode. Barcode should be %1 character length." ).arg(g_poPrefs->getBarcodeLength()) );
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
        if( poQuery->numRowsAffected() > 0 )
        {
            boCanBeSaved = false;
            if( p_qsErrorMessage->length() ) p_qsErrorMessage->append( "\n\n" );
            p_qsErrorMessage->append( tr( "Invalid barcode. This barcode already saved into database."  ) );
            lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }

    return boCanBeSaved;
}
//-----------------------------------------------------------------------------------------------------------








/*void cDlgPatientCardEdit::slotEnableButtons()
{
    // If this is a NOT service card and not partner card
    if( m_poPatientCard->patientCardTypeId() != 1 && m_poPatientCard->parentId() == 0 )
    {
        if( m_poPatientCard->active() )
        {
            pbDeactivate->setEnabled( true );
            pbSell->setEnabled( false );
            pbRefill->setEnabled( true );
        }
        else
        {
            pbDeactivate->setEnabled( false );
            pbSell->setEnabled( true );
            pbRefill->setEnabled( false );
        }
    }
}*/
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
/*void cDlgPatientCardEdit::setPatientCardOwner( const unsigned int p_uiPatientId )
{
    for( int i=0; i<cmbPatient->count(); i++ )
    {
        if( cmbPatient->itemData(i).toUInt() == p_uiPatientId )
        {
            cmbPatient->setCurrentIndex(i);
            break;
        }
    }
}*/
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
/*void cDlgPatientCardEdit::on_pbSave_clicked()
{
    if( m_bIsCardDeactivated )
    {
        QDialog::accept();
    }

    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    boCanBeSaved = _checkCardJustForSave( &qsErrorMessage );

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

            cDBPatientCardType  obDBPatientCardType;

            obDBPatientCardType.load( cmbCardType->itemData( cmbCardType->currentIndex() ).toUInt() );

            int     inNewUnits = ledUnits->text().toInt() - m_poPatientCard->units();

            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setActive( cbActive->isChecked() );
            m_poPatientCard->setPatientCardTypeId( obDBPatientCardType.id() );
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
                obDBShoppingCart.setPatientCardTypeId( m_poPatientCard->patientCardTypeId() );
                obDBShoppingCart.setPanelId( 0 );
                obDBShoppingCart.setLedgerTypeId( m_bNewCard?cDBLedger::LT_PC_SELL:cDBLedger::LT_PC_REFILL );
                obDBShoppingCart.setItemName( QString("%1 - %2").arg(m_poPatientCardType->name()).arg(m_poPatientCard->barcode()) );
                obDBShoppingCart.setItemCount( 1 );
                obDBShoppingCart.setItemNetPrice( m_poPatientCardType->price() );
                obDBShoppingCart.setItemVAT( m_poPatientCardType->vatpercent() );
                obDBShoppingCart.setItemDiscount( inPriceTotal - inPriceDiscounted );
                obDBShoppingCart.setItemSumPrice( inPriceDiscounted );

                cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

                obDlgCassaAction.setPayWithCash();

                int             inCassaAction   = obDlgCassaAction.exec();
                int             inPayType       = 0;
                QString         qsComment       = QString("[%1] - %2").arg(m_poPatientCard->barcode()).arg(m_poPatientCard->comment());
                bool            bShoppingCart   = false;
                unsigned int    uiCouponId = 0;
                cDBDiscount     obDBDiscount;

                obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

                if( inCassaAction == QDialog::Accepted && !bShoppingCart )
                {
//                    if( uiCouponId > 0 )
//                    {
//                        obDBDiscount.load( uiCouponId );
//
//                        obDBShoppingCart.setItemDiscount( obDBShoppingCart.itemDiscount()+obDBDiscount.discount(obDBShoppingCart.itemSumPrice()) );
//                    }
                    g_obCassa.cassaProcessPatientCardSell( *m_poPatientCard, obDBShoppingCart, qsComment, m_bNewCard, inPayType );
                }
                else if( inCassaAction != QDialog::Accepted )
                {
                    // Nem tortent meg az eladas
                    return;
                }
            }

            for( int i=0; i<inNewUnits; i++ )
            {
                cDBPatientcardUnit  obDBPatientcardUnit;

                obDBPatientcardUnit.setLicenceId( m_poPatientCard->licenceId() );
                obDBPatientcardUnit.setPatientCardId( m_poPatientCard->id() );
                obDBPatientcardUnit.setUnitTime( obDBPatientCardType.unitTime() );
                obDBPatientcardUnit.setUnitPrice( obDBPatientCardType.price()/inNewUnits );
                obDBPatientcardUnit.setValidDateFrom( obDBPatientCardType.validDateFrom() );
                obDBPatientcardUnit.setValidDateTo( obDBPatientCardType.validDateTo() );
                obDBPatientcardUnit.setActive( true );
                obDBPatientcardUnit.save();
            }

            m_poPatientCard->save();
            m_poPatientCard->synchronizeUnits();

            if( !m_bIsCardDeactivated )
            {
                QDialog::accept();
            }
            m_bIsCardDeactivated = false;
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }

}*/

/*
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

        cCurrency   cPrice( m_poPatientCardType->price(), cCurrency::CURR_GROSS, m_poPatientCardType->vatpercent() );

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
*/

/*
void cDlgPatientCardEdit::on_cmbPatient_currentIndexChanged(int)
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
*/

/*
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
*/


