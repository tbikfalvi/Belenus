#ifndef DLGPATIENTORIGINEDIT_H
#define DLGPATIENTORIGINEDIT_H

#include "belenus.h"
#include "ui_dlgdiscountedit.h"
#include "../db/dbdiscount.h"

class cDlgDiscountEdit : public QDialog, protected Ui::dlgDiscountEdit
{
    Q_OBJECT

public:
    cDlgDiscountEdit( QWidget *p_poParent = 0, cDBDiscount *p_poDiscount = NULL );
    virtual ~cDlgDiscountEdit();

private:
    bool    m_bLoading;

public slots:
    virtual void accept ();

protected:
    cDBDiscount *m_poDiscount;

private slots:
    void slotRefreshWarningColors();
    void slotFillHCDComboList();
    void slotCheckValue();
    void on_ledName_textChanged(const QString &arg1);
};

#endif
