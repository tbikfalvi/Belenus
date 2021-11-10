#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include <QAction>
#include <QTranslator>
#include <QFrame>
#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "../framework/qtframework.h"
#include "../language/language.h"
#include "http.h"

extern QTranslator              *poTransApp;
extern QTranslator              *poTransQT;
extern QApplication             *apMainApp;
extern cQTMySQLConnection       *g_poDB;
extern cBlnsHttp                *g_poBlnsHttp;
extern cLanguage                 g_obLanguage;

namespace Ui { class dlgMain; }

class dlgMain : public QDialog
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

    enum userInfo
    {
        INFO_NoHttpServer = 1,
        INFO_HttpFailed,
        INFO_HttpDisabled,
        INFO_HttpEnabled,
        INFO_HttpSuspended,
        INFO_HttpContinued,
        INFO_NoPaymentMethod,
        INFO_NoPCType,
        INFO_Custom
    };

public:
    explicit dlgMain(QWidget *parent = 0, QString p_qsAppVersion = "1.0.0.0");
    ~dlgMain();
    bool checkTimeValues();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );

public slots:
    void slotShowMainWindow();

signals:
    void signalStartCounter(int p_nTimerLength, int p_nExtendedLength);
    void signalContinueCounter();
    void signalStopCounter();
    void signalResetCounter(int p_nTimerLength, int p_nExtendedLength);
    void signalSetTimer(int p_nTimerLength);
    void signalSetExtTimer(int p_nExtendedLength);

private slots:
    void on_pbHide_clicked();
    void on_pbExit_clicked();
    void on_cmbLang_currentIndexChanged(const QString &arg1);
    void on_pbRetranslate_clicked();
    void on_BlnsHttpErrorOccured();
    void on_BlnsHttpActionFinished( QString p_qsInfo );
    void on_BlnsHttpStepProgress();
    void on_pbResetSQL_clicked();
    void on_pbResetHTTP_clicked();
    void on_chkShowWindowOnStart_clicked();
    void on_pbSyncAllPatientCard_clicked();
    void on_ledTimerPCStatusSync_textEdited(const QString &arg1);
    void on_ledTimerPCOnlineSync_textEdited(const QString &arg1);
    void on_ledTimerMailSendCheck_textEdited(const QString &arg1);
    void on_pbClearPCData_clicked();
    void on_pbAuthenticate_clicked();
    void on_pbSyncOnlinePC_clicked();
    void on_pbStartStopHTTP_clicked();
    void on_cmbOnlinePatientCardType_currentIndexChanged(int index);
    void on_cmbOnlinePaymentMethod_currentIndexChanged(int index);
    void on_sliFileLogLevel_valueChanged(int value);
    void on_ledWebServerAddress_textEdited(const QString &arg1);
    void on_chkHttpCommunicationEnabled_clicked();
    void on_pbTest_clicked();
    void on_ledPassword_returnPressed();
    void on_pbClearPatientCard_clicked();
    void on_pbClearAllPatientCard_clicked();
    void on_PatientCardUpdated( unsigned int p_uiPatientCardId, QString p_qsBarcode );
    void slotShowModuleNotification( QString p_qsMessage );
    void on_pbSaveSettings_clicked();
    void on_pbRegisterLicence_clicked();
    void on_pbActivateLicence_clicked();
    void on_pbChangeLicence_clicked();

private:
    Ui::dlgMain         *ui;
    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;
    QMenu               *menuConnection;
    QMenu               *menuUserActions;

    QAction             *actionStatusSQL;
    QAction             *actionStatusHttp;
    QAction             *actionShowMainWindow;
    QAction             *actionUserSendAllPatientCard;
    QAction             *actionUserWaitingClearPatientCards;
//    QAction             *actionUserGetOnlinePatientCards;
    QAction             *actionClearPatientCard;
    QAction             *actionClearAllPatientCard;
    QAction             *actionExit;
/*
    QAction             *actionShow;
    QAction             *actionHide;
    QAction             *actionStart;
    QAction             *actionStop;
    QAction             *actionReset;
    QAction             *actionLogoff;
    QAction             *actionReboot;
    QAction             *actionShutdown;
    QAction             *actionResetSQL;
    QAction             *actionResetHTTP;
*/
    int                  m_nTimer;
    bool                 m_bMousePressed;
    int                  m_nMouseX;
    int                  m_nMouseY;
    QString              m_qsLang;
    bool                 m_bShowMainWindowOnStart;
    bool                 m_bStartTimerOnStart;
    bool                 m_bReloadLanguage;
    bool                 m_bSyncPCToServer;
    bool                 m_bSyncPCFromServer;
    bool                 m_bSendMailToServer;
    bool                 m_bRegisterLicenceKey;
    bool                 m_bReactivateLicenceKey;
    bool                 m_bStartRegisterLicenceKey;
    bool                 m_bStartReactivateLicenceKey;
    bool                 m_bValidateLicenceKey;
    int                  m_nTimerPCStatusSync;
//    int                  m_nTimerPCOnlineSync;
    int                  m_nTimerSendMailCheck;
    int                  m_nTimerLicenceValidationCheck;
    int                  m_nIndexUser;
    int                  m_nIndexPCStatusSync;
//    int                  m_nIndexPCOnlineSync;
    int                  m_nIndexSendMailSync;
    int                  m_nIndexLicenceValidation;
    int                  m_enGroup;
    QString              m_qsRPSW;
    int                  m_nIndexUpdateSyncDataCount;
//    unsigned int         m_uiPatientCardTypeId;
    bool                 m_bStartFinished;
//    unsigned int         m_uiPaymentMethodId;
    QString              m_qsHttpStatus;
    int                  m_nLogLevel;
    int                  m_nIndexCheckEnablers;
    bool                 m_bHttpEnabledBySetting;
    bool                 m_bHttpEnabledByUser;
    bool                 m_bHttpSuspendedByUser;

    bool                 m_FlagNoHttpServer;
    bool                 m_FlagHttpFailed;
    bool                 m_FlagHttpDisabled;
    bool                 m_FlagHttpEnabled;
    bool                 m_FlagHttpSuspended;
    bool                 m_FlagHttpContinued;
//    bool                 m_FlagNoPaymentMethod;
//    bool                 m_FlagNoPCType;

    QString              m_qsServerAddress;
    bool                 m_bServerAddressChanged;

    unsigned int         m_uiLicenceId;

    void                _setActions();
    void                _setMenu();
    void                _sendPCData( unsigned int p_uiId, QString p_qsBarcode );
    QString             _getPatientCardTypeName( unsigned int p_uiId );
    void                _setAuthInfoType( authType p_tAuthType );
    authType            _authenticateUser();
    void                _setGUIEnabled( bool p_bEnabled = true );
    bool                _isInGroup( groupUser p_enGroup );
    authType            _loginUser( QString p_qsName );
    void                _setPCTypeForHttp();
    void                _displayUserNotification( userInfo p_tUserInfo, QString p_qsInfoText = "", QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information );
    void                _checkIfHttpDisabledByUser();
    void                _checkIfHttpChangedByUser();
    void                _disableHttpBySetting();
    QString             _bytearrayToString( QString p_qsString );
    bool                _isAppicationRunning( QString p_qsAppName );
    void                _saveSettings();
    void                _licenceRegistrationAdmin();
    void                _licenceReactivationAdmin();
    void                _resetLicenceValidationTimers();
};

#endif // DLGMAIN_H
