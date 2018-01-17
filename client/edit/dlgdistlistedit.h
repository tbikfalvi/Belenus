#ifndef DLGDISTLISTEDIT_H
#define DLGDISTLISTEDIT_H

#include "belenus.h"
#include "ui_dlgDistListEdit.h"
#include "../db/dbdistlist.h"

class cDlgDistListEdit : public QDialog, protected Ui::dlgDistListEdit
{
    Q_OBJECT

public:
    cDlgDistListEdit( QWidget *p_poParent = 0, cDBDistList *p_poDistList = NULL );
    virtual ~cDlgDistListEdit();

protected:
    cDBDistList *m_poDistList;

private slots:
    void on_pbPatientRemoveAll_clicked();
    void on_pbPatientRemove_clicked();
    void on_pbPatientAddAll_clicked();
    void on_pbPatientAdd_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
