#ifndef CREPORT__TEMPLATE_FILENAME___H
#define CREPORT__TEMPLATE_FILENAME___H

#include <QWidget>

#include "creport.h"

class cReport__Template_ClassName__ : public cReport
{
    Q_OBJECT
public:
    explicit cReport__Template_ClassName__(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();
};

#endif // CREPORT__TEMPLATE_FILENAME___H
