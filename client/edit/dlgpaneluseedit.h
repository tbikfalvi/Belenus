#ifndef DLGPANELUSEEDIT_H
#define DLGPANELUSEEDIT_H

#include "belenus.h"
#include "ui_dlgpaneluseedit.h"
#include "../db/dbpaneluses.h"

class cDlgPanelUseEdit : public QDialog, private Ui::dlgPanelUseEdit
{
    Q_OBJECT

public:
    cDlgPanelUseEdit( QWidget *p_poParent = 0, cDBPanelUses *p_poPanelUses = NULL );
    virtual ~cDlgPanelUseEdit();

protected:
    cDBPanelUses   *m_poPanelUses;

private slots:
    void on_pbSave_clicked();
    void on_pbCancel_clicked();
    void on_ledUsePrice_textEdited(const QString &arg1);
};

#endif // DLGPANELTYPEEDIT_H
