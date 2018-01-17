#ifndef DLGWAITLISTINFO_H
#define DLGWAITLISTINFO_H

#include "../framework/dlgcrud.h"

class cDlgWaitlistInfo : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgWaitlistInfo( QWidget *p_poParent = 0 );
    virtual ~cDlgWaitlistInfo();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGWAITLIST_H
