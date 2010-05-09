#ifndef DLGATTENDANCE_H
#define DLGATTENDANCE_H

#include "../framework/dlgcrud.h"

class cDlgAttendance : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgAttendance( QWidget *p_poParent = 0 );
    virtual ~cDlgAttendance();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGATTENDANCE_H
