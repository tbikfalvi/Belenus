#ifndef DLGZIPREGIONCITY_H
#define DLGZIPREGIONCITY_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgZipRegionCity : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel *lblRegion;
    QComboBox *cmbRegion;
    QSpacerItem *horizontalSpacer1;
    QSpacerItem *horizontalSpacer2;

    cDlgZipRegionCity( QWidget *p_poParent = 0 );
    virtual ~cDlgZipRegionCity();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGZIPREGIONCITY_H
