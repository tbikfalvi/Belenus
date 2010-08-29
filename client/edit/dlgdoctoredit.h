#ifndef DLGDOCTOREDIT_H
#define DLGDOCTOREDIT_H

#include "belenus.h"
#include "ui_dlgdoctoredit.h"
#include "db/dbdoctor.h"

class cDlgDoctorEdit : public QDialog, protected Ui::dlgDoctorEdit
{
    Q_OBJECT

public:
    cDlgDoctorEdit( QWidget *p_poParent = 0, cDBDoctor *p_poDoctor = NULL );
    virtual ~cDlgDoctorEdit();

public slots:
    virtual void accept ();

protected:
    cDBDoctor *m_poDoctor;
};

#endif
