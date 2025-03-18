#ifndef DLGPANELTIMECOPY_H
#define DLGPANELTIMECOPY_H

#include "../belenus.h"
#include "ui_dlgpaneltypecopy.h"
#include "dlgprogress.h"

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

private:
    cDlgProgress        *m_dlgProgress;
};

#endif
