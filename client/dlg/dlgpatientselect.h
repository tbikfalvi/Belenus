#ifndef CDLGPATIENTSELECT_H
#define CDLGPATIENTSELECT_H

#include <QLabel>

#include "belenus.h"
#include "../../framework/dlgcrud.h"

class cDlgPatientSelect : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel      *lblName;
    QLineEdit   *ledName;
    QLabel      *lblUniqueId;
    QLineEdit   *ledUniqueId;

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

};

#endif // CDLGPATIENTSELECT_H
