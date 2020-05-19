#ifndef DLGREMOVEUNITS_H
#define DLGREMOVEUNITS_H

#include "belenus.h"
#include "ui_dlgremoveunits.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgRemoveUnits : public QDialog, protected Ui::dlgRemoveUnits
{
    Q_OBJECT

public:
    cDlgRemoveUnits( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgRemoveUnits();

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;
    QStringList          m_qslList;
    int                  m_nUnitMax;

private slots:
    void slotUpdateUnitCount();
    void slotEnableButtons();
    void on_pbRemove_clicked();
    void on_pbCancel_clicked();
};

#endif
