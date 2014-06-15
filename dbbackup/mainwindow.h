#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void        processMain();

protected:
    void timerEvent( QTimerEvent *p_poEvent );

private slots:
    void on_pbExit_clicked();

private:

    Ui::MainWindow *ui;
    bool            m_bProcessFinished;
    int             m_nTimer;
};

#endif // MAINWINDOW_H
