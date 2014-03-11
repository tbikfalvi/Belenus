#ifndef DLGPANELSTATUSESEDIT_H
#define DLGPANELSTATUSESEDIT_H

#include "belenus.h"
#include "ui_dlgpanelstatusesedit.h"
#include "../db/dbpanelstatuses.h"

class cDlgPanelStatusesEdit : public QDialog, protected Ui::dlgPanelStatusesEdit
{
    Q_OBJECT

public:
    cDlgPanelStatusesEdit( QWidget *p_poParent = 0, cDBPanelStatuses *p_poPanelStatuses = NULL );
    virtual ~cDlgPanelStatusesEdit();

public slots:

protected:
    cDBPanelStatuses *m_poPanelStatuses;

private slots:
    void on_pbCancel_clicked();
    void on_pbOk_clicked();
    void on_chkAllowToSkip_clicked(bool checked);
};

#endif
