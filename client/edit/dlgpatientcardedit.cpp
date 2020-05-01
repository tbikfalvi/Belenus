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
#include "../communication_rfid.h"
#include "../edit/dlgaddunits.h"
#include "../edit/dlgremoveunits.h"

extern cCommRFID       *g_poCommRFID;

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgPatientCardEdit::cDlgPatientCardEdit( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
//-----------------------------------------------------------------------------------------------------------
{
    m_bIsCardDeactivated = false;
//    m_bIsValidationChanged = false;

    setupUi( this );

    m_dlgProgress = new cDlgProgress( this );

    setWindowTitle( tr( "Patient card" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    pbCheckBarcode->setIcon( QIcon("./resources/40x40_question.png") );
    pbChangeValidity->setIcon( QIcon("./resources/40x40_edit.png") );
    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbDeactivate->setIcon( QIcon("./resources/40x40_delete.png") );
    pbSell->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbRefill->setIcon( QIcon("./resources/40x40_cassa.png") );
    pbRFID->setIcon( QIcon("./resources/40x40_rfid.png") );
    pbAddUnits->setIcon( QIcon("./resources/40x40_patientcard.png") );
    pbRemoveUnits->setIcon( QIcon("./resources/40x40_patientcard.png") );

    deValidDateFrom->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );
    deValidDateTo->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    m_poPatientCard     = p_poPatientCard;

    chkIsRFID->setChecked( false );

    // Fill controls with values
    if( m_poPatientCard )
    {
        ledBarcode->setText( m_poPatientCard->barcode() );
        chkService->setChecked( m_poPatientCard->servicecard() );
        if( m_poPatientCard->RFID().length() > 0 )
        {
            chkIsRFID->setChecked( true );
            pbRFID->setText( tr("Reassign RFID") );
        }
        ledRFID->setText( m_poPatientCard->RFID() );
//        cmbCardType->addItem( tr("<Not selected>"), 0 );
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
/*
                cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
                if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
                    cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
*/
            }

            poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
            while( poQuery->next() )
            {
                cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
                if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                    cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
            }
        }

        lblUnitInfo->setText( g_obGen.getPatientCardInformationString( m_poPatientCard->barcode() ) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCard->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCard->validDateTo(),"yyyy-MM-dd") );
        pteComment->setPlainText( m_poPatientCard->comment() );
    }

    ledRFID->setVisible( false );
