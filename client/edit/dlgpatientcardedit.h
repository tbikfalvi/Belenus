#ifndef DLGPATIENTCARDEDIT_H
#define DLGPATIENTCARDEDIT_H

#include "belenus.h"
#include "ui_dlgpatientcardedit.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardEdit : public QDialog, protected Ui::dlgPatientCardEdit
{
    Q_OBJECT

public:
    cDlgPatientCardEdit( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgPatientCardEdit();
    void setPatientCardOwner( const unsigned int p_uiPatientId );

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;
    bool                 m_bNewCard;
    bool                 m_bRefillCard;
    bool                 m_bIsCardActivated;
    bool                 m_bIsCardDeactivated;

    bool                _checkCardJustForSave( QString *p_qsErrorMessage );

private slots:
    void on_pbRefill_clicked();
    void on_pbSell_clicked();
    void on_pbDeactivate_clicked();
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_ledBarcode_lostFocus();
    void on_cmbPatient_currentIndexChanged(int index);
    void on_cmbCardType_currentIndexChanged(int index);
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
