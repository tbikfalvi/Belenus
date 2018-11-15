#ifndef DLGEMAILS_H
#define DLGEMAILS_H

#include "../framework/dlgcrud.h"

class cDlgEmails : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgEmails( QWidget *p_poParent = 0 );
    virtual ~cDlgEmails();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGEMAILS_H
