#ifndef WNDMAIN_H
#define WNDMAIN_H

#include "ui_wndmain.h"
#include "creport.h"

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

    enum authType
    {
        AUTH_NEEDED = 0,
        AUTH_OK,
        AUTH_USER_NOTFOUND,
        AUTH_PASSWORD_INCORRECT,
        AUTH_CONNECTION_FAILED,
        AUTH_ERROR
    };

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

private:

    cReport     *m_repDaily;

    int          m_nReportDailyIndex;
    int          m_nReportLedgerIndex;
    int          m_nReportPatientcardsActiveIndex;
    int          m_nReportPatientcardsInactiveIndex;
    int          m_nReportPatientcardsUsageIndex;

    void        _initActions();
    void        _initToolbar();
    void        _initFilterbar();
    void        _initTabInformation();

    void        _setAuthInfoType( authType p_tAuthType );

    void        _showReportDaily( bool p_bShow );
    void        _showReportLedger( bool p_bShow );
    void        _showReportPatientcardsActive( bool p_bShow );
    void        _showReportPatientcardsInactive( bool p_bShow );
    void        _showReportPatientcardsUsage( bool p_bShow );

    authType    _authenticateUser();
    void        _setReportsEnabled( bool p_bEnable = true );

private slots:
    void on_tabReports_tabCloseRequested(int index);
    void on_action_Demo_triggered();
    void on_action_Toolbar_triggered(bool checked);
    void on_action_FilterBar_triggered(bool checked);
    void on_action_Bookkeeping_Daily_triggered(bool checked);
    void on_action_Bookkeeping_Ledger_triggered(bool checked);
    void on_action_Patientcards_Active_triggered(bool checked);
    void on_action_Patientcards_Inactive_triggered(bool checked);
    void on_action_Patientcards_Usage_triggered(bool checked);
    void on_pbAuthenticate_clicked();
};

#endif // WNDMAIN_H
