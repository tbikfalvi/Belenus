
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

void dlgComment::setDefault(QString p_qsComment)
{
    teComment->setText( p_qsComment );
}

QString dlgComment::resultComment()
{
    return teComment->toPlainText();
}

void dlgComment::on_pbOk_clicked()
{
    QDialog::accept();
}

void dlgComment::on_pbCancel_clicked()
{
    QDialog::reject();
}
