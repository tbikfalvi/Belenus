#ifndef DLGCHANGEPCVALIDITY_H
#define DLGCHANGEPCVALIDITY_H

#include <QDate>

#include "../belenus.h"
#include "ui_dlgchangepcvalidity.h"

class cDlgChangePCValidity : public QDialog, protected Ui::dlgChangePCValidity
{
    Q_OBJECT

public:
    enum selValidity
    {
        SV_NONE = 0,
        SV_ONEDAY,
        SV_ONEWEEK,
        SV_TWOWEEKS,
        SV_ONEMONTH,
        SV_THREEMONTHS,
        SV_HALFYEAR,
        SV_CUSTOM
    };

    cDlgChangePCValidity( QWidget *p_poParent = 0 );
    virtual ~cDlgChangePCValidity() {}
    selValidity selectionValidity( QDate *p_qdDate );

private:

    selValidity m_selValidity;

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked()  { QDialog::reject(); }
    void selectionChanged();
};

#endif
