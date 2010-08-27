#ifndef DLGADDRESS_H
#define DLGADDRESS_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "../framework/dlgcrud.h"

class cDlgAddress : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayout;
    QLabel *lblPatient;
    QComboBox *cmbPatient;
    QSpacerItem *horizontalSpacer1;
    QSpacerItem *horizontalSpacer2;

    cDlgAddress( QWidget *p_poParent = 0 );
    virtual ~cDlgAddress();

    void setPatientId( const unsigned int p_uiPatientId );

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGADDRESS_H
