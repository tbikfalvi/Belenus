#include <QPushButton>
#include <QMessageBox>

#include "dlginputstart.h"
#include "../crud/dlgpatientselect.h"
#include "../db/dbpatientcard.h"
#include "communication_rfid.h"

extern cCommRFID       *g_poCommRFID;


cDlgInputStart::cDlgInputStart( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Entering ...") );
    setWindowIcon( QIcon("./resources/belenus.ico") );

    lblAction->setText( tr("Entering code ...") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbPatient->setIcon( QIcon("./resources/40x40_patient.png") );
    pbCardcode->setIcon( QIcon("./resources/40x40_patientcard.png") );
    pbProduct->setIcon( QIcon("./resources/40x40_product.png") );
    pbTime->setIcon( QIcon("./resources/40x40_clock.png") );

    m_bTime         = false;
    m_bCard         = false;
    m_bPat          = false;
    m_bProd         = false;
    m_bInitCalled   = false;

    pbTime->setEnabled( false );
    pbCardcode->setEnabled( false );
    pbProduct->setEnabled( false );
    pbPatient->setEnabled( false );

    pbPatient->setVisible( false );

    if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledInputStart->setEchoMode( QLineEdit::Password );
    }
    else
    {
        ledInputStart->setEchoMode( QLineEdit::Normal );
    }
}

cDlgInputStart::~cDlgInputStart()
{
}

void cDlgInputStart::init( const QString &p_qsText )
{
    m_bInitCalled = true;

    if( m_bTime )
    {
        pbTime->setEnabled( true );
        lblAction->setText( tr("Entering time period ...") );
    }
    else if( m_bCard || m_bProd )
    {
        pbCardcode->setEnabled( true );
        lblAction->setText( tr("Entering barcode ...") );
        if( g_poPrefs->getBarcodePrefix().length() > 0 )
        {
            ledInputStart->setText( g_poPrefs->getBarcodePrefix() );
        }
    }
    else if( m_bPat  )
    {
        pbPatient->setEnabled( true );
        lblAction->setText( tr("Entering patient name ...") );
    }

    if( p_qsText.length() > 0 )
        setInitialText( p_qsText );

    if( m_bCard )
    {
        m_nTimer = startTimer( 500 );
    }

    m_bInitCalled = false;
}

//----------------------------------------------------------------------------------------------
void cDlgInputStart::timerEvent(QTimerEvent *)
{
    if( g_poCommRFID != NULL && g_poCommRFID->isRFIDConnected() )
    {
        QString qsRFID = g_poCommRFID->readRFID();

        if( qsRFID.length() > 0 )
        {
            try
            {
                cDBPatientCard  obDBPatientCard;

                // remove \n\r from the end
                qsRFID = qsRFID.left( qsRFID.length()-2 );

                obDBPatientCard.loadRFID( qsRFID );
                ledInputStart->setText( obDBPatientCard.barcode() );
                killTimer( m_nTimer );
                m_bPat = false;
                m_bCard = true;
                m_bProd = false;
                m_bTime = false;
                QDialog::accept();
            }
            catch( cSevException &e )
            {
                g_obLogger(cSeverity::INFO) << "RFID [" << qsRFID << "] not found in database" << EOM;
                g_obGen.showTrayWarning( tr( "Reading card data failed or this card is not registered in database." ) );
            }

            g_obLogger(cSeverity::INFO) << "RFID read [" << qsRFID << "] " << EOM;
        }
    }
}

void cDlgInputStart::setInitialText( const QString &p_stText )
{
    ledInputStart->setText( p_stText );
}

QString cDlgInputStart::getEditText()
{
    return ledInputStart->text();
}

