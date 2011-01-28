#ifndef REPCARDUSES_H
#define REPCARDUSES_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportCardUses : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportCardUses( QWidget *parent = 0 );
    ~cDlgReportCardUses();

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
    QLabel          *lblPatientCard;
    QComboBox       *cmbPatientCard;
    QLabel          *lblPatient;
    QComboBox       *cmbPatient;

protected slots:
    virtual void refreshReport();
};

#endif // REPCARDUSES_H
