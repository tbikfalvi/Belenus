#ifndef DLGCASSAACTION_H
#define DLGCASSAACTION_H

#include "../belenus.h"
#include "ui_dlgcassaaction.h"

class cDlgCassaAction : public QDialog, protected Ui::dlgCassaAction
{
    Q_OBJECT

public:

    enum tePayType
    {
        PAY_CASH = 0,
        PAY_CREDITCARD
    };

    cDlgCassaAction( QWidget *p_poParent = 0, int p_nMoney = 0 );
    virtual ~cDlgCassaAction();

    void setPayWithCash();
    void setPayWithVoucher();
    void setPayWithCreditcard();
    void setCassaAction();
    QString cassaResult( int *p_nPayType, QString *p_qsComment );

private:
    QString convertCurrency(const QString &text) const;

    int     m_nHeightSmall;
    int     m_nHeightBig;

private slots:
    void on_pbComment_clicked();
    void on_pbCancel_clicked();
    void on_pbOk_clicked();
    void updateMoneyBack();
    void ledAmountToPay_textEdited(QString text);
    void ledAmountGiven_textEdited(QString text);
};

#endif
