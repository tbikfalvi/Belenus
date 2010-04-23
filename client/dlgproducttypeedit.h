#ifndef DLGPRODUCTTYPEEDIT_H
#define DLGPRODUCTTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgproducttypeedit.h"
#include "dbproducttype.h"

class cDlgProductTypeEdit : public QDialog, protected Ui::dlgProductTypeEdit
{
    Q_OBJECT

public:
    cDlgProductTypeEdit( QWidget *p_poParent = 0, cDBProductType *p_poProductType = NULL );
    virtual ~cDlgProductTypeEdit();

public slots:
    virtual void accept ();

protected:
    cDBProductType *m_poProductType;
};

#endif // DLGPRODUCTTYPEEDIT_H
