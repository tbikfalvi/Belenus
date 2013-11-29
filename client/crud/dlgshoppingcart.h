#ifndef DLGSHOPPINGCART_H
#define DLGSHOPPINGCART_H

#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"
#include "../db/dbshoppingcart.h"
#include "../dlg/dlgcassaaction.h"

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
    QPushButton *pbPatientCard;
    QPushButton *pbProduct;

    cDlgShoppingCart( QWidget *p_poParent = 0 );
    virtual ~cDlgShoppingCart();
    void setPanelFilter( unsigned int p_uiPanelId );

signals:
    void signalPaymentProcessed( unsigned int p_uiPanelId, unsigned int p_uiLedgerId, int p_nPaymentType );
    void signalSellPatientCard();
    void signalSellProduct();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbPayment_clicked();
    virtual void on_pbPatientCard_clicked();
    virtual void on_pbProduct_clicked();
};

#endif // DLGSHOPPINGCART_H
