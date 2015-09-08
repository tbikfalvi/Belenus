#ifndef CREPORTPATIENTCARDSELL_H
#define CREPORTPATIENTCARDSELL_H

#include <QWidget>

#include "creport.h"

class cReportPatientcardSell : public cReport
{
    Q_OBJECT
public:
    explicit cReportPatientcardSell(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();

private:
    int             _reportPartPC( int p_PCLedgerType );
};

#endif // CREPORTPATIENTCARDSELL_H
