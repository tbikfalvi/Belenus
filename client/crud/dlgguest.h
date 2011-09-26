#ifndef DLGGUEST_H
#define DLGGUEST_H

#include "../framework/dlgcrud.h"

class cDlgGuest : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgGuest( QWidget *p_poParent = 0 );
    virtual ~cDlgGuest();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGGUEST_H
