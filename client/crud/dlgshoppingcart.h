#ifndef DLGSHOPPINGCART_H
#define DLGSHOPPINGCART_H

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgShoppingCart : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel      *lblPanel;
    QComboBox   *cmbPanel;
    QLabel      *lblGuest;
    QComboBox   *cmbGuest;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbPayment;

    cDlgShoppingCart( QWidget *p_poParent = 0 );
    virtual ~cDlgShoppingCart();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbPayment_clicked();
};

#endif // DLGSHOPPINGCART_H
