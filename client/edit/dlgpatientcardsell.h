#ifndef DLGPATIENTCARDSELL_H
#define DLGPATIENTCARDSELL_H

#include "belenus.h"
#include "ui_dlgpatientcardsell.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardSell : public QDialog, protected Ui::dlgPatientCardSell
{
    Q_OBJECT

public:
    cDlgPatientCardSell( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgPatientCardSell();
    void setPatientCardOwner( const unsigned int p_uiPatientId );

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;

private slots:
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_cmbCardType_currentIndexChanged(int index);
    void on_cmbPatient_currentIndexChanged(int index);
    void on_pbSell_clicked();
    void on_pbCancel_clicked();
    void on_pbSelectPatient_clicked();
    void on_pbCreatePatient_clicked();
};

#endif
