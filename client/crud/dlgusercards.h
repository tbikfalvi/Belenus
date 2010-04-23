#ifndef DLGUSERCARDS_H
#define DLGUSERCARDS_H

#include "../framework/dlgcrud.h"

class cDlgUserCards : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgUserCards( QWidget *p_poParent = 0 );
    virtual ~cDlgUserCards();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};


#endif // DLGUSERCARDS_H
