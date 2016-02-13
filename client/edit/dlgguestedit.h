#ifndef DLG_GUESTEDIT_H
#define DLG_GUESTEDIT_H

#include "belenus.h"
#include "ui_dlgguestedit.h"
#include "../db/dbguest.h"
#include "../db/dbpostponed.h"
#include "../db/dbaddress.h"
#include "../db/dbpatientcard.h"
#include "../dlg/dlgprogress.h"

class cDlgGuestEdit : public QDialog, private Ui::dlgGuestEdit
{
    Q_OBJECT

public:
    cDlgGuestEdit( QWidget *p_poParent = 0, cDBGuest *p_poGuest = NULL, cDBPostponed *p_poPostponed = NULL );
    ~cDlgGuestEdit();

    unsigned int     guestId();

protected:
    cDBGuest        *m_poGuest;
    cDBPostponed    *m_poPostponed;
    cDBAddress      *m_poAddress;
    cDBPatientCard  *m_poPatientCard;
    cDlgProgress    *m_dlgProgress;

private slots:
    void on_pbEditDiscount_clicked();
    void slotUpdateDiscountSample();
    void on_pbSaveExit_clicked();
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_pbSellCard_clicked();
    void on_pbDislink_clicked();
    void on_pbAssignCard_clicked();
    void on_pbHistory_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();

private:
    bool m_bInit;

    void _fillPatientCardData();
    bool _saveGuestData();
    QString _convertCurrency(const QString &text) const;

};

#endif
