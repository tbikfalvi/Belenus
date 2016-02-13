#ifndef CREPORTPRODUCTHISTORY_H
#define CREPORTPRODUCTHISTORY_H

#include <QWidget>

#include "creport.h"

class cReportProductHistory : public cReport
{
    Q_OBJECT
public:
    explicit cReportProductHistory(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTPRODUCTHISTORY_H
