#ifndef DLGSENDMAIL_H
#define DLGSENDMAIL_H

#include "../belenus.h"
#include "ui_dlgsendmail.h"

class dlgSendMail : public QDialog, protected Ui::dlgSendMail
{
    Q_OBJECT

public:
    dlgSendMail( QWidget *p_poParent = 0, QString p_qsTitle = "" );
    virtual ~dlgSendMail();


private slots:
    void slotRefreshWarningColors();
    void on_pbSend_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();
    void on_pbTo_clicked();
};

#endif
