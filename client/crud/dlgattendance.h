#ifndef DLGATTENDANCE_H
#define DLGATTENDANCE_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgAttendance : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel *lblPatient;
    QComboBox *cmbPatient;
    QSpacerItem *horizontalSpacer1;
    QSpacerItem *horizontalSpacer2;

    cDlgAttendance( QWidget *p_poParent = 0 );
    virtual ~cDlgAttendance();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGATTENDANCE_H
