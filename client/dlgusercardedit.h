#ifndef DLGUSERCARDEDIT_H
#define DLGUSERCARDEDIT_H

#include "ui_dlgusercardedit.h"
#include "dbusercard.h"

class cDlgUserCardEdit : public QDialog, protected Ui::dlgUserCardEdit
{
    Q_OBJECT

public:
    cDlgUserCardEdit( QWidget *p_poParent = 0, cDBUserCard *p_poUserCard = NULL );
    virtual ~cDlgUserCardEdit();

public slots:
    virtual void accept ();

protected:
    cDBUserCard *m_poUserCard;

private:

};

#endif // DLGUSERCARDEDIT_H
