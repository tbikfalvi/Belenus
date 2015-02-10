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

    rbDefault->setVisible( false );
    rbDefault->setChecked( true );
    lblInfo->setText( "" );

    on_ledMainBarcode_textChanged( "" );
    on_ledAssignBarcode_textChanged( "" );
}
//===========================================================================================================
cDlgPatientCardAssign::~cDlgPatientCardAssign()
//-----------------------------------------------------------------------------------------------------------
{
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
    m_bMainCardOk       = true;
    m_bAssignCardOk     = true;
    m_uiParentGroupM    = 0;
    m_uiParentGroupA    = 0;
    m_bCanBeParentM     = true;
    m_bCanBeParentA     = true;

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
                                  tr("'%1' card can not be assigned.\n%2")
                                    .arg(ledMainBarcode->text())
                                    .arg( m_qsReasonM ) );
        }
        if( !m_bAssignCardOk )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("'%1' card can not be assigned.\n%2")
                                  .arg(ledAssignBarcode->text())
                                  .arg( m_qsReasonA ) );
        }
    }
    else
    {
        if( !m_bMainCardOk )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("'%1' card can not be processed.\n%2")
                                    .arg(ledMainBarcode->text())
                                    .arg( m_qsReasonM ) );
        }

    }
}
//===========================================================================================================
void cDlgPatientCardAssign::on_pbAssign_clicked()
//-----------------------------------------------------------------------------------------------------------
{

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

        if( obDBPatientCard.isAssignedCardExists() )
        {
            if( obDBPatientCard.parentId() == 0 )
            {
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
            }
            else
            {
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
            else if( obDBPatientCard.patientCardTypeId() == 1 )
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

            if( obDBPatientCard.patientCardTypeId() == 1 )
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





/*

//===========================================================================================================
void cDlgPatientCardAssign::
//-----------------------------------------------------------------------------------------------------------
{


//===========================================================================================================
void cDlgPatientCardAssign::on_pbMainVerify_clicked()
//-----------------------------------------------------------------------------------------------------------
{

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledMainBarcode->text() );
        if( obDBPatientCard.isPatientCardCanBeParent() )
        {
            m_bMainCardOk = true;
        }
        else
        {
            QString qsReason = "";

            if( obDBPatientCard.active() == 0 )
            {
                qsReason = tr( "Patientcard is inactive." );
            }
            else if( obDBPatientCard.units() < 1 or obDBPatientCard.timeLeft() < 1 )
            {
                qsReason = tr( "Patientcard is empty." );
            }
            else if( obDBPatientCard.patientCardTypeId() < 2 )
            {
                qsReason = tr( "Service cards can not be main card." );
            }
            else if( obDBPatientCard.parentId() > 0 )
            {
                qsReason = tr( "This card has been assigned to another card." );
            }
            else if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                qsReason = tr( "This patientcard has been lost and replaced\nand can not be used or sold again." );
            }
            else if( QDate::fromString( obDBPatientCard.validDateTo(), "yyyy-MM-dd" ) < QDate::currentDate() )
            {
                qsReason = tr( "The validity of this patientcard has been expired on %1" ).arg( obDBPatientCard.validDateTo() );
            }

            QMessageBox::warning( this, tr("Warning"),
                                  tr("This card can not be main card.\n%1").arg( qsReason ) );
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            QMessageBox::warning( this, tr("Warning"), tr("This barcode has not found in the database.\n"
                                                          "Please select an existing barcode for main card.") );
        }
    }

    pbMainVerify->setIcon( (m_bMainCardOk ? QIcon("./resources/40x40_ok.png") : QIcon("./resources/40x40_cancel.png")) );

    pbAssign->setEnabled( isCardsCanBeAssigned() );
}
//===========================================================================================================
void cDlgPatientCardAssign::on_pbAssignVerify_clicked()
//-----------------------------------------------------------------------------------------------------------
{

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledAssignBarcode->text() );

        QString qsReason = "";

        if( obDBPatientCard.active() )
        {
            if( obDBPatientCard.parentId() > 0 )
            {
                cDBPatientCard  obParent;

                obParent.load( obDBPatientCard.parentId() );
                qsReason = tr( "This patientcard has been already assigned to another card.\n"
                              "The main card barcode is: %1\n" ).arg( obParent.barcode() );
            }
            else if( obDBPatientCard.isAssignedCardExists() )
            {
                qsReason = tr( "This patientcard is already a main card." );
            }
            else if( obDBPatientCard.patientCardTypeId() == 1 )
            {
                qsReason = tr( "Service cards can not be assigned card." );
            }
            else if( obDBPatientCard.pincode().compare("LOST") == 0 )
            {
                qsReason = tr( "This patientcard has been lost and replaced\nand can not be used or sold again." );
            }
            else if( QDate::fromString( obDBPatientCard.validDateTo(), "yyyy-MM-dd" ) < QDate::currentDate() )
            {
                qsReason = tr( "The validity of this patientcard has been expired on %1" ).arg( obDBPatientCard.validDateTo() );
            }
        }

        if( qsReason.length() == 0 )
        {
            m_bAssignCardOk = true;
        }
        else
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("This card can not be assigned to the main card.\n%1").arg( qsReason ) );
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
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
                obDBPatientCard.setBarcode( ledAssignBarcode->text() );
                obDBPatientCard.save();
                m_bAssignCardOk = true;
            }
        }
    }

    pbAssignVerify->setIcon( (m_bAssignCardOk ? QIcon("./resources/40x40_ok.png") : QIcon("./resources/40x40_cancel.png")) );

    pbAssign->setEnabled( isCardsCanBeAssigned() );
}
*/



