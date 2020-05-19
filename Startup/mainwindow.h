#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QDomDocument>
#include <QFile>
#include <QtSql>

#include "cregistry.h"

extern QTranslator     *poTransStartup;
extern QTranslator     *poTransQT;
extern QApplication    *apMainApp;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbDefault_clicked();
    void on_pbDirectoryStartup_clicked();
    void on_pbDirectoryTarget_clicked();
    void on_pbDirectoryResource_clicked();
    void on_pbDirectoryBackup_clicked();
    void on_pbStart_clicked();
    void on_ledDirectoryResource_editingFinished();
    void on_ledDirectoryBackup_editingFinished();
    void on_process_selected();
    void on_pbLangEn_clicked();
    void on_pbLangDe_clicked();
    void on_pbLangHu_clicked();
    void on_pbLangIt_clicked();

private:
    Ui::MainWindow          *ui;
    QFile                   *m_obLog;
    int                      m_nProcessType;
    QString                  m_qsErrorReportFile;
    QString                  m_qsInfoFile;
    QString                  m_qsLangInstaller;

    QSqlDatabase            *m_poDB;
    QFile                   *m_obFile;
    QDomDocument            *obProcessDoc;

    QString                  m_qsPathWampServer;
    QString                  m_qsUninstallWampExec;
    QString                  m_qsRootPassword;

    bool                     m_bWampServerAlreadyInstalled;
    bool                     m_bDatabaseAlreadyInstalled;
    bool                     m_bRootUserExists;
    bool                     m_bBelenusUserExists;

    QString                  m_qsProcessErrorMsg;
    QString                  m_qsLanguage;
    QString                  m_qsClientInstallDir;

    void _setProgressText( QString qsMessage );

    void _updateEnvironmentVariables();
    bool _createPaths();
    bool _createPath( QString p_qsPath );
    bool _createSettingsFile();
//    bool _updateSettingsFile();
    void _progressStep();
    bool _copyUpdaterFiles();
    bool _copyXmlFile();
    void _executeInstaller();
    void _executeUpdater();
    bool _copyFile( QString p_qsSrc, QString p_qsDst );
    void _logProcess( QString p_qsLog, bool p_bInsertNewLine = true );
    bool _emptyTargetDirectory( QString p_qsPath );
    void _executeWampUninstall();
    void _deleteRegistryKey( QString p_qsRegPath, QString p_qsKey = "" );
    void _removeShortcuts();
    void _removeDirectory( QString p_qsPath );

    void                    _checkInstallComponents();
    bool                    _installWampServer();
    bool                    _processWampServerInstall( QString *p_qsMessage );
    int                     _checkWampServer();

    bool                    _processDatabaseInstall();
    bool                    _processRootCreate();
    bool                    _processDatabaseCreate();
    bool                    _processBelenusUserCreate();
    bool                    _processBelenusUserRights();
    bool                    _processBelenusTablesCreate();
    bool                    _processBelenusTablesFill();
    bool                    _processBelenusDeviceFill();
    bool                    _processClientInstall();
    bool                    _copyInstallFiles( QString p_qsFileName, bool p_bInstall = true );
    bool                    _createFolderShortcut();
    bool                    _createTargetDirectory( QString p_qsPath );
    bool                    _createClientLanguageSelectFile();
};

#endif // MAINWINDOW_H
