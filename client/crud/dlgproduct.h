#ifndef DLGPRODUCT_H
#define DLGPRODUCT_H

#include "../framework/dlgcrud.h"

class cDlgProduct : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProduct( QWidget *p_poParent = 0 );
    virtual ~cDlgProduct();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPRODUCT_H
