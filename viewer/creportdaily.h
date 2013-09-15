#ifndef CREPORTDAILY_H
#define CREPORTDAILY_H

#include <QWidget>
#include <QStringList>

#include "creport.h"

class cReportDaily : public cReport
{
    Q_OBJECT
public:
    explicit cReportDaily(QWidget *parent = 0, QString p_qsReportName = "");
    
signals:
    
public slots:

protected slots:
    virtual void refreshReport();

private:
    void            _reportPartGeneral();
    unsigned int    _reportPartProductSell();
    unsigned int    _reportPartPatientCardSell();
    unsigned int    _reportPartPanelUse();
    void            _reportPartPanelUseUnits();
    void            _reportPartPaymentMethods();
    unsigned int    _reportPartExpenses();
    void            _reportPartStorno();
    void            _reportPartIncomeSummary( unsigned int p_uiTotalPrice,
                                              unsigned int p_uiPatientCardTotal,
                                              unsigned int p_uiDeviceUsagesTotal,
                                              int p_nExpenses );
    QString         _countPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId );
    int             _sumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId );
    int             _sumUserIncome( unsigned int p_uiUserId, QString p_qsStart, QString p_qsStop );
    int             _sumPanelUse( QString p_qsCassaId, unsigned int p_uiPanelTypeId );
    int             _sumPaymentMethod( QString p_qsCassaId, unsigned int p_uiPaymentMethodId );
    int             _sumCassaIncome( unsigned int p_uiCassaId );
    int             _countPanelUseUnits( QString p_qsCassaId, unsigned int p_uiPanelTypeId );

    QStringList      m_qslCassaIds;
    QStringList      m_qslCassaOwners;
};

#endif // CREPORTDAILY_H
