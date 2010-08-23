#ifndef DLGILLNESSGROUP_H
#define DLGILLNESSGROUP_H

#include "../framework/dlgcrud.h"

class cDlgIllnessGroup : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgIllnessGroup( QWidget *p_poParent = 0 );
    virtual ~cDlgIllnessGroup();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGILLNESSGROUP_H
