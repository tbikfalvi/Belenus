#ifndef DLGPRODUCTEDIT_H
#define DLGPRODUCTEDIT_H

#include "belenus.h"
#include "ui_dlgproductedit.h"
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
    void slot_PriceCalculate();
    void on_pbProductStorage_clicked();
    void on_pbProductRemoveAll_clicked();
    void on_pbProductRemove_clicked();
    void on_pbProductAddAll_clicked();
    void on_pbProductAdd_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
    void on_ledPriceSell_textChanged(const QString &arg1);
    void on_ledVatpercentSell_textChanged(const QString &arg1);
};

#endif
