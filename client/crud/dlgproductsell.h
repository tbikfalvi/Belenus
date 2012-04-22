#ifndef DLGPRODUCTSELL_H
#define DLGPRODUCTSELL_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QGroupBox>

#include "../framework/dlgcrud.h"
#include "../db/dbproduct.h"
#include "../db/dbshoppingcart.h"

class cDlgProductSell : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgProductSell( QWidget *p_poParent, QString p_qsBarcode = "" );
    virtual ~cDlgProductSell();

    QVBoxLayout *verticalLayoutTop;
    QGroupBox   *gbFilter;
    QHBoxLayout *layoutFilterGroup;
    QLabel      *lblBarcode;
    QLineEdit   *ledBarcode;
    QSpacerItem *spacerFilterGroup;
    QGroupBox   *gbItemToSell;
    QHBoxLayout *layoutItemToSellGroup;
    QLabel      *lblName;
    QLineEdit   *ledName;
    QLabel      *lblItemCount;
    QLineEdit   *ledItemCount;
    QPushButton *pbItemCountIncrease;
    QPushButton *pbItemCountDecrease;
    QLabel      *lblTotalPrice;
    QLineEdit   *ledTotalPrice;
    QPushButton *pbPayment;
    QPushButton *pbToCart;
    QPushButton *pbCancel;

private:
    cDBProduct   m_obProduct;

    void _calculateTotalPrice();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

signals:
    void signalPaymentProcessed( const cDBShoppingCart &p_DBShoppingCart );

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    void on_pbItemCountIncrease_clicked();
    void on_pbItemCountDecrease_clicked();
    void on_pbPayment_clicked();
    void on_pbToCart_clicked();
    void enablePayment();
};

#endif // DLGPRODUCTSELL_H
