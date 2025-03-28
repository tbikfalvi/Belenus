#ifndef DLG_MAINWINDOW_H
#define DLG_MAINWINDOW_H

#include "adminClientThread.h"
#include "ui_mainwindow.h"



class MainWindow : public QDialog, protected Ui::UMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *p_poParent = 0 );
    virtual ~MainWindow();

public slots:
    void connected();
    void disconnected();
    void socketError( QAbstractSocket::SocketError socketError );

private:
    void enableConnectionButton();

private slots:
    /* slots for UI signals */
    void on_pbExit_clicked();
    void on_bRemoveKey_clicked();
    void on_bGetLogs_clicked();
    void on_pushButton_clicked();
    void on_bResetCode2_clicked();
    void on_bRegister_clicked();
    void on_bConnect_clicked();
    void on_iPassword_textChanged(QString );
    void on_iUsername_textChanged(QString );
    void on_iHost_textChanged(QString );

    /* slots for signals from Connection */
    void onSqlResult(int id, SqlResult *res);
    void onLogMessage(QString msg);
    void onRegisterKeyResponse(Result::ResultCode);

protected:

    AdminClientThread _connection;
};

#endif // DLG_MAINWINDOW_H
