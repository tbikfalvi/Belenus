#ifndef DLGPRODUCTTYPES_H
#define DLGPRODUCTTYPES_H

#include "../framework/dlgcrud.h"

class cDlgProductTypes : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProductTypes( QWidget *p_poParent = 0 );
    virtual ~cDlgProductTypes();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPRODUCTTYPES_H
