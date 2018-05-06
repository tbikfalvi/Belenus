#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientcardadd.h"

cDlgPatientCardAdd::cDlgPatientCardAdd( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Adding PatientCard to database") );
    setWindowIcon( QIcon( "./resources/40x40_patientcardadd.png" ) );

    pbSave->setIcon( QIcon("./resources/40x40_save.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    m_poPatientCard = new cDBPatientCard;

    m_poPatientCard->createNew();

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
    if( m_poPatientCard != NULL )
        delete m_poPatientCard;

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

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE barcode=\"%1\"" ).arg(ledBarcode->text()) );
    if( poQuery->numRowsAffected() > 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Invalid barcode. This barcode already saved into database." ) );
        lblInformation->setText( tr("Barcode already saved into database.") );
        ledBarcode->setFocus();
    }
    else
    {
        if( ledBarcode->text().length() != g_poPrefs->getBarcodeLength() )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this, tr( "Error" ),
                                  tr( "Invalid barcode length.\n"
                                      "The length of the new barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
            lblInformation->setText( tr("Invalid barcode length.") );
            ledBarcode->setFocus();
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCard->createNew();
            m_poPatientCard->setBarcode( ledBarcode->text() );
            m_poPatientCard->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPatientCard->save();

            lblInformation->setText( tr("Patientcard successfully saved to database.") );
            m_pTimer->start(500);
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgPatientCardAdd::on_pbExit_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardAdd::updateInformation()
{
    ledBarcode->setFocus();
    ledBarcode->selectAll();
    m_pTimer->stop();
}
