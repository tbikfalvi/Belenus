#ifndef CDLGPOSTPONEDPATIENTSELECT_H
#define CDLGPOSTPONEDPATIENTSELECT_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

#include "belenus.h"
#include "../framework/dlgcrud.h"

class cDlgPostponedPatientSelect : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayoutTop;
    QLabel      *lblName;
    QLineEdit   *ledName;
    QSpacerItem *horizontalSpacerTop;
    QLabel      *lblUniqueId;
    QLineEdit   *ledUniqueId;
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgPostponedPatientSelect( QWidget *p_poParent = 0 );
    ~cDlgPostponedPatientSelect();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbSelect_clicked();
};

#endif // CDLGPOSTPONEDPATIENTSELECT_H
