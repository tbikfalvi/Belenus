#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

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

private:
    Ui::MainWindow *ui;

    int m_nProcessType;

    void _updateEnvironmentVariables();
    bool _createPaths();
    bool _createPath( QString p_qsPath );
    bool _createSettingsFile();
    bool _updateSettingsFile();
    void _progressStep();
    bool _copyUpdaterFiles();
    bool _copyFile( QString p_qsSrc, QString p_qsDst );
};

#endif // MAINWINDOW_H
