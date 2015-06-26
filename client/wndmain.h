#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QString>
#include <vector>

#include "ui_wndmain.h"
#include "mdipanels.h"
#include "dlg/dlgprogress.h"
#include "db/dbshoppingcart.h"
#include "dlg/dlgcassaaction.h"
#include "dlg/dlgsecondarywindow.h"
#include "db/dbapplicationaction.h"
#include "advertisementwindow.h"

using namespace std;

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

public:

    enum msgBoxType
    {
        MSG_INFORMATION = 0,
        MSG_WARNING,
        MSG_ATTENTION,
        MSG_ERROR,
        MSG_QUESTION
    };

    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

    bool showLogIn();
    void initPanels();

    void loginUser();
    void logoutUser();

    void startMainTimer();
    void autoSynchronizeGlobalData();
    void showProgress();
    void setCommunicationEnabled( bool p_bEnabled = false );

    int customMsgBox( QWidget *parent, msgBoxType msgtype, QString buttonstext, QString msg, QString details = "" );

protected:
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void keyReleaseEvent ( QKeyEvent *p_poEvent );
    void timerEvent( QTimerEvent *p_poEvent );
    void closeEvent( QCloseEvent *p_poEvent );

private:

    QLabel                   m_lblStatusLeft;
    QPushButton              m_pbStatusHttp;
    QPushButton              m_pbStatusCommunication;
    QPushButton              m_pbStatusKeyboard;
    QLabel                   m_lblHttpCount;
    QLabel                   m_lblStatusRight;
    cMdiPanels              *mdiPanels;
    cDlgProgress            *m_dlgProgress;
    int                      m_nTimer;
    cDlgSecondaryWindow     *m_dlgSecondaryWindow;
    unsigned int             m_uiPatientId;
    bool                     m_bCtrlPressed;
    bool                     m_bSerialRegistration;
    int                      m_inRegistrationTimeout;
    bool                     m_bGlobalDataRequested;
    int                      m_inGlobalDataRequestTimeout;
    QString                  m_qsStatusText;
    cDBApplicationAction     m_obApplicationAction;
    int                      m_nEnterAction;
    int                      m_inPanelStartMinute;
    QString                  m_qsPanelStartBarcode;
    int                      m_inCommunicationCounter;
    bool                     m_bActionProcessing;
    bool                     m_bProgressErrorVisible;
    int                      m_nProgressCounter;
    bool                     m_bBlnsHttpConnected;
    bool                     m_bBlnsHttpErrorVisible;
    unsigned int             m_uiBlnsErrorAppeared;
    int                      m_nCommunicationErrorCounter;
    int                      m_nCommResetStep;
    bool                     m_bClosingShift;
    bool                     m_bShoppingCartHasItem;
    int                      m_nHttpCommCounter;
    bool                     m_bMainWindowActive;

    vector<cDlgAdvertisementWindow *>   m_obAdWnd;

    void showElementsForComponents();
    void enableElementsByLogin( bool p_bEnable );
    void updateTitle();
    void updateStatusText( QString p_qsStatusText="" );
    void processInputPatientCard( QString p_stBarcode );
    void processInputProduct( QString p_stBarcode );
    void processInputTimePeriod( int p_inMinute );
    void showAdWindows();
    void _resetCommunication();
    void _updateAllPatientcardToWeb();
    void _removeAllPatientcardFromWeb();
    void _removePatientcardFromWeb();
    void _setStatusText( QString p_qsText, bool p_bError = false );
    void _processHttpActions();
    void _checkVersions();
    void _checkIsActivationNeeded();

public slots:
    void processDeviceUsePayment( unsigned int p_uiPanelId, unsigned int p_uiLedgerId, int p_nPaymentType );
    void processProductSellPayment( const cDBShoppingCart &p_obDBShoppingCart );

    void slotMainWindowActivated();

