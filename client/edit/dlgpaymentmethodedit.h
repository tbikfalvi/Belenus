#ifndef DLGPAYMENTMETHODEDIT_H
#define DLGPAYMENTMETHODEDIT_H

#include "belenus.h"
#include "ui_dlgpaymentmethodedit.h"
#include "db/dbpaymentmethod.h"

class cDlgPaymentMethodEdit : public QDialog, protected Ui::dlgPaymentMethodEdit
{
    Q_OBJECT

public:
    cDlgPaymentMethodEdit( QWidget *p_poParent = 0, cDBPaymentMethod *p_poPaymentMethod = NULL );
    virtual ~cDlgPaymentMethodEdit();

public slots:
    virtual void accept ();

protected:
    cDBPaymentMethod *m_poPaymentMethod;
};

#endif
