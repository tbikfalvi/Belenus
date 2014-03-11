#ifndef DLGPATIENTCARDSELECT_H
#define DLGPATIENTCARDSELECT_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "dlgpatientcard.h"

class cDlgPatientCardSelect : public cDlgPatientCard
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgPatientCardSelect( QWidget *p_poParent = 0, unsigned int p_uiPatientId = 0 );
    virtual ~cDlgPatientCardSelect();

    unsigned int selected();

private:
    QString         m_qsCity;
    QString         m_qsCondPatient;

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

#endif // DLGPATIENTCARDSELECT_H
