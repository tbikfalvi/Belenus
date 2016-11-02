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

private:
    bool    m_bChangeLicenceKey;

private slots:
    void on_pbActivateKey_clicked();
    void on_pbValidateCode_clicked();
    void on_pbValidateApplication_clicked();
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
    void on_pbChangeKey_clicked();
};

#endif
