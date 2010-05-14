#ifndef DLGPATIENTCARDTYPE_H
#define DLGPATIENTCARDTYPE_H

#include "../framework/dlgcrud.h"

class cDlgPatientCardType : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatientCardType( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCardType();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPATIENTCARDTYPE_H
