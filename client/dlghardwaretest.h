#ifndef CDLGHARDWARETEST_H
#define CDLGHARDWARETEST_H

#include "belenus.h"
#include "ui_dlghardwaretest.h"

class cDlgHardwareTest : public QDialog, private Ui::dlgHardwaretest
{
    Q_OBJECT

public:
    cDlgHardwareTest( QWidget *p_poParent = 0 );
    ~cDlgHardwareTest();

private:

private slots:
    void on_pbExit_clicked();
    void on_pbReconnect_clicked();
    void on_pbTestHardwareConnection_clicked();
};

#endif // CDLGHARDWARETEST_H
