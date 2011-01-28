#ifndef REPPATIENTCARDOBS_H
#define REPPATIENTCARDOBS_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportPatientCardObs : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportPatientCardObs( QWidget *parent = 0 );
    ~cDlgReportPatientCardObs();

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

#endif // REPPATIENTCARDOBS_H
