#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

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

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void    timerEvent( QTimerEvent *p_poEvent );

private slots:
    void    on_pbExit_clicked();
    void    on_pbSelect_clicked();
    void    on_pbStart_clicked();
    void    setControlsEnabled( bool p_bEnable );
//    void    processBackup();
//    void    processRestore();
    void    processExecute();
    QString saveSettingU( QString p_Identifier, unsigned int p_Value );
    QString saveSettingS( QString p_Identifier, QString p_Value );
    QString saveSettingB( QString p_Identifier, bool p_Value );
    QString saveSettingI( QString p_Identifier, int p_Value );

private:

    Ui::MainWindow  *ui;
    int              m_nTimer;
    teAction         m_teAction;
    QString          m_qsFileName;
};

#endif // MAINWINDOW_H
