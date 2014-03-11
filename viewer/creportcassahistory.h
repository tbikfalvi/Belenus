#ifndef CREPORTCASSAHISTORY_H
#define CREPORTCASSAHISTORY_H

#include <QWidget>

#include "creport.h"

class cReportCassaHistory : public cReport
{
    Q_OBJECT
public:
    explicit cReportCassaHistory(QWidget *parent = 0, QString p_qsReportName = "", bool p_bFilterEnable = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTCASSAHISTORY_H
