#ifndef DLGPANELSTATUSESEDIT_H
#define DLGPANELSTATUSESEDIT_H

#include "belenus.h"
#include "../ui_dlgpanelstatusesedit.h"
#include "../db/dbpanelstatuses.h"

class cDlgPanelStatusesEdit : public QDialog, protected Ui::dlgPanelStatusesEdit
{
    Q_OBJECT

public:
    cDlgPanelStatusesEdit( QWidget *p_poParent = 0, cDBPanelStatuses *p_poPanelStatuses = NULL );
    virtual ~cDlgPanelStatusesEdit();

public slots:
    virtual void accept ();

protected:
    cDBPanelStatuses *m_poPanelStatuses;
};

#endif
