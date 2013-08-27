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
    virtual void refreshReport();

private:
    void            _reportPartCassaList();
    unsigned int    _reportPartProductSell();
    unsigned int    _reportPartPatientCardSell();
    unsigned int    _reportPartPanelUse();
    void            _reportPartPanelUseUnits();
    void            _reportPartPaymentMethods();
    int             _sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    QString         _countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    int             _sumPanelUse( unsigned int p_uiPanelTypeId );
    int             _countPanelUseUnits( unsigned int p_uiPanelTypeId );
    int             _sumPaymentMethod( unsigned int p_uiPaymentMethodId );
    int             _sumCassaIncome( unsigned int p_uiCassaId );
};

#endif // CREPORTLEDGER_H
