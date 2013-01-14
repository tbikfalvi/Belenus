#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_pbDeleteAllFiles_clicked();
    void on_pbGenerateUpdateFile_clicked();
    void on_pbCreateInstallFile_clicked();
    void on_pbCreateList_clicked();
    void on_pbSelectDir_clicked();
    void on_pbDeleteFiles_clicked();
    void on_pbAddFiles_clicked();
};

#endif // MAINWINDOW_H
