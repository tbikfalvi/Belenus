#ifndef DLGADDRESSEDIT_H
#define DLGADDRESSEDIT_H

#include "belenus.h"
#include "ui_dlgaddressedit.h"
#include "../db/dbaddress.h"

class cDlgAddressEdit : public QDialog, protected Ui::dlgAddressEdit
{
    Q_OBJECT

public:
    cDlgAddressEdit( QWidget *p_poParent = 0, cDBAddress *p_poAddress = NULL );
    virtual ~cDlgAddressEdit();

public slots:
    virtual void accept ();

protected:
    cDBAddress *m_poAddress;
};

#endif
