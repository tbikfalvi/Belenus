#ifndef DLGPRODUCTSELL_H
#define DLGPRODUCTSELL_H

#include <QVector>

#include "ui_dlgproductsell.h"
#include "../db/dbproduct.h"
#include "../db/dbshoppingcart.h"

typedef struct
{
    unsigned int    uiId;
    QString         qsName;
    QString         qsBarcode;
    int             inNetPrice;
    int             inVatPercent;
    int             inProductCount;
} tsProducts;

class cDlgProductSell : public QDialog, protected Ui::dlgProductSell
{
    Q_OBJECT

public:
    cDlgProductSell( QWidget *p_poParent, QString p_qsBarcode = "" );
    virtual ~cDlgProductSell();
/*
    QVBoxLayout *verticalLayoutTop;
    QGroupBox   *gbFilter;
    QHBoxLayout *layoutFilterGroup;
    QLabel      *lblBarcode;
    QLineEdit   *ledBarcode;
    QSpacerItem *spacerFilterGroup;
    QSpacerItem *spacerToPay;
    QGroupBox   *gbItemToSell;
    QHBoxLayout *layoutItemToSellGroup;
    QLabel      *lblFilterName;
    QLineEdit   *ledFilterName;
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
    QPushButton *pbEditProducts;
    QPushButton *pbRefresh;
*/
private:

    QVector<tsProducts*>    qvProducts;
    cDBProduct              m_obProduct;

    void _calculateTotalPrice();

//protected:
//    virtual void setupTableView();
//    virtual void enableButtons();

signals:
    void signalPaymentProcessed( const cDBShoppingCart &p_DBShoppingCart );

protected slots:
//    virtual void refreshTable();
//    virtual void newClicked( bool );
//    virtual void deleteClicked( bool );
//    virtual void editClicked( bool );
//    void on_pbItemCountIncrease_clicked();
//    void on_pbItemCountDecrease_clicked();
    void on_pbPayment_clicked();
//    void on_pbToCart_clicked();
//    void on_pbEditProducts_clicked();
//    void enablePayment();
private slots:
    void on_pbRefresh_clicked();
    void on_cmbProduct_currentIndexChanged(int index);
    void on_cmbProductCount_currentIndexChanged(int index);
    void on_pbCancel_clicked();
};

#endif // DLGPRODUCTSELL_H
