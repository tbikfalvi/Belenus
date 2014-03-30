#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFile>
#include <QCloseEvent>
#include <QStringList>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void closeEvent( QCloseEvent *p_poEvent );

private:

    QStringList      m_qslMissingFiles;
    QFile           *m_obLog;
    int              m_nTimer;
    bool             m_bUpdateProcessFinished;

    void             executeUpdateProcess();

    void             checkEnvironment( bool p_bVisible );
    bool             isEnvironmentOk();
    bool             checkDirectories();
    bool             checkFiles();

    void             checkInternetConnection();

    bool            _checkAndCreateDirectory( QString p_qsPath );
    bool            _createTargetDirectory( QString p_qsPath );
    bool            _isFileExists( QString p_qsFilename );
    bool            _createRegistryEntries();

    void            _logProcess( QString p_qsLog, bool p_bInsertNewLine = true );
};

#endif // MAINWINDOW_H
