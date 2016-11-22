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
#include "http.h"

extern QTranslator          *poTransApp;
extern QTranslator          *poTransQT;
extern QApplication         *apMainApp;
extern cQTMySQLConnection   *g_poDB;
extern cBlnsHttp            *g_poBlnsHttp;

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

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();
    bool checkTimeValues();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );

public slots:
    void slotSettings();

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

    void on_pbClearPCData_clicked();

    void on_pbAuthenticate_clicked();

    void on_pbSyncOnlinePC_clicked();

private:
    Ui::dlgMain         *ui;
    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;
    QMenu               *menuConnection;
    QAction             *actionSettings;
    QAction             *actionShow;
    QAction             *actionHide;
    QAction             *actionStart;
    QAction             *actionStop;
    QAction             *actionReset;
    QAction             *actionLogoff;
    QAction             *actionReboot;
    QAction             *actionShutdown;
    QAction             *actionExit;
    QAction             *actionResetSQL;
    QAction             *actionResetHTTP;
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
    int                  m_nTimerPCStatusSync;
    int                  m_nTimerPCOnlineSync;
    int                  m_nIndexUser;
    int                  m_nIndexPCStatusSync;
    int                  m_nIndexPCOnlineSync;
    int                  m_enGroup;
    QString              m_qsRPSW;
    int                  m_nIndexUpdateSyncDataCount;

    void                _setActions();
    void                _setMenu();
    void                _sendPCData( unsigned int p_uiId, QString p_qsBarcode );
    QString             _getPatientCardTypeName( unsigned int p_uiId );
    void                _setAuthInfoType( authType p_tAuthType );
    authType            _authenticateUser();
    void                _setGUIEnabled( bool p_bEnabled = true );
    bool                _isInGroup( groupUser p_enGroup );
    authType            _loginUser( QString p_qsName );
};

#endif // DLGMAIN_H
