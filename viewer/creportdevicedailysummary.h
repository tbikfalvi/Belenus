#ifndef CREPORTDEVICEDAILYSUMMARY_H
#define CREPORTDEVICEDAILYSUMMARY_H

#include <QWidget>

#include "creport.h"

class cReportDeviceDailySummary : public cReport
{
    Q_OBJECT
public:
    explicit cReportDeviceDailySummary(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false);
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTDEVICEDAILYSUMMARY_H
