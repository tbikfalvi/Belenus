#ifndef DLG_PATIENTEDIT_H
#define DLG_PATIENTEDIT_H

#include "belenus.h"
#include "ui_dlgpatientedit.h"
#include "../db/dbpatient.h"
#include "../db/dbpostponed.h"
#include "../db/dbaddress.h"
#include "../db/dbpatientcard.h"

class cDlgPatientEdit : public QDialog, private Ui::dlgPatientEdit
{
    Q_OBJECT

public:
    cDlgPatientEdit( QWidget *p_poParent = 0, cDBPatient *p_poPatient = NULL, cDBPostponed *p_poPostponed = NULL );
    ~cDlgPatientEdit();

public slots:

private:
    bool m_bInit;

    bool SavePatientData();
    void FillHealthInsuranceCombo();
    void FillCompanyCombo();
    void FillDoctorTypeCombo();
    void FillDoctorCombo();
    void FillDefaultAddress();
    void FillPhoneNumber();
    void FillIllnessGroupCombo();
    void checkRegionZipCity();
    void calculateDiscount(int p_inDiscountType);

protected:
    cDBPatient      *m_poPatient;
    cDBPostponed    *m_poPostponed;
    cDBAddress      *m_poAddress;
    cDBPatientCard  *m_poPatientCard;

private slots:
    void on_rbDiscountPercent_clicked();
    void on_rbDiscountValue_clicked();
    void on_pbIllnessGroup_clicked();
    void on_cmbDoctorType_currentIndexChanged(int index);
    void on_pbVerifyAddress_clicked();
    void on_deDateBirth_dateChanged(QDate date);
    void on_cmbDoctor_currentIndexChanged(int index);
    void on_cmbCompany_currentIndexChanged(int index);
    void on_cmbHealthInsurance_currentIndexChanged(int index);
    void on_pbAddressAdd_clicked();
    void on_pbDoctor_clicked();
    void on_pbCompany_clicked();
    void on_pbHealthInsurance_clicked();
    void on_pbAttendances_clicked();
    void on_pbCitySearch_clicked();
    void on_ledZip_textEdited(QString );
    void on_pbFinishLater_clicked();
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
