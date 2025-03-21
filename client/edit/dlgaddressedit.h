#ifndef DLGADDRESSEDIT_H
#define DLGADDRESSEDIT_H

#include "belenus.h"
#include "ui_dlgaddressedit.h"
#include "../db/dbaddress.h"

class cDlgAddressEdit : public QDialog, protected Ui::dlgAddressEdit
{
    Q_OBJECT

public:
    cDlgAddressEdit( QWidget *p_poParent = 0, cDBAddress *p_poAddress = NULL );
    virtual ~cDlgAddressEdit();

protected:
    cDBAddress *m_poAddress;

private:
    void checkRegionZipCity();

public slots:
    virtual void accept ();

private slots:
    void on_ledZip_textEdited(QString );
    void on_pbCitySearch_clicked();
};

#endif
