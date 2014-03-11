
#include "belenus.h"
#include "cdlgtest.h"
#include "ui_cdlgtest.h"

cDlgTest::cDlgTest(QWidget *parent) : QDialog(parent), ui(new Ui::cDlgTest)
{
    ui->setupUi(this);

    ui->ledVat->setText( "0" );

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
