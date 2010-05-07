#ifndef DLGPATIENT_H
#define DLGPATIENT_H

#include "../framework/dlgcrud.h"

class cDlgPatient : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatient( QWidget *p_poParent = 0 );
    virtual ~cDlgPatient();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENT_H
