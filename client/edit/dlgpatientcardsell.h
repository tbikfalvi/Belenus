#ifndef DLGPATIENTCARDSELL_H
#define DLGPATIENTCARDSELL_H

#include "belenus.h"
#include "ui_dlgpatientcardsell.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"
#include "../dlg/dlgprogress.h"

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

    void timerEvent( QTimerEvent *p_poEvent );

private:
    cDlgProgress                *m_dlgProgress;
    bool                 m_bDlgLoaded;
    int                          m_nTimer;
    int                          m_nTimerCounter;

private slots:
    void slotRefreshWarningColors();
    void slotEnableButtons();
    void on_cmbCardType_currentIndexChanged(int index);
    void on_cmbPatient_currentIndexChanged(int index);
    void on_pbSell_clicked();
    void on_pbCancel_clicked();
    void on_pbSelectPatient_clicked();
    void on_pbCreatePatient_clicked();
    void on_chkRFID_clicked();
    void on_pbRFID_clicked();
};

#endif
