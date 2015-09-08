#ifndef CREPORTCARDDETAILS_H
#define CREPORTCARDDETAILS_H

#include <QWidget>

#include "creport.h"

class cReportCardDetails : public cReport
{
    Q_OBJECT
public:
    explicit cReportCardDetails(QWidget *parent = 0, QString p_qsReportName = "", bool p_bIsAdmin = false);

signals:

public slots:

protected slots:
    virtual void refreshReport();
};

#endif // CREPORTCARDDETAILS_H
