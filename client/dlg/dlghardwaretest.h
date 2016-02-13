#ifndef CDLGHARDWARETEST_H
#define CDLGHARDWARETEST_H

#include <QVector>

#include "belenus.h"
#include "ui_dlghardwaretest.h"

class cDlgHardwareTest : public QDialog, private Ui::dlgHardwaretest
{
    Q_OBJECT

public:
    cDlgHardwareTest( QWidget *p_poParent = 0 );
    ~cDlgHardwareTest();

private:
    QTimer      *timerHWTest;
    QTimer      *timerRelayTest;
    QTimer      *timerModulTest;
    QTimer      *timerModulButtonTest;
    int          m_nRelayCount;

    QVector<QLabel*>    qvLblOk;
    QVector<QLabel*>    qvLblFail;
    QVector<QLabel*>    qvLblStatus;

    int                 nCounterOk[14];
    int                 nCounterFail[14];

    bool                m_bModuleOn;
    int                 m_nTime;

    int                 m_nModelCount;

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
    void on_pbInitHardware_clicked();
    void on_pbTestModules_clicked();
    void on_pbModulOnOff_clicked();
    void slotTestModules();
    void on_pbStopModuleTest_clicked();
    void on_pbTestModuleButtons_clicked();
    void on_pbStopModuleButtonTest_clicked();
    void slotTestModulButtons();
};

#endif // CDLGHARDWARETEST_H
