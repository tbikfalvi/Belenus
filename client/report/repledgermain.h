#ifndef REPLEDGERMAIN_H
#define REPLEDGERMAIN_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "dlgpreview.h"

class cDlgLedgerMain : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgLedgerMain( QWidget *parent = 0 );
    ~cDlgLedgerMain();

protected:
    QLabel*        lblDate;
    QDateTimeEdit* dteStartDate;
    QLabel*        lblTo;
    QDateTimeEdit* dteEndDate;

protected slots:
    virtual void refreshReport();
};

#endif // REPLEDGERMAIN_H
