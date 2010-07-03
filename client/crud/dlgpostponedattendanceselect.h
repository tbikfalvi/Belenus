#ifndef DLGPOSTPONEDATTENDANCESELECT_H
#define DLGPOSTPONEDATTENDANCESELECT_H

#include "dlgattendance.h"

class cDlgPostponedAttendanceSelect : public cDlgAttendance
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgPostponedAttendanceSelect( QWidget *p_poParent = 0 );
    ~cDlgPostponedAttendanceSelect();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void on_pbSelect_clicked();

};

#endif // DLGPOSTPONEDATTENDANCESELECT_H
