#ifndef DLGPATIENTCARDEDIT_H
#define DLGPATIENTCARDEDIT_H

#include "belenus.h"
#include "ui_dlgpatientcardedit.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgprogress.h"

class cDlgPatientCardEdit : public QDialog, protected Ui::dlgPatientCardEdit
{
    Q_OBJECT

public:
    cDlgPatientCardEdit( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgPatientCardEdit();
//    void setPatientCardOwner( const unsigned int p_uiPatientId );

protected:
//    cDBPatientCardType  *m_poPatientCardType;
    void timerEvent( QTimerEvent *p_poEvent );

private slots:
    void slotRefreshWarningColors();
    void on_ledBarcode_textChanged(const QString &arg1);
    void on_pbCheckBarcode_clicked();
    void on_pbChangeValidity_clicked();
    void on_pbSave_clicked();
    void on_pbDeactivate_clicked();
    void on_pbSell_clicked();
    void on_pbRefill_clicked();
    void on_pbCancel_clicked()              { QDialog::reject(); }
//    void slotEnableButtons();
//    void on_ledBarcode_lostFocus();
//    void on_cmbPatient_currentIndexChanged(int index);
//    void on_cmbCardType_currentIndexChanged(int index);
    void on_pbRFID_clicked();
    void on_chkIsRFID_clicked();
    void on_pbAddUnits_clicked();
    void on_pbRemoveUnits_clicked();

private:
    cDlgProgress                *m_dlgProgress;
    int                          m_nTimer;
    int                          m_nTimerCounter;
//    bool                 m_bDlgLoaded;
//    bool                 m_bNewCard;
//    bool                 m_bRefillCard;
//    bool                 m_bIsCardActivated;
    cDBPatientCard      *m_poPatientCard;
    bool                 m_bIsCardDeactivated;
//    bool                 m_bIsValidationChanged;
    QString              m_qsUnitCondition;

    bool                _checkCardJustForSave( QString *p_qsErrorMessage );

};

#endif
