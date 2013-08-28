#ifndef CREPORTPRODUCTS_H
#define CREPORTPRODUCTS_H

#include <QWidget>

#include "creport.h"

class cReportProducts : public cReport
{
    Q_OBJECT
public:
    explicit cReportProducts(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTPRODUCTS_H
