#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QString>

#include "ui_wndmain.h"
#include "mdipanels.h"
#include "dlg/dlgprogress.h"

using namespace std;

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

    bool showLogIn();
    void initPanels();

    void loginUser();
    void logoutUser();

    void checkDemoLicenceKey();
    void startMainTimer();

protected:
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void keyReleaseEvent ( QKeyEvent *p_poEvent );
    void timerEvent( QTimerEvent *p_poEvent );
    void closeEvent( QCloseEvent *p_poEvent );

private:
    cMdiPanels         *mdiPanels;
    cDlgProgress       *m_dlgProgress;
    int                 m_nTimer;

    unsigned int        m_uiPatientId;
    unsigned int        m_uiAttendanceId;

    bool                m_bCtrlPressed;

    void updateTitle();
    void processInputPatient( QString p_stPatientName );
    void processInputPatientCard( QString p_stBarcode );
    void processInputTimePeriod( int p_inSecond );

private slots:
    void updateToolbar();
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
    void on_action_DeviceStart_triggered();
    void on_action_DeviceReset_triggered();
    void on_action_PatientSelect_triggered();
    void on_action_PatientEmpty_triggered();
    void on_action_AttendanceNew_triggered();
    void on_action_PanelStatuses_triggered();
    void on_action_UseWithCard_triggered();
    void on_action_UseByTime_triggered();
    void on_action_Cards_triggered();
    void on_action_CardTypes_triggered();
    void on_action_PCSaveToDatabase_triggered();
    void on_action_Cassa_triggered();
    void on_action_Accounting_triggered();
    void on_action_DeviceSkipStatus_triggered();
    void on_action_PostponedPatient_triggered();
    void on_action_PostponedAttendance_triggered();
    void on_action_ValidateSerialKey_triggered();
    void on_action_PatientCardSell_triggered();
    void on_action_EditActualPatient_triggered();
    void on_action_DeviceSettings_triggered();
    void on_action_SelectActualAttendance_triggered();
    void on_action_DeselectActualAttendance_triggered();
    void on_action_EditActualAttendance_triggered();
    void on_action_PayCash_triggered();
    void on_action_IllnessGroup_triggered();
    void on_action_Company_triggered();
    void on_action_Doctor_triggered();
    void on_action_HealthInsurance_triggered();
    void on_action_RegionZipCity_triggered();
    void on_action_ReportPatients_triggered();
    void on_action_CassaHistory_triggered();
    void on_action_ReportAttendances_triggered();
    void on_action_ReportPatientcardUses_triggered();
    void on_action_EditLicenceInformation_triggered();
    void on_action_ReportPatientcards_triggered();
    void on_action_Discounts_triggered();
    void on_action_PatientcardsObsolete_triggered();
    void on_action_SynchronizeDatabase_triggered();
};

#endif
