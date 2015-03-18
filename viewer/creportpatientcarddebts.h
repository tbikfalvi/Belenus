#ifndef CREPORTPATIENTCARDDEBTS_H
#define CREPORTPATIENTCARDDEBTS_H

#include <QWidget>

#include "creport.h"

class cReportPatientcardDebts : public cReport
{
    Q_OBJECT
public:
    explicit cReportPatientcardDebts(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();

private:
    QString     _patientCardType( unsigned int p_uiPatientCardTypeId );
    int         _unitPrice( unsigned int p_uiPatientCardId, unsigned int p_uiLedgerId );
};

#endif // CREPORTPATIENTCARDDEBTS_H
