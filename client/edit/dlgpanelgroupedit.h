#ifndef DLGPANELGROUPEDIT_H
#define DLGPANELGROUPEDIT_H

#include "belenus.h"
#include "ui_dlgpanelgroupedit.h"
#include "../db/dbpanelgroups.h"

class cDlgPanelGroupEdit : public QDialog, private Ui::dlgPanelGroupEdit
{
    Q_OBJECT

public:
    cDlgPanelGroupEdit( QWidget *p_poParent = 0, cDBPanelGroups *p_poPanelGroups = NULL );
    virtual ~cDlgPanelGroupEdit();

protected:
    cDBPanelGroups *m_poPanelGroups;

public slots:
    virtual void accept();
};

#endif // DLGPANELGROUPEDIT_H
