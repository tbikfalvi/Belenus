#ifndef REP_SAMPLE__H
#define REP_SAMPLE__H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportSAMPLE : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportSAMPLE( QWidget *parent = 0 );
    ~cDlgReportSAMPLE();

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

protected slots:
    virtual void refreshReport();
};

#endif // REP_SAMPLE__H
