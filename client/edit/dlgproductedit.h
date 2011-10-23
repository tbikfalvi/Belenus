#ifndef DLGPRODUCTEDIT_H
#define DLGPRODUCTEDIT_H

#include "belenus.h"
#include "../ui_dlgproductedit.h"
#include "../db/dbproduct.h"

class cDlgProductEdit : public QDialog, protected Ui::dlgProductEdit
{
    Q_OBJECT

public:
    cDlgProductEdit( QWidget *p_poParent = 0, cDBProduct *p_poProduct = NULL );
    virtual ~cDlgProductEdit();

protected:
    cDBProduct *m_poProduct;

private slots:
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
