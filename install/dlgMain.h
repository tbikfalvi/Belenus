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

//====================================================================================

#define CONST_PAGE_WELCOME              0
#define CONST_PAGE_INSTALL_SELECTION    1
#define CONST_PAGE_COMPONENT_SELECTION  2
#define CONST_PAGE_WAMP_INSTALL         3
#define CONST_PAGE_DATABASE_INSTALL     4
#define CONST_PAGE_HARDWARE_INSTALL     5
#define CONST_PAGE_INTERNET_INSTALL     6
#define CONST_PAGE_CLIENT_INSTALL       7
#define CONST_PAGE_PROCESS              8
#define CONST_PAGE_FINISH               9

//====================================================================================
class dlgMain : public QDialog, protected Ui::dlgMain
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void closeEvent( QCloseEvent *p_poEvent );

private:

    QString                  m_qsPathPrograms;
    QString                  m_qsPathDesktop;
    QString                  m_qsPathWampServer;

    bool                     m_bBelenusAlreadyInstalled;



    QFile                   *m_obFile;
    int                      m_nTimer;

    QVector<int>             m_vPages;
    int                      m_nCurrentPage;
    QRadioButton            *m_pInstallType;
    QString                  m_qsRootPassword;
    int                      m_nComPort;
    int                      m_nCountDevices;

    bool                     m_bInstallStarted;         // Identifies if install process started
                                                        // true after component selection
    bool                     m_bProcessWamp;
    bool                     m_bProcessDatabase;
    bool                     m_bProcessHWConnection;
    bool                     m_bProcessInternet;
    bool                     m_bProcessBelenusClient;
    bool                     m_bRestartRequired;
    bool                     m_bStartWampInstall;
    bool                     m_bInitializeWamp;
    bool                     m_bCreateDatabase;
    bool                     m_bDatabaseInstalled;
    bool                     m_bDemoMode;
    bool                     m_bInstallClient;
    bool                     m_bInstallFinished;

    QSqlDatabase            *m_poDB;
    CS_Communication        *m_poHardware;
    QString                  m_qsClientInstallDir;
    QString                  m_qsIPAddress;
    int                      m_nPort;

    void                    _initializeInstall();

    void                    _initializePage( int p_nPage );
    void                    _initializeInstallSelection();
    void                    _initializeComponentSelection();
    void                    _initializeWampInstall();
    void                    _initializeDatabaseInstall();
    void                    _initializeHardwareInstall();
    void                    _initializeInternetInstall();
    void                    _initializeClientInstall();
    void                    _initializeInstallProcess();
    void                    _initializeFinishPage();

    bool                    _processPage( int p_nPage );
    bool                    _processSelectionPage();
    bool                    _processComponentSelectionPage();
    bool                    _processWampInstallPage();
    bool                    _processDatabaseInstallPage();
    bool                    _processHardwareInstallPage();
    bool                    _processInternetInstallPage();
    bool                    _processClientInstallPage();

    bool                    _processWampServerInstall();
    bool                    _initializeWampServer();
    bool                    _processDatabaseCreate();
    bool                    _processBelenusUserCreate();
    bool                    _processBelenusUserRights();
    bool                    _processBelenusTablesCreate();
    bool                    _processBelenusTablesFill();
    bool                    _processBelenusDeviceFill();
    bool                    _processClientInstall();

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

    void                    _exitInstaller( bool m_bRestartPC = false );

private slots:
    void on_pbTestHWConnection_clicked();
    void on_cmbCOMPorts_currentIndexChanged(int index);
    void on_pbCheckRootPsw_clicked();
    void on_chkInternet_clicked();
    void on_chkBelenus_clicked();
    void on_chkHardware_clicked();
    void on_chkDatabase_clicked();
    void on_chkWamp_clicked();
    void on_pbSelectDir_clicked();
    void on_pbStartExit_clicked();
    void on_pbExitRestart_clicked();
    void on_rbRemove_clicked();
    void on_rbUpdate_clicked();
    void on_rbInstall_clicked();
    void on_pbCancel_clicked();
    void on_pbPrev_clicked();
    void on_pbNext_clicked();
};
//====================================================================================
#endif // DLGMAIN_H
//====================================================================================
