#ifndef DLGPRODUCTACTIONTYPEEDIT_H
#define DLGPRODUCTACTIONTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgproductactiontypeedit.h"
#include "../db/dbproductactiontype.h"

class cDlgProductActionTypeEdit : public QDialog, protected Ui::dlgProductActionTypeEdit
{
    Q_OBJECT

public:
    cDlgProductActionTypeEdit( QWidget *p_poParent = 0, cDBProductActionType *p_poProductActionType = NULL );
    virtual ~cDlgProductActionTypeEdit();

protected:
    cDBProductActionType *m_poProductActionType;

private slots:
    void on_chkDecrease_toggled(bool checked);
    void on_chkIncrease_toggled(bool checked);
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
