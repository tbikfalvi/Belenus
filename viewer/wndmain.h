#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QVector>

#include "../framework/qtframework.h"
#include "ui_wndmain.h"
#include "creportdaily.h"
#include "creportledger.h"
#include "creportpatientcardtype.h"
#include "creportcardinactive.h"
#include "creportcarddetails.h"

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

    void                setLoginData(QString p_qsName = "", QString p_qsPassword = "" );

signals:
    void                setCheckedReportDaily( bool p_bChecked );
    void                setCheckedReportLedger( bool p_bChecked );
    void                setCheckedReportPatientcardType( bool p_bChecked );
    void                setCheckedReportPatientcardInactive( bool p_bChecked );
    void                setCheckedReportPatientcardDetails( bool p_bChecked );

public slots:
    void                slotCheckReportDaily( bool p_bChecked );
    void                slotCheckReportLedger( bool p_bChecked );
    void                slotCheckReportPatientcardType( bool p_bChecked );
    void                slotCheckReportPatientcardInactive( bool p_bChecked );
    void                slotCheckReportPatientcardDetails( bool p_bChecked );

private:

    QString             m_qsRPSW;

    cReportDaily            *m_repDaily;
    cReportLedger           *m_repLedger;
    cReportPatientCardType  *m_repCardType;
    cReportCardInactive     *m_repCardInactive;
    cReportCardDetails      *m_repCardDetails;

    QVector<cReport*>    m_qvReports;

    bool                 m_bReportTabSwitching;

    void                _initActions();
    void                _initToolbar();
    void                _initFilterbar();
    void                _initTabInformation();

    void                _setAuthInfoType( authType p_tAuthType );

    authType            _authenticateUser();
    void                _setReportsEnabled( bool p_bEnable = true );
    void                _setFiltersEnabled( bool p_bEnable );
    void                _setFiltersEnabledReport( cReport *obReport );

    void                _updateReportIndexes();

private slots:
    void on_tabReports_tabCloseRequested(int index);
    void on_action_Toolbar_triggered(bool checked);
    void on_action_FilterBar_triggered(bool checked);
    void on_pbAuthenticate_clicked();
    void on_tabReports_currentChanged(int index);
    void on_dtFilterDateStart_dateChanged(const QDate &date);
    void on_dtFilterDateStop_dateChanged(const QDate &date);
    void on_ledFilterDataName_textEdited(const QString &arg1);
    void on_cmbFilterDataTypes_currentIndexChanged(int index);
    void on_pbRefresh_clicked();
};

#endif // WNDMAIN_H
