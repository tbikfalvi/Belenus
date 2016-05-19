//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgpatientcardassign.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//===========================================================================================================

#include <QPushButton>
#include <QMessageBox>

//===========================================================================================================

#include "belenus.h"
#include "../framework/tracer.h"
#include "../db/dbpatientcard.h"
#include "dlgpatientcardassign.h"
#include "dlgcassaaction.h"
#include "../db/dbpatientcardunits.h"
#include "../db/dbledger.h"

//===========================================================================================================
cDlgPatientCardAssign::cDlgPatientCardAssign( QWidget *p_poParent, QString p_qsMainBarcode ) : QDialog( p_poParent )
//-----------------------------------------------------------------------------------------------------------
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/belenus.ico") );

    m_bMainCardOk   = false;
    m_bAssignCardOk = false;
    m_qsBarcodeMain    = "";
    m_qsBarcodeAssign  = "";

    pbCheckCards->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssign->setIcon( QIcon("./resources/40x40_patientcards.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_exit.png") );

    ledMainBarcode->setText( p_qsMainBarcode );

    connect( rbActAssign,       SIGNAL(clicked()), this, SLOT(slotRadioClicked()) );
    connect( rbActAssignOld,    SIGNAL(clicked()), this, SLOT(slotRadioClicked()) );
    connect( rbActSetOldToMain, SIGNAL(clicked()), this, SLOT(slotRadioClicked()) );
    connect( rbActRemoveChild,  SIGNAL(clicked()), this, SLOT(slotRadioClicked()) );

    rbDefault->setVisible( false );
    rbDefault->setChecked( true );
    lblInfo->setText( "" );

    on_ledMainBarcode_textChanged( "" );
    on_ledAssignBarcode_textChanged( "" );

    m_dlgProgress = new cDlgProgress( this );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "CardAssign", QPoint(525,365) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//===========================================================================================================
