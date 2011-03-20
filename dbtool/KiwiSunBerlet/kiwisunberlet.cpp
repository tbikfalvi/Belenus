#include "kiwisunberlet.h"
#include "ui_kiwisunberlet.h"

KiwiSunBerlet::KiwiSunBerlet(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    setWindowIcon( QIcon( QString(":/icons/resources/KiwiSunBerlet.ico") ) );

    dateImportStart->setDate( QDate::currentDate() );
}

KiwiSunBerlet::~KiwiSunBerlet()
{
    delete ui;
}

void KiwiSunBerlet::on_pbSelectPCU_clicked()
{

}

void KiwiSunBerlet::on_pbImportPCU_clicked()
{

}
