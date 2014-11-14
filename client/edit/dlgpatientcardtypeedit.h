#ifndef DLGPATIENTCARDTYPEEDIT_H
#define DLGPATIENTCARDTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgpatientcardtypeedit.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgprogress.h"

class cDlgPatientCardTypeEdit : public QDialog, protected Ui::dlgPatientCardTypeEdit
{
    Q_OBJECT

public:
    cDlgPatientCardTypeEdit( QWidget *p_poParent = 0, cDBPatientCardType *p_poPatientCardType = NULL );
    virtual ~cDlgPatientCardTypeEdit();

protected:
    cDBPatientCardType *m_poPatientCardType;

private slots:
    void on_listValidInterval_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_pbDelete_clicked();
    void on_pbEdit_clicked();
    void on_pbAdd_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
    void on_rbInterval_toggled(bool checked);
    void on_ledPrice_textChanged(const QString &arg1);

private:
    cDlgProgress            *m_dlgProgress;
};

#endif
