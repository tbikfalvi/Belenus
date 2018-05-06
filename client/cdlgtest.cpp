
#include <QMenu>
#include <QCryptographicHash>
#include <QMessageBox>

#include "belenus.h"
#include "cdlgtest.h"
#include "ui_cdlgtest.h"
#include "licenceManager.h"

extern cLicenceManager g_obLicenceManager;

cDlgTest::cDlgTest(QWidget *parent) : QDialog(parent), ui(new Ui::cDlgTest)
{
    ui->setupUi(this);

    ui->ledVat->setText( "0" );

    ui->pbImageClick->setIcon( QIcon( "./resources/20x20_ok.png" ) );

    connect( ui->ledCurrencyValue, SIGNAL(textEdited(QString)), this, SLOT(on_pbCalculate_clicked()) );
}

cDlgTest::~cDlgTest()
{
    delete ui;
}

void cDlgTest::on_pbCalculate_clicked()
{
    cCurrency::currType ctTest = cCurrency::CURR_GROSS;

    if( ui->chkWithVAT->isChecked() ) ctTest = cCurrency::CURR_NET;

    cCurrency currTest( ui->ledCurrencyValue->text(), ctTest, ui->ledVat->text().toInt() );

    ui->lblCurrencyFull->setText( currTest.currencyFullStringShort() );
}

void cDlgTest::on_pbImageClick_clicked()
{
    QMenu   qmMenu;

    qmMenu.addAction( tr("Test connection") );
    qmMenu.addAction( tr("Enable") );
    qmMenu.addAction( tr("Disable") );

    QPoint globalPos = ui->pbImageClick->mapToGlobal(this->pos());

    qmMenu.exec( QPoint(globalPos) );
}

void cDlgTest::on_pbCreateLicenceCodes_clicked()
{
    QString qsText = ui->teLicenceArea->toPlainText();

    qsText.append( "\n" );
    qsText.append( g_obLicenceManager.createLicenceKey( ui->ledLicenceNumber->text() ) );
    ui->teLicenceArea->setPlainText( qsText );

    qsrand( ui->ledLicenceNumber->text().toUInt() );

    QString qsNumber = "";
    int i = 0;

    do
    {
        qsNumber = QString::number( (qrand()*qrand())%1000000 );
        i++;

    } while( qsNumber.length() != 6 || i < 10 );

    ui->ledLicenceNumber->setText( qsNumber );
}

void cDlgTest::on_pbGenerateMd5Hash_clicked()
{
    ui->ledTimeStamp->setText( QString::number( QDateTime::currentDateTime().toTime_t() ) );

    QString qsSource = "";

    qsSource.append( ui->ledToken->text() );
    qsSource.append( ui->ledTimeStamp->text() );
    qsSource.append( ui->ledBarcode->text() );

    QString blah = QString(QCryptographicHash::hash(qsSource.toStdString().c_str(),QCryptographicHash::Md5).toHex());

    ui->ledMd5Hash->setText( blah );
}

void cDlgTest::on_pbBerletHaszn_clicked()
{
    QString m_qsFile = ui->ledBerletLehasznalas->text();
    QFile   qfFile( m_qsFile );

    if( !qfFile.exists() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The following file is missing:\n"
                                 "%1\n\n"
                                 "Please create the file and fulfill it with proper data.\n"
                                 "For more information about import file, check the manual\n"
                                 "or contact the application provider.").arg( m_qsFile ) );
        return;
    }

    if( !qfFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Unable to read the following file:\n"
                                                      "%1").arg( m_qsFile ) );
        return;
    }

    char strLine[1000];

    memset( strLine, 0, 1000 );

    while( qfFile.readLine( strLine, 1000 ) > 0 )
    {
        QStringList qslData = QString( strLine ).split( "\t" );

        QString barcode     = qslData.at(0);
        QString dateused    = qslData.at(1);
        int unitsleft       = qslData.at(2).toInt();

        unsigned int id;

        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientcardid FROM patientCards WHERE barcode = \"" + barcode + "\"" );

        if( poQuery->size() != 1 )
        {
            // létre kell hozni
            g_poDB->executeQTQuery( QString( "INSERT INTO patientcards SET licenceId=%1, barcode=\"%2\", comment=\"Adatbazis javitas soran letrehozva\" " ).arg( g_poPrefs->getLicenceId() ).arg( barcode ) );
        }
        else
        {
            // update-elni kell
            poQuery->first();

            id = poQuery->value(0).toUInt();

            poQuery = g_poDB->executeQTQuery( QString( "SELECT patientcardunitid FROM patientcardunits WHERE patientcardid=%1 AND active=1" ).arg( id ) );

            int darab = poQuery->size() - unitsleft;

            for( int i=0; i<darab; i++ )
            {
                poQuery->next();

                unsigned int unitid = poQuery->value( 0 ).toUInt();

                g_poDB->executeQTQuery( QString( "UPDATE patientcardunits SET datetimeused=\"%1\", active=0 WHERE patientcardunitid=%2 AND active=1" ).arg( dateused ).arg( unitid ) );
            }
        }
    }

    qfFile.close();

    QMessageBox::information( this, tr("Information"), "Kesz" );
}
