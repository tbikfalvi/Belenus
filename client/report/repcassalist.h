#ifndef REPCASSALIST_H
#define REPCASSALIST_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportCassaList : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportCassaList( QWidget *parent = 0 );
    ~cDlgReportCassaList();

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

#endif // REPCASSALIST_H
