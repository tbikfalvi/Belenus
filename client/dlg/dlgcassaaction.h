#ifndef DLGCASSAACTION_H
#define DLGCASSAACTION_H

#include "../belenus.h"
#include "../ui_dlgcassaaction.h"

class cDlgCassaAction : public QDialog, protected Ui::dlgCassaAction
{
    Q_OBJECT

public:
    cDlgCassaAction( QWidget *p_poParent = 0 );
    virtual ~cDlgCassaAction();


private:

private slots:

};

#endif
