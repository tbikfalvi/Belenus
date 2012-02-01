#ifndef DLG_GUESTEDIT_H
#define DLG_GUESTEDIT_H

#include "belenus.h"
#include "ui_dlgguestedit.h"
#include "../db/dbguest.h"
#include "../db/dbpostponed.h"
#include "../db/dbaddress.h"
#include "../db/dbpatientcard.h"

class cDlgGuestEdit : public QDialog, private Ui::dlgGuestEdit
{
    Q_OBJECT

public:
    cDlgGuestEdit( QWidget *p_poParent = 0, cDBGuest *p_poGuest = NULL, cDBPostponed *p_poPostponed = NULL );
    ~cDlgGuestEdit();

public slots:

private:
    bool m_bInit;

    bool SaveGuestData();

protected:
    cDBGuest      *m_poGuest;
    cDBPostponed    *m_poPostponed;
    cDBAddress      *m_poAddress;
    cDBPatientCard  *m_poPatientCard;

private slots:
    void on_pbHistory_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
