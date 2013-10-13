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
    void actionCassaInOut();
    void actionPayment();
    QString cassaResult( int *p_nPayType, QString *p_qsComment, bool *p_bShoppingCart );

private:
    QString convertCurrency(const QString &text) const;

    cDBShoppingCart     *m_poShoppingCart;
    int                  m_nHeightSmall;
    int                  m_nHeightBig;
    bool                 m_bShoppingCart;

private slots:
    void on_pbShoppingCart_clicked();
    void on_pbComment_clicked();
    void on_pbCancel_clicked();
    void on_pbOk_clicked();
    void updateMoneyBack();
    void ledAmountToPay_textEdited(QString text);
    void ledAmountGiven_textEdited(QString text);
    void on_cmbPaymentType_currentIndexChanged(int index);
    void on_pbPayCash_clicked();
    void on_pbPayCard_clicked();
    void on_pbPayOther_clicked();
};

#endif
