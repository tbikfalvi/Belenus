#ifndef CREPORTCARDINACTIVE_H
#define CREPORTCARDINACTIVE_H

#include <QWidget>

#include "creport.h"

class cReportCardInactive : public cReport
{
    Q_OBJECT
public:
    explicit cReportCardInactive(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTCARDINACTIVE_H
