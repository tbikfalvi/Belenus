#ifndef DLGPATIENTCARDADD_H
#define DLGPATIENTCARDADD_H

#include "../belenus.h"
#include "../ui_dlgpatientcardadd.h"

class cDlgPatientCardAdd : public QDialog, protected Ui::dlgPatientCardAdd
{
    Q_OBJECT

public:
    cDlgPatientCardAdd( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCardAdd();

private slots:
    void on_pbExit_clicked();
};

#endif
