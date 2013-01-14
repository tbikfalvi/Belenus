#ifndef CREPORTDAILY_H
#define CREPORTDAILY_H

#include <QWidget>

#include "creport.h"

class cReportDaily : public cReport
{
    Q_OBJECT
public:
    explicit cReportDaily(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
};

#endif // CREPORTDAILY_H
