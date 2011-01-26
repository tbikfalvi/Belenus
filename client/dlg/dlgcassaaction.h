#ifndef DLGCASSAACTION_H
#define DLGCASSAACTION_H

#include "../belenus.h"
#include "ui_dlgcassaaction.h"

class cDlgCassaAction : public QDialog, protected Ui::dlgCassaAction
{
    Q_OBJECT

public:
    cDlgCassaAction( QWidget *p_poParent = 0 );
    virtual ~cDlgCassaAction();

    void setInitialMoney( int p_nMoney );
    void setPayWithCash();
    void setPayWithVoucher();
    void setPayWithCreditcard();
    void setCassaAction();
    QString cassaResult( int *p_nPayType, QString *p_qsComment );

private:

private slots:
    void on_ledMoney_textEdited(QString text);
    void on_pbCancel_clicked();
    void on_pbOk_clicked();
};

#endif
