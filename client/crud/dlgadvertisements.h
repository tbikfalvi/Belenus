#ifndef DLGADVERTISEMENTS_H
#define DLGADVERTISEMENTS_H

#include "../framework/dlgcrud.h"

class cDlgAdvertisements : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgAdvertisements( QWidget *p_poParent = 0 );
    virtual ~cDlgAdvertisements();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGADVERTISEMENTS_H
