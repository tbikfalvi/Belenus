#ifndef DLGADDGUEST_H
#define DLGADDGUEST_H

#include "belenus.h"
#include "ui_dlgaddguest.h"
#include "../db/dbguest.h"

class cDlgAddGuest : public QDialog, protected Ui::dlgAddGuest
{
    Q_OBJECT

public:
    cDlgAddGuest(QWidget *p_poParent = 0);
    virtual ~cDlgAddGuest();

    unsigned int     patientId();

protected:
    cDBGuest        *m_poPatient;

private:
    bool             m_bDlgLoaded;

    bool _isEmailFormatOk();
    bool _saveGuestData();

private slots:
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_pbAdd_clicked();
    void on_pbCancel_clicked();
};

#endif
