#ifndef CREPORTREPORTGUESTS_H
#define CREPORTREPORTGUESTS_H

#include <QWidget>

#include "creport.h"

class cReportGuests : public cReport
{
    Q_OBJECT
public:
    explicit cReportGuests(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTREPORTGUESTS_H
