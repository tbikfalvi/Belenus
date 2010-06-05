#ifndef DLGUSERS_H
#define DLGUSERS_H

#include "../framework/dlgcrud.h"

class cDlgUsers : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgUsers( QWidget *p_poParent = 0 );
    virtual ~cDlgUsers();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif
