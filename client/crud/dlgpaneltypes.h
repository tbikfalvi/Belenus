#ifndef DLGPANELTYPES_H
#define DLGPANELTYPES_H

#include "../framework/dlgcrud.h"

class cDlgPanelTypes : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPanelTypes( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelTypes();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPANELTYPES_H
