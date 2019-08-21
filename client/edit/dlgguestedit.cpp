//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgguestedit.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//===========================================================================================================
// Szolarium studioba betero vendegek adatait kezelo ablak.
//===========================================================================================================

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <iostream>

//===========================================================================================================

#include "dlgguestedit.h"
#include "belenus.h"
#include "../framework/sevexception.h"
#include "../db/dbpatientcardtype.h"
#include "../db/dbdiscount.h"
#include "../crud/dlgaddress.h"
#include "../crud/dlgpatientcardselect.h"
#include "../edit/dlgdiscountedit.h"
#include "../dlg/dlginputstart.h"
#include "dlgpatientcardsell.h"

//===========================================================================================================
//  cDlgGuestEdit
//-----------------------------------------------------------------------------------------------------------
//  Konstruktor
//-----------------------------------------------------------------------------------------------------------
//  Parameterek:
//      p_poParent      Az ablakot meghivo szulo widget-re mutato pointer
//      p_poGuest       A vendeg adatait tartamlazo objektumra mutato pointer
//-----------------------------------------------------------------------------------------------------------
cDlgGuestEdit::cDlgGuestEdit( QWidget *p_poParent, cDBGuest *p_poGuest, cDBPostponed* ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgGuestEdit::cDlgGuestEdit" );

    setupUi( this );

    m_bInit = true;

    m_dlgProgress = new cDlgProgress( this );

    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    pbSaveExit->setIcon( QIcon("./resources/40x40_ok.png") );
    pbSave->setIcon( QIcon("./resources/40x40_save.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbHistory->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssignCard->setIcon( QIcon("./resources/40x40_patientcard_join.png") );
    pbDislink->setIcon( QIcon("./resources/40x40_patientcard_disjoin.png") );
    pbSellCard->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    pbEditDiscount->setIcon( QIcon("./resources/40x40_edit.png") );
    lblAssignCardInfo->setPixmap( QPixmap("./resources/40x40_information.png") );

    deBirthDate->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    chkService->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    chkEmployee->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    chkRegularCustomer->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    m_poPatientCard = new cDBPatientCard();

    m_poGuest = p_poGuest;

    if( m_poGuest )
    {
        ledName->setText( m_poGuest->name() );
        if( m_poGuest->gender() == 1 )              rbGenderMale->setChecked(true);
        else if( m_poGuest->gender() == 2 )         rbGenderFemale->setChecked(true);
        if( m_poGuest->dateBirth().length() > 0 )
        {
            deBirthDate->setDate( QDate::fromString(m_poGuest->dateBirth(),"yyyy-MM-dd") );
            int nYear = QDate::fromString(m_poGuest->dateBirth(),"yyyy-MM-dd").daysTo( QDate::currentDate() ) / 365.25;

            if( nYear < 18 )                    rbAge1->setChecked( true );
            else if( 17 < nYear && nYear < 21 ) rbAge2->setChecked( true );
            else if( 20 < nYear && nYear < 31 ) rbAge3->setChecked( true );
            else if( 30 < nYear && nYear < 41 ) rbAge4->setChecked( true );
            else if( 40 < nYear && nYear < 51 ) rbAge5->setChecked( true );
            else if( 50 < nYear && nYear < 61 ) rbAge6->setChecked( true );
            else if( 60 < nYear )               rbAge7->setChecked( true );
        }
        else
        {
            deBirthDate->setDate( QDate(2000,1,1) );
            switch( m_poGuest->ageType() )
            {
                case 1:
                    rbAge1->setChecked( true );
                    break;
                case 2:
                    rbAge2->setChecked( true );
                    break;
                case 3:
                    rbAge3->setChecked( true );
                    break;
                case 4:
                    rbAge4->setChecked( true );
                    break;
                case 5:
                    rbAge5->setChecked( true );
                    break;
                case 6:
                    rbAge6->setChecked( true );
                    break;
                case 7:
                    rbAge7->setChecked( true );
                    break;
                default:
                    rbAge0->setChecked( true );
            }
        }
        QDate   qdRegistration( QDate::fromString(m_poGuest->dateCreated().left(10),"yyyy-MM-dd") );
        ledMembership->setText( m_poGuest->membership() );
        ledUniqueId->setText( m_poGuest->uniqueId() );
        ledEmail->setText( m_poGuest->email() );
        ledPhone->setText( m_poGuest->mobile() );
        teAddress->setText( m_poGuest->address() );
        teComment->setText( m_poGuest->comment() );
        chkReturning->setChecked( m_poGuest->isReturning() );
        ledDateOfRegistration->setText( qdRegistration.toString(g_poPrefs->getDateFormat()) );
        chkService->setChecked( m_poGuest->service() );
        chkEmployee->setChecked( m_poGuest->employee() );
        chkRegularCustomer->setChecked( m_poGuest->regularCustomer() );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT skinTypeId, skinTypeName FROM skintypes WHERE active=1 AND archive<>\"DEL\" ORDER BY skinTypeName " ) );
        while( poQuery->next() )
        {
            cmbSkinType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poGuest->skinTypeId() == poQuery->value( 0 ) )
                cmbSkinType->setCurrentIndex( cmbSkinType->count()-1 );
        }

        cDBDiscount obDBDiscount;

        try
        {
            obDBDiscount.loadGuest( m_poGuest->id() );
            pbEditDiscount->setEnabled( true );
            if( obDBDiscount.discountPercent() > 0 )
            {
                rbDiscountPercent->setChecked( true );
                ledDiscount->setText( QString::number(obDBDiscount.discountPercent()) );
            }
            else
            {
                rbDiscountValue->setChecked( true );
                ledDiscount->setText( QString::number(obDBDiscount.discountValue()) );
            }
        }
        catch( cSevException &e )
        {
            pbEditDiscount->setEnabled( false );
            if( QString(e.what()).compare("Discount id not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
            else
            {
                rbDiscountPercent->setChecked( false );
                rbDiscountValue->setChecked( false );
                ledDiscount->setText( "" );
            }
        }
        slotUpdateDiscountSample();

        ledLoyaltyPoints->setText( QString::number( m_poGuest->loyaltyPoints() ) );

        _fillPatientCardData();

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            if( m_poGuest->licenceId() == 0 && m_poGuest->id() > 0 )
            {
                ledName->setEnabled( false );
                rbGenderFemale->setEnabled( false );
                rbGenderMale->setEnabled( false );
                ledEmail->setEnabled( false );
                rbAge0->setEnabled( false );
                rbAge1->setEnabled( false );
                rbAge2->setEnabled( false );
                rbAge3->setEnabled( false );
                rbAge4->setEnabled( false );
                rbAge5->setEnabled( false );
                rbAge6->setEnabled( false );
                rbAge7->setEnabled( false );
                chkReturning->setEnabled( false );

                pbSave->setEnabled( false );
            }
        }
    }

    connect( ledName, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledEmail, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbGenderMale, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbGenderFemale, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbAge0, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );

    connect( chkRegularCustomer, SIGNAL(toggled(bool)), this, SLOT(slotUpdateDiscountSample()) );
    connect( chkEmployee, SIGNAL(toggled(bool)), this, SLOT(slotUpdateDiscountSample()) );
    connect( chkService, SIGNAL(toggled(bool)), this, SLOT(slotUpdateDiscountSample()) );

    slotEnableButtons();
    slotRefreshWarningColors();

    // Temporary until patient history will be implemented
    pbHistory->setVisible( false );

    if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledBarcode->setEchoMode( QLineEdit::Password );
    }
    else
    {
        ledBarcode->setEchoMode( QLineEdit::Normal );
    }

    m_bInit = false;
}
//===========================================================================================================
//  ~cDlgGuestEdit
//-----------------------------------------------------------------------------------------------------------
cDlgGuestEdit::~cDlgGuestEdit()
{
    cTracer obTrace( "cDlgGuestEdit::~cDlgGuestEdit" );

    delete m_dlgProgress;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbSaveExit_clicked()
{
    bool    boCanBeSaved    = true;
    QString qsErrorMessage  = "";

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Guest name cannot be empty." ) );
    }
    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Gender of the guest must be set." ) );
    }
    if( ledEmail->text().length() > 0 )
    {

    }

    if( boCanBeSaved )
    {
        if( _saveGuestData() )
            QDialog::accept();
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbSave_clicked()
{
    bool    boCanBeSaved    = true;
    QString qsErrorMessage  = "";

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Guest name cannot be empty." ) );
    }
    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Gender of the guest must be set." ) );
    }

    if( boCanBeSaved )
    {
        _saveGuestData();
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
    slotEnableButtons();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbAssignCard_clicked()
{
    m_dlgProgress->showProgress();

    cDlgPatientCardSelect   obDlgSelect( this );

    m_dlgProgress->hideProgress();

    if( obDlgSelect.exec() == QDialog::Accepted )
    {
        unsigned int uiPCardId = obDlgSelect.selected();

        m_poPatientCard->load( uiPCardId );

        if( m_poPatientCard->patientId() == 0 )
        {
            m_poPatientCard->setPatientId( m_poGuest->id() );
            m_poPatientCard->save();
        }
        else
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectPatientWithCard WHERE patientCardId = %1 AND patientId = %2 AND licenceId = %3" ).arg(uiPCardId).arg(m_poGuest->id()).arg(g_poPrefs->getLicenceId()) );

            if( poQuery->size() == 0 )
            {
                g_poDB->executeQTQuery( QString( "INSERT INTO connectpatientwithcard (patientCardId, patientId, licenceId) VALUES ('%1', '%2', '%3')" ).arg(uiPCardId).arg(m_poGuest->id()).arg(g_poPrefs->getLicenceId()) );
            }
        }
    }
    _fillPatientCardData();
    slotEnableButtons();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbSellCard_clicked()
{
//    hide();

    cDlgInputStart  obDlgInputStart( this );

    if( obDlgInputStart.exec() == QDialog::Accepted && obDlgInputStart.m_bCard )
    {
        cDBPatientCard  obDBPatientCard;
        QString         qsBarcode = obDlgInputStart.getEditText();

        obDBPatientCard.createNew();

        try
        {
            obDBPatientCard.load( qsBarcode );

            if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("This patientcard has been lost and replaced\n"
                                         "and can not be used or sold again.") );
                return;
            }
            if( obDBPatientCard.active() )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("This patientcard is in use. Active patientcard can not be sold again.") );
                return;
            }
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
            else
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This barcode has not found in the database.\n"
                                              "Do you want to register it for a new patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCard.createNew();
                    obDBPatientCard.setLicenceId( g_poPrefs->getLicenceId() );
                    obDBPatientCard.setBarcode( qsBarcode );
                    obDBPatientCard.save();
                }
            }
        }

        if( obDBPatientCard.id() > 0 )
        {
            cDlgPatientCardSell obDlgPatientCardSell( this, &obDBPatientCard );
            obDlgPatientCardSell.setPatientCardOwner( m_poGuest->id() );
            if( obDlgPatientCardSell.exec() == QDialog::Accepted )
            {
                m_poPatientCard->load( obDBPatientCard.id() );

                if( m_poPatientCard->patientId() == 0 )
                {
                    m_poPatientCard->setPatientId( m_poGuest->id() );
                    m_poPatientCard->save();
                }
                else
                {
                    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectPatientWithCard WHERE patientCardId = %1 AND patientId = %2 AND licenceId = %3" ).arg(obDBPatientCard.id()).arg(m_poGuest->id()).arg(g_poPrefs->getLicenceId()) );

                    if( poQuery->size() == 0 )
                    {
                        g_poDB->executeQTQuery( QString( "INSERT INTO connectpatientwithcard (patientCardId, patientId, licenceId) VALUES ('%1', '%2', '%3')" ).arg(obDBPatientCard.id()).arg(m_poGuest->id()).arg(g_poPrefs->getLicenceId()) );
                    }
                }
                _fillPatientCardData();
            }
        }

        slotEnableButtons();

        if( QMessageBox::question( this, tr( "Question" ),
                                   tr( "Do you want to save changes then close the dialog?" ),
                                   QMessageBox::Yes|QMessageBox::No ) == QMessageBox::Yes )
        {
            on_pbSaveExit_clicked();
        }
    }

