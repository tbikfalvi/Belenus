#ifndef DLGPAYMENTMETHOD_H
#define DLGPAYMENTMETHOD_H

#include "../framework/dlgcrud.h"

class cDlgPaymentMethod : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPaymentMethod( QWidget *p_poParent = 0 );
    virtual ~cDlgPaymentMethod();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPAYMENTMETHOD_H
