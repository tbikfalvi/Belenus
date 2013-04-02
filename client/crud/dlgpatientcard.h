#ifndef DLGPATIENTCARD_H
#define DLGPATIENTCARD_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgPatientCard : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel      *lblPatientCardType;
    QComboBox   *cmbPatientCardType;
    QLabel      *lblBarcode;
    QLineEdit   *ledBarcode;
    QSpacerItem *horizontalSpacer1;
    QPushButton *pbPatientCardType;
    QPushButton *pbPatientCardReplace;

    cDlgPatientCard( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCard();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

private:
    QWidget     *m_poParent;
    QString      m_qsCondition;

    bool         _isPatientCardNotForService();

protected slots:
    virtual void refreshTable( QString p_qsCondition = "");
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void _slotPatientCardTypes();
    virtual void _slotPatientCardReplace();
};

#endif // DLGPATIENTCARD_H
