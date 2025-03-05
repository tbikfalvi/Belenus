#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "../framework/qtframework.h"

namespace Ui { class MainWindow; }

extern cQTMySQLConnection       *g_poDB;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum teAction
    {
        ACT_BACKUP = 0,
        ACT_RESTORE,
        ACT_EXECUTE,
        ACT_FINISHED
    };

    explicit MainWindow(QWidget *parent = 0, QString p_qsVersion = "", teAction p_teAction = ACT_BACKUP, QString p_qsFileName = "" );
    ~MainWindow();

protected:
    void    timerEvent( QTimerEvent *p_poEvent );

private slots:
    void    on_pbExit_clicked();
    void    on_pbSelect_clicked();
    void    on_pbStart_clicked();
    void    setControlsEnabled( bool p_bEnable );
    void    processBackup();
    void    processRestore();
    void    processExecute();
    QString         loadSetting( QString p_Identifier, QString p_Default ) throw (cSevException);

private:

    Ui::MainWindow      *ui;
    QSystemTrayIcon     *trayIcon;
    int                  m_nTimer;
    teAction             m_teAction;
    QString              m_qsFileName;
    QString              m_qsDirDbBinaries;
    QString              m_qsDirDbBackup;
};

#endif // MAINWINDOW_H
