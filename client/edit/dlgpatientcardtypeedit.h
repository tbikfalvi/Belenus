#ifndef DLGPATIENTCARDTYPEEDIT_H
#define DLGPATIENTCARDTYPEEDIT_H

#include "belenus.h"
#include "../../belenus-build-desktop/ui_dlgpatientcardtypeedit.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardTypeEdit : public QDialog, protected Ui::dlgPatientCardTypeEdit
{
    Q_OBJECT

public:
    cDlgPatientCardTypeEdit( QWidget *p_poParent = 0, cDBPatientCardType *p_poPatientCardType = NULL );
    virtual ~cDlgPatientCardTypeEdit();

protected:
    cDBPatientCardType *m_poPatientCardType;

private slots:
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
    void on_rbInterval_toggled(bool checked);
};

#endif
