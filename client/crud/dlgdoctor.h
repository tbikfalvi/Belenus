#ifndef DLGDOCTOR_H
#define DLGDOCTOR_H

#include "../framework/dlgcrud.h"

class cDlgDoctor : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgDoctor( QWidget *p_poParent = 0 );
    virtual ~cDlgDoctor();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGDOCTOR_H
