#ifndef DLGLICENCEEDIT_H
#define DLGLICENCEEDIT_H

#include "belenus.h"
#include "ui_dlglicenceedit.h"

class dlgLicenceEdit : public QDialog, protected Ui::dlgLicenceEdit
{
    Q_OBJECT

public:
    dlgLicenceEdit( QWidget *p_poParent = 0 );
    virtual ~dlgLicenceEdit();

protected:

private:
    unsigned int    m_uiId;

    void checkRegionZipCity();

public slots:
    virtual void accept ();

private slots:
    void on_ledZip_textEdited(QString );
    void on_pbCitySearch_clicked();
};


#endif // DLGLICENCEEDIT_H
