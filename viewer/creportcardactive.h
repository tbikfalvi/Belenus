#ifndef CREPORTCARDACTIVE_H
#define CREPORTCARDACTIVE_H

#include <QWidget>

#include "creport.h"

class cReportCardActive : public cReport
{
    Q_OBJECT
public:
    explicit cReportCardActive(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTCARDACTIVE_H
