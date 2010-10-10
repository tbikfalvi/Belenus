#ifndef DLGCOMPANYEDIT_H
#define DLGCOMPANYEDIT_H

#include "belenus.h"
#include "ui_dlgcompanyedit.h"
#include "db/dbcompany.h"

class cDlgCompanyEdit : public QDialog, protected Ui::dlgCompanyEdit
{
    Q_OBJECT

public:
    cDlgCompanyEdit( QWidget *p_poParent = 0, cDBCompany *p_poCompany = NULL );
    virtual ~cDlgCompanyEdit();

public slots:
    virtual void accept ();

private:
    void checkRegionZipCity();

protected:
    cDBCompany *m_poCompany;

private slots:
    void on_pbCitySearch_clicked();
    void on_ledZip_textEdited(QString );
};

#endif
