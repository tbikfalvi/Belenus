#ifndef DLGPANELGROUPS_H
#define DLGPANELGROUPS_H

#include "../framework/dlgcrud.h"

class cDlgPanelGroups : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPanelGroups( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelGroups();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPANELGROUPS_H
