
#include <QPushButton>
#include <QMessageBox>

#include "dlgcomment.h"

dlgComment::dlgComment( QWidget *p_poParent, QString p_qsTitle ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( p_qsTitle );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
}

dlgComment::~dlgComment()
{
}

QString dlgComment::resultComment()
{
    return ledComment->text();
}

void dlgComment::on_pbOk_clicked()
{
    QDialog::accept();
}

void dlgComment::on_pbCancel_clicked()
{
    QDialog::reject();
}
