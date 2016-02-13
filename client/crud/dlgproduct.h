#ifndef DLGPRODUCT_H
#define DLGPRODUCT_H

#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgProduct : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProduct( QWidget *p_poParent = 0 );
    virtual ~cDlgProduct();

    QHBoxLayout *horizontalLayout;
    QLabel      *lblFilterProductType;
    QComboBox   *cmbFilterProductType;
    QSpacerItem *horizontalSpacer1;
    QLabel      *lblFilterName;
    QLineEdit   *ledFilterName;
    QLabel      *lblFilterBarcode;
    QLineEdit   *ledFilterBarcode;
    QLabel      *lblFilterMinCount;
    QLineEdit   *ledFilterMinCount;
    QLabel      *lblFilterMaxCount;
    QLineEdit   *ledFilterMaxCount;
    QPushButton *pbProductType;
    QPushButton *pbProductActionType;
    QPushButton *pbStockIncrease;
    QPushButton *pbStockDecrease;
    QPushButton *pbStockProcess;

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
    virtual void _slotProductTypes();
    virtual void _slotProductActionTypes();
    virtual void _slotStockIncrease();
    virtual void _slotStockDecrease();
    virtual void _slotStockProcess();
};

#endif // DLGPRODUCT_H
