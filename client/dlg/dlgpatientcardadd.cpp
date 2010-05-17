#include <QPushButton>
#include <QMessageBox>

#include "dlgpatientcardadd.h"

cDlgPatientCardAdd::cDlgPatientCardAdd( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Adding PatientCard to database") );

    pbSave->setIcon( QIcon("./resources/40x40_save.gif") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.gif") );
}

cDlgPatientCardAdd::~cDlgPatientCardAdd()
{
}

void cDlgPatientCardAdd::on_pbExit_clicked()
{
    QDialog::reject();
}
