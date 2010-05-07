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

private slots:

protected:
    cDBPatient *m_poPatient;

};

#endif
