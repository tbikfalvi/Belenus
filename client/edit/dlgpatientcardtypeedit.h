#ifndef DLGPATIENTCARDTYPEEDIT_H
#define DLGPATIENTCARDTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgpatientcardtypeedit.h"
#include "db/dbpatientcardtype.h"

class cDlgPatientCardTypeEdit : public QDialog, protected Ui::dlgPatientCardTypeEdit
{
    Q_OBJECT

public:
    cDlgPatientCardTypeEdit( QWidget *p_poParent = 0, cDBPatientCardType *p_poPatientCardType = NULL );
    virtual ~cDlgPatientCardTypeEdit();

public slots:
    virtual void accept ();

protected:
    cDBPatientCardType *m_poPatientCardType;
};

#endif
