#ifndef DLGSERIALREG_H
#define DLGSERIALREG_H

#include "../belenus.h"
#include "ui_dlgserialreg.h"

class cDlgSerialReg : public QDialog, protected Ui::dlgSerialReg
{
    Q_OBJECT

public:
    cDlgSerialReg( QWidget *p_poParent = 0 );
    virtual ~cDlgSerialReg();

private slots:
    void on_pbCancel_clicked();
    void on_pbValidate_clicked();
};

#endif
