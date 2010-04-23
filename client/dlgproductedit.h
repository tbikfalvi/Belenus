#ifndef DLGPRODUCTEDIT_H
#define DLGPRODUCTEDIT_H

#include "belenus.h"
#include "ui_dlgproductedit.h"
#include "dbproduct.h"

class cDlgProductEdit : public QDialog, protected Ui::dlgProductEdit
{
    Q_OBJECT

public:
    cDlgProductEdit( QWidget *p_poParent = 0, cDBProduct *p_poProduct = NULL );
    virtual ~cDlgProductEdit();

public slots:
    virtual void accept ();

protected:
    cDBProduct *m_poProduct;
};

#endif // DLGPRODUCTEDIT_H
