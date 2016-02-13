#ifndef DLGADVERTISEMENTEDIT_H
#define DLGADVERTISEMENTEDIT_H

#include "belenus.h"
#include "ui_dlgAdvertisementedit.h"
#include "db/dbadvertisements.h"

class cDlgAdvertisementEdit : public QDialog, protected Ui::dlgAdvertisementEdit
{
    Q_OBJECT

public:
    cDlgAdvertisementEdit( QWidget *p_poParent = 0, cDBAdvertisements *p_poAdvertisement = NULL, bool p_bCanBeInactivate = true );
    virtual ~cDlgAdvertisementEdit();

public slots:
    virtual void accept ();

protected:
    cDBAdvertisements *m_poAdvertisement;
private slots:
    void slotRefreshWarningColors();
    void on_btnMainBackground_clicked();
    void on_pbChangePath_clicked();
    void on_pbAddImage_clicked();
    void on_pbRemoveImage_clicked();
    void on_pbMoveUp_clicked();
    void on_pbMoveDown_clicked();
    void on_listImages_itemSelectionChanged();
};

#endif