//    show();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbDislink_clicked()
{
    m_dlgProgress->showProgress();

    cDlgPatientCardSelect   obDlgSelect( this, m_poGuest->id() );

    unsigned int uiPCardId = 0;

    m_dlgProgress->hideProgress();

    if( obDlgSelect.exec() == QDialog::Accepted )
    {
        uiPCardId = obDlgSelect.selected();

        if( QMessageBox::question( this, tr( "Question" ),
                                   tr( "Are you sure you want to disjoin the selected card from patient?" ),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            m_poPatientCard->load( uiPCardId );
            if( m_poPatientCard->patientId() == m_poGuest->id() )
            {
                m_poPatientCard->setPatientId( 0 );
                m_poPatientCard->save();
            }
            g_poDB->executeQTQuery( QString( "DELETE FROM connectPatientWithCard WHERE patientCardId = %1 AND patientId = %2" ).arg(uiPCardId).arg(m_poGuest->id()) );
            m_poPatientCard->createNew();
        }
    }
    ledBarcode->setText( "" );
    _fillPatientCardData();
    slotEnableButtons();
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::on_pbHistory_clicked()
{
    QMessageBox::information( this, tr("Information"), tr("Not implemented yet.") );
}
//===========================================================================================================
//  slotRefreshWarningColors
//-----------------------------------------------------------------------------------------------------------
//  A dialog-ban talalhato label-ek adat-fuggo atszinezeseert felelos procedura
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::slotRefreshWarningColors()
{
    lblName->setStyleSheet( "QLabel {font: normal;}" );
    lblGender->setStyleSheet( "QLabel {font: normal;}" );
    lblAge->setStyleSheet( "QLabel {font: normal;}" );
    lblEmail->setStyleSheet( "QLabel {font: normal;}" );

    if( ledName->text().length() == 0 )
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() )
        lblGender->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( rbAge0->isChecked() )
        lblAge->setStyleSheet( "QLabel {font: bold; color: blue;}" );

    if( ledEmail->text().length() == 0 )
        lblEmail->setStyleSheet( "QLabel {font: bold; color: blue;}" );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::slotEnableButtons()
{
    pbAssignCard->setEnabled( m_poGuest->id()>0 );
    pbSellCard->setEnabled( m_poGuest->id()>0 );
    pbHistory->setEnabled( /*m_poGuest->id()>0*/false );
    pbDislink->setEnabled( ledBarcode->text().length()>0 );
}
//===========================================================================================================
//  _fillPatientCardData
//-----------------------------------------------------------------------------------------------------------
void cDlgGuestEdit::_fillPatientCardData()
{
    ledBarcode->setText( "" );
    ledBarcode->setToolTip( "" );
    lblAssignCardInfo->setToolTip( "" );

    if( m_poGuest->id() == 0 )  return;

    QSqlQuery  *poQuery;
    QString     qsBarcodes      = "";
    QString     qsPatientCards  = tr("Assigned patientcards:\nBarcode\tPatientcard type");

    poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM patientcards WHERE patientcards.patientId=%1 " ).arg( m_poGuest->id() ) );

    while( poQuery->next() )
    {
        if( !qsBarcodes.contains( poQuery->value(0).toString() ) )
        {
            qsBarcodes.append( QString("%1, ").arg( poQuery->value(0).toString() ) );
            qsPatientCards.append( QString("\n%1\t%2").arg( poQuery->value(0).toString() ).arg( poQuery->value(1).toString() ) );
        }
    }

    poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM "
                                               "patientcards, connectpatientwithcard WHERE "
                                               "patientcards.patientcardid=connectpatientwithcard.patientcardid AND "
                                               "connectpatientwithcard.patientId=%1 " ).arg( m_poGuest->id() ) );
    while( poQuery->next() )
    {
        if( !qsBarcodes.contains( poQuery->value(0).toString() ) )
        {
            qsBarcodes.append( QString("%1, ").arg( poQuery->value(0).toString() ) );
            qsPatientCards.append( QString("\n%1").arg( poQuery->value(0).toString() ) );
//            qsPatientCards.append( QString("\n%1\t%2").arg( poQuery->value(0).toString() ).arg( poQuery->value(1).toString() ) );
        }
    }

    if( qsBarcodes.length() > 0 )
    {
        ledBarcode->setText( qsBarcodes.left( qsBarcodes.length()-2 ) );
        ledBarcode->setToolTip( qsPatientCards );
        lblAssignCardInfo->setToolTip( qsPatientCards );
    }
/*
    try
    {
        m_poPatientCard->loadPatient( m_poGuest->id() );

        ledBarcode->setText( m_poPatientCard->barcode() );

        cDBPatientCardType  obDBPatientCardType;

        obDBPatientCardType.load( m_poPatientCard->patientCardTypeId() );
//        ledPatientcardType->setText( obDBPatientCardType.name() );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patient id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
        }
        else
        {
            try
            {
                QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectPatientWithCard WHERE patientId = %2 AND licenceId = %3" ).arg(m_poGuest->id()).arg(g_poPrefs->getLicenceId()) );

                if( poQuery->size() > 0 )
                {
                    poQuery->last();
                    m_poPatientCard->load( poQuery->value(0).toUInt() );
                }
                ledBarcode->setText( m_poPatientCard->barcode() );

                cDBPatientCardType  obDBPatientCardType;

                obDBPatientCardType.load( m_poPatientCard->patientCardTypeId() );
//                ledPatientcardType->setText( obDBPatientCardType.name() );
            }
            catch( cSevException &e )
            {
                if( QString(e.what()).compare("Patientcard id not found") != 0 )
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
                }
            }
        }
    }
*/
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
bool cDlgGuestEdit::_saveGuestData()
{
    bool bRet = false;

    try
    {
        m_poGuest->setLicenceId( g_poPrefs->getLicenceId() );
        m_poGuest->setName( ledName->text() );
        if( rbGenderMale->isChecked() )
            m_poGuest->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poGuest->setGender( 2 );
        m_poGuest->setDateBirth( deBirthDate->date().toString("yyyy-MM-dd") );
        m_poGuest->setMembership( ledMembership->text() );
        m_poGuest->setEmail( ledEmail->text() );
        m_poGuest->setMobile( ledPhone->text() );
        m_poGuest->setAddress( teAddress->toPlainText() );
        m_poGuest->setComment( teComment->toPlainText() );
        m_poGuest->setIsReturning( chkReturning->isChecked() );
        if( rbAge0->isChecked() ) m_poGuest->setAgeType( 0 );
        else if( rbAge1->isChecked() ) m_poGuest->setAgeType( 1 );
        else if( rbAge2->isChecked() ) m_poGuest->setAgeType( 2 );
        else if( rbAge3->isChecked() ) m_poGuest->setAgeType( 3 );
        else if( rbAge4->isChecked() ) m_poGuest->setAgeType( 4 );
        else if( rbAge5->isChecked() ) m_poGuest->setAgeType( 5 );
        else if( rbAge6->isChecked() ) m_poGuest->setAgeType( 6 );
        else if( rbAge7->isChecked() ) m_poGuest->setAgeType( 7 );
        m_poGuest->setService( chkService->isChecked() );
        m_poGuest->setEmployee( chkEmployee->isChecked() );
        m_poGuest->setRegularCustomer( chkRegularCustomer->isChecked() );
        m_poGuest->setSkinTypeId( cmbSkinType->itemData( cmbSkinType->currentIndex() ).toUInt() );

        m_poGuest->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return bRet;
}

QString cDlgGuestEdit::_convertCurrency(const QString &text) const
{
    QString qsValue = text;
    QString qsRet = "";

    qsValue.remove(QChar(','));

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

    return qsRet;
}

void cDlgGuestEdit::slotUpdateDiscountSample()
{
    m_poGuest->setRegularCustomer( chkRegularCustomer->isChecked() );
    m_poGuest->setEmployee( chkEmployee->isChecked() );
    m_poGuest->setService( chkService->isChecked() );

    lblDiscountedPrice->setText( QString("%1 %2").arg( _convertCurrency( QString::number(m_poGuest->getDiscountedPrice( 10000 )) ) ).arg( g_poPrefs->getCurrencyShort() ) );
}


void cDlgGuestEdit::on_pbEditDiscount_clicked()
{
    cDBDiscount obDBDiscount;

    obDBDiscount.loadGuest( m_poGuest->id() );

    cDlgDiscountEdit    dlgDiscountEdit( this, &obDBDiscount );

    dlgDiscountEdit.exec();

    slotUpdateDiscountSample();
}

unsigned int cDlgGuestEdit::guestId()
{
    return m_poGuest->id();
}

