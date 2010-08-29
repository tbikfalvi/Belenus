#ifndef DLGHEALTHINSURANCEEDIT_H
#define DLGHEALTHINSURANCEEDIT_H

#include "belenus.h"
#include "../ui_dlghealthinsuranceedit.h"
#include "../db/dbhealthinsurance.h"

class cDlgHealthInsuranceEdit : public QDialog, protected Ui::dlgHealthInsuranceEdit
{
    Q_OBJECT

public:
    cDlgHealthInsuranceEdit( QWidget *p_poParent = 0, cDBHealthInsurance *p_poHealthInsurance = NULL );
    virtual ~cDlgHealthInsuranceEdit();

public slots:
    virtual void accept ();

protected:
    cDBHealthInsurance *m_poHealthInsurance;
};

#endif
