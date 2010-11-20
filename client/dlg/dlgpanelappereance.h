#ifndef DLG_PANELAPPEREANCE_H
#define DLG_PANELAPPEREANCE_H

#include "../belenus.h"
#include "../ui_dlgpanelappereance.h"

class cDlgPanelAppereance : public QDialog, protected Ui::dlgPanelAppereance
{
    Q_OBJECT

public:
    cDlgPanelAppereance( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelAppereance();

private slots:


private slots:
    void on_pbBackgroundColor_clicked();
};

#endif // DLG_PANELAPPEREANCE_H
