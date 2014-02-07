#ifndef DLGPATIENTCARDTYPE_H
#define DLGPATIENTCARDTYPE_H

#include <QLabel>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgPatientCardType : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatientCardType( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCardType();

    QSpacerItem *horizontalSpacer1;
    QLabel      *lblFilterName;
    QLineEdit   *ledFilterName;
    QLabel      *lblFilterUnits;
    QLineEdit   *ledFilterUnits;
//    QPushButton *pbPatientCard;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

private:
    QWidget     *m_poParent;

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
//    virtual void _slotPatientCards();
};

#endif // DLGPATIENTCARDTYPE_H
