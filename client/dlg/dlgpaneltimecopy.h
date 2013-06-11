#ifndef DLGSERIALREG_H
#define DLGSERIALREG_H

#include "../belenus.h"
#include "../../Build/belenus/ui_dlgpaneltypecopy.h"

class cDlgPanelTypeCopy : public QDialog, protected Ui::dlgPanelTypeCopy
{
    Q_OBJECT

public:
    cDlgPanelTypeCopy( QWidget *p_poParent = 0, unsigned int uiPanelId = 0 );
    virtual ~cDlgPanelTypeCopy();

private slots:
    void updateButtonCopy();
    void on_pbExit_clicked();
    void on_pbCopyTimes_clicked();
};

#endif
