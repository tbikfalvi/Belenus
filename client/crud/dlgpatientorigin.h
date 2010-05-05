#ifndef DLGPATIENTORIGIN_H
#define DLGPATIENTORIGIN_H

#include "../framework/dlgcrud.h"

class cDlgPatientOrigin : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatientOrigin( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientOrigin();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENTORIGIN_H
