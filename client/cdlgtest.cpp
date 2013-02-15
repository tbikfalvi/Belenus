
#include "belenus.h"
#include "cdlgtest.h"
#include "ui_cdlgtest.h"

cDlgTest::cDlgTest(QWidget *parent) : QDialog(parent), ui(new Ui::cDlgTest)
{
    ui->setupUi(this);
}

cDlgTest::~cDlgTest()
{
    delete ui;
}

void cDlgTest::on_pbCalculate_clicked()
{
    cCurrency::currType ctTest = cCurrency::CURR_NET;

    if( ui->rbNet->isChecked() ) ctTest = cCurrency::CURR_NET;
    else if( ui->rbGros->isChecked() ) ctTest = cCurrency::CURR_GROSS;

    cCurrency   currTest( ui->ledCurrencyValue->text(), ctTest, ui->ledVat->text().toInt() );

    ui->lblCurrencyFull->setText( currTest.currencyFullStringShort() );
}
