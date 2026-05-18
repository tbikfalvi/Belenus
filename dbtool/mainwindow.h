#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include "../framework/qtframework.h"
#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QListWidgetItem>

#include "dlgprogress.h"

extern QTranslator          *poTransTool;
extern QTranslator          *poTransQT;
extern QApplication         *apMainApp;
extern QString               g_qsCurrentPath;
extern cQTMySQLConnection   *g_poDB;

//====================================================================================

const int   CONST_PAGE_START            = 0;
const int   CONST_PAGE_DATABASE_ADMIN   = 1;
const int   CONST_PAGE_SELECT_PROCESS   = 2;
const int   CONST_PAGE_EXECUTE_PROCESS  = 3;

//====================================================================================

namespace Ui { class MainWindow; }

//====================================================================================
//
//====================================================================================

class MainWindow : public QMainWindow
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
    explicit MainWindow(QWidget *parent = 0, QString p_qsVersion = "");
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent *p_poEvent );

private:
    void            _initializePage();
    void            _loadCurrentVersion();
    void            _connectDatabase();
    bool            _isSystemVerificationOk();
    void            _logProcessInfo( QString p_qsText );
    void            _calculateRelatedRecords();
    void            _calculateDeactivatePatientcards();
    void            _processDeactivatePatientcards();
    void            _calculateDeleteDeactivatedPatientcards();
    void            _processDeleteDeactivatedPatientcards();
    void            _calculateDeleteObsoletePatientcardUnits();
    void            _processDeleteObsoletePatientcardUnits();
    void            _calculateDeleteLedgerEntries();
    void            _processDeleteLedgerEntries();
    void            _calculateDeleteDeviceLedger();
    void            _processDeleteDeviceLedger();
    void            _calculateDeleteCassa();
    void            _processDeleteCassa();
    void            _executeDBUpgrade();

    Ui::MainWindow  *ui;

    QString         m_qsRPSW;

    int             m_enGroup;
    unsigned int    m_uiUserId;

    QDir             m_qdExpCurrentDir;
    int              m_nCurrentPage;

    QString          m_qsLanguage;
    int              m_nLicenceId;

    cDlgProgress    *m_dlgProgress;

    QStringList      m_qslVersions;
    QStringList      m_qslDBUpdates;
    bool             m_bUpgradeDatabase;

private slots:
    void on_pbExpSelectDir_clicked();
    void on_pbNext_clicked();
    void on_pbPrev_clicked();
    void on_pbStartExit_clicked();
    void on_cmbLanguage_currentIndexChanged(int index);
    void on_pbLogin_clicked();
    void on_pbExecuteProcess_clicked();
    void on_rbDefault_toggled(bool checked);
    void on_pbUpgradeDB_clicked();
};

#endif // MAINWINDOW_H
