#ifndef REPLEDGERMAIN_H
#define REPLEDGERMAIN_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCheckBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgLedgerMain : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgLedgerMain( QWidget *parent = 0 );
    ~cDlgLedgerMain();

private:
    QSqlQuery       *poReportResult;
    QString          qsQuery;
    unsigned int     uiColumnCount;

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
