#ifndef REPLEDGERMAIN_H
#define REPLEDGERMAIN_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCheckBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgRepLedgerMain : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgRepLedgerMain( QWidget *parent = 0 );
    ~cDlgRepLedgerMain();

private:
    QSqlQuery       *poReportResult;
    QString          qsQuery;
    unsigned int     uiColumnCount;
    int              inSumUsedWithCard;
    int              inSumUsedWithPayment;
    int              inSumTimeReal;
    int              inSumTimeInterrupted;
    int              inSumPatientCount;
    int              inSumNoCardUse;
    int              inSumNoPaymentUse;
    int              inSumNoCashUse;
    int              inSumNoCreditCardUse;
    int              inSumCash;
    int              inSumNet;
    int              inSumTotal;
    int              inSumCount;
    int              inSumDiscount;
    int              inSumDCount;
    int              inSumSumCount;

    QString convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

protected:
    QLabel          *lblDate;
    QDateTimeEdit   *dteStartDate;
    QLabel          *lblTo;
    QDateTimeEdit   *dteEndDate;
    QCheckBox       *chkShowTimes;

protected slots:
    virtual void refreshReport();
};

#endif // REPLEDGERMAIN_H
