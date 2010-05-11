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
    void on_rbRelay12_on_toggled(bool checked);
    void on_rbRelay11_on_toggled(bool checked);
    void on_rbRelay10_On_toggled(bool checked);
    void on_rbRelay09_on_toggled(bool checked);
    void on_rbRelay08_on_toggled(bool checked);
    void on_rbRelay07_On_toggled(bool checked);
    void on_rbRelay06_On_toggled(bool checked);
    void on_rbRelay05_On_toggled(bool checked);
    void on_rbRelay04_On_toggled(bool checked);
    void on_rbRelay03_On_toggled(bool checked);
    void on_rbRelay02_On_toggled(bool checked);
    void on_rbRelay01_On_toggled(bool checked);
    void on_pbRelayRunTest_clicked();
    void on_pbExit_clicked();
    void on_pbReconnect_clicked();
    void on_pbTestHardwareConnection_clicked();
};

#endif // CDLGHARDWARETEST_H
