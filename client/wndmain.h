#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QString>

#include "ui_wndmain.h"
#include "mdipanels.h"

using namespace std;

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

    bool showLogIn();
    void initPanels();

private:
    cMdiPanels         *mdiPanels;

    void updateTitle();

private slots:
    void on_action_Preferences_triggered();
    void on_action_Users_triggered();
    void on_action_Logs_triggered();
    void on_action_Hardwaretest_triggered();
    void on_action_LogOut_triggered();
    void on_action_Paneltypes_triggered();
    void on_action_Patientorigin_triggered();
    void on_action_ReasonToVisit_triggered();
    void on_action_Patients_triggered();
    void on_action_PatientNew_triggered();
    void on_action_Attendances_triggered();
    void on_actionDeviceStart_triggered();
    void on_actionDevicePause_triggered();
    void on_actionDeviceReset_triggered();
    void on_actionPatientSelect_triggered();
    void on_actionPatientEmpty_triggered();
    void on_actionAttendanceNew_triggered();
    void on_action_EnterCode_triggered();

    void refreshPanelButtons( bool p_boPanelWorking );
};

#endif
