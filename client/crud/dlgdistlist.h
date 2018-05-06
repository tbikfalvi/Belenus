#ifndef DLGDISTLIST_H
#define DLGDISTLIST_H

#include "../framework/dlgcrud.h"

class cDlgDistList : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgDistList( QWidget *p_poParent = 0 );
    virtual ~cDlgDistList();

    QPushButton *pbProduct;
    QPushButton *pbProductActionType;

private:
    QWidget     *m_poParent;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGDISTLIST_H
