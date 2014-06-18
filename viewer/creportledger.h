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
    void            _reportPartPanelUseType( tePanelUse p_tePanelUse );
//    void            _reportPartPanelUseUnits();
    int             _reportPartPaymentMethods();
    unsigned int    _reportPartExpenses();
    void            _reportPartIncomeSummary( unsigned int p_uiProductSellTotal,
                                              unsigned int p_uiPatientCardTotal,
                                              unsigned int p_uiDeviceUsagesTotal,
                                              int p_nExpenses,
                                              int p_nIncomeCard );
    int             _sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    QString         _countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    int             _sumPanelUse( unsigned int p_uiPanelTypeId );
//    int             _countPanelUseUnits( unsigned int p_uiPanelTypeId );
    int             _countPanelUse( unsigned int p_uiPanelTypeId, tePanelUse p_tePanelUse );
    int             _sumPaymentMethod( unsigned int p_uiPaymentMethodId );
    int             _sumCassaIncome( unsigned int p_uiCassaId );
    QString         _countsumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId, unsigned int *p_uiPricePCSell );
    int             _sumPanelUse( QString p_qsCassaId, unsigned int p_uiPanelTypeId );

    QStringList      m_qslCassaIds;
    QStringList      m_qslCassaOwners;
};

#endif // CREPORTLEDGER_H
