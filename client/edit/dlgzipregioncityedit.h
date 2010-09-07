#ifndef DLGZIPREGIONCITYEDIT_H
#define DLGZIPREGIONCITYEDIT_H

#include "belenus.h"
#include "ui_dlgzipregioncityedit.h"
#include "db/dbzipregioncity.h"

class cDlgZipRegionCityEdit : public QDialog, protected Ui::dlgZipRegionCityEdit
{
    Q_OBJECT

public:
    cDlgZipRegionCityEdit( QWidget *p_poParent = 0, cDBZipRegionCity *p_poZipRegionCity = NULL );
    virtual ~cDlgZipRegionCityEdit();

public slots:
    virtual void accept ();

protected:
    cDBZipRegionCity *m_poZipRegionCity;

private slots:
    void on_cmbRegion_editTextChanged(QString );
};

#endif
