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
    QHBoxLayout *horizontalLayoutTop;
    QLabel      *lblName;
    QLineEdit   *ledName;
    QLabel      *lblEmail;
    QLineEdit   *ledEmail;
    QSpacerItem *horizontalSpacerTop;
    QLabel      *lblUniqueId;
    QLineEdit   *ledUniqueId;
    QPushButton *pbSelect;
    QPushButton *pbCancel;
    QLabel          *lblMessage;

    cDlgPatientSelect( QWidget *p_poParent = 0, QString p_qsNameFilter ="" );
    ~cDlgPatientSelect();
    void setSearchPatientName( QString p_stName );
    unsigned int selectedPatientId() { return m_uiSelectedId; }

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

#endif // CDLGPATIENTSELECT_H
