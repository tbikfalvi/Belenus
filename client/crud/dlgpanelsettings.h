#ifndef DLGPANELSETTINGS_H
#define DLGPANELSETTINGS_H

#include "../framework/dlgcrud.h"

class cDlgPanelSettings : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;

    cDlgPanelSettings( QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPanelSettings();

private:
    unsigned int    m_uiPanelId;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPANELSETTINGS_H
