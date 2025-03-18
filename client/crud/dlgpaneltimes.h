#ifndef DLGPANELTIMES_H
#define DLGPANELTIMES_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QComboBox>
#include <QPushButton>

#include "../framework/dlgcrud.h"

class cDlgPanelTimes : public cDlgCrud
{
    Q_OBJECT

public:

    cDlgPanelTimes( QWidget *p_poParent = 0 );
    virtual         ~cDlgPanelTimes();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
};

#endif // DLGPANELTIMES_H
