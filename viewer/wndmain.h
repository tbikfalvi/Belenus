#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QVector>

#include "../framework/qtframework.h"
#include "ui_wndmain.h"
// <_NEW_REPORT_> report header file include-olasa
#include "creportdaily.h"
#include "creportledger.h"
#include "creportpatientcardtype.h"
#include "creportcardinactive.h"
#include "creportcarddetails.h"
#include "creportcassahistory.h"
#include "creportpatientcardsell.h"
#include "creportproducts.h"
#include "creportpatientcarddebts.h"
#include "creportproductstatus.h"
#include "creportproducthistory.h"
#include "creportpatientcardusages.h"
#include "creportmonthclose.h"

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

    enum groupUser
    {
        GROUP_MIN = 0,
        GROUP_USER,
        GROUP_ADMIN,
        GROUP_SYSTEM,
        GROUP_ROOT,
        GROUP_MAX
    };

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

    void                    setLoginData(QString p_qsName = "", QString p_qsPassword = "" );

signals: // <_NEW_REPORT_>  report signal-ja
    void                    setCheckedReportDaily( bool p_bChecked );
    void                    setCheckedReportLedger( bool p_bChecked );
    void                    setCheckedReportCassaHistory( bool p_bChecked );
    void                    setCheckedReportPatientcardType( bool p_bChecked );
    void                    setCheckedReportPatientcardInactive( bool p_bChecked );
    void                    setCheckedReportPatientcardDetails( bool p_bChecked );
    void                    setCheckedReportPatientcardUsages( bool p_bChecked );
    void                    setCheckedReportPatientcardSells( bool p_bChecked );
    void                    setCheckedReportProducts( bool p_bChecked );
    void                    setCheckedReportPatientcardDebts( bool p_bChecked );
    void                    setCheckedReportProductStatus( bool p_bChecked );
    void                    setCheckedReportProductHistory( bool p_bChecked );
    void                    setCheckedReportMonthClose( bool p_bChecked );

public slots: // <_NEW_REPORT_> report slot-ja
    void                    slotCheckReportDaily( bool p_bChecked );
    void                    slotCheckReportLedger( bool p_bChecked );
    void                    slotCheckReportCassaHistory( bool p_bChecked );
    void                    slotCheckReportPatientcardType( bool p_bChecked );
    void                    slotCheckReportPatientcardInactive( bool p_bChecked );
    void                    slotCheckReportPatientcardDetails( bool p_bChecked );
    void                    slotCheckReportPatientcardUsages( bool p_bChecked );
    void                    slotCheckReportPatientcardSells( bool p_bChecked );
    void                    slotCheckReportProducts( bool p_bChecked );
    void                    slotCheckReportPatientcardDebts( bool p_bChecked );
    void                    slotCheckReportProductStatus( bool p_bChecked );
    void                    slotCheckReportProductHistory( bool p_bChecked );
    void                    slotCheckReportMonthClose( bool p_bChecked );

private:

    QString                  m_qsRPSW;

    // <_NEW_REPORT_> report osztaly objektum pointer-e
    cReportDaily            *m_repDaily;
    cReportLedger           *m_repLedger;
    cReportCassaHistory     *m_repCassaHistory;
    cReportPatientCardType  *m_repCardType;
    cReportCardInactive     *m_repCardInactive;
    cReportCardDetails      *m_repCardDetails;
    cReportPCUsages         *m_repCardUsages;
    cReportPatientcardSell  *m_repCardSells;
    cReportProducts         *m_repProducts;
    cReportPatientcardDebts *m_repCardDebts;
    cReportProductStatus    *m_repProdStatus;
    cReportProductHistory   *m_repProdHistory;
    cReportMonthClose       *m_repMonthClose;

    QVector<cReport*>        m_qvReports;

    bool                     m_bReportTabSwitching;

    int                      m_enGroup;

    bool                    _isInGroup( groupUser p_enGroup );
    void                    _initActions();
    void                    _initToolbar();
    void                    _initFilterbar();
    void                    _initTabInformation();

    void                    _setAuthInfoType( authType p_tAuthType );

    authType                _authenticateUser();
    void                    _setReportsEnabled( bool p_bEnable = true );
    void                    _setFiltersEnabled( bool p_bEnable );
    void                    _setFiltersEnabledReport( cReport *obReport );

    void                    _updateReportIndexes();

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
    void on_cmbFilterDataSubTypes_currentIndexChanged(int index);
    void on_pbRefresh_clicked();
    void on_chkFilterIsVisible_clicked();
    void on_pbPrint_clicked();
    void on_rbOrientationPortrait_clicked();
    void on_rbOrientationLandscape_clicked();
};

#endif // WNDMAIN_H
