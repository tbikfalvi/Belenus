#ifndef DLGPANELSETTING_H
#define DLGPANELSETTING_H

#include "belenus.h"
#include "ui_dlgpanelsetting.h"
#include "../db/dbpanels.h"

class cDlgPanelSetting : public QDialog, private Ui::dlgPanelSetting
{
    Q_OBJECT

public:
    cDlgPanelSetting( QWidget *p_poParent = 0, cDBPanel *p_poPanels = NULL );
    virtual ~cDlgPanelSetting();

protected:
    cDBPanel *m_poPanels;

public slots:
    virtual void accept();
};

#endif // DLGPANELSETTING_H
