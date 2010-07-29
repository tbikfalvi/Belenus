#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientcardadd.h"

cDlgPatientCardAdd::cDlgPatientCardAdd( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Adding PatientCard to database") );
    setWindowIcon( QIcon( "./resources/40x40_patientcardadd.png" ) );

    pbSave->setIcon( QIcon("./resources/40x40_save.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    if( g_poPrefs->getBarcodePrefix().length() > 0 )
    {
        ledBarcode->setText( g_poPrefs->getBarcodePrefix() );
    }

    lblInformation->setText( "" );

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateInformation()));
}

cDlgPatientCardAdd::~cDlgPatientCardAdd()
{
    delete m_pTimer;
}

void cDlgPatientCardAdd::on_ledBarcode_textChanged(QString )
{
    lblInformation->setText( tr("") );
}

void cDlgPatientCardAdd::on_pbSave_clicked()
{
    bool         boCanBeSaved = true;
    QSqlQuery   *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\" AND patientCardId<>%2" ).arg(ledBarcode->text()).arg(m_poPatientCard->id()) );
    if( poQuery->numRowsAffected() > 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. This barcode already saved into database." ) );
        lblInformation->setText( tr("Barcode already saved into database.") );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCard = new cDBPatientCard;

            m_poPatientCard->createNew();
            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPatientCard->save();

            lblInformation->setText( tr("Patientcard successfully saved to database.") );
            m_pTimer->start(500);

            delete m_poPatientCard;
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        }
    }
}

void cDlgPatientCardAdd::on_pbExit_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardAdd::updateInformation()
{
    ledBarcode->selectAll();
    m_pTimer->stop();
}
