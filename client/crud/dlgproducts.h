#ifndef DLGPRODUCTS_H
#define DLGPRODUCTS_H

#include "../framework/dlgcrud.h"

class cDlgProducts : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProducts( QWidget *p_poParent = 0 );
    virtual ~cDlgProducts();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPRODUCTS_H
