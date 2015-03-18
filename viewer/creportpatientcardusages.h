#ifndef CREPORTPCUSAGES_H
#define CREPORTPCUSAGES_H

#include <QWidget>

#include "creport.h"

class cReportPCUsages : public cReport
{
    Q_OBJECT
public:
    explicit cReportPCUsages(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    virtual void refreshReport();

private:

};

#endif // CREPORTPCUSAGES_H
