#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QVector>

#include "../framework/qtframework.h"
#include "ui_wndmain.h"
#include "creportdaily.h"
#include "creportledger.h"
#include "creportcardactive.h"
#include "creportcardinactive.h"
#include "creportcardusage.h"

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
    cReportLedger       *m_repLedger;
    cReportCardActive   *m_repCardActive;
    cReportCardInactive *m_repCardInactive;
    cReportCardUsage    *m_repCardUsage;

    QVector<cReport*>    m_qvReports;

    void                _initActions();
    void                _initToolbar();
    void                _initFilterbar();
    void                _initTabInformation();

    void                _setAuthInfoType( authType p_tAuthType );

    authType            _authenticateUser();
    void                _setReportsEnabled( bool p_bEnable = true );
    void                _setFiltersEnabled( bool p_bEnable );

    void                _updateReportIndexes();

private slots:
    void on_tabReports_tabCloseRequested(int index);
    void on_action_Demo_triggered();
    void on_action_Toolbar_triggered(bool checked);
    void on_action_FilterBar_triggered(bool checked);
    void on_pbAuthenticate_clicked();
    void on_tabReports_currentChanged(int index);
};

#endif // WNDMAIN_H
