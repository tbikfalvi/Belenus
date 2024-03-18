#ifndef CREPORTDEVICEPATIENTDISPERSION_H
#define CREPORTDEVICEPATIENTDISPERSION_H

#include <QWidget>

#include "creport.h"

class cReportDevicePatientDispersion : public cReport
{
    Q_OBJECT
public:
    explicit cReportDevicePatientDispersion(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false);
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTDEVICEPATIENTDISPERSION_H
