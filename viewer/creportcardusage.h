#ifndef CREPORTCARDUSAGE_H
#define CREPORTCARDUSAGE_H

#include <QWidget>

#include "creport.h"

class cReportCardUsage : public cReport
{
    Q_OBJECT
public:
    explicit cReportCardUsage(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
};

#endif // CREPORTCARDUSAGE_H
