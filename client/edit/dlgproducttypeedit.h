#ifndef DLGPRODUCTTYPEEDIT_H
#define DLGPRODUCTTYPEEDIT_H

#include "belenus.h"
#include "../../belenus-build-desktop/ui_dlgproducttypeedit.h"
#include "../db/dbproducttype.h"

class cDlgProductTypeEdit : public QDialog, protected Ui::dlgProductTypeEdit
{
    Q_OBJECT

public:
    cDlgProductTypeEdit( QWidget *p_poParent = 0, cDBProductType *p_poProductType = NULL );
    virtual ~cDlgProductTypeEdit();

protected:
    cDBProductType *m_poProductType;

private slots:
    void on_chkStore_toggled(bool checked);
    void on_chkStorageRoom_toggled(bool checked);
    void on_pbProductRemoveAll_clicked();
    void on_pbProductRemove_clicked();
    void on_pbProductAddAll_clicked();
    void on_pbProductAdd_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
