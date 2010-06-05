#ifndef DLGREASONTOVISIT_H
#define DLGREASONTOVISIT_H

#include "../framework/dlgcrud.h"

class cDlgReasonToVisit : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgReasonToVisit( QWidget *p_poParent = 0 );
    virtual ~cDlgReasonToVisit();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGREASONTOVISIT_H
