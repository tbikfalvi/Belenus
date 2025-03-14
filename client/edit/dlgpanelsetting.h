#ifndef DLGPANELSETTING_H
#define DLGPANELSETTING_H

#include "belenus.h"
#include "ui_dlgpanelsetting.h"
#include "../db/dbpanels.h"

class cDlgPanelSetting : public QDialog, private Ui::dlgPanelSetting
{
    Q_OBJECT

public:
    cDlgPanelSetting(QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPanelSetting();

protected:
    unsigned int     m_uiPanelId;

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
    void on_pbTime_clicked();
    void on_pbChangeDir_clicked();
    void on_pbEmptyImage_clicked();
    void on_pbWTReset_clicked();
    void on_pbCTReset_clicked();
    void on_pbEnableSystemAdmin_clicked();
    void on_pbEnableDeviceAdmin_clicked();
};

#endif // DLGPANELSETTING_H
