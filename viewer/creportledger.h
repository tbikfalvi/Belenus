#ifndef CREPORTLEDGER_H
#define CREPORTLEDGER_H

#include <QWidget>

#include "creport.h"

class cReportLedger : public cReport
{
    Q_OBJECT
public:
    explicit cReportLedger(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
};

#endif // CREPORTLEDGER_H
