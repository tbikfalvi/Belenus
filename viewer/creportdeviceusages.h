#ifndef CREPORTDEVICEUSAGES_H
#define CREPORTDEVICEUSAGES_H

#include <QWidget>

#include "creport.h"

class cReportDeviceUsages : public cReport
{
    Q_OBJECT
public:
    explicit cReportDeviceUsages(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false);
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTDEVICEUSAGES_H
