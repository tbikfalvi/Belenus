#ifndef DLGUSERCARDTYPEEDIT_H
#define DLGUSERCARDTYPEEDIT_H

#include "belenus.h"
#include "ui_dlgusercardtypeedit.h"
#include "dbusercardtype.h"

class cDlgUserCardTypeEdit : public QDialog, protected Ui::dlgUserCardTypeEdit
{
    Q_OBJECT

public:
    cDlgUserCardTypeEdit( QWidget *p_poParent = 0, cDBUserCardType *p_poUserCardType = NULL );
    virtual ~cDlgUserCardTypeEdit();

public slots:
    void on_rbValidDates_clicked();
    void on_rbValidDays_clicked();
    virtual void accept ();

protected:
    cDBUserCardType *m_poUserCardType;
};

#endif // DLGUSERCARDTYPEEDIT_H
