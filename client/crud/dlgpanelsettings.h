#ifndef DLGPANELSETTINGS_H
#define DLGPANELSETTINGS_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QComboBox>
#include <QPushButton>

#include "../framework/dlgcrud.h"

class cDlgPanelSettings : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout     *horizontalLayout1;
    QLabel          *lblTitle;
    QLineEdit       *ledTitle;
    QSpacerItem     *horizontalSpacer1;
    QLabel          *lblType;
    QComboBox       *cmbPanelType;
    QHBoxLayout     *horizontalLayout2;
    QLabel          *lblWorkTime;
    QLineEdit       *ledWorkTimeHour;
    QLineEdit       *ledWorkTimeMin;
    QLineEdit       *ledWorkTimeSec;
    QPushButton     *pbWTReset;
    QLabel          *lblMaxWorkTime;
    QLineEdit       *ledMaxWorkTime;
    QLabel          *lblGroup;
    QComboBox       *cmbPanelGroup;
    QSpacerItem     *horizontalSpacer2;
    QSpacerItem     *horizontalSpacer3;
    QPushButton     *pbCopyToAll;
    QHBoxLayout     *horizontalLayout3;
    QPushButton     *pbEnableSystemAdmin;
    QPushButton     *pbEnableDeviceAdmin;

    cDlgPanelSettings( QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPanelSettings();

private:
    bool             m_bIsSettingChanged;

protected:
    unsigned int     m_uiPanelId;

    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void saveClicked( bool );
    virtual void on_pbWTReset_clicked( bool );
    virtual void on_pbCopyToAll_clicked( bool );
    virtual void on_pbEnableSystemAdmin_clicked();
    virtual void on_pbEnableDeviceAdmin_clicked();
};

#endif // DLGPANELSETTINGS_H
