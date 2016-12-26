#include "dlglineedit.h"
#include "ui_dlglineedit.h"

dlgLineEdit::dlgLineEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLineEdit)
{
    ui->setupUi(this);
}

dlgLineEdit::~dlgLineEdit()
{
    delete ui;
}

void dlgLineEdit::setTitle(QString p_qsTitle)
{
    ui->lblTitle->setText( p_qsTitle );
}

QString dlgLineEdit::value()
{
    return ui->ledValue->text();
}
