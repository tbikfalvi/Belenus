#ifndef DLGGUEST_H
#define DLGGUEST_H

#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgGuest : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgGuest( QWidget *p_poParent = 0 );
    virtual ~cDlgGuest();

    QHBoxLayout *horizontalLayout;
    QLabel      *lblFilterName;
    QLineEdit   *ledFilterName;
    QLabel      *lblFilterGender;
    QComboBox   *cmbFilterGender;
    QLabel      *lblFilterAgeType;
    QComboBox   *cmbFilterAgeType;
    QSpacerItem *horizontalSpacer1;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGGUEST_H
