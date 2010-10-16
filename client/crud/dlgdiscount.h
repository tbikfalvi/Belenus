#ifndef DLGDISCOUNT_H
#define DLGDISCOUNT_H

#include "../framework/dlgcrud.h"

class cDlgDiscount : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgDiscount( QWidget *p_poParent = 0 );
    virtual ~cDlgDiscount();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGDISCOUNT_H
