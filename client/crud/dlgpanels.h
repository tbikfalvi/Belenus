#ifndef DLGPANELS_H
#define DLGPANELS_H

#include "../framework/dlgcrud.h"

class cDlgPanels : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPanels( QWidget *p_poParent = 0 );
    virtual ~cDlgPanels();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPANELS_H
