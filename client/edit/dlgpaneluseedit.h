#ifndef DLGPANELUSEEDIT_H
#define DLGPANELUSEEDIT_H

#include "belenus.h"
#include "ui_dlgpaneluseedit.h"
#include "../db/dbpaneluses.h"

class cDlgPanelUseEdit : public QDialog, private Ui::dlgPanelUseEdit
{
    Q_OBJECT

public:
    cDlgPanelUseEdit( QWidget *p_poParent = 0, cDBPanelUses *p_poPanelUses = NULL, unsigned int p_inPanelId = 0 );
    virtual ~cDlgPanelUseEdit();

protected:
    cDBPanelUses   *m_poPanelUses;
    int             m_inPanelId;

private slots:
    void on_pbSave_clicked();
    void on_pbCancel_clicked();
};

#endif // DLGPANELTYPEEDIT_H
