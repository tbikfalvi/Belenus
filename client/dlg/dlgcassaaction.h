#ifndef DLGCASSAACTION_H
#define DLGCASSAACTION_H

#include "../belenus.h"
#include "../db/dbshoppingcart.h"
#include "ui_dlgcassaaction.h"

class cDlgCassaAction : public QDialog, protected Ui::dlgCassaAction
{
    Q_OBJECT

public:

    enum tePayType
    {
        PAY_CASH = 1,
        PAY_CREDITCARD,
        PAY_OTHER
    };

    cDlgCassaAction( QWidget *p_poParent = 0, cDBShoppingCart *p_poShoppingCart = NULL );
    virtual ~cDlgCassaAction();

    void setPayWithCash();
    void setPayWithVoucher();
    void setPayWithCreditcard();
    void actionPayment();
    QString cassaResult( int *p_nPayType, bool *p_bShoppingCart, unsigned int *p_uiCouponId );

private:
    cDBShoppingCart     *m_poShoppingCart;
    int                  m_nHeightSmall;
    int                  m_nHeightBig;
    bool                 m_bShoppingCart;

private slots:
    void checkGivenValues();
    void on_pbShoppingCart_clicked();
    void on_pbComment_clicked();
    void on_pbCancel_clicked();
    void on_pbOk_clicked();
    void updateShoppingCartItem();
    void on_pbPayCash_clicked();
    void on_pbPayCard_clicked();
    void on_pbPayOther_clicked();
    void on_ledVoucherGiven_textChanged(const QString &arg1);
    void on_cmbCoupon_currentIndexChanged(int index);
    void on_ledCashGiven_textChanged(const QString &arg1);
    void on_ledCardGiven_textChanged(const QString &arg1);
};

#endif
