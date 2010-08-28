#ifndef DLGPANELSETTINGS_H
#define DLGPANELSETTINGS_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QComboBox>

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
    QLineEdit       *ledWorkTime;
    QLabel          *lblMaxWorkTime;
    QLineEdit       *ledMaxWorkTime;
    QSpacerItem     *horizontalSpacer2;

    cDlgPanelSettings( QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPanelSettings();

private:

protected:
    unsigned int    m_uiPanelId;

    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void saveClicked( bool );
};

#endif // DLGPANELSETTINGS_H