cDlgPatientCardAssign::~cDlgPatientCardAssign()
//-----------------------------------------------------------------------------------------------------------
{
    g_poPrefs->setDialogSize( "CardAssign", QPoint( width(), height() ) );
}
//===========================================================================================================
void cDlgPatientCardAssign::getCardsBarcode(QString *p_qsBarcodeMain, QString *p_qsBarcodeAssign)
//-----------------------------------------------------------------------------------------------------------
{
    *p_qsBarcodeMain    = m_qsBarcodeMain;
    *p_qsBarcodeAssign  = m_qsBarcodeAssign;
}
//===========================================================================================================
void cDlgPatientCardAssign::on_ledMainBarcode_textChanged(const QString &/*arg1*/)
//-----------------------------------------------------------------------------------------------------------
{
    lblMainBarcode->setStyleSheet( "QLabel {font: normal;}" );
    if( ledMainBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        lblMainBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    m_bMainCardOk = false;
    _disableControls();
}
//===========================================================================================================
void cDlgPatientCardAssign::on_ledMainBarcode_returnPressed()
//-----------------------------------------------------------------------------------------------------------
{
    ledAssignBarcode->setFocus();
}
//===========================================================================================================
void cDlgPatientCardAssign::on_ledAssignBarcode_textChanged(const QString &/*arg1*/)
//-----------------------------------------------------------------------------------------------------------
{
    lblAssignBarcode->setStyleSheet( "QLabel {font: normal;}" );

    if( ledAssignBarcode->text().length() == 0 )
    {
        lblAssignBarcode->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }
    else if( ledAssignBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        lblAssignBarcode->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    m_bAssignCardOk = false;
    _disableControls();
}
//===========================================================================================================
void cDlgPatientCardAssign::on_ledAssignBarcode_returnPressed()
//-----------------------------------------------------------------------------------------------------------
{
    pbCheckCards->setFocus();
    on_pbCheckCards_clicked();
}
//===========================================================================================================
void cDlgPatientCardAssign::on_pbCheckCards_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    m_bMainCardOk               = true;
    m_bAssignCardOk             = true;
    m_bCanBeParentM             = true;
    m_bCanBeParentA             = true;
    m_uiMainCardId              = 0;
    m_uiMainCardParentId        = 0;
    m_uiAssignedCardId          = 0;
    m_uiAssignedCardParentId    = 0;
    m_uiParentGroupM            = 0;
    m_uiParentGroupA            = 0;

    if( ledMainBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( this, tr( "Error" ),
                              tr( "Invalid barcode length.\n"
                                  "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
        ledMainBarcode->setFocus();
        return;
    }
    else if( ledMainBarcode->text().compare( ledAssignBarcode->text() ) == 0 )
    {
        QMessageBox::warning( this, tr( "Error" ),
                              tr( "Barcode of the Main card and Assigned card can not be the same." ) );
        ledAssignBarcode->setFocus();
        return;
    }
    else if( !_checkIfExists( ledMainBarcode->text() ) )
    {
        return;
    }

    _fillOldCardAssignStructure();

    if( ledAssignBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        m_bAssignCardOk = false;
        m_qsReasonA = tr( "Invalid barcode length.\n"
                          "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength());
        pbAssign->setEnabled( _isCardsCanBeAssigned() );
    }
    else if( ledAssignBarcode->text().compare( ledMainBarcode->text() ) == 0 )
    {
        QMessageBox::warning( this, tr( "Error" ),
                              tr( "Barcode of the Main card and Assigned card can not be the same." ) );
        ledAssignBarcode->setFocus();
        return;
    }
    else if( !_checkIfExists( ledAssignBarcode->text() ) )
    {
        return;
    }

    if( m_bAssignCardOk )
    {
        _loadAssignedCard();

        if( m_uiParentGroupM > 0 && m_uiParentGroupM == m_uiParentGroupA )
        {
            QMessageBox::warning( this, tr( "Error" ),
                                  tr( "These cards has already been assigned to each other." ) );
            return;
        }
        else if( m_uiParentGroupM > 0 && m_uiParentGroupA > 0 )
        {
            QMessageBox::warning( this, tr( "Error" ),
                                  tr( "These cards has already been assigned to another cards." ) );
            return;
        }

        if( !m_bCanBeParentM && !m_bCanBeParentA )
        {
            QMessageBox::warning( this, tr( "Error" ),
                                  tr( "Both cards has issues prevent to assign them\n\n."
                                      "'Old' card: \n%1\n"
                                      "'New' card: \n%2").arg( m_qsReasonM ).arg( m_qsReasonA ) );
            return;
        }
        if( !m_bMainCardOk )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("'%1' card can not be assigned.\n\n%2")
                                    .arg(ledMainBarcode->text())
                                    .arg( m_qsReasonM ) );
        }
        if( !m_bAssignCardOk )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("'%1' card can not be assigned.\n\n%2")
                                  .arg(ledAssignBarcode->text())
                                  .arg( m_qsReasonA ) );
        }
    }
    else
    {
        if( !m_bMainCardOk )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("'%1' card can not be processed.\n\n%2")
                                    .arg(ledMainBarcode->text())
                                    .arg( m_qsReasonM ) );
        }

    }

    if( _isCardsCanBeAssigned() )
    {
        if( m_uiMainCardParentId == 0 )
        {
            rbActAssign->setEnabled( true );
        }
        if( m_uiAssignedCardParentId == 0 )
        {
            rbActAssignOld->setEnabled( true );
        }
    }
    pbCheckCards->setIcon( QIcon("./resources/40x40_ok.png") );
}
//===========================================================================================================
void cDlgPatientCardAssign::on_pbAssign_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    if( rbActAssign->isChecked() )
    {
        _assignNewCardToOldCard();
    }
    else if( rbActAssignOld->isChecked() )
    {
        _assignOldAndOtherCardsToNewCard();
    }
    else if( rbActSetOldToMain->isChecked() )
    {
        _setSelectedCardToMainCard();
    }
    else if( rbActRemoveChild->isChecked() )
    {
        _removeAndDeactivateAssignedCards();
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::on_pbCancel_clicked()
//-----------------------------------------------------------------------------------------------------------
{
    QDialog::reject();
}
//===========================================================================================================
bool cDlgPatientCardAssign::_isCardsCanBeAssigned()
//-----------------------------------------------------------------------------------------------------------
{
    return ( m_bMainCardOk && m_bAssignCardOk );
}
//===========================================================================================================
void cDlgPatientCardAssign::_fillOldCardAssignStructure()
//-----------------------------------------------------------------------------------------------------------
{
    tvAssignStructure->takeTopLevelItem( 0 );
    try
    {
        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( ledMainBarcode->text() );
        m_uiMainCardId          = obDBPatientCard.id();
        m_uiMainCardParentId    = obDBPatientCard.parentId();

        if( obDBPatientCard.isAssignedCardExists() )
        {
            g_obLogger(cSeverity::DEBUG) << "Assigned cards exists" << EOM;

            if( obDBPatientCard.parentId() == 0 )
            {
                g_obLogger(cSeverity::DEBUG) << "Parent not exists" << EOM;

                m_uiParentGroupM = obDBPatientCard.id();

                QTreeWidgetItem *itemParent = new QTreeWidgetItem( tvAssignStructure );
                itemParent->setText( 0, obDBPatientCard.barcode() );

                itemParent->setTextColor( 0, QColor("blue") );
                QFont   qfTemp = itemParent->font(0);
                qfTemp.setBold( true );
                itemParent->setFont( 0, qfTemp );

                QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM patientCards WHERE parentCardId = %1" )
                                                                    .arg( obDBPatientCard.id() ) );
                while( poQuery->next() )
                {
                    QTreeWidgetItem *itemChild = new QTreeWidgetItem( itemParent );
                    itemChild->setText( 0, poQuery->value(0).toString() );
                }
                itemParent->setExpanded( true );

                rbActSetOldToMain->setEnabled( false );
                rbActRemoveChild->setEnabled( true );
            }
            else
            {
                g_obLogger(cSeverity::DEBUG) << "Parent exists" << EOM;

                m_uiParentGroupM = obDBPatientCard.parentId();

                cDBPatientCard  obParent;

                obParent.load( obDBPatientCard.parentId() );

                QTreeWidgetItem *itemParent = new QTreeWidgetItem( tvAssignStructure );
                itemParent->setText( 0, obParent.barcode() );

                QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM patientCards WHERE parentCardId = %1" )
                                                                    .arg( obParent.id() ) );
                while( poQuery->next() )
                {
                    QTreeWidgetItem *itemChild = new QTreeWidgetItem( itemParent );
                    itemChild->setText( 0, poQuery->value(0).toString() );
                    if( obDBPatientCard.barcode().compare( poQuery->value(0).toString() ) == 0 )
                    {
                        itemChild->setTextColor( 0, QColor("blue") );
                        QFont   qfTemp = itemChild->font(0);
                        qfTemp.setBold( true );
                        itemChild->setFont( 0, qfTemp );
                    }
                }
                itemParent->setExpanded( true );

                rbActSetOldToMain->setEnabled( true );
                rbActRemoveChild->setEnabled( false );
            }
        }

        m_bCanBeParentM = obDBPatientCard.isPatientCardCanBeParent();

        if( !m_bCanBeParentM )
        {
            m_qsReasonM = "";

            if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                m_qsReasonM = tr( "This patientcard has been lost and replaced\nand can not be used or sold again." );
                m_bMainCardOk = false;
            }
            else if( obDBPatientCard.isServiceCard() )
            {
                m_qsReasonM = tr( "Service cards can not be main card." );
                m_bMainCardOk = false;
            }
            else if( obDBPatientCard.units() < 1 or obDBPatientCard.timeLeft() < 1 )
            {
                m_qsReasonM = tr( "Patientcard is empty." );
            }
            else if( QDate::fromString( obDBPatientCard.validDateTo(), "yyyy-MM-dd" ) < QDate::currentDate() )
            {
                m_qsReasonM = tr( "The validity of this patientcard has been expired on %1" ).arg( obDBPatientCard.validDateTo() );
            }
            else if( obDBPatientCard.active() == 0 )
            {
                m_qsReasonM = tr( "Patientcard is inactive." );
            }
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::slotRadioClicked()
//-----------------------------------------------------------------------------------------------------------
{
    pbAssign->setEnabled( true );

    if( rbActAssign->isChecked() )
    {
        lblInfo->setText( rbActAssign->toolTip() );
    }
    else if( rbActAssignOld->isChecked() )
    {
        lblInfo->setText( rbActAssignOld->toolTip() );
    }
    else if( rbActSetOldToMain->isChecked() )
    {
        lblInfo->setText( rbActSetOldToMain->toolTip() );
    }
    else if( rbActRemoveChild->isChecked() )
    {
        lblInfo->setText( rbActRemoveChild->toolTip() );
    }
    else
    {
        lblInfo->setText( "" );
        pbAssign->setEnabled( false );
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::_disableControls()
//-----------------------------------------------------------------------------------------------------------
{
    tvAssignStructure->takeTopLevelItem( 0 );
    pbCheckCards->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssign->setEnabled( false );
    rbActAssign->setEnabled( false );
    rbActAssignOld->setEnabled( false );
    rbActSetOldToMain->setEnabled( false );
    rbActRemoveChild->setEnabled( false );
    rbDefault->setVisible( false );
    rbDefault->setChecked( true );
    lblInfo->setText( "" );
}
//===========================================================================================================
bool cDlgPatientCardAssign::_checkIfExists(QString p_qsBarcode)
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  obDBPatientCard;
    bool            bRet = true;

    try
    {
        obDBPatientCard.load( p_qsBarcode );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            bRet = false;
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("'%1' barcode has not found in the database.\n"
                                          "Do you want to register it for a new patientcard?").arg( p_qsBarcode ),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                obDBPatientCard.createNew();
                obDBPatientCard.setLicenceId( g_poPrefs->getLicenceId() );
                obDBPatientCard.setBarcode( p_qsBarcode );
                obDBPatientCard.save();
            }
            else
            {
                bRet = false;
            }
        }
    }
    return bRet;
}
//===========================================================================================================
void cDlgPatientCardAssign::_loadAssignedCard()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledAssignBarcode->text() );
        m_uiAssignedCardId          = obDBPatientCard.id();
        m_uiAssignedCardParentId    = obDBPatientCard.parentId();

        if( obDBPatientCard.isAssignedCardExists() )
        {
            if( obDBPatientCard.parentId() == 0 )
            {
                m_uiParentGroupA = obDBPatientCard.id();
            }
            else
            {
                m_uiParentGroupA = obDBPatientCard.parentId();
            }
        }

        m_bCanBeParentA = obDBPatientCard.isPatientCardCanBeParent();

        if( !m_bCanBeParentA )
        {
            m_qsReasonA = "";

            if( obDBPatientCard.isServiceCard() )
            {
                m_qsReasonA = tr( "Service cards can not be main card." );
                m_bAssignCardOk = false;
            }
            else if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                m_qsReasonA = tr( "This patientcard has been lost and replaced\nand can not be used or sold again." );
                m_bAssignCardOk = false;
            }
            else if( obDBPatientCard.units() < 1 or obDBPatientCard.timeLeft() < 1 )
            {
                m_qsReasonA = tr( "Patientcard is empty." );
            }
            else if( QDate::fromString( obDBPatientCard.validDateTo(), "yyyy-MM-dd" ) < QDate::currentDate() )
            {
                m_qsReasonA = tr( "The validity of this patientcard has been expired on %1" ).arg( obDBPatientCard.validDateTo() );
            }
            else if( obDBPatientCard.active() == 0 )
            {
                m_qsReasonA = tr( "Patientcard is inactive." );
            }
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::_assignNewCardToOldCard()
//-----------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to assign the selected cards?\n\n"
                                  "The old (existing) card will be the main (parent) card and\n"
                                  "the new card will be the assigned (child) card."),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
    {
        return;
    }

    _processAssignNewToOld();

    ledMainBarcode->setText( "" );
    ledAssignBarcode->setText( "" );
    _disableControls();
    m_dlgProgress->hideProgress();
    pbCancel->setFocus();
}
//===========================================================================================================
void cDlgPatientCardAssign::_processAssignNewToOld()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  obDBPatientCardMain;
    cDBPatientCard  obDBPatientCardAssign;

    obDBPatientCardMain.load( ledMainBarcode->text() );
    obDBPatientCardAssign.load( ledAssignBarcode->text() );

    unsigned int uiTimeLeft = obDBPatientCardMain.timeLeft() + obDBPatientCardAssign.timeLeft();
    unsigned int uiUnits    = obDBPatientCardMain.units() + obDBPatientCardAssign.units();

    if( obDBPatientCardAssign.active() )
    {
        cDBPatientcardUnit  obDBPatientcardUnit;

        obDBPatientcardUnit.setPatientCardId( obDBPatientCardAssign.id() );
        obDBPatientcardUnit.replacePatientCard( obDBPatientCardMain.id() );

        obDBPatientCardMain.synchronizeUnits();
        obDBPatientCardMain.synchronizeTime();
        obDBPatientCardMain.save();

        obDBPatientCardAssign.setParentId( obDBPatientCardMain.id() );
        obDBPatientCardAssign.synchronizeUnits();
        obDBPatientCardAssign.synchronizeTime();
        obDBPatientCardAssign.setComment( tr("Partner card of \"%1\"").arg(obDBPatientCardMain.barcode()) );
        obDBPatientCardAssign.save();

    }
    else
    {
        obDBPatientCardAssign.setActive( true );
        obDBPatientCardAssign.setPatientCardTypeId( obDBPatientCardMain.patientCardTypeId() );
        obDBPatientCardAssign.setParentId( obDBPatientCardMain.id() );
        obDBPatientCardAssign.setPatientId( 0 );
        obDBPatientCardAssign.setValidDateFrom( obDBPatientCardMain.validDateFrom() );
        obDBPatientCardAssign.setValidDateTo( obDBPatientCardMain.validDateTo() );
        obDBPatientCardAssign.setComment( tr("Partner card of \"%1\"").arg(obDBPatientCardMain.barcode()) );

        cCurrency cPrice( QString::number(g_poPrefs->getPatientCardPartnerPrice()/100) );

        cDBShoppingCart obDBShoppingCart;

        obDBShoppingCart.setLicenceId( g_poPrefs->getLicenceId() );
        obDBShoppingCart.setGuestId( obDBPatientCardAssign.patientId() );
        obDBShoppingCart.setProductId( 0 );
        obDBShoppingCart.setPatientCardId( obDBPatientCardAssign.id() );
        obDBShoppingCart.setPatientCardTypeId( obDBPatientCardAssign.patientCardTypeId() );
        obDBShoppingCart.setPanelId( 0 );
        obDBShoppingCart.setLedgerTypeId( cDBLedger::LT_PC_ASSIGN_PARTNER );
        obDBShoppingCart.setItemName( QString("%1 -> %2").arg(obDBPatientCardMain.barcode()).arg(obDBPatientCardAssign.barcode()) );
        obDBShoppingCart.setItemCount( 1 );
        obDBShoppingCart.setItemNetPrice( cPrice.currencyValue().toInt() );
        obDBShoppingCart.setItemVAT( g_poPrefs->getPatientCardLostPriceVat() );
        obDBShoppingCart.setItemDiscount( 0 );
        obDBShoppingCart.setItemSumPrice( cPrice.currencyValue().toInt() );

        cDlgCassaAction     obDlgCassaAction( this, &obDBShoppingCart );

        obDlgCassaAction.setPayWithCash();

        int             inCassaAction   = obDlgCassaAction.exec();
        int             inPayType       = 0;
        QString         qsComment       = tr("Assign patientcard [%1]<-[%2]").arg(obDBPatientCardMain.barcode()).arg(obDBPatientCardAssign.barcode());
        bool            bShoppingCart   = false;
        unsigned int    uiCouponId = 0;

        obDlgCassaAction.cassaResult( &inPayType, &bShoppingCart, &uiCouponId );

        if( inCassaAction == QDialog::Accepted && !bShoppingCart )
        {
            g_obCassa.cassaProcessPatientCardSell( obDBPatientCardAssign, obDBShoppingCart, qsComment, true, inPayType );
        }
        else if( inCassaAction != QDialog::Accepted )
        {
            // Nem tortent meg az eladas
            return;
        }

        obDBPatientCardAssign.synchronizeUnits();
        obDBPatientCardAssign.synchronizeTime();
        obDBPatientCardAssign.save();
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::_assignOldAndOtherCardsToNewCard()
//-----------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to assign the selected cards?\n\n"
                                  "The new card will be the main (parent) card and\n"
                                  "the old (existing) card will be the assigned (child) card.\n"
                                  "All of the currently assigned card will be linked to the new card."),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
    {
        return;
    }

    _processAssignOldToNew();

    ledMainBarcode->setText( "" );
    ledAssignBarcode->setText( "" );
    _disableControls();
    m_dlgProgress->hideProgress();
    pbCancel->setFocus();
}
//===========================================================================================================
void cDlgPatientCardAssign::_processAssignOldToNew()
//-----------------------------------------------------------------------------------------------------------
{
    _processAssignNewToOld();
    ledMainBarcode->setText( ledAssignBarcode->text() );
    ledAssignBarcode->setText( "" );
    _processSelectedToMain();
}
//===========================================================================================================
void cDlgPatientCardAssign::_setSelectedCardToMainCard()
//-----------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to set the selected card to main cards?\n\n"
                                  "All of the assigned card will be linked to this selected card."),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
    {
        return;
    }

    _processSelectedToMain();

    ledMainBarcode->setText( "" );
    ledAssignBarcode->setText( "" );
    _disableControls();
    m_dlgProgress->hideProgress();
    pbCancel->setFocus();
}
//===========================================================================================================
void cDlgPatientCardAssign::_processSelectedToMain()
//-----------------------------------------------------------------------------------------------------------
{
    m_dlgProgress->showProgress();

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledMainBarcode->text() );

        if( obDBPatientCard.id() > 1 && obDBPatientCard.parentId() > 1 )
        {
            g_poDB->executeQTQuery( QString( "UPDATE patientcardunits "
                                             "SET patientCardId=%1 WHERE "
                                             "patientCardId=%2 AND "
                                             "active=1" ).arg( obDBPatientCard.id() )
                                                         .arg( obDBPatientCard.parentId() ) );
            g_poDB->executeQTQuery( QString( "UPDATE patientcards "
                                             "SET parentCardId=%1 WHERE "
                                             "patientCardId=%2 OR "
                                             "parentCardId=%2").arg( obDBPatientCard.id() )
                                                               .arg( obDBPatientCard.parentId() ) );
            obDBPatientCard.setParentId( 0 );
            obDBPatientCard.save();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//===========================================================================================================
void cDlgPatientCardAssign::_removeAndDeactivateAssignedCards()
//-----------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to unlink the assigned cards?\n\n"
                                  "All of the assigned card will be deactivated."),
                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
    {
        return;
    }

    _processRemoveAndDeactivate();

    ledMainBarcode->setText( "" );
    ledAssignBarcode->setText( "" );
    _disableControls();
    m_dlgProgress->hideProgress();
    pbCancel->setFocus();
}
//===========================================================================================================
void cDlgPatientCardAssign::_processRemoveAndDeactivate()
//-----------------------------------------------------------------------------------------------------------
{
    m_dlgProgress->showProgress();

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledMainBarcode->text() );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardId FROM patientCards WHERE parentCardId = %1" )
                                                            .arg( obDBPatientCard.id() ) );
        m_dlgProgress->showProgressBar( poQuery->size() );
        while( poQuery->next() )
        {
            cDBPatientCard  obTemp;

            obTemp.load( poQuery->value(0).toUInt() );
            obTemp.deactivate();
            m_dlgProgress->stepProgressBar();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

