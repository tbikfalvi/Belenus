#ifndef DLGDISCOUNT_H
#define DLGDISCOUNT_H

#include <QRadioButton>
#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgDiscount : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgDiscount( QWidget *p_poParent = 0 );
    virtual ~cDlgDiscount();

    QHBoxLayout     *horizontalLayout;
    QRadioButton    *rbFilterAll;
    QRadioButton    *rbFilterGeneral;
    QRadioButton    *rbFilterGuest;
    QRadioButton    *rbFilterProduct;
    QLabel          *lblFilterName;
    QLineEdit       *ledFilterName;
    QSpacerItem     *horizontalSpacer1;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    void slotFilterRadioClicked();
    void slotFilterNameChanged();
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );

};

#endif // DLGDISCOUNT_H
