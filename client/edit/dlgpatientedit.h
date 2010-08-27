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
    bool SavePatientData();
    void FillHealthInsuranceCombo();
    void FillCompanyCombo();
    void FillDoctorCombo();

protected:
    cDBPatient      *m_poPatient;
    cDBPostponed    *m_poPostponed;
    cDBAddress      *m_poAddress;
    cDBPatientCard  *m_poPatientCard;

private slots:
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
