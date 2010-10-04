#ifndef REPPATIENTCARD_H
#define REPPATIENTCARD_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportPatientCard : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportPatientCard( QWidget *parent = 0 );
    ~cDlgReportPatientCard();

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

#endif // REPPATIENTCARD_H
