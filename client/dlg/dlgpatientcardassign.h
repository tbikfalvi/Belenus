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

    bool    isCardsCanBeAssigned();

    bool    m_bMainCardOk;
    bool    m_bAssignCardOk;

private slots:

    void on_ledMainBarcode_textChanged(const QString &arg1);
    void on_pbMainVerify_clicked();
    void on_ledAssignBarcode_textChanged(const QString &arg1);
    void on_pbAssignVerify_clicked();
    void on_pbAssign_clicked();
    void on_pbCancel_clicked();
};

#endif
