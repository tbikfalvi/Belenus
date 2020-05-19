#ifndef DLGUSEREDIT_H
#define DLGUSEREDIT_H

#include "belenus.h"
#include "ui_dlguseredit.h"
#include "../db/dbuser.h"

class cDlgUserEdit : public QDialog, protected Ui::dlgUserEdit
{
    Q_OBJECT

public:
    cDlgUserEdit( QWidget *p_poParent = 0, cDBUser *p_poUser = NULL );
    virtual ~cDlgUserEdit();

public slots:
    virtual void accept ();

protected:
    void timerEvent( QTimerEvent *p_poEvent );

protected:
    cDBUser *m_poUser;
    QString  m_qsDefaultPwd;

private:
    int                          m_nTimer;

};

#endif
