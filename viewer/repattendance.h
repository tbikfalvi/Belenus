#ifndef REPATTENDANCE_H
#define REPATTENDANCE_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportAttendance : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportAttendance( QWidget *parent = 0 );
    ~cDlgReportAttendance();

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
    QLabel          *lblPatient;
    QComboBox       *cmbPatient;

protected slots:
    virtual void refreshReport();
};

#endif // REPATTENDANCE_H
