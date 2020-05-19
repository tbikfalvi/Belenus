#ifndef CREPORTDEVICEMINUTEUSAGES_H
#define CREPORTDEVICEMINUTEUSAGES_H

#include <QWidget>

#include "creport.h"

class cReportDeviceMinuteUsages : public cReport
{
    Q_OBJECT
public:
    explicit cReportDeviceMinuteUsages(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false);
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTDEVICEMINUTEUSAGES_H
