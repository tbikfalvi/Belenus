#ifndef DLGPWDCONFIRM_H
#define DLGPWDCONFIRM_H

#include "../belenus.h"
#include "ui_dlgpwdconfirm.h"

class cDlgPwdConfirm : public QDialog, protected Ui::dlgPwdConfirm
{
    Q_OBJECT

public:
    cDlgPwdConfirm( QWidget *p_poParent = 0 );
    virtual ~cDlgPwdConfirm();

public slots:
    virtual void accept ();
};

#endif
