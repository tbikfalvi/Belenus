#ifndef DLGILLNESSGROUPEDIT_H
#define DLGILLNESSGROUPEDIT_H

#include "belenus.h"
#include "ui_dlgillnessgroupedit.h"
#include "db/dbillnessgroup.h"

class cDlgIllnessGroupEdit : public QDialog, protected Ui::dlgIllnessGroupEdit
{
    Q_OBJECT

public:
    cDlgIllnessGroupEdit( QWidget *p_poParent = 0, cDBIllnessGroup *p_poIllnessGroup = NULL );
    virtual ~cDlgIllnessGroupEdit();

public slots:
    virtual void accept ();

protected:
    cDBIllnessGroup *m_poIllnessGroup;
};

#endif
