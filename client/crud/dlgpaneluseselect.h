#ifndef DLGPANELUSESELECT_H
#define DLGPANELUSESELECT_H

#include "dlgpanelsettings.h"

class cDlgPanelUseSelect : public cDlgPanelSettings
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgPanelUseSelect( QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0, int p_inPanelUseTime = 0 );
    ~cDlgPanelUseSelect();

    int getPanelUsePrice();

private:

    int m_inPanelUseTime;
    int m_inPanelUsePrice;

protected:
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void on_pbSelect_clicked();

};

#endif // DLGPANELUSESELECT_H
