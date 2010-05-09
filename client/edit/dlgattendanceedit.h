#ifndef DLGATTENDANCEEDIT_H
#define DLGATTENDANCEEDIT_H

#include "belenus.h"
#include "../ui_dlgattendanceedit.h"
#include "../db/dbattendance.h"

class cDlgAttendanceEdit : public QDialog, protected Ui::dlgAttendanceEdit
{
    Q_OBJECT

public:
    cDlgAttendanceEdit( QWidget *p_poParent = 0, cDBAttendance *p_poAttendance = NULL );
    virtual ~cDlgAttendanceEdit();

protected:
    cDBAttendance *m_poAttendance;
};

#endif
