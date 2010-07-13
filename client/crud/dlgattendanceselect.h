#ifndef DLGATTENDANCESELECT_H
#define DLGATTENDANCESELECT_H

#include "dlgattendance.h"

class cDlgAttendanceSelect : public cDlgAttendance
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgAttendanceSelect( QWidget *p_poParent = 0 );
    ~cDlgAttendanceSelect();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void on_pbSelect_clicked();

};

#endif // DLGATTENDANCESELECT_H
