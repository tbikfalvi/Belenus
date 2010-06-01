#ifndef DLGPATIENTORIGINEDIT_H
#define DLGPATIENTORIGINEDIT_H

#include "belenus.h"
#include "ui_dlgpatientoriginedit.h"
#include "db/dbpatientorigin.h"

class cDlgPatientOriginEdit : public QDialog, protected Ui::dlgPatientOriginEdit
{
    Q_OBJECT

public:
    cDlgPatientOriginEdit( QWidget *p_poParent = 0, cDBPatientOrigin *p_poPatientOrigin = NULL );
    virtual ~cDlgPatientOriginEdit();

public slots:
    virtual void accept ();

protected:
    cDBPatientOrigin *m_poPatientOrigin;
};

#endif
