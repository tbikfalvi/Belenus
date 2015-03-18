#ifndef CREPORTPRODUCTSTATUS_H
#define CREPORTPRODUCTSTATUS_H

#include <QWidget>

#include "creport.h"

class cReportProductStatus : public cReport
{
    Q_OBJECT
public:
    explicit cReportProductStatus(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false );
    
signals:
    
public slots:
    
protected slots:
    virtual void refreshReport();

private:
    void        _reportPartProductTypeGroup( QString p_qsSubTitle, unsigned int p_uiProductTypeId );
    int         _countProductToDate( unsigned int p_uiProductId, const QDate p_qdFilterDate );
};

#endif // CREPORTPRODUCTSTATUS_H
