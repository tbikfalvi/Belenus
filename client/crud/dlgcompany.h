#ifndef DLGCOMPANY_H
#define DLGCOMPANY_H

#include "../framework/dlgcrud.h"

class cDlgCompany : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgCompany( QWidget *p_poParent = 0 );
    virtual ~cDlgCompany();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGCOMPANY_H
