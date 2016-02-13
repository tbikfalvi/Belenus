#ifndef DLGSKINTYPEEDIT_H
#define DLGSKINTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgskintypeedit.h"
#include "../db/dbskintypes.h"

class cDlgSkinTypeEdit : public QDialog, private Ui::dlgSkinTypeEdit
{
    Q_OBJECT

public:
    cDlgSkinTypeEdit( QWidget *p_poParent = 0, cDBSkinTypes *p_poSkinTypes = NULL );
    virtual ~cDlgSkinTypeEdit();

protected:
    cDBSkinTypes *m_poSkinTypes;

public slots:
    virtual void accept();
};

#endif // DLGSKINTYPEEDIT_H
