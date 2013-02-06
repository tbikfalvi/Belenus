#include "cdlgtest.h"
#include "ui_cdlgtest.h"

cDlgTest::cDlgTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cDlgTest)
{
    ui->setupUi(this);
}

cDlgTest::~cDlgTest()
{
    delete ui;
}
