#ifndef CDLGHARDWARETEST_H
#define CDLGHARDWARETEST_H

#include "belenus.h"
#include "../ui_dlghardwaretest.h"

class cDlgHardwareTest : public QDialog, private Ui::dlgHardwaretest
{
    Q_OBJECT

public:
    cDlgHardwareTest( QWidget *p_poParent = 0 );
    ~cDlgHardwareTest();

private:
    QTimer      *timerHWTest;
    QTimer      *timerRelayTest;
    int          m_nRelayCount;

private slots:
    void on_pbSwitchRelay12_clicked();
    void on_pbSwitchRelay11_clicked();
    void on_pbSwitchRelay10_clicked();
    void on_pbSwitchRelay09_clicked();
    void on_pbSwitchRelay08_clicked();
    void on_pbSwitchRelay07_clicked();
    void on_pbSwitchRelay06_clicked();
    void on_pbSwitchRelay05_clicked();
    void on_pbSwitchRelay04_clicked();
    void on_pbSwitchRelay03_clicked();
    void on_pbSwitchRelay02_clicked();
    void on_pbSwitchRelay01_clicked();
    void on_pbRelayRunTest_clicked();
    void on_pbExit_clicked();
    void on_pbReconnect_clicked();
    void on_pbTestHardwareConnection_clicked();
    void updateHWStatus();
    void updateRelayStatus();
    void on_pbAddressModules_clicked();
};

#endif // CDLGHARDWARETEST_H
