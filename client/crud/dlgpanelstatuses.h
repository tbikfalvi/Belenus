#ifndef DLGPANELSTATUSES_H
#define DLGPANELSTATUSES_H

#include "../framework/dlgcrud.h"

class cDlgPanelStatuses : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPanelStatuses( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelStatuses();

    bool isStatusChanged();

private:
    bool    m_bStatusChanged;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENTORIGIN_H
