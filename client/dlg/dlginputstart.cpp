#include <QPushButton>
#include <QMessageBox>

#include "dlginputstart.h"
#include "../crud/dlgpatientselect.h"

cDlgInputStart::cDlgInputStart( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Entering ...") );
    lblAction->setText( tr("Entering code ...") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbPatient->setIcon( QIcon("./resources/40x40_patient.png") );
    pbCardcode->setIcon( QIcon("./resources/40x40_patientcard.png") );
    pbTime->setIcon( QIcon("./resources/40x40_clock.png") );

    m_bTime         = false;
    m_bCard         = false;
    m_bPat          = false;
    m_bInitCalled   = false;

    pbTime->setEnabled( false );
    pbCardcode->setEnabled( false );
    pbPatient->setEnabled( false );
}

cDlgInputStart::~cDlgInputStart()
{
}

void cDlgInputStart::init()
{
    m_bInitCalled = true;

    if( m_bTime )
    {
        pbTime->setEnabled( true );
        lblAction->setText( tr("Entering time period ...") );
    }
    else if( m_bCard )
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

    bool boIsANumber = false;
    ledInputStart->text().toUInt( &boIsANumber );

    m_bTime = true;
    m_bCard = true;
    m_bPat  = true;

    pbTime->setEnabled( true );
    pbCardcode->setEnabled( true );
    pbPatient->setEnabled( true );

    if( ledInputStart->text().length() == 0 ||
        ledInputStart->text().length() > 3 ||
        !boIsANumber )
    {
        m_bTime = false;
        pbTime->setEnabled( false );
    }
    if( ledInputStart->text().length() < g_poPrefs->getBarcodeLength() ||
        ledInputStart->text().contains(' ') )
    {
        m_bCard = false;
        pbCardcode->setEnabled( false );
    }
    if( ledInputStart->text().length() == 0 ||
        boIsANumber )
    {
        m_bPat = false;
        pbPatient->setEnabled( false );
    }

    if( m_bTime )
    {
        lblAction->setText( tr("Entering time period ...") );
    }
    else if( m_bCard )
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
    m_bPat = true;
    m_bCard = false;
    m_bTime = false;
    QDialog::accept();
}

void cDlgInputStart::on_pbCardcode_clicked()
{
    if( ledInputStart->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Barcode of patientcard should be %1 character length.").arg(g_poPrefs->getBarcodeLength()) );
        ledInputStart->setFocus();
        return;
    }
    m_bPat = false;
    m_bCard = true;
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
    m_bTime = true;
    QDialog::accept();
}

void cDlgInputStart::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgInputStart::on_ledInputStart_returnPressed()
{
    if( m_bPat && !m_bCard && !m_bTime ) on_pbPatient_clicked();
    else if( !m_bPat && m_bCard && !m_bTime ) on_pbCardcode_clicked();
    else if( !m_bPat && !m_bCard && m_bTime ) on_pbTime_clicked();
    else
    {
        QMessageBox::information( this, tr("Attention"),tr("Please click on the desired button for the defined search value!"));
    }
}
