#ifndef DLGPATIENTCARDUSE_H
#define DLGPATIENTCARDUSE_H

#include "belenus.h"
#include "../../build/belenus/ui_dlgpatientcarduse.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardUse : public QDialog, protected Ui::dlgPatientCardUse
{
    Q_OBJECT

public:
    cDlgPatientCardUse( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPatientCardUse();
    void getUseUnitsTime( int *p_inUnitsUse, QString *p_qsTimeUse );

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    int                  m_inUnitLength;
    int                  m_inUnitsUse;
    QString              m_qsTimeUse;

private slots:
    void on_cmbTimeUse_currentIndexChanged(int index);
    void on_cmbNoUnits_currentIndexChanged(int index);
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
