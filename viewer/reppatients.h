#ifndef REPPATIENTS_H
#define REPPATIENTS_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>

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
    QLabel          *lblDate;
    QDateTimeEdit   *dteStartDate;
    QLabel          *lblTo;
    QDateTimeEdit   *dteEndDate;
    QSpacerItem     *horizontalSpacer1;
    QHBoxLayout     *horizontalLayout2;
    QLabel          *lblAge;
    QComboBox       *cmbAge;
    QLabel          *lblGender;
    QRadioButton    *rbBoth;
    QRadioButton    *rbMale;
    QRadioButton    *rbFemale;
    QSpacerItem     *horizontalSpacer2;
    QHBoxLayout     *horizontalLayout3;
    QCheckBox       *chkRegularCustomer;
    QCheckBox       *chkEmployee;
    QCheckBox       *chkService;
    QSpacerItem     *horizontalSpacer3;
    QHBoxLayout     *horizontalLayout4;
    QLabel          *lblHealthInsurance;
    QComboBox       *cmbHealthInsurance;
    QSpacerItem     *horizontalSpacer4;
    QHBoxLayout     *horizontalLayout5;
    QLabel          *lblCompany;
    QComboBox       *cmbCompany;
    QSpacerItem     *horizontalSpacer5;
    QHBoxLayout     *horizontalLayout6;
    QLabel          *lblDoctor;
    QComboBox       *cmbDoctor;
    QSpacerItem     *horizontalSpacer6;

protected slots:
    virtual void refreshReport();
};

#endif // REPPATIENTS_H
