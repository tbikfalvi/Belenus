#ifndef DLGPATIENTCARD_H
#define DLGPATIENTCARD_H

#include "../framework/dlgcrud.h"

class cDlgPatientCard : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatientCard( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCard();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENTCARD_H
