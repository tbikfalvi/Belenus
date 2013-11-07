//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgpatientcardassign.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Hosszabb process-ek alatt megjeleno dialog, homokora helyett
//====================================================================================

#include <QPushButton>
#include <QMessageBox>

//====================================================================================

#include "belenus.h"
#include "../framework/tracer.h"
#include "../db/dbpatientcard.h"
#include "dlgpatientcardassign.h"

//====================================================================================
cDlgPatientCardAssign::cDlgPatientCardAssign( QWidget *p_poParent, QString p_qsMainBarcode ) : QDialog( p_poParent )
//====================================================================================
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/belenus.ico") );

    m_bMainCardOk   = false;
    m_bAssignCardOk = false;

    pbMainVerify->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssignVerify->setIcon( QIcon("./resources/40x40_question.png") );

    ledMainBarcode->setText( p_qsMainBarcode );
}
//====================================================================================
cDlgPatientCardAssign::~cDlgPatientCardAssign()
//====================================================================================
{
}
//====================================================================================
void cDlgPatientCardAssign::getCardsBarcode(QString *p_qsBarcodeMain, QString *p_qsBarcodeAssign)
//====================================================================================
{
    *p_qsBarcodeMain    = ledMainBarcode->text();
    *p_qsBarcodeAssign  = ledAssignBarcode->text();
}
//====================================================================================
void cDlgPatientCardAssign::on_ledMainBarcode_textChanged(const QString &arg1)
//====================================================================================
{
    m_bMainCardOk   = false;
    pbMainVerify->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssign->setEnabled( isCardsCanBeAssigned() );
}
//====================================================================================
void cDlgPatientCardAssign::on_pbMainVerify_clicked()
//====================================================================================
{
    if( ledMainBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( this, tr( "Error" ),
                              tr( "Invalid barcode length.\n"
                                  "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
        return;
    }

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
//====================================================================================
void cDlgPatientCardAssign::on_ledAssignBarcode_textChanged(const QString &arg1)
//====================================================================================
{
    m_bAssignCardOk = false;
    pbAssignVerify->setIcon( QIcon("./resources/40x40_question.png") );
    pbAssign->setEnabled( isCardsCanBeAssigned() );
}
//====================================================================================
void cDlgPatientCardAssign::on_pbAssignVerify_clicked()
//====================================================================================
{
    if( ledAssignBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( this, tr( "Error" ),
                              tr( "Invalid barcode length.\n"
                                  "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
        return;
    }

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( ledAssignBarcode->text() );

        QString qsReason = "";

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
        else if( obDBPatientCard.patientCardTypeId() < 2 )
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
//====================================================================================
void cDlgPatientCardAssign::on_pbAssign_clicked()
//====================================================================================
{
    QDialog::accept();
}
//====================================================================================
void cDlgPatientCardAssign::on_pbCancel_clicked()
//====================================================================================
{
    QDialog::reject();
}
//====================================================================================
bool cDlgPatientCardAssign::isCardsCanBeAssigned()
//====================================================================================
{
    return ( m_bMainCardOk && m_bAssignCardOk );
}
//====================================================================================
