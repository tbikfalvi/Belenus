#ifndef DLGPANELSTATUSES_H
#define DLGPANELSTATUSES_H

#include "../framework/dlgcrud.h"

class cDlgPanelStatuses : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPanelStatuses( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelStatuses();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENTORIGIN_H
