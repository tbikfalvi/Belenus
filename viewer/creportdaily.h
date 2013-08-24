#ifndef CREPORTDAILY_H
#define CREPORTDAILY_H

#include <QWidget>

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
    unsigned int    _reportPartProductSellMorning();
    unsigned int    _reportPartProductSellAfternoon();
    unsigned int    _reportPartPatientCardSell();
    unsigned int    _reportPartPanelUse();
    void            _reportPartPaymentMethods();
    QString         _countPatientCardTypeSellMorning( unsigned int p_uiPatientCardTypeId );
    QString         _countPatientCardTypeSellAfternoon( unsigned int p_uiPatientCardTypeId );
    int             _sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    int             _sumUserIncome( unsigned int p_uiUserId, QString p_qsStart, QString p_qsStop );
    int             _sumPanelUseMorning( unsigned int p_uiPanelTypeId );
    int             _sumPanelUseAfternoon( unsigned int p_uiPanelTypeId );
};

#endif // CREPORTDAILY_H
