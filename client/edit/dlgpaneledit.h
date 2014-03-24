#ifndef DLGPANELEDIT_H
#define DLGPANELEDIT_H

#include "belenus.h"
#include "ui_dlgpaneledit.h"
#include "../db/dbpanels.h"

class cDlgPanelEdit : public QDialog, private Ui::dlgPanelEdit
{
    Q_OBJECT

public:
    cDlgPanelEdit( QWidget *p_poParent = 0, cDBPanel *p_poPanels = NULL );
    virtual ~cDlgPanelEdit();

protected:
    cDBPanel *m_poPanels;

public slots:
    virtual void accept();
};

#endif // DLGPANELEDIT_H
