#ifndef DLGPATIENTCARDADD_H
#define DLGPATIENTCARDADD_H

#include "../belenus.h"
#include "ui_dlgpatientcardadd.h"
#include "../db/dbpatientcard.h"

class cDlgPatientCardAdd : public QDialog, protected Ui::dlgPatientCardAdd
{
    Q_OBJECT

public:
    cDlgPatientCardAdd( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCardAdd();

private:
    QTimer          *m_pTimer;
    cDBPatientCard  *m_poPatientCard;

private slots:
    void on_ledBarcode_textChanged(QString );
    void on_pbSave_clicked();
    void on_pbExit_clicked();
    void updateInformation();
};

#endif
