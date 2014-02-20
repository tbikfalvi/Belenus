#ifndef CDLGMANAGEDATABASE_H
#define CDLGMANAGEDATABASE_H

#include "belenus.h"
#include "ui_dlgmanagedatabase.h"

class cDlgManageDatabase : public QDialog, private Ui::dlgManageDatabase
{
    Q_OBJECT

public:
    cDlgManageDatabase( QWidget *p_poParent = 0 );
    ~cDlgManageDatabase();

private:

private slots:
    void slotUpdateExecuteButton();
    void on_pbExecute_clicked();
    void on_pbExit_clicked();
    void _actionDeactivatePC();
    void _actionDeleteNotUsedPCT();
    void _actionDeleteLedgerEntries();
};

#endif // CDLGMANAGEDATABASE_H
