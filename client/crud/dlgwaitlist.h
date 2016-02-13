#ifndef DLGWAITLIST_H
#define DLGWAITLIST_H

#include "../framework/dlgcrud.h"

class cDlgWaitlist : public cDlgCrud
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgWaitlist( QWidget *p_poParent = 0 );
    virtual ~cDlgWaitlist();

    unsigned int selectedId() { return m_uiSelectedId; }

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbSelect_clicked();
};

#endif // DLGWAITLIST_H
