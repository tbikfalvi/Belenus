#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QFile>

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
    void on_cmbLanguage_currentIndexChanged(int);
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

private:
    Ui::MainWindow          *ui;
    QFile                   *m_obLog;
    int                      m_nProcessType;
    QString                  m_qsErrorReportFile;
    QString                  m_qsInfoFile;
    QString                  m_qsLangInstaller;

    void _updateEnvironmentVariables();
    bool _createPaths();
    bool _createPath( QString p_qsPath );
    bool _createSettingsFile();
    bool _updateSettingsFile();
    void _progressStep();
    bool _copyUpdaterFiles();
    bool _copyXmlFile();
    void _executeInstaller();
    void _executeUpdater();
    bool _copyFile( QString p_qsSrc, QString p_qsDst );
    void _logProcess( QString p_qsLog, bool p_bInsertNewLine = true );
};

#endif // MAINWINDOW_H
