#ifndef REPPATIENTS_H
#define REPPATIENTS_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QRadioButton>

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
    QVBoxLayout     *verticalLayout1;
    QHBoxLayout     *horizontalLayout1;
    QHBoxLayout     *horizontalLayout2;
    QSpacerItem     *horizontalSpacer1;
    QSpacerItem     *horizontalSpacer2;
    QLabel          *lblDate;
    QDateTimeEdit   *dteStartDate;
    QLabel          *lblTo;
    QDateTimeEdit   *dteEndDate;
    QLabel          *lblAge;
    QComboBox       *cmbAge;
    QLabel          *lblGender;
    QRadioButton    *rbBoth;
    QRadioButton    *rbMale;
    QRadioButton    *rbFemale;

protected slots:
    virtual void refreshReport();
};

#endif // REPPATIENTS_H
