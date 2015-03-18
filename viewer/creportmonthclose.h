#ifndef CREPORTMONTHCLOSE_H
#define CREPORTMONTHCLOSE_H

#include <QWidget>

#include "creport.h"

class cReportMonthClose : public cReport
{
    Q_OBJECT
public:
    explicit cReportMonthClose(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    virtual void refreshReport();

private:
    void            _reportPartCassaList();
    unsigned int    _reportPartProductSell();
    unsigned int    _reportPartPatientCardSell();
    int             _sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    QString         _countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId );
    QString         _countsumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId, unsigned int *p_uiPricePCSell );

    QStringList      m_qslCassaIds;
};

#endif // CREPORTMONTHCLOSE_H
