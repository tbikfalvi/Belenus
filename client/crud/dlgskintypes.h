#ifndef DLGSKINTYPES_H
#define DLGSKINTYPES_H

#include "../framework/dlgcrud.h"

class cDlgSkinTypes : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgSkinTypes( QWidget *p_poParent = 0 );
    virtual ~cDlgSkinTypes();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGSKINTYPES_H
