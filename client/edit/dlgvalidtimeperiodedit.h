#ifndef DLGVALIDTIMEPERIODEDIT_H
#define DLGVALIDTIMEPERIODEDIT_H

#include "belenus.h"
#include "ui_dlgvalidtimeperiodedit.h"
#include "db/dbvalidtimeperiods.h"

class cDlgValidTimePeriodEdit : public QDialog, protected Ui::dlgValidTimePeriodEdit
{
    Q_OBJECT

public:
    cDlgValidTimePeriodEdit( QWidget *p_poParent = 0, const QString &p_qsValidTimePeriod = "" );
    virtual ~cDlgValidTimePeriodEdit();

    QString     validPeriod() const;

public slots:
    virtual void accept ();

protected:
    QString     m_qsValidTimePeriod;
    int         m_nValidWeekDays;
};

#endif
