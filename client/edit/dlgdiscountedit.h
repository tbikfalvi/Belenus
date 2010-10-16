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
    void fillHCDComboList();

public slots:
    virtual void accept ();

protected:
    cDBDiscount *m_poDiscount;

private slots:
    void on_rbDoctor_clicked();
    void on_rbCompany_clicked();
    void on_rbHealthInsurance_clicked();
    void on_rbService_clicked();
    void on_rbEmployee_clicked();
    void on_rbRegularCustomer_clicked();
};

#endif
