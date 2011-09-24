#ifndef DLGPANELTYPEEDIT_H
#define DLGPANELTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgpaneltypeedit.h"
#include "../db/dbpaneltypes.h"

class cDlgPanelTypeEdit : public QDialog, private Ui::dlgPanelTypeEdit
{
    Q_OBJECT

public:
    cDlgPanelTypeEdit( QWidget *p_poParent = 0, cDBPanelTypes *p_poPanelTypes = NULL );
    virtual ~cDlgPanelTypeEdit();

protected:
    cDBPanelTypes *m_poPanelTypes;

public slots:
    virtual void accept();
};

#endif // DLGPANELTYPEEDIT_H
