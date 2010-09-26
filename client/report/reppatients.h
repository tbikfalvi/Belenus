#ifndef REPPATIENTS_H
#define REPPATIENTS_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "belenus.h"
#include "dlgpreview.h"

class cDlgReportPatients : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgReportPatients( QWidget *parent = 0 );
    ~cDlgReportPatients();

private:
    QSqlQuery       *poReportResult;
    QString          qsQuery;
    unsigned int     uiColumnCount;

protected:


protected slots:
    virtual void refreshReport();
};

#endif // REPPATIENTS_H
