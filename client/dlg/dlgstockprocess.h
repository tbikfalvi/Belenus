#ifndef DLGSTOCKPROCESS_H
#define DLGSTOCKPROCESS_H

#include "../belenus.h"
#include "ui_dlgStockProcess.h"

class dlgStockProcess : public QDialog, protected Ui::dlgStockProcess
{
    Q_OBJECT

public:
    dlgStockProcess( QWidget *p_poParent = 0, QString p_qsTitle = "" );
    virtual ~dlgStockProcess();

private slots:
    void on_pbExit_clicked();
    void on_rbBarcode_clicked();
    void on_rbName_clicked();
    void on_ledBarcode_editingFinished();
    void on_ledName_editingFinished();
    void on_pbStockIncrease_clicked();
    void on_pbStockDecrease_clicked();

private:
    QString     m_qsFilter;
    bool        m_bComboFillInProgress;

    void        _fillProductCombo();
    void        _restartProcess();
};

#endif