void cDlgInputStart::on_ledInputStart_textChanged(QString )
{
    if( m_bInitCalled ) return;

    ledInputStart->setText( ledInputStart->text().remove( ' ' ) );

    bool boIsANumber = false;
    ledInputStart->text().toUInt( &boIsANumber );

    m_bTime = true;
    m_bCard = true;
//    m_bPat  = true;
    m_bPat  = false;
    m_bProd = true;

    pbTime->setEnabled( true );
    pbCardcode->setEnabled( true );
    pbProduct->setEnabled( true );
//    pbPatient->setEnabled( true );

    if( ledInputStart->text().length() == 0 ||
        ledInputStart->text().length() > 3 ||
        !boIsANumber )
    {
        m_bTime = false;
        pbTime->setEnabled( false );
    }
    if( ledInputStart->text().length() != g_poPrefs->getBarcodeLength() ||
        ledInputStart->text().contains(' ') )
    {
        m_bCard = false;
        pbCardcode->setEnabled( false );
    }
    if( ledInputStart->text().contains(' ') ||
        ledInputStart->text().length() < 4 ||
        ( g_poPrefs->isBarcodeLengthDifferent() && ledInputStart->text().length() == g_poPrefs->getBarcodeLength() ) )
    {
        m_bProd = false;
        pbProduct->setEnabled( false );
    }
    if( ledInputStart->text().length() == 0 ||
        boIsANumber )
    {
        m_bPat = false;
        pbPatient->setEnabled( false );
    }

    if( _IsServiceCard() )
    {
        m_bCard = true;
        m_bTime = false;
        m_bPat  = false;
        m_bProd = false;
        pbCardcode->setEnabled( true );
        pbTime->setEnabled( false );
        pbPatient->setEnabled( false );
        pbProduct->setEnabled( false );
    }

    if( m_bTime )
    {
        lblAction->setText( tr("Entering time period ...") );
    }
    else if( m_bCard || m_bProd )
    {
        lblAction->setText( tr("Entering barcode ...") );
    }
    else if( m_bPat )
    {
        lblAction->setText( tr("Entering patient name ...") );
    }
}

void cDlgInputStart::on_pbPatient_clicked()
{
//    m_bPat = true;
//    m_bCard = false;
//    m_bProd = false;
//    m_bTime = false;
//    QDialog::accept();
}

void cDlgInputStart::on_pbCardcode_clicked()
{
    if( !_IsServiceCard() )
    {
        if( ledInputStart->text().length() != g_poPrefs->getBarcodeLength() )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("Barcode of patientcard should be %1 character length.").arg(g_poPrefs->getBarcodeLength()) );
            ledInputStart->setFocus();
            return;
        }
    }
    else
    {
        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( 1 );
        ledInputStart->setText( obDBPatientCard.barcode() );
    }
    m_bPat = false;
    m_bCard = true;
    m_bProd = false;
    m_bTime = false;
    QDialog::accept();
}

void cDlgInputStart::on_pbProduct_clicked()
{
    m_bPat = false;
    m_bCard = false;
    m_bProd = true;
    m_bTime = false;
    QDialog::accept();
}

void cDlgInputStart::on_pbTime_clicked()
{
    bool boIsANumber = false;

    ledInputStart->text().toUInt( &boIsANumber );

    if( !boIsANumber )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Invalid value entered.\n"
                                 "Please use only numbers.") );
        ledInputStart->setFocus();
        return;
    }
    else if( ledInputStart->text().toUInt() > g_poPrefs->getMaxTreatLength() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Invalid value entered.\n"
                                 "Time value can not be greater than %1 minutes.").arg(g_poPrefs->getMaxTreatLength()) );
        ledInputStart->setFocus();
        return;
    }

    m_bPat = false;
    m_bCard = false;
    m_bProd = false;
    m_bTime = true;
    QDialog::accept();
}

void cDlgInputStart::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgInputStart::on_ledInputStart_returnPressed()
{
    if( m_bPat && !m_bCard && !m_bProd && !m_bTime ) on_pbPatient_clicked();
    else if( !m_bPat && m_bCard && !m_bProd && !m_bTime ) on_pbCardcode_clicked();
    else if( !m_bPat && !m_bCard && m_bProd && !m_bTime ) on_pbProduct_clicked();
    else if( !m_bPat && !m_bCard && !m_bProd && m_bTime ) on_pbTime_clicked();
    else
    {
        QMessageBox::information( this, tr("Attention"),tr("Please click on the desired button for the defined search value!"));
    }
}

bool cDlgInputStart::_IsServiceCard()
{
    QString qsVerif;
    bool    bRet = false;

    for( int i=0; i<ledInputStart->text().length(); i++ )
    {
        qsVerif.append( '0' );
    }

    if( qsVerif.compare(ledInputStart->text()) == 0 )
    {
        bRet = true;
    }

    return bRet;
}
