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

//#include <windows.h>
//#include <shlobj.h>

#include <QDialog>
#include <QFile>
#include <QSettings>
#include <QVector>

//====================================================================================

#include "ui_dlgMain.h"

//====================================================================================

#define CONST_PAGE_WELCOME              0
#define CONST_PAGE_INSTALL_SELECTION    1
#define CONST_PAGE_COMPONENT_SELECTION  2
#define CONST_PAGE_WAMP_INSTALL         3
#define CONST_PAGE_DATABASE_INSTALL     4
#define CONST_PAGE_FINISH               5

//====================================================================================
class dlgMain : public QDialog, protected Ui::dlgMain
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();

private:
    QFile           *m_obFile;

    QVector<int>     m_vPages;
    int              m_nCurrentPage;
    QRadioButton    *m_pInstallType;
    bool             m_bProcessWamp;
    bool             m_bProcessDatabase;
    bool             m_bProcessHWConnection;
    bool             m_bProcessBelenusClient;
    bool             m_bRestartRequired;

    void _initializePage( int p_nPage );
    void _initializeInstallSelection();
    void _initializeComponentSelection();
    void _initializeFinishPage();

    bool _processPage( int p_nPage );
    bool _processInstallSelection();
    bool _processComponentSelection();

    bool _processWampInstall();

    void _refreshPages();
    bool _isRegKeyExists( QString p_qsKeyName );
    void _setEnableNextButton();

    void _exitInstaller( bool m_bRestartPC = false );

private slots:
    void on_chkBelenus_clicked();
    void on_chkHardware_clicked();
    void on_chkDatabase_clicked();
    void on_chkWamp_clicked();
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
