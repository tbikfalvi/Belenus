#ifndef CREPORTPATIENTCARDTYPE_H
#define CREPORTPATIENTCARDTYPE_H

#include <QWidget>

#include "creport.h"

class cReportPatientCardType : public cReport
{
    Q_OBJECT
public:
    explicit cReportPatientCardType(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORTPATIENTCARDTYPE_H
