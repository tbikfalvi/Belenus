#ifndef DLGDEMO_H
#define DLGDEMO_H

#include <QDialog>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>

#include "dlgpreview.h"

class cDlgDemo : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgDemo( QWidget *parent = 0 );
    ~cDlgDemo();

protected:
    QLabel*        lblDate;
    QDateTimeEdit* dteStartDate;
    QLabel*        lblTo;
    QDateTimeEdit* dteEndDate;
    QLabel*        lblEventType;
    QComboBox*     cmbEventType;

protected slots:
    virtual void refreshReport();
};

#endif // DLGDEMO_H
