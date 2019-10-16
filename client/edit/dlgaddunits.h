#ifndef DLGADDUNITS_H
#define DLGADDUNITS_H

#include "belenus.h"
#include "ui_dlgaddunits.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgAddUnits : public QDialog, protected Ui::dlgAddUnits
{
    Q_OBJECT

public:
    cDlgAddUnits( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgAddUnits();

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;

private slots:
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_pbAdd_clicked();
    void on_pbCancel_clicked();
};

#endif
