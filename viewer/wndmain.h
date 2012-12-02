#ifndef WNDMAIN_H
#define WNDMAIN_H

#include "ui_wndmain.h"
#include "creportdaily.h"
#include "../framework/qtframework.h"

extern cQTMySQLConnection  *g_poDB;

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

signals:
    void                setCheckedReportDaily( bool p_bChecked );
    void                setCheckedReportLedger( bool p_bChecked );
    void                setCheckedReportPatientcardActive( bool p_bChecked );
    void                setCheckedReportPatientcardInactive( bool p_bChecked );
    void                setCheckedReportPatientcardUsage( bool p_bChecked );

public slots:
    void                slotCheckReportDaily( bool p_bChecked );
    void                slotCheckReportLedger( bool p_bChecked );
    void                slotCheckReportPatientcardActive( bool p_bChecked );
    void                slotCheckReportPatientcardInactive( bool p_bChecked );
    void                slotCheckReportPatientcardUsage( bool p_bChecked );

private:

    QString             m_qsRPSW;

    cReportDaily        *m_repDaily;

    int                  m_nReportDailyIndex;
    int                  m_nReportLedgerIndex;
    int                  m_nReportPatientcardsActiveIndex;
    int                  m_nReportPatientcardsInactiveIndex;
    int                  m_nReportPatientcardsUsageIndex;

    void                _initActions();
    void                _initToolbar();
    void                _initFilterbar();
    void                _initTabInformation();

    void                _setAuthInfoType( authType p_tAuthType );

    authType            _authenticateUser();
    void                _setReportsEnabled( bool p_bEnable = true );

private slots:
    void on_tabReports_tabCloseRequested(int index);
    void on_action_Demo_triggered();
    void on_action_Toolbar_triggered(bool checked);
    void on_action_FilterBar_triggered(bool checked);
    void on_pbAuthenticate_clicked();
};

#endif // WNDMAIN_H
