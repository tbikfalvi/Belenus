#ifndef DLGREASONTOVISITEDIT_H
#define DLGREASONTOVISITEDIT_H

#include "belenus.h"
#include "ui_dlgreasontovisitedit.h"
#include "db/dbreasontovisit.h"

class cDlgReasonToVisitEdit : public QDialog, protected Ui::dlgReasonToVisitEdit
{
    Q_OBJECT

public:
    cDlgReasonToVisitEdit( QWidget *p_poParent = 0, cDBReasonToVisit *p_poReasonToVisit = NULL );
    virtual ~cDlgReasonToVisitEdit();

public slots:
    virtual void accept ();

protected:
    cDBReasonToVisit *m_poReasonToVisit;
};

#endif
