#ifndef CREPORTPATIENTCARDUNITCHANGE_H
#define CREPORTPATIENTCARDUNITCHANGE_H

#include <QWidget>

#include "creport.h"

class cReportPatientcardUnitChange : public cReport
{
    Q_OBJECT
public:
    explicit cReportPatientcardUnitChange(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();

private:

};

#endif // CREPORTPATIENTCARDUNITCHANGE_H
