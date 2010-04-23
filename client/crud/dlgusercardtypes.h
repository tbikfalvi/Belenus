#ifndef DLGUSERCARDTYPES_H
#define DLGUSERCARDTYPES_H

#include "../framework/dlgcrud.h"

class cDlgUserCardTypes : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgUserCardTypes( QWidget *p_poParent = 0 );
    virtual ~cDlgUserCardTypes();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif
