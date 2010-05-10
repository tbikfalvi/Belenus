#ifndef CDLGPATIENTSELECT_H
#define CDLGPATIENTSELECT_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

#include "belenus.h"
#include "../framework/dlgcrud.h"

class cDlgPatientSelect : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel      *lblName;
    QLineEdit   *ledName;
    QSpacerItem *horizontalSpacer1;
    QLabel      *lblUniqueId;
    QLineEdit   *ledUniqueId;
    QSpacerItem *horizontalSpacer2;
    QVBoxLayout *verticalCustomLayout;
    QPushButton *pbSelect;
    QSpacerItem *spacer3;
    QPushButton *pbCancel;

    cDlgPatientSelect( QWidget *p_poParent = 0 );
    ~cDlgPatientSelect();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbSelect_clicked();
    virtual void on_pbCancel_clicked();

};

#endif // CDLGPATIENTSELECT_H
