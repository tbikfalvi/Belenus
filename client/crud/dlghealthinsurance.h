#ifndef DLGHEALTHINSURANCE_H
#define DLGHEALTHINSURANCE_H

#include "../framework/dlgcrud.h"

class cDlgHealthInsurance : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgHealthInsurance( QWidget *p_poParent = 0 );
    virtual ~cDlgHealthInsurance();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGHEALTHINSURANCE_H