lblCardType->setVisible( false );
cmbCardType->setEnabled( false/*m_poPatientCard->active()*/ );
cmbCardType->setVisible( false );

    cmbPatient->setEnabled( m_poPatientCard->active() );
    deValidDateFrom->setEnabled( false );
    deValidDateTo->setEnabled( false );
    pbChangeValidity->setEnabled( m_poPatientCard->active() );
    pbDeactivate->setEnabled( m_poPatientCard->active() );
    pbSell->setEnabled( !m_poPatientCard->active() );
    pbRefill->setEnabled( m_poPatientCard->active() );
    pbAddUnits->setEnabled( m_poPatientCard->active() );
    pbRemoveUnits->setEnabled( m_poPatientCard->active() );

    // If this is a service card, do not deactivate or modify sensitive data
    if( m_poPatientCard->patientCardTypeId() == 1 )
    {
        ledBarcode->setEnabled( false );
        pbCheckBarcode->setEnabled( false );
//        cmbCardType->setEnabled( false );
        cmbPatient->setEnabled( false );
        pbChangeValidity->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        pbDeactivate->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
        pbSell->setEnabled( false );
        pbRefill->setEnabled( false );
        pbAddUnits->setEnabled( false );
        pbRemoveUnits->setEnabled( false );
    }

    // If this is a partner card, do not modify it
    if( m_poPatientCard->parentId() > 0 )
    {
        ledBarcode->setEnabled( false );
        pbCheckBarcode->setEnabled( false );
//        cmbCardType->setEnabled( false );
        pbChangeValidity->setEnabled( false );
        pbSell->setEnabled( false );
        pbRefill->setEnabled( false );
        pbAddUnits->setEnabled( false );
        pbRemoveUnits->setEnabled( false );
    }

    slotRefreshWarningColors();

    if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledBarcode->setEchoMode( QLineEdit::Password );
    }
    else
    {
        ledBarcode->setEchoMode( QLineEdit::Normal );
    }

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
void cDlgPatientCardEdit::timerEvent(QTimerEvent *)
//-----------------------------------------------------------------------------------------------------------
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
                chkIsRFID->setChecked( true );

                killTimer( m_nTimer );
                m_dlgProgress->hideProgress();
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
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::slotRefreshWarningColors()
//-----------------------------------------------------------------------------------------------------------
{
    lblBarcode->setStyleSheet( "QLabel {font: normal;}" );
    chkIsRFID->setStyleSheet( "QCheckBox {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        lblBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( chkIsRFID->isChecked() && ledRFID->text().length() < 1 )
    {
        chkIsRFID->setStyleSheet( "QCheckBox {font: bold; color: red;}" );
    }
/*
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    if( cmbCardType->currentIndex() == 0 )
    {
        lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
*/
    if( cmbPatient->currentIndex() == 0 )
    {
        lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }

    if( deValidDateTo->date() < QDate::currentDate() )
    {
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_ledBarcode_textChanged(const QString &/*arg1*/)
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
    cDlgChangePCValidity    obDlgChangePCValidity( this, m_poPatientCard->id() );

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
                                       tr("Are you sure you want to change the validity date to\n"
                                          " %1 \n"
                                          "for the selected patientcard units?\n\n"
                                          "Please note this change cannot be undone!")
                                       .arg( qdNewDate.toString(g_poPrefs->getDateFormat().replace("-",".")) ),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
            {
                if( qdNewDate > deValidDateTo->date() )
                {
                    deValidDateTo->setDate( qdNewDate );
                }
                m_qsUnitCondition = obDlgChangePCValidity.unitCondition();
                m_poPatientCard->updateActiveUnits( qdNewDate, m_qsUnitCondition );
                m_poPatientCard->synchronizeUnits();
            }
        }
        lblUnitInfo->setText( g_obGen.getPatientCardInformationString( m_poPatientCard->barcode() ) );
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
//    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblPatient->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    boCanBeSaved = _checkCardJustForSave( &qsErrorMessage );

    if( m_poPatientCard->active() )
    {
/*
        if( cmbCardType->currentIndex() == 0 )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Invalid Patientcard type.\nFor active patientcard other type should be selected.\nIf you want to connect the card to this type,\ndeactivate the card with unchecking the Active checkbox." ) );
            lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
*/
        if( cmbPatient->currentIndex() == 0 )
        {
            lblPatient->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        }
    }

    if( boCanBeSaved )
    {
        m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPatientCard->setBarcode( ledBarcode->text() );
        if( chkIsRFID->isChecked() )
        {
            m_poPatientCard->setRFID( ledRFID->text() );
        }
//        m_poPatientCard->setPatientCardTypeId( cmbCardType->itemData( cmbCardType->currentIndex() ).toUInt() );
        m_poPatientCard->setPatientId( cmbPatient->itemData( cmbPatient->currentIndex() ).toUInt() );
        m_poPatientCard->setValidDateTo( deValidDateTo->date().toString("yyyy-MM-dd") );
        m_poPatientCard->setComment( pteComment->toPlainText() );
        m_poPatientCard->save();
/*        if( m_bIsValidationChanged )
        {
            m_poPatientCard->updateActiveUnits( deValidDateTo->date(), m_qsUnitCondition );
        }
        m_poPatientCard->synchronizeUnits();*/

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

        m_poPatientCard->deactivate();

//        cmbCardType->setCurrentIndex( 0 );
        cmbPatient->setCurrentIndex( 0 );
        deValidDateFrom->setDate( QDate(0,0,0) );
        deValidDateTo->setDate( QDate(0,0,0) );

//        cmbCardType->setEnabled( m_poPatientCard->active() );
        cmbPatient->setEnabled( m_poPatientCard->active() );
        deValidDateFrom->setEnabled( false );
        deValidDateTo->setEnabled( false );
        pbChangeValidity->setEnabled( m_poPatientCard->active() );
        pbDeactivate->setEnabled( m_poPatientCard->active() );
        pbSell->setEnabled( !m_poPatientCard->active() );
        pbRefill->setEnabled( m_poPatientCard->active() );
        lblUnitInfo->setText( g_obGen.getPatientCardInformationString( m_poPatientCard->barcode() ) );

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
    else if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
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
void cDlgPatientCardEdit::on_pbRFID_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    m_nTimerCounter = 0;
    m_nTimer = startTimer( 500 );
    m_dlgProgress->showInformation( tr( "Please read your RFID card!" ) );
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_chkIsRFID_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    if( !chkIsRFID->isChecked() )
    {
        chkIsRFID->setEnabled( false );
        ledRFID->setText( "" );
    }
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbAddUnits_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    cDlgAddUnits    obDlgAddUnits( this, m_poPatientCard );

    obDlgAddUnits.exec();

    m_poPatientCard->synchronizeUnits();
    lblUnitInfo->setText( g_obGen.getPatientCardInformationString( m_poPatientCard->barcode() ) );
}
//-----------------------------------------------------------------------------------------------------------
void cDlgPatientCardEdit::on_pbRemoveUnits_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    cDlgRemoveUnits    obDlgRemoveUnits( this, m_poPatientCard );

    obDlgRemoveUnits.exec();

    m_poPatientCard->synchronizeUnits();
    lblUnitInfo->setText( g_obGen.getPatientCardInformationString( m_poPatientCard->barcode() ) );
}
//-----------------------------------------------------------------------------------------------------------
