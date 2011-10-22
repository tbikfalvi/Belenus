#ifndef DLGPRODUCTTYPE_H
#define DLGPRODUCTTYPE_H

#include "../framework/dlgcrud.h"

class cDlgProductType : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProductType( QWidget *p_poParent = 0 );
    virtual ~cDlgProductType();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPRODUCTTYPE_H
