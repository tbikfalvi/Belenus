#ifndef DLGPATIENTCARDREFILL_H
#define DLGPATIENTCARDREFILL_H

#include "belenus.h"
#include "ui_dlgpatientcardrefill.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardRefill : public QDialog, protected Ui::dlgPatientCardRefill
{
    Q_OBJECT

public:
    cDlgPatientCardRefill( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgPatientCardRefill();
    void setPatientCardOwner( const unsigned int p_uiPatientId );

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;

private slots:
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_cmbCardType_currentIndexChanged(int index);
    void on_cmbPatient_currentIndexChanged(int index);
    void on_pbSell_clicked();
    void on_pbCancel_clicked();
    void on_pbSelectPatient_clicked();
};

#endif
