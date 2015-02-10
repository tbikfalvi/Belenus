#ifndef DLGPATIENTCARDASSIGN_H
#define DLGPATIENTCARDASSIGN_H

#include "ui_dlgpatientcardassign.h"

class cDlgPatientCardAssign : public QDialog, protected Ui::dlgPatientCardAssign
{
    Q_OBJECT

public:
    cDlgPatientCardAssign( QWidget *p_poParent = 0, QString p_qsMainBarcode = "" );
    virtual ~cDlgPatientCardAssign();

    void    getCardsBarcode( QString *p_qsBarcodeMain, QString *p_qsBarcodeAssign );

protected:

private:

    bool            _isCardsCanBeAssigned();
    void            _fillOldCardAssignStructure();
    void            _disableControls();
    bool            _checkIfExists( QString p_qsBarcode );
    void            _loadAssignedCard();

    bool             m_bMainCardOk;
    bool             m_bAssignCardOk;
    QString          m_qsBarcodeMain;
    QString          m_qsBarcodeAssign;
    unsigned int     m_uiParentGroupM;
    unsigned int     m_uiParentGroupA;
    bool             m_bCanBeParentM;
    bool             m_bCanBeParentA;
    QString          m_qsReasonM;
    QString          m_qsReasonA;

private slots:

    void on_ledMainBarcode_textChanged(const QString &arg1);
    void on_ledMainBarcode_returnPressed();
    void on_ledAssignBarcode_textChanged(const QString &arg1);
    void on_ledAssignBarcode_returnPressed();
    void on_pbCheckCards_clicked();
    void on_pbAssign_clicked();
    void on_pbCancel_clicked();
    void slotRadioClicked();
};

#endif
