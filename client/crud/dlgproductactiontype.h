#ifndef DLGPRODUCTACTIONTYPE_H
#define DLGPRODUCTACTIONTYPE_H

#include "../framework/dlgcrud.h"

class cDlgProductActionType : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProductActionType( QWidget *p_poParent = 0 );
    virtual ~cDlgProductActionType();

    QPushButton *pbProduct;
    QPushButton *pbProductType;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

private:
    QWidget     *m_poParent;

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void _slotProduct();
    virtual void _slotProductTypes();
};

#endif // DLGPRODUCTTYPE_H
