#ifndef DLGPANELSETTINGS_H
#define DLGPANELSETTINGS_H

#include "../belenus.h"
#include "../ui_dlgpanelsettings.h"

class cDlgPanelSettings : public QDialog, protected Ui::dlgPanelSettings
{
    Q_OBJECT

public:
    cDlgPanelSettings( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelSettings();

private slots:

};

#endif // DLGPANELSETTINGS_H
