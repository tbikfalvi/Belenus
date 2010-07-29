#ifndef DLGPATIENTCARDEDIT_H
#define DLGPATIENTCARDEDIT_H

#include "belenus.h"
#include "../ui_dlgpatientcardedit.h"
#include "../db/dbpatientcard.h"
#include "../db/dbpatientcardtype.h"

class cDlgPatientCardEdit : public QDialog, protected Ui::dlgPatientCardEdit
{
    Q_OBJECT

public:
    cDlgPatientCardEdit( QWidget *p_poParent = 0, cDBPatientCard *p_poPatientCard = NULL );
    virtual ~cDlgPatientCardEdit();
    void activatePatientCard();
    void setPatientCardOwner( const unsigned int p_uiPatientId );

protected:
    cDBPatientCard      *m_poPatientCard;
    cDBPatientCardType  *m_poPatientCardType;

private:
    bool                 m_bDlgLoaded;
    bool                 m_bNewCard;

    QString convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

private slots:
    void on_cmbCardType_currentIndexChanged(int index);
    void on_cbActive_toggled(bool checked);
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif
