#ifndef CDLGMANAGEDATABASE_H
#define CDLGMANAGEDATABASE_H

#include "belenus.h"
#include "ui_dlgmanagedatabase.h"
#include "dlgprogress.h"

class cDlgManageDatabase : public QDialog, private Ui::dlgManageDatabase
{
    Q_OBJECT

public:
    cDlgManageDatabase( QWidget *p_poParent = 0 );
    ~cDlgManageDatabase();

private:
    cDlgProgress            *m_dlgProgress;

private slots:
    void slotUpdateExecuteButton();
    void on_pbExecute_clicked();
    void on_pbExit_clicked();
    void _actionUpdatePatientCardUnits();
    void _actionDeactivatePC();
    void _actionDeleteInactivePC();
    void _actionDeleteNotUsedPCT();
    void _actionDeleteLedgerEntries();
    void _actionRepairPatientcardsWithoutType();
};

#endif // CDLGMANAGEDATABASE_H
