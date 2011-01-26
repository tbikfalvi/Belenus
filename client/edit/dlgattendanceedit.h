#ifndef DLGATTENDANCEEDIT_H
#define DLGATTENDANCEEDIT_H

#include "belenus.h"
#include "ui_dlgattendanceedit.h"
#include "../db/dbattendance.h"
#include "db/dbpostponed.h"

class cDlgAttendanceEdit : public QDialog, protected Ui::dlgAttendanceEdit
{
    Q_OBJECT

public:
    cDlgAttendanceEdit( QWidget *p_poParent = 0, cDBAttendance *p_poAttendance = NULL, cDBPostponed *p_poPostponed = NULL );
    virtual ~cDlgAttendanceEdit();

protected:
    cDBAttendance   *m_poAttendance;
    cDBPostponed    *m_poPostponed;

private:
    bool SaveAttendanceData();

private slots:
    void on_pbFinishLater_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
