#ifndef DLG_PATIENTEDIT_H
#define DLG_PATIENTEDIT_H

#include "belenus.h"
#include "ui_dlgpatientedit.h"
#include "db/dbpatient.h"

class cDlgPatientEdit : public QDialog, private Ui::dlgPatientEdit
{
    Q_OBJECT

public:
    cDlgPatientEdit( QWidget *p_poParent = 0, cDBPatient *p_poPatient = NULL );
    ~cDlgPatientEdit();

public slots:

private:

protected:
    cDBPatient *m_poPatient;

private slots:
    void on_pbFinishLater_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
