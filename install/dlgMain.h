//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgMain.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DLGMAIN_H
#define DLGMAIN_H

//====================================================================================

#include <QDialog>
#include <QFile>
#include <QSettings>
#include <QVector>
#include <QtSql>

//====================================================================================

#include "ui_dlgMain.h"
#include "../client/communication.h"
#include "cregistry.h"

//====================================================================================

#define CONST_PAGE_WELCOME               0
#define CONST_PAGE_INSTALL_SELECTION     1
#define CONST_PAGE_COMPONENT_SELECTION   2
#define CONST_PAGE_WAMP_INSTALL          3
#define CONST_PAGE_INIT_SQL              4
#define CONST_PAGE_HARDWARE_INSTALL      5
#define CONST_PAGE_CLIENT_INSTALL        6
#define CONST_PAGE_PROCESS               7
#define CONST_PAGE_FINISH                8

#define CONST_INSTALL_APP_VERSION       "1.0"

//====================================================================================

extern cRegistry     g_obReg;
extern QTranslator  *poTransSetup;
extern QTranslator  *poTransQT;
extern QApplication *apMainApp;
extern QString       g_qsCurrentPath;

//====================================================================================
class dlgMain : public QDialog, protected Ui::dlgMain
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0,
                     bool bUninstall = false,
                     bool bSilent = false,
                     int nDeviceNum = 3,
                     int nComPort = 0,
                     QString qsLangInstall = "hu",
                     QString qsLangApp = "hu");
    ~dlgMain();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void closeEvent( QCloseEvent *p_poEvent );

private:

    QFile                   *m_obLog;

    QString                  m_qsPathWindows;
    QString                  m_qsPathPrograms;
    QString                  m_qsPathDesktop;
    QString                  m_qsPathWampServer;

//    bool                     m_bBelenusAlreadyInstalled;
    bool                     m_bWampServerAlreadyInstalled;
    bool                     m_bDatabaseAlreadyInstalled;
    bool                     m_bRootUserExists;
    bool                     m_bBelenusUserExists;

    QStringList              m_qslComponents;

    QFile                   *m_obFile;
    int                      m_nTimer;

    QVector<int>             m_vPages;
    int                      m_nCurrentPage;
    QRadioButton            *m_pInstallType;
    QString                  m_qsRootPassword;
    QString                  m_qsRootPasswordNew;
    int                      m_nComPort;
    int                      m_nCountDevices;

//    bool                     m_bInstallStarted;         // Identifies if install process started
                                                        // true after component selection
    bool                     m_bProcessWamp;
    bool                     m_bProcessDatabase;
    bool                     m_bProcessHWConnection;
    bool                     m_bProcessBelenusClient;
    bool                     m_bProcessViewer;
    bool                     m_bRestartRequired;
    bool                     m_bStartWampInstall;
    bool                     m_bInitializeWamp;
    bool                     m_bDemoMode;
    bool                     m_bInstallClient;
    bool                     m_bInstallFinished;

    QSqlDatabase            *m_poDB;
    CS_Communication        *m_poHardware;
    QString                  m_qsClientInstallDir;
    QString                  m_qsIPAddress;
    int                      m_nPort;
    bool                     m_bIsWindows32Bit;

    bool                     m_bUninstallCalled;
    bool                     m_bSilentIstallCalled;
    QString                  m_qsProcessErrorMsg;
    QString                  m_qsIniFileName;

    QStringList              m_qslFiles;
    QString                  m_qsLanguage;
    QString                  m_qsUninstallWampExec;

    QString                  m_qsAppVersion;
    QString                  m_qsDBVersion;

    QString                  m_qsLangInstall;
    QString                  m_qsLangApp;

    void                    _initializeInstall();
    void                    _uninstallBelenus();


    void                    _initializePage( int p_nPage );
    void                    _initializeWelcomePage();
    void                    _initializeSelectionPage();
    void                    _initializeComponentSelectionPage();
    void                    _initializeWampInstallPage();
    void                    _initializeSQLPage();
    void                    _installWampServer();
    void                    _installSQLServer();
    void                    _initializeHardwareInstallPage();
    void                    _initializeClientInstallPage();
    void                    _initializeInstallProcessPage();
    void                    _initializeFinishPage();

    bool                    _processPage( int p_nPage );
    bool                    _processWelcomePage();
    bool                    _processSelectionPage();
    bool                    _processComponentSelectionPage();
    bool                    _processWampInstallPage();
    bool                    _processSQLPage();
    bool                    _processHardwareInstallPage();
    bool                    _processClientInstallPage();

    bool                    _processWampServerInstall( QString *p_qsMessage );
    int                     _checkWampServer();
    bool                    _initializeMySQL();
    bool                    _processRootCreate();
    bool                    _processRootModify();
    bool                    _processDatabaseCreate();
    bool                    _processBelenusUserCreate();
    bool                    _processBelenusUserRights();
    bool                    _processBelenusTablesCreate();
    bool                    _processBelenusTablesFill();
    bool                    _processBelenusDeviceFill();
    void                    _processInstall();
    bool                    _processDatabaseInstall();
    int                     _getProcessActionCount();

    bool                    _copyUninstallFiles();
    bool                    _processClientInstall();
    bool                    _processHWSettings();

    bool                    _processDatabaseUpdate();

    void                    _refreshPages();
    bool                    _isRegPathExists( QString p_qsPath );
    bool                    _isRegStringMatch( QString p_qsPath, QString p_qsKey, QString p_qsValue );
    void                    _setEnableNextButton();
    void                    _fillAvailableComPorts();
    bool                    _emptyTargetDirectory( QString p_qsPath );
    bool                    _createTargetDirectory( QString p_qsPath );
    bool                    _copyClientFile( QString p_qsFileName, bool p_bInstall = true );
    bool                    _copyInstallFiles( QString p_qsFileName, bool p_bInstall = true );
    bool                    _createFolderShortcut();
    void                    _logProcess( QString p_qsLog, bool p_bInsertNewLine = true );
    bool                    _removeInstalledFilesFolders();
    void                    _getApplicationVersions();

    void                    _exitInstaller( bool m_bRestartPC = false );

private slots:
    void on_chkShowSQLPassword_clicked();
    void on_cmbLanguage_currentIndexChanged(int index);
    void on_pbStartWampInstall_clicked();
    void on_pbTestHWConnection_clicked();
    void on_cmbCOMPorts_currentIndexChanged(int index);
    void on_chkBelenus_clicked();
    void on_chkHardware_clicked();
    void on_chkDatabase_clicked();
    void on_chkWamp_clicked();
    void on_chkViewer_clicked();
    void on_pbSelectDir_clicked();
    void on_pbStartExit_clicked();
    void on_pbExitRestart_clicked();
    void on_rbRemove_clicked();
    void on_rbUpdate_clicked();
    void on_rbInstall_clicked();
    void on_pbCancel_clicked();
    void on_pbPrev_clicked();
    void on_pbNext_clicked();
    void on_cmbLanguageApp_currentIndexChanged(int);
};
//====================================================================================
#endif // DLGMAIN_H
//====================================================================================
