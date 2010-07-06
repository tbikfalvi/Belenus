#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcarduse.h"
#include "../db/dbpatientcardtype.h"

cDlgPatientCardUse::cDlgPatientCardUse( QWidget *p_poParent, cDBPatientCard *p_poPatientCard )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;

    setWindowTitle( QString::fromStdString(m_poPatientCard->barcode()) );
    pbSave->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( QString::fromStdString( m_poPatientCard->barcode() ) );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name, unitTime FROM patientCardTypes WHERE archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientCardTypeId() == poQuery->value( 0 ) )
            {
                cmbCardType->setCurrentIndex( cmbCardType->count()-1 );
                m_inUnitLength = poQuery->value( 2 ).toInt();
            }
        }
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE archive<>\"DEL\"" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPatientCard->patientId() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
        cbActive->setChecked( m_poPatientCard->active() );
        ledUnits->setText( QString::number(m_poPatientCard->units()) );
        teTimeLeft->setTime( QTime::fromString(QString::fromStdString(m_poPatientCard->timeLeft()),"hh:mm:ss") );
        deValidDate->setDate( QDate::fromString(QString::fromStdString(m_poPatientCard->validDate()),"yyyy-MM-dd") );
        pteComment->setPlainText( QString::fromStdString( m_poPatientCard->comment() ) );
    }
    gbIdentification->setEnabled( false );
    gbInformation->setEnabled( false );

    for( int i=0; i<m_poPatientCard->units(); i++ )
    {
        cmbNoUnits->addItem( QString::number(i+1), m_inUnitLength*(i+1) );
    }
}

cDlgPatientCardUse::~cDlgPatientCardUse()
{
    if( m_poPatientCardType ) delete m_poPatientCardType;
}

void cDlgPatientCardUse::getUseUnitsTime( int *p_inUnitsUse, QString *p_qsTimeUse )
{
    *p_inUnitsUse   = m_inUnitsUse;
    *p_qsTimeUse    = m_qsTimeUse;
}

void cDlgPatientCardUse::on_pbSave_clicked()
{
    m_qsTimeUse     = teTimeUse->time().toString("mm:ss");

    QDialog::accept();
}

void cDlgPatientCardUse::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardUse::on_cmbNoUnits_currentIndexChanged(int index)
{
    teTimeUse->setTime( QTime( 0, cmbNoUnits->itemData(index).toInt(), 0, 0 ) );
    m_inUnitsUse = cmbNoUnits->itemText(index).toInt();
}
