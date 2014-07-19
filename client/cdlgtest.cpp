
#include <QMenu>

#include "belenus.h"
#include "cdlgtest.h"
#include "ui_cdlgtest.h"

cDlgTest::cDlgTest(QWidget *parent) : QDialog(parent), ui(new Ui::cDlgTest)
{
    ui->setupUi(this);

    ui->ledVat->setText( "0" );

    ui->pbImageClick->setIcon( QIcon( "./resources/20x20_gibbig_off.png" ) );

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