private slots:
    void on_action_SellProduct_triggered();
    void updateToolbar();
    void slotOpenShoppingCart( unsigned int p_uiPanelId );
    void slotOpenScheduleTable( unsigned int p_uiPanelId );
    void slotStatusChanged( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId, const QString p_qsStatus );
    void slotSetCounterText( unsigned int p_uiPanelId, const QString &p_qsCounter );
    void slotSetWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime );
    void slotSetInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo );
    void slotReplacePatientCard( const QString &p_qsBarcode );
    void slotAssignPartnerCard( const QString &p_qsBarcode );
    void on_action_CassaActionStorno_triggered();
    void on_action_ShoppingCart_triggered();
    void on_action_SellProduct_triggered( QString p_qsBarcode );
    void on_action_ProductActionType_triggered();
    void on_action_Guests_triggered();
    void on_action_Preferences_triggered();
    void on_action_Users_triggered();
    void on_action_Logs_triggered();
    void on_action_Hardwaretest_triggered();
    void on_action_LogOut_triggered();
    void on_action_CloseShift_triggered();
    void on_action_Paneltypes_triggered();
    void on_action_Panelgroups_triggered();
    void on_action_Patientorigin_triggered()        {}
    void on_action_ReasonToVisit_triggered()        {}
    void on_action_Patients_triggered()             {}
    void on_action_PatientNew_triggered();
    void on_action_DeviceClear_triggered();
    void on_action_DeviceStart_triggered();
    void on_action_DeviceReset_triggered();
    void on_action_PatientSelect_triggered();
    void on_action_PatientEmpty_triggered();
    void on_action_PanelStatuses_triggered();
    void on_action_UseDevice_triggered();
    void on_action_UseDeviceLater_triggered();
    void on_action_Cards_triggered();
    void on_action_CardTypes_triggered();
    void on_action_ProductTypes_triggered();
    void on_action_Products_triggered();
    void on_action_PCSaveToDatabase_triggered();
    void on_action_Cassa_triggered();
    void on_action_Accounting_triggered();
    void on_action_DeviceSkipStatus_triggered();
    void on_action_ValidateSerialKey_triggered();
    void on_action_PatientCardSell_triggered();
    void on_action_PatientCardAssign_triggered();
    void on_action_ReplaceLostCard_triggered();
    void on_action_EditActualPatient_triggered();
    void on_action_DeviceSettings_triggered();
    void on_action_PayCash_triggered();
    void on_action_IllnessGroup_triggered()             {}
    void on_action_Company_triggered()                  {}
    void on_action_Doctor_triggered()                   {}
    void on_action_HealthInsurance_triggered()          {}
    void on_action_RegionZipCity_triggered()            {}
    void on_action_ReportPatients_triggered()           {}
    void on_action_CassaHistory_triggered();
    void on_action_ReportPatientcardUses_triggered();
    void on_action_EditLicenceInformation_triggered();
    void on_action_ReportPatientcards_triggered();
    void on_action_Discounts_triggered();
    void on_action_PatientcardsObsolete_triggered();
    void on_action_SynchronizeDatabase_triggered()      {}
    void on_action_AcquireGlobalData_triggered()        {}
    void on_action_EstablishConnection_triggered()      {}
    void on_action_EmptyDemoDB_triggered();
    void on_action_PaymentMethods_triggered();
    void on_action_TestDlgStarted();
    void on_pbLogin_clicked();
    void on_ledPassword_returnPressed();
    void on_action_ReportViewer_triggered();
    void on_action_About_triggered();
    void on_action_ManageDatabase_triggered();
    void on_action_ManageDevicePanels_triggered();
    void on_action_ManageSkinTypes_triggered();
    void on_action_Import_triggered();
    void on_action_Export_triggered();
    void on_BlnsHttpErrorOccured();
    void on_BlnsHttpActionFinished( QString p_qsInfo );
    void on_BlnsHttpIconClicked();
    void on_BlnsHttpStepProgress();
    void on_BlnsHttpHideProgress();
    void on_action_Advertisements_triggered();
    void on_CommunicationButtonClicked();
    void on_action_PatientcardInformation_triggered();
    void on_KeyboardEnabled();
    void on_KeyboardDisabled();
    void on_BlnsHttpProcessStopped();
};

#endif
