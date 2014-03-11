#ifndef DLGCASSAINOUT_H
#define DLGCASSAINOUT_H

#include "../belenus.h"
#include "ui_dlgcassainout.h"

class cDlgCassaInOut : public QDialog, protected Ui::dlgCassaInOut
{
    Q_OBJECT

public:
    cDlgCassaInOut( QWidget *p_poParent = 0, QString p_qsTitle = "" );
    virtual ~cDlgCassaInOut();

    int         resultAmount();
    QString     resultComment();

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
};

#endif
