#ifndef DLGSHOPPINGCART_H
#define DLGSHOPPINGCART_H

#include "../framework/dlgcrud.h"

class cDlgShoppingCart : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgShoppingCart( QWidget *p_poParent = 0 );
    virtual ~cDlgShoppingCart();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGSHOPPINGCART_H
